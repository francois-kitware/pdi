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

#ifndef PDI_STATE_H__
#define PDI_STATE_H__

#include <pdi.h>

#include <pdi/state_fwd.h>
#include <pdi/value_fwd.h>
#include <pdi/plugin_fwd.h>
#include <pdi/datatype.h>

struct loaded_plugin_s
{
	char *name;
	
	PDI_plugin_t *impl;
	
};

struct PDI_dimension_s
{
	char *name;
	
	PDI_value_t *value;
	
};

struct PDI_variable_value_s
{
	// PDI_inout_t ORed
	int access;
	
	void *data;
	
};

struct PDI_param_s
{
	char *name;
	
	PDI_type_t type;
	
	void *value;
	
};

struct PDI_variable_s
{
	char *name;
	
	PDI_type_t type;
	
	PC_tree_t config;
	
	PDI_variable_value_t content;
	
};

struct PDI_state_s
{
	int nb_params;
	
	PDI_param_t *params;
	
	int nb_variables;
	
	PDI_variable_t *variables;
	
	int nb_plugins;
	
	PDI_plugin_t *plugins;
	
	PDI_errfunc_f *errfunc;
	
};

extern PDI_state_t PDI_EXPORT PDI_state;

#endif // PDI_STATE_H__