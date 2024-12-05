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

#include "core.param.h"
#include "debug/debug.param.h"
#include "debug/debug_macros.h"
#include "debug/debug_print.h"
#include "globals/assert.h"
#include "statistics.h"

#include "vp/last_val.h"
#include "vp/novp.h"
#include "vp/vp.h"
#include "vp/vp.param.h"


/******************************************************************************/
/* include the table of possible value predictors */

#include "vp/vp_table.def"

/******************************************************************************/
/* Macros */

#define DEBUG(proc_id, args...) _DEBUG(proc_id, DEBUG_VP, ##args)


/******************************************************************************/
/* Global Variables */

Vp_Data* g_vp_data = NULL;


/******************************************************************************/
// Local prototypes

/******************************************************************************/
/* set_vp_data set the global vp_data pointer (so I don't have to pass it around
 * everywhere */
void set_vp_data(Vp_Data* new_vp_data) {
  g_vp_data = new_vp_data;
}

/******************************************************************************/
/* init_vp:  initializes all value prediction structures */

void init_vp_data(uns8 proc_id, Vp_Data* vp_data) {
  ASSERT(vp_data->proc_id, vp_data);
  memset(vp_data, 0, sizeof(Vp_Data));

  vp_data->proc_id = proc_id;
  /* initialize value predictor */
  vp_data->vp = &vp_table[VP_MECH];
  vp_data->vp->init_func();

  g_vp_data = vp_data;
}

/******************************************************************************/
/* vp_predict_op:  predicts the value of a (load) instruction */

void vp_predict_op(Vp_Data* vp_data, Op* op) {
  uns64 pred_val = 0;

  ASSERT(vp_data->proc_id, vp_data->proc_id == op->proc_id);
  STAT_EVENT(op->proc_id, VP_ELIGIBLE_OPS_ON_PATH + op->off_path);

  if(PERFECT_VP) {
    op->vp                    = TRUE;
    op->oracle_info.vpval     = op->oracle_info.ldval;
    op->oracle_info.vpmispred = FALSE;
  } else {
    op->vp                = vp_data->vp->pred_func(op, &pred_val);
    op->oracle_info.vpval = pred_val;
    // misprediction when VP is confident and yet generated a wrong value
    op->oracle_info.vpmispred = (op->vp && op->oracle_info.ldval !=
                                             op->oracle_info.vpval) ?
                                  TRUE :
                                  FALSE;
  }

  STAT_EVENT(op->proc_id, VP_ON_PATH_NOPRED + op->vp +
                            op->oracle_info.vpmispred + 3 * op->off_path);
}

/******************************************************************************/
/* vp_resolve_op: called on ops when they complete
 * either in exec or after dcache stage */

void vp_resolve_op(Vp_Data* vp_data, Op* op) {
  ASSERT(vp_data->proc_id, vp_data->proc_id == op->proc_id);

  if(PERFECT_VP) {
    // do nothing
  } else {
    vp_data->vp->update_func(op, op->oracle_info.ldval);
  }
}

/******************************************************************************/
/* vp_retire_op: called to update critical branch predictor state that should
 * only be updated on the right path and retire the timestamp of the branch.
 */

void vp_retire_op(Vp_Data* vp_data, Op* op) {}

/******************************************************************************/
/* vp_recover_op: called on the last mispredicted op when the recovery happens
 */

void vp_recover_op(Vp_Data* vp_data) {}

/******************************************************************************/
/* op_vp_eligible: returns true if the op is eligible for VP
 */

Flag op_vp_eligible(Op* op) {
  // Only VP scalar loads
  if(op->table_info->mem_type == MEM_LD && op->oracle_info.mem_size <= 8) {
    return TRUE;
  }
  return FALSE;
}