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
 * File         : vp/last_val.cc
 * Author       : Rahul Bera, SAFARI Research Group (write2bera@gmail.com)
 * Date         : 02/DEC/2024
 * Description  :
 ***************************************************************************************/

#include "vp/last_val.h"
#include <assert.h>
#include "core.param.h"
#include "globals/assert.h"
#include "statistics.h"
#include "vp/vp.param.h"

typedef struct LVTEntry_struct {
  Flag  valid;
  uns64 pc;
  uns64 val;
  uns   conf;
} LVTEntry;

LVTEntry*** lv_table = NULL;  // array of tables across cores

//-----------------------------------------//
// Internal functions
//-----------------------------------------//
uns get_lvt_index(uns64 raw) {
  return 0;
}

Flag search_lvt(uns proc_id, uns64 pc, uns set, uns* way) {
  uns ii;
  for(ii = 0; ii < LAST_VAL_NUM_WAYS; ++ii) {
    if(lv_table[proc_id][set][ii].valid &&
       lv_table[proc_id][set][ii].pc == pc) {
      *way = ii;
      return TRUE;
    }
  }
  *way = 0;
  return FALSE;
}

uns find_lvt_victim(uns proc_id, uns set) {
  // confidence based replacement policy
  // entry with the lowest confidence gets replaced
  uns ii;
  uns min_conf = UINT32_MAX, min_conf_index = 0;
  for(ii = 0; ii < LAST_VAL_NUM_WAYS; ++ii) {
    if(!lv_table[proc_id][set][ii].valid)
      return ii;
    if(lv_table[proc_id][set][ii].conf < min_conf) {
      min_conf_index = ii;
      min_conf       = lv_table[proc_id][set][ii].conf;
    }
  }
  return min_conf_index;
}

void evict_lvt(uns proc_id, uns set, uns way) {
  lv_table[proc_id][set][way].pc    = 0;
  lv_table[proc_id][set][way].val   = 0;
  lv_table[proc_id][set][way].conf  = 0;
  lv_table[proc_id][set][way].valid = FALSE;
}

//-----------------------------------------//
// Init VP
//-----------------------------------------//
void vp_last_val_init(void) {
  uns ii, jj;

  lv_table = (LVTEntry***)malloc(sizeof(LVTEntry**) * NUM_CORES);
  assert(lv_table);
  for(ii = 0; ii < NUM_CORES; ++ii) {
    lv_table[ii] = (LVTEntry**)malloc(sizeof(LVTEntry*) * LAST_VAL_NUM_SETS);
    assert(lv_table[ii]);
    for(jj = 0; jj < LAST_VAL_NUM_SETS; ++jj) {
      lv_table[ii][jj] = (LVTEntry*)malloc(sizeof(LVTEntry) *
                                           LAST_VAL_NUM_WAYS);
      assert(lv_table[ii][jj]);
      memset(lv_table[ii][jj], 0, sizeof(LVTEntry) * LAST_VAL_NUM_WAYS);
    }
  }
}

//-----------------------------------------//
// Value prediction generation
//-----------------------------------------//
Flag vp_last_val_pred(Op* op, uns64* pred_val) {
  uns   proc_id = op->proc_id;
  uns64 pc      = op->inst_info->addr;
  uns   set     = get_lvt_index(pc);
  uns   way     = 0;
  Flag  found   = FALSE;

  found = search_lvt(proc_id, pc, set, &way);

  if(found && lv_table[proc_id][set][way].conf >= LAST_VAL_CONF_THRESH) {
    *pred_val = lv_table[proc_id][set][way].val;
    return TRUE;
  }

  *pred_val = 0;
  return FALSE;
}

//-----------------------------------------//
// Update function
//-----------------------------------------//
void vp_last_val_update(Op* op, uns64 true_val) {
  uns       proc_id = op->proc_id;
  uns64     pc      = op->inst_info->addr;
  uns       set     = get_lvt_index(pc);
  uns       way     = 0;
  Flag      found   = FALSE;
  LVTEntry* lventry = NULL;

  found = search_lvt(proc_id, pc, set, &way);

  if(found) {
    lventry = &lv_table[proc_id][set][way];
    if(lventry->val == true_val) {
      lventry->conf = MIN2(lventry->conf + 1, (1u << LAST_VAL_CONF_BITS) - 1);
    } else {
      lventry->conf = LAST_VAL_EXP_DECR ? lventry->conf : 0;
      lventry->val  = true_val;
    }
  } else {  // not found
    way = find_lvt_victim(proc_id, set);
    evict_lvt(proc_id, set, way);
    lventry        = &lv_table[proc_id][set][way];
    lventry->pc    = pc;
    lventry->val   = true_val;
    lventry->conf  = 0;
    lventry->valid = TRUE;
  }
}

//-----------------------------------------//
// Called at retire
//-----------------------------------------//
void vp_last_val_retire(Op* op) {}

//-----------------------------------------//
// Called at recovery
//-----------------------------------------//
void vp_last_val_recover(void) {}