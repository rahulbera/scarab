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

/*BEGIN_LEGAL
BSD License

Copyright (c)2013 Intel Corporation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */

//
// @ORIGINAL_AUTHORS: Harish Patil
//

#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <string>

#include "control_manager.H"
#include "instlib.H"
#include "pin.H"

using namespace INSTLIB;
using namespace CONTROLLER;

#undef UNUSED   // there is a name conflict between PIN and Scarab
#undef WARNING  // there is a name conflict between PIN and Scarab

#include "../pin_lib/decoder.h"

#undef UNUSED   // there is a name conflict between PIN and Scarab
#undef WARNING  // there is a name conflict between PIN and Scarab

#include "../../ctype_pin_inst.h"
#include "../../table_info.h"

#if defined(__GNUC__)
#if defined(__APPLE__)
#define ALIGN_LOCK \
  __attribute__((aligned(16))) /* apple only supports 16B alignment */
#else
#define ALIGN_LOCK __attribute__((aligned(64)))
#endif
#else
#define ALIGN_LOCK __declspec(align(64))
#endif

LOCALVAR PIN_LOCK ALIGN_LOCK output_lock;

// Track the number of instructions executed
ICOUNT icount;

// Contains knobs and instrumentation to recognize start/stop points
CONTROL_MANAGER control("controller_");

// Knobs that control trace generation
KNOB<string> Knob_output(KNOB_MODE_WRITEONCE, "pintool", "o", "trace.bz2",
                         "trace outputfilename");

/*** globals ***/
FILE* output_stream;

ctype_pin_inst mailbox;
bool           mailbox_full = false;
static BOOL    inside_roi   = FALSE;

VOID Handler(EVENT_TYPE ev, VOID* v, CONTEXT* ctxt, VOID* ip, THREADID tid,
             BOOL bcast) {
  PIN_GetLock(&output_lock, tid + 1);

  std::cout << "tid: " << tid << " ";
  std::cout << "ip: " << ip << " " << icount.Count();

  switch(ev) {
    case EVENT_START:
      std::cout << " ==== Tracing STRT ====" << endl;
      inside_roi = TRUE;
      break;

    case EVENT_STOP:
      std::cout << " ==== Tracing STOP ====" << endl;
      inside_roi = FALSE;
      PIN_ExitApplication(0);
      break;

    case EVENT_THREADID:
      std::cout << "ThreadID" << endl;
      break;

    default:
      ASSERTX(false);
      break;
  }
  PIN_ReleaseLock(&output_lock);
}

/*** function definitions ***/
INT32 Usage() {
  std::cerr << "This pin tool creates a trace that Scarab Trace frontend\n";
  std::cerr << KNOB_BASE::StringKnobSummary() << endl;
  return -1;
}

LOCALFUN VOID Fini(int n, void* v) {
  pin_decoder_print_unknown_opcodes();
  if(output_stream) {
    if(mailbox_full) {
      fwrite(&mailbox, sizeof(mailbox), 1, output_stream);
    }
    pclose(output_stream);
  }
}

void dump_instruction() {
  if(!inside_roi) {
    return;
  }

  if(!output_stream) {
    std::cout << "Output stream closed unexpectedly. Ending tracing\n";
    PIN_ExitApplication(0);
  }

  ctype_pin_inst* info = pin_decoder_get_latest_inst();
  if(mailbox_full) {
    mailbox.instruction_next_addr = info->instruction_addr;
    fwrite(&mailbox, sizeof(mailbox), 1, output_stream);
  }
  mailbox      = *info;
  mailbox_full = true;
}

VOID per_inst_instrumentation(INS ins, VOID* v) {
  pin_decoder_insert_analysis_functions(ins);
  INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)dump_instruction, IARG_END);
}

int main(int argc, char* argv[]) {
  if(PIN_Init(argc, argv)) {
    return Usage();
  }

  PIN_InitLock(&output_lock);
  icount.Activate();

  // Activate alarm, must be done before PIN_StartProgram
  control.RegisterHandler(Handler, 0, FALSE);
  control.Activate();

  if(!Knob_output.Value().empty()) {
    char popename[1024];
    sprintf(popename, "bzip2 > %s", Knob_output.Value().c_str());
    output_stream = popen(popename, "w");
  } else {
    cout << "No trace specified. Only verifying opcodes." << endl;
  }

  pin_decoder_init(true, &std::cerr);

  INS_AddInstrumentFunction(per_inst_instrumentation, 0);
  PIN_AddFiniFunction(Fini, 0);

  PIN_StartProgram();

  return 0;
}
