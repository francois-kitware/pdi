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

#ifndef DATA_DESCRIPTOR_H__
#define DATA_DESCRIPTOR_H__

#include <memory>
#include <stack>

#include <paraconf.h>
#include <pdi.h>

#include "pdi/datatype.h"

namespace PDI
{

/** Describe the content of a buffer.
 * \author Corentin Roussel <corentin.roussel@cea.fr>
 * \date 2017-09-08
 */
class Data_descriptor
{
public:
	/** Create an empty data descriptor
	 */
	Data_descriptor(const std::string &name);
	
	/** Create an empty descriptor
	 */
	Data_descriptor(const char *name);
	
	Data_descriptor(const Data_descriptor &) = delete;
	
	Data_descriptor(Data_descriptor &&) = delete;
	
	~Data_descriptor();
	
	Data_descriptor &operator= (const Data_descriptor &) = delete;
	
	Data_descriptor &operator= (Data_descriptor &&) = delete;
	
	/** initialize the descriptor
	 */
	PDI_status_t init(PC_tree_t config, bool is_metadata, const PDI_datatype_t &type);
	
	/** Return the datatype
	 */
	const PDI_datatype_t &get_type() const;
	
	/** Return the PC_tree_t config
	 */
	PC_tree_t get_config() const;
	
	/** Return true if the data is a metadata
	 */
	bool is_metadata() const;
	
	const std::string &name() const
	{
		return m_name;
	}
	
	/** Returns a reference to the value of the data behind this descriptor
	 */
	Data_ref value()
	{
		return (m_values.empty() ? Data_ref() : *m_values.top());
	}
	
	/** Shares some data with PDI. The user code should not modify it before
	* a call to either PDI_release or PDI_reclaim.
	* \param[in,out] data the accessed data
	* \param[in] access whether the data can be accessed for read or write
	*                   by PDI
	* \return an error status
	* \pre the user code owns the data buffer
	* \post ownership of the data buffer is shared between PDI and the user code
	*
	* the access parameter is a binary OR of PDI_IN & PDI_OUT.
	* * PDI_IN means PDI can set the buffer content
	* * PDI_OUT means the buffer contains data that can be accessed by PDI
	*/
	PDI_status_t share(void *data, Data_ref::Free_function freefunc, PDI_inout_t access);
	
	/** Requests for PDI to access a data buffer.
	* \param[in,out] buffer a pointer to the accessed data buffer
	* \param[in] inout the access properties (PDI_IN, PDI_OUT, PDI_INOUT)
	* \return an error status
	* \pre PDI owns the data buffer
	* \post ownership of the data buffer is shared between PDI and the user code
	*/
	PDI_status_t access(void **buffer, PDI_inout_t inout);
	
	/** Releases ownership of a data shared with PDI. PDI is then responsible to
	* free the associated memory whenever necessary.
	* \param[in] name name of the data to release
	* \return an error status
	* \pre ownership of the data buffer is shared between PDI and the user code
	* \pre PDI owns the data buffer
	*/
	PDI_status_t release();
	
	/** Reclaims ownership of a data buffer shared with PDI. PDI is then responsible to
	* free the associated memory whenever necessary.
	* \param[in] name name of the data to reclaim
	* \return an error status
	* \pre ownership of the data buffer is shared between PDI and the user code
	* \post the user code owns the data buffer
	*/
	PDI_status_t reclaim();
	
private:
	/// References to the values of this descriptor
	std::stack<std::unique_ptr<Data_ref>> m_values;
	
	PC_tree_t m_config;
	
	bool m_metadata;
	
	PDI_datatype_t m_type;
	
	const std::string m_name;
	
}; // class Data_descriptor

} // namespace PDI

#endif // DATA_DESCRIPTOR_H__