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

/***************************************************************************************
 * File         : vp/novp.c
 * Author       : Rahul Bera, SAFARI Research Group (write2bera@gmail.com)
 * Date         : 04/DEC/2024
 * Description  :
 ***************************************************************************************/

#include "vp/novp.h"

//-----------------------------------------//
// Init VP
//-----------------------------------------//
void vp_novp_init(void) {}

//-----------------------------------------//
// Value prediction generation
//-----------------------------------------//
Flag vp_novp_pred(Op* op, uns64* pred_val) {
  *pred_val = 0xdeadbeef;
  return FALSE;
}

//-----------------------------------------//
// Update function
//-----------------------------------------//
void vp_novp_update(Op* op, uns64 true_val) {}

//-----------------------------------------//
// Called at retire
//-----------------------------------------//
void vp_novp_retire(Op* op) {}

//-----------------------------------------//
// Called at recovery
//-----------------------------------------//
void vp_novp_recover(void) {}