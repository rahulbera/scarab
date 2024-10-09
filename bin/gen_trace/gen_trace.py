#!/usr/bin/env python3
#
#  Copyright 2024 HPS/SAFARI Research Groups
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy of
#  this software and associated documentation files (the "Software"), to deal in
#  the Software without restriction, including without limitation the rights to
#  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
#  of the Software, and to permit persons to whom the Software is furnished to do
#  so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

#----------------------------------------------------------------------------------------
#  File         : gen_trace.py
#  Author       : SAFARI Research Group
#  Date         : 06/OCT/2024
#  Description  : Genrates trace for trace-driven simulation
#----------------------------------------------------------------------------------------

from __future__ import print_function
import argparse
import os
import sys
import math

sys.path.append(os.path.dirname(os.path.dirname(__file__)))
from scarab_globals import *

parser = argparse.ArgumentParser(description="Launch Scarab")
parser.add_argument('--fastfwd', default=None, help="Number of instructions to fast forward.")
parser.add_argument('--tracelen', default=None, help="Number of instructions to trace.")
parser.add_argument('--bmkname', default=None, help="Name of the benchmark.")
parser.add_argument('--bmkdir', default=None, help="Home directory where workload would be found.")
parser.add_argument('--bmkcmd', default=None, help="Commandline of the workload to run")
args = parser.parse_args()

TRACING_RUN_CMD_TEMPLATE = (
'export OMP_NUM_THREADS=1\n'
'cd {run_dir_path}\n'
'setarch x86_64 -R $PIN_ROOT/pin '
'-t {scarab_home}/src/pin/pin_trace/obj-intel64/gen_trace.so '
'-fast_forward {ff} -trace_len {len} -o {trace_path} -- '
'{run_command}\n')


# Numric abbreviation
def get_abbreviation(val):
    vv = int(val) / 1e9
    if vv >= 1:
        return str(math.floor(vv)) + "B"

    vv = int(val) / 1e6
    return str(math.floor(vv)) + "M"


def main():
    len_abbv = get_abbreviation(args.tracelen)
    trace_name_base = args.bmkname + ".skip-" + str(args.fastfwd) + ".len-" + len_abbv
    trace_dir       = args.bmkdir + "/" + trace_name_base + ".Trace"
    trace_path      = trace_dir + "/" + trace_name_base + ".scarabtrace.bz2"
    trace_stdout    = trace_name_base + ".stdout"
    trace_stderr    = trace_name_base + ".stderr"
    
    trace_dir = "{}/{}.Trace".format(args.bmkdir, trace_name_base)
    os.makedirs(trace_dir, exist_ok=True)
                
    with open('{}/BENCHMARK_RUN_CMD'.format(trace_dir), 'w') as f:
        f.write(args.bmkcmd)
        
    cmd_file_path = '{}/TRACE_RUN_CMD'.format(trace_dir)
    with open(cmd_file_path, 'w') as f:
        f.write(TRACING_RUN_CMD_TEMPLATE.format(
            run_dir_path=args.bmkdir,
            scarab_home=scarab_paths.sim_dir,
            ff=args.fastfwd,
            len=args.tracelen,
            trace_path=trace_path,
            run_command=args.bmkcmd
    ))

    cmd = command.Command('bash ' + cmd_file_path, 
                          run_dir=args.bmkdir, 
                          results_dir=trace_dir,
                          stdout=trace_stdout, 
                          stderr=trace_stderr)
    print(cmd)
    cmd.run()


if __name__ == '__main__':
    main()
