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

#ifndef PDI_VALUE_H__
#define PDI_VALUE_H__

#include <pdi.h>

#include <pdi/value_fwd.h>
#include <pdi/state_fwd.h>

/* Grammar
VALUE   := INTVAL | STRVAL
STRVAL  := ( CHAR | '\' '\' | '\' '$' | REF | '$' '(' INTVAL ')' )*
INTVAL  := INTVAL2 ( OP1 INTVAL2 )*
INTVAL2 := INTVAL3 ( OP2 INTVAL3 )*
INTVAL3 := INTVAL4 ( OP3 INTVAL4 )*
INTVAL4 := TERM ( OP4 TERM )*
TERM    := ( CONST | REF | '(' INTVAL ')' )
REF     := '$' ( IREF | '{' IREF '}' )
IREF    := ID ( '[' INTVAL ']' )*
CONST ~= (0x)? [0-9]+ ( \.  )
OP1   ~= \|
OP2   ~= &
OP3   ~= =
OP4   ~= \+ | -
OP5   ~= \* | / | %
ID    ~= [a-zA-Z_][a-zA-Z0-9_]*
CHAR  ~= [^$\\]
*/

typedef struct PDI_refval_s PDI_refval_t;

typedef struct PDI_exprval_s PDI_exprval_t;

typedef struct PDI_strval_s PDI_strval_t;

/** A parsed value
 */
struct PDI_value_s
{
	/// the kind of value this is
	PDI_valkind_t kind;
	
	union {
		/// in case this is a PDI_VAL_CONST, the constant value
		int constval;
		
		/// in case of PDI_VAL_REF the referenced value (not owned)
		PDI_refval_t *refval;
		
		/// in case of PDI_VAL_EXPR the expression (owned)
		PDI_exprval_t *exprval;
		
		/// in case of PDI_VAL_STR the string (owned)
		PDI_strval_t *strval;
		
	} c;
	
};

/** Builds (i.e. parse) a value from a string
 * 
 * \param[in] val_str the string to parse
 * \param[out] value a pointer to the parsed value
 * \return an error status
 */
PDI_status_t PDI_EXPORT PDI_value_parse(const char *val_str, PDI_value_t *value);

/** Evaluates a value as an integer
 * 
 * \param[in] value the value to evaluate
 * \param[out] res a pointer to the integer value
 * \return an error status
 */
PDI_status_t PDI_EXPORT PDI_value_int(const PDI_value_t *value, int *res);

/** Evaluates a value as a string
 * 
 * \param[in] value the value to evaluate
 * \param[out] res a pointer to the string value
 * \return an error status
 */
PDI_status_t PDI_EXPORT PDI_value_str(const PDI_value_t *value, char **res);

/** Destroys a PDI value
 * 
 * \param[in] value the value to destroy
 * \return an error status
 */
PDI_status_t PDI_EXPORT PDI_value_destroy(PDI_value_t *value);

#endif // PDI_VALUE_H__