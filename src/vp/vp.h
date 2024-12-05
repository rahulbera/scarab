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
 * File         : vp/vp.h
 * Author       : Rahul Bera, SAFARI Research Group (write2bera@gmail.com)
 * Date         : 02/DEC/2024
 * Description  :
 ***************************************************************************************/

#ifndef __VP_H__
#define __VP_H__

#include "globals/global_types.h"
#include "op.h"

/**************************************************************************************/
// Value predictor data

// Forward declarations
struct Vp_struct;

typedef struct Vp_Data_Struct {
  uns               proc_id;
  struct Vp_struct* vp;

} Vp_Data;


/**************************************************************************************/
/* Value Predictor Interfaces */

/* IMPORTANT: please make sure that this enum matches EXACTLY the names and
 * order in vp/vp_table.def !!!!!!! */
typedef enum Vp_Id_enum {
  NO_VP,
  LAST_VAL_VP,
  NUM_VP,
} Vp_Id;

typedef struct Vp_struct {
  Vp_Id       id;
  const char* name;
  void (*init_func)(void);         /* called to initialize the predictor */
  Flag (*pred_func)(Op*, uns64*);  /* called to value predict */
  void (*update_func)(Op*, uns64); /* called to update the vp when true value is
                                     known (at the end of execute or retire) */
  void (*retire_func)(Op*);   /* called to retire an instruction and update the
                                 state of the vp that has to be updated after
                                 retirement */
  void (*recover_func)(void); /* called to recover the vp when a misprediction
                                 is realized */
} Vp;

/**************************************************************************************/
/* External variables */

extern Vp       vp_table[];
extern Vp_Data* g_vp_data;

/**************************************************************************************/
/* Prototypes */

Flag op_vp_eligible(Op*);  // returns true if the op is eligible for VP

void set_vp_data(Vp_Data*);
void init_vp_data(uns8, Vp_Data*);
void vp_predict_op(Vp_Data*, Op*);
void vp_resolve_op(Vp_Data*, Op*);
void vp_retire_op(Vp_Data*, Op*);
void vp_recover_op(Vp_Data*);


/**************************************************************************************/


#endif /* #ifndef __VP_H__ */
