/*******************************************************************************
 * Copyright (C) 2018 Institute of Bioorganic Chemistry Polish Academy of Science (PSNC)
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

#ifndef PDI_GLOBAL_CONTEXT_MOCK_H_
#define PDI_GLOBAL_CONTEXT_MOCK_H_

#include <gmock/gmock.h>
#include <pdi/global_context.h>
#include <pdi/paraconf_wrapper.h>
#include <pdi/plugin.h>

struct MockGlobalContext : public PDI::Global_context {
	MockGlobalContext(PC_tree_t conf, MPI_Comm* world) :
		Global_context(conf, world)
	{}
	PDI::Paraconf_wrapper fw;
	
	MOCK_METHOD1(desc, PDI::Data_descriptor&(const std::string&));
	MOCK_METHOD1(desc, PDI::Data_descriptor&(const char*));
	
	MOCK_METHOD1(BracketOp1, PDI::Data_descriptor&(const std::string&));
	PDI::Data_descriptor& operator [] (const std::string& str) override
	{
		return BracketOp1(str);
	}
	
	MOCK_METHOD1(BracketOp2, PDI::Data_descriptor&(const char* name));
	PDI::Data_descriptor& operator [] (const char* str) override
	{
		return BracketOp2(str);
	}
	
	MOCK_METHOD0(begin, PDI::Context::Iterator());
	MOCK_METHOD0(end, PDI::Context::Iterator());
	
	MOCK_METHOD1(event, void(const char* name));
	
};


#endif //PDI_GLOBAL_CONTEXT_MOCK_H_