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

/**
* \file conf.h
* \brief contains load_conf prototype
* \author Julien Bigot (CEA) <julien.bigot@cea.fr>
*/

#ifndef PDI_CONF_H_
#define PDI_CONF_H_

#include "config.h"

#include <string>

#include <paraconf.h>

namespace PDI
{

/** Returns the length of a node.
 *
 * - for a sequence: the number of nodes,
 * - for a mapping: the number of pairs,
 * - for a scalar: the string length.
 *
 * throws an Error if the provided tree is in error
 *
 * \param[in] tree the sequence or mapping
 * \return the length
 */
int len(PC_tree_t tree);

/** Returns the int value of a scalar node
 *
 * throws an Error if the provided tree is in error
 *
 * \param[in] tree the int-valued node
 * \return the int value of the scalar node
 */
long to_long(PC_tree_t tree);

/** Returns the floating point value of a scalar node
 *
 * throws an Error if the provided tree is in error
 *
 * \param[in] tree the floating-point-valued node
 * \return the floating point value of the scalar node
 */
double to_double(PC_tree_t tree);

/** Returns the string content of a scalar node
 *
 * throws an Error if the provided tree is in error
 *
 * \param[in] tree the node
 * \return the content of the scalar node
 */
std::string to_string(PC_tree_t tree);

/** Returns the boolean value of a scalar node
 *
 * throws an Error if the provided tree is in error
 *
 * \param[in] tree the node
 * \return the boolean value of the scalar node
 */
bool to_bool(PC_tree_t tree);

} // namespace PDI

#endif // PDI_CONF_H_