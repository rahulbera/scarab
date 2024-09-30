/* Copyright 2020 HPS/SAFARI Research Groups
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __DECODER_H__
#define __DECODER_H__

#undef UNUSED   // there is a name conflict between PIN and Scarab
#undef WARNING  // there is a name conflict between PIN and Scarab

#include "pin.H"

#undef UNUSED   // there is a name conflict between PIN and Scarab
#undef WARNING  // there is a name conflict between PIN and Scarab
#include <ostream>
#include "../../ctype_pin_inst.h"
#include "../../table_info.h"
#include "x86_decoder.h"

using namespace std;

void pin_decoder_init(bool translate_x87_regs, std::ostream* err_ostream);

void            pin_decoder_insert_analysis_functions(const INS& ins);
void            insert_analysis_functions(ctype_pin_inst* info, const INS& ins);
ctype_pin_inst* pin_decoder_get_latest_inst();

void pin_decoder_print_unknown_opcodes();

vector<PIN_MEM_ACCESS_INFO>
  get_gather_scatter_mem_access_infos_from_gather_scatter_info(
    const CONTEXT* ctxt, const PIN_MULTI_MEM_ACCESS_INFO* infos_from_pin);

ctype_pin_inst create_sentinel();
ctype_pin_inst create_dummy_jump(uint64_t eip, uint64_t tgt);
ctype_pin_inst create_dummy_nop(uint64_t eip, Wrongpath_Nop_Mode_Reason reason);

// RBERA: taking this definition straight out of PIN 3.7
// FIXME: Register value should not be read like this
// Support for newer advanced instructions (e.g., AMX)
// will likely be broken. Should we assert?
const UINT32 MAX_BYTES_PER_PIN_REG = 64;
const UINT32 MAX_WORDS_PER_PIN_REG = (MAX_BYTES_PER_PIN_REG/2);
const UINT32 MAX_DWORDS_PER_PIN_REG = (MAX_WORDS_PER_PIN_REG/2);
const UINT32 MAX_QWORDS_PER_PIN_REG = (MAX_DWORDS_PER_PIN_REG/2);
const UINT32 MAX_FLOATS_PER_PIN_REG = (MAX_BYTES_PER_PIN_REG/sizeof(float));
const UINT32 MAX_DOUBLES_PER_PIN_REG = (MAX_BYTES_PER_PIN_REG/sizeof(double));

/*! @ingroup CONTEXT_API
 *  A container large enough to hold any architectural register.
 *  Implemented as a union to allow viewing the value as different types (signed/unsigned integer or floating point)
 *  and allow access in blocks of various sizes.
 */
union PIN_REGISTER
{
    UINT8  byte[MAX_BYTES_PER_PIN_REG];
    UINT16 word[MAX_WORDS_PER_PIN_REG];
    UINT32 dword[MAX_DWORDS_PER_PIN_REG];
    UINT64 qword[MAX_QWORDS_PER_PIN_REG];

    INT8   s_byte[MAX_BYTES_PER_PIN_REG];
    INT16  s_word[MAX_WORDS_PER_PIN_REG];
    INT32  s_dword[MAX_DWORDS_PER_PIN_REG];
    INT64  s_qword[MAX_QWORDS_PER_PIN_REG];

    FLT32  flt[MAX_FLOATS_PER_PIN_REG];
    FLT64  dbl[MAX_DOUBLES_PER_PIN_REG];
};


#endif  // __DECODER_H__
