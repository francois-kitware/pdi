/*******************************************************************************
 * Copyright (C) 2015-2018 Commissariat a l'energie atomique et aux energies alternatives (CEA)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of CEA nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "config.h"

#include <iostream>
#include <memory>
#include <vector>

#include <dlfcn.h>

#include "pdi/paraconf_wrapper.h"
#include "pdi/plugin.h"
#include "pdi/ref_any.h"
#include "pdi/error.h"

#include "pdi/global_context.h"


namespace PDI {

using std::cerr;
using std::endl;
using std::exception;
using std::move;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace {

typedef unique_ptr<Plugin> (*plugin_loader_f)(Context&, PC_tree_t, MPI_Comm*);


void load_data(Context& ctx, PC_tree_t node, bool is_metadata)
{
	int map_len = len(node);
	
	for (int map_id = 0; map_id < map_len; ++map_id) {
		Data_descriptor& dsc = ctx.desc(to_string(PC_get(node, "{%d}", map_id)).c_str());
		dsc.metadata(is_metadata);
		dsc.creation_template(PC_get(node, "<%d>", map_id));
	}
}

plugin_loader_f PDI_NO_EXPORT get_plugin_ctr(const char* plugin_name)
{
	string plugin_symbol = string{"PDI_plugin_"} + plugin_name + string{"_loader"};
	void* plugin_ctor_uncast = dlsym(NULL, plugin_symbol.c_str());
	
	// case where the library was not prelinked
	if (!plugin_ctor_uncast) {
		string libname = string{"libpdi_"} + plugin_name + string{"_plugin.so"};
		void* lib_handle = dlopen(libname.c_str(), RTLD_NOW);
		if (!lib_handle) {
			throw Error{PDI_ERR_PLUGIN, "Unable to load `%s' plugin file: %s", plugin_name, dlerror()};
		}
		plugin_ctor_uncast = dlsym(lib_handle, plugin_symbol.c_str());
	}
	
	if (!plugin_ctor_uncast) {
		throw Error{PDI_ERR_PLUGIN, "Unable to load `%s' plugin from file: %s", plugin_name, dlerror()};
	}
	
	return reinterpret_cast<plugin_loader_f>(plugin_ctor_uncast);
}

} // namespace <anonymous>

std::unordered_map<std::string, std::unique_ptr<Plugin>>& Global_context::get_plugins()
{
	return m_plugins;
}


std::unordered_map<std::string, Data_descriptor>& Global_context::get_descriptors()
{
	return m_descriptors;
}

Global_context::Global_context(PC_tree_t conf, MPI_Comm* world)
{
	// no metadata is not an error
	PC_tree_t metadata = PC_get(conf, ".metadata");
	if (!PC_status(metadata)) {
		load_data(*this, metadata, true);
	}
	
	// no data is spurious, but not an error
	PC_tree_t data = PC_get(conf, ".data");
	if (!PC_status(data)) {
		load_data(*this, data, false);
	}
	
	int nb_plugins = len(PC_get(conf, ".plugins"), 0);
	for (int plugin_id = 0; plugin_id < nb_plugins; ++plugin_id) {
		//TODO: what to do if a single plugin fails to load?
		string plugin_name = to_string(PC_get(conf, ".plugins{%d}", plugin_id));
		try {
			m_plugins.emplace(plugin_name, get_plugin_ctr(plugin_name.c_str())(*this, PC_get(conf, ".plugins<%d>", plugin_id), world));
		} catch (const exception& e) {
			throw Error{PDI_ERR_SYSTEM, "Error while loading plugin `%s': %s", plugin_name.c_str(), e.what()};
		}
	}
}

Global_context::Iterator Global_context::begin()
{
	return Context::get_iterator(m_descriptors.begin());
}

Global_context::Iterator Global_context::end()
{
	return Context::get_iterator(m_descriptors.end());
}

Data_descriptor& Global_context::desc(const char* name)
{
	return m_descriptors.emplace(name, Data_descriptor{*this, name}).first->second;
}

Data_descriptor& Global_context::desc(const string& name)
{
	return desc(name.c_str());
}

Data_descriptor& Global_context::operator[](const char* name)
{
	return desc(name);
}

Data_descriptor& Global_context::operator[](const string& name)
{
	return desc(name.c_str());
}

void Global_context::event(const char* name)
{
	for (auto& elmnt : m_plugins) {
		vector<Error> errors;
		try {
			elmnt.second->event(name);
			//TODO: remove the faulty plugin in case of error?
		} catch (const Error& e) {
			errors.emplace_back(e.status(), "for plugin `%s': %s", elmnt.first.c_str(), e.what());
		} catch (const std::exception& e) {
			errors.emplace_back(PDI_ERR_SYSTEM, "for plugin `%s': %s", elmnt.first.c_str(), e.what());
		} catch (...) {
			errors.emplace_back(PDI_ERR_SYSTEM, "for plugin `%s'", elmnt.first.c_str());
		}
		if ( 1 == errors.size() ) throw Error{errors.front().status(), "While triggering named event `%s' %s", name, errors.front().what()};
		if ( !errors.empty() ) {
			string errmsg = "multiple errors while triggering named event `";
			errmsg += name;
			errmsg += "':";
			for ( auto&& err: errors ) {
				errmsg += " ";
				errmsg += err.what();
			}
			throw Error{PDI_ERR_PLUGIN, "%s", errmsg.c_str()};
		}
	}
}

}