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

#ifndef PDI_DATATYPE_H__
#define PDI_DATATYPE_H__

#include <pdi.h>

#include <pdi/datatype_fwd.h>
#include <pdi/value.h>

struct PDI_type_s
{
	PDI_type_kind_t kind;
	
	union
	{
		PDI_scalar_type_t scalar;
		
		PDI_array_type_t *array;
		
		PDI_struct_type_t *struct_;
		
	} c;
	
};

struct PDI_array_type_s
{
	int ndims;
	
	PDI_value_t *sizes;
	
	PDI_value_t *subsizes;
	
	PDI_value_t *starts;
	
	PDI_type_t type;
	
};

struct PDI_member_s
{
	PDI_value_t displacement;
	
	PDI_type_t type;
	
	char *name;
	
};

struct PDI_struct_type_s
{
	int nb_member;
	
	PDI_member_t *members;
	
};

PDI_status_t PDI_EXPORT PDI_datatype_load(PC_tree_t node, PDI_type_t* type);

PDI_status_t PDI_EXPORT PDI_datatype_destroy(PDI_type_t *type);

#endif // PDI_DATATYPE_H__