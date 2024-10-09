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

# ----------------------------------------------------------------------------------------
#  File         : gen_trace.py
#  Author       : SAFARI Research Group
#  Date         : 07/OCT/2024
#  Description  : Generates jobfile from a simpoint descriptor CSV
# ----------------------------------------------------------------------------------------

from __future__ import print_function
import argparse
import os
import sys
import csv
import re

sys.path.append(os.path.dirname(os.path.dirname(__file__)))
from scarab_globals import *

parser = argparse.ArgumentParser(description="Launch Scarab")
parser.add_argument('--simdesc', default=None, help="Simpoint descriptor CSV file.")
args = parser.parse_args()

def parse_simpoint_csv_path(simpoint_csv_path):
  """Parse simpoints CSV file to extract:
         - The weight of each simpoint in the benchmark
         - The icounts (starting point) of each simpoint
         - The lengths (instruction count) of each simpoint
         - The total number of instructions in the benchmark

  Args:
      simpoint_csv_path (string): path to the simpoints csv

  Returns:
      [type]: [description]
  """
  weights = []
  icounts = []
  lengths = []
  simpoint_nums = []
  total_instructions_in_workload = 0

  with open(simpoint_csv_path) as f:
    for line in f.readlines():
      if len(line.strip()) == 0:
        continue
      elif line.strip()[0] == '#':
        m = re.match(r'# Total instructions in workload = ([0-9]+)', line)
        if m:
          total_instructions_in_workload = int(m.group(1))
        continue
      words = line.strip().split(',')
      simpoint_nums.append(int(words[2]))
      icounts.append(int(words[3]))
      lengths.append(int(words[4]) - icounts[-1])
      weights.append(float(words[5]))

  # while min(weights) < __args__.min_simpoint_weight:
  #   assert math.isclose(sum(weights), 1.0, abs_tol=1e-4)
  #   min_idx = weights.index(min(weights))
  #   del weights[min_idx]
  #   del icounts[min_idx]
  #   del lengths[min_idx]
  #   del simpoint_nums[min_idx]
  #   sum_weights = sum(weights)
  #   weights = [w / sum_weights for w in weights]

  weights_map = {}
  icounts_map = {}
  length_map = {}
  for simpoint_num, weight, icount, length in zip(simpoint_nums, weights, icounts, lengths):
    weights_map[simpoint_num] = weight
    icounts_map[simpoint_num] = icount
    length_map[simpoint_num] = length

  return weights_map, icounts_map, length_map, total_instructions_in_workload

JOBFILE_PREAMBLE = (
    "#!/bin/bash\n"
    "#\n"
    "# This is a jobfile that contains commands to run via slurm.\n"
    "# To launch the jobs, simple source the file as\n"
    "# source ./<filename>\n"
    "# \n"
    "#"
)

SLURM_PREAMBLE = (
    "sbatch -p cpu_part --mincpus=1 "
    "--exclude kratos[10] -c 1 "
    "-J {jobname} "
    "-o {jobout} "
    "-e {joberr} "
)

# JOB_DESC_TEMPLATE = (
#   "./gen_trace/gen_trace.py "
#   "--fastfwd {ff_count} "
#   "--tracelen {trace_len} "
#   "--bmkname {bmkname} "
#   "--bmkdir {bmkdir} "
#   "--bmkcmd \"{bmkcmd}\"\n"
# )

JOB_DESC_TEMPLATE = (
    "{scarab_home}/bin/gen_trace/gen_trace_wrapper.sh "
    "{ff_count} "
    "{trace_len} "
    "{bmkname} "
    "{bmkdir} "
    "\"{bmkcmd}\""
)

def read_all_simpoints(bmkname, bmkdir, bmkcmd, simpoint_csv_path):
  if not os.path.isfile(simpoint_csv_path):
    print('ERROR: could not find the simpoint output file in {}'.format(simpoint_csv_path))
    sys.exit(1)
  weights_map, icounts_map, length_map, total_instructions_in_workload = parse_simpoint_csv_path(simpoint_csv_path)
  
  for simpoint_num in weights_map:
    cmd = JOB_DESC_TEMPLATE.format(
      scarab_home=scarab_paths.sim_dir,
      ff_count=icounts_map[simpoint_num],
      trace_len=length_map[simpoint_num],
      bmkname=bmkname,
      bmkdir=bmkdir,
      bmkcmd=bmkcmd
    )
    
    # Slurm log files
    slurm_log_dir = bmkdir + "/slurmlog"
    os.makedirs(slurm_log_dir, exist_ok=True)
    
    slurm_job_basename = "slurm_gentrace_{}.s{}.t{}".format(bmkname, 
                                             icounts_map[simpoint_num], 
                                             length_map[simpoint_num])
    slurm_job_out = "{}/{}.out".format(slurm_log_dir, slurm_job_basename)
    slurm_job_err = "{}/{}.err".format(slurm_log_dir, slurm_job_basename)
    
    # Prepare slurm command
    slurm_cmd = SLURM_PREAMBLE.format(
        jobname=slurm_job_basename,
        jobout=slurm_job_out,
        joberr=slurm_job_err
    )
    
    # Print the slurm job commandline
    print(slurm_cmd + " " + cmd)

def main():
    if not os.path.isfile(args.simdesc):
        print('ERROR: could not find the simpoint output file in {}'.format(args.simdesc))
        sys.exit(1)
        
    print(JOBFILE_PREAMBLE)
        
    with open(args.simdesc, mode='r') as file:
        csv_reader = csv.DictReader(file)
        for row in csv_reader:
            read_all_simpoints(row['workload_name'], row['home_dir'], row['runcmd'], row['simpoint_file'])


if __name__ == '__main__':
    main()
