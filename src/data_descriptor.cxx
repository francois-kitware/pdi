/*******************************************************************************
 * Copyright (c) 2015, Julien Bigot - CEA (julien.bigot@cea.fr)
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

#include <memory>

#include "pdi/data_reference.h"
#include "pdi/datatype.h"
#include "pdi/state.h"

#include "status.h"

#include "pdi/data_descriptor.h"


namespace PDI
{

using std::stack;
using std::string;
using std::unique_ptr;


Data_descriptor::Data_descriptor(const char *name):
	m_config(PC_parse_string(const_cast<char *>(""))),
	m_metadata(false),
	m_name(name)
{
	PDI_datatype_init_scalar(&m_type, PDI_T_UNDEF);
}

Data_descriptor::Data_descriptor(const string &name):
	m_config(PC_parse_string(const_cast<char *>(""))),
	m_metadata(false),
	m_name(name)
{
	PDI_datatype_init_scalar(&m_type, PDI_T_UNDEF);
}

Data_descriptor::~Data_descriptor()
{
	PDI_datatype_destroy(&m_type);
}

PDI_status_t Data_descriptor::init(PC_tree_t config, bool is_metadata, const PDI_datatype_t &type)
{
	m_config = config;
	m_metadata = is_metadata;
	PDI_datatype_destroy(&m_type);
	return PDI_datatype_copy(&m_type, &type);
}

PDI_status_t Data_descriptor::access(void **buffer, PDI_inout_t inout)
{
	*buffer = NULL;
	
	if (m_values.empty()) return PDI_make_err(PDI_ERR_VALUE, "Cannot access a non shared value");
	
	m_values.push(unique_ptr<Data_ref>(new Data_ref(*m_values.top())));
	if (m_values.top()->grant(inout & PDI_IN, inout & PDI_OUT)) { // got the requested rights
		*buffer = *m_values.top();
		return PDI_OK;
	} else { // cannot get the requested rights
		m_values.pop();
		return PDI_make_err(PDI_ERR_RIGHT, "Cannot grant priviledge for data '%s'", m_name.c_str());
	}
}

PDI_status_t Data_descriptor::share(void *buffer, Data_ref::Free_function freefunc, PDI_inout_t access)
{
	/// for metadata, unlink happens on share
	if (!m_values.empty() && is_metadata()) {
		m_values.top()->null_release();
		m_values.pop();
	}
	
	// make a reference and put it in the store
	m_values.push(std::unique_ptr<Data_ref>(new Data_ref(buffer, freefunc, this->get_type(), access & PDI_OUT, access & PDI_IN)));
	Data_ref &ref = *m_values.top();
	
	// Provide reference to the plug-ins
	for (auto &&plugin : PDI_state.plugins) {
		// Notify the plug-ins of reference availability
		plugin.second->data(m_name, ref);
	}
	
	return PDI_OK;
}

PDI_status_t Data_descriptor::release()
{
	// move reference out of the store
	if (m_values.empty()) return PDI_make_err(PDI_ERR_VALUE, "Cannot release a non shared value");
	
	m_values.pop();
	
	return PDI_OK;
}


PDI_status_t Data_descriptor::reclaim()
{
	if (m_values.empty()) return PDI_make_err(PDI_ERR_VALUE, "Cannot reclaim a non shared value");
	
	// if the content is a metadata, keep it
	if (is_metadata()) {
		m_values.top()->copy_release();
	} else { // otherwise, reclaim the data
		m_values.top()->null_release();
		m_values.pop();
	}
	
	return PDI_OK;
}

const PDI_datatype_t &Data_descriptor::get_type() const
{
	return m_type;
}

bool Data_descriptor::is_metadata() const
{
	return m_metadata;
}

PC_tree_t Data_descriptor::get_config() const
{
	return m_config;
}

} // namespace PDI