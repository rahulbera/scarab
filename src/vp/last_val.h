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
 * File         : vp/last_val.h
 * Author       : Rahul Bera, SAFARI Research Group (write2bera@gmail.com)
 * Date         : 02/DEC/2024
 * Description  :
 ***************************************************************************************/

#ifndef __LAST_VAL_H__
#define __LAST_VAL_H__

#include "vp/vp.h"

// internal functions
uns  get_lvt_index(uns64);
Flag search_lvt(uns, uns64, uns, uns*);
uns  find_lvt_victim(uns, uns);
void evict_lvt(uns, uns, uns);

/*************Interface to Scarab***************/
void vp_last_val_init(void);
Flag vp_last_val_pred(Op*, uns64*);
void vp_last_val_update(Op*, uns64);
void vp_last_val_retire(Op*);
void vp_last_val_recover(void);


#endif  // __LAST_VAL_H__
