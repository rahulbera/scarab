import yaml
import argparse
import re
import sys
import os

sys.path.append(os.path.dirname(os.path.dirname(__file__)))
from scarab_globals import *


class Trace:
    def __init__(self, name, path, workload, category, subcategory):
        self.name = name
        self.path = path
        self.workload = workload
        self.category = category
        self.subcategory = subcategory


class Experiment:
    def __init__(self, name, params, pintool_args, scarab_args):
        self.name = name
        self.params = params
        self.pintool_args = pintool_args
        self.scarab_args = scarab_args


def load_yaml(file_path):
    with open(file_path, "r") as file:
        return yaml.safe_load(file)


def replace_variables(value, definitions):
    pattern = r"\$\((.*?)\)"
    matches = re.findall(pattern, value)
    for match in matches:
        if match not in definitions:
            sys.exit(f"Encountered undefined variable: {match}")
        value = value.replace(f"$({match})", definitions[match])
    return value


def create_traces(data):
    trace_list = []
    for suite, traces in data.items():
        for entry in traces:
            for name, info in entry.items():
                path = info.get("path")
                workload = info.get("workload")
                category = info.get("category")
                subcategory = info.get("subcategory")
                trace_list.append(Trace(name, path, workload, category, subcategory))
    return trace_list


def create_experiments(data):
    definitions = {
        list(d.keys())[0]: list(d.values())[0] for d in data.get("definitions", [])
    }
    experiments = []

    for exp in data.get("experiments", []):
        for name, details in exp.items():
            params = details.get("params", "")
            pintool_args = details.get("pintool_args", "")
            scarab_args = details.get("scarab_args", "")

            params = replace_variables(params, definitions)
            pintool_args = replace_variables(pintool_args, definitions)
            scarab_args = replace_variables(scarab_args, definitions)

            experiments.append(Experiment(name, params, pintool_args, scarab_args))

    return experiments


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
    "sbatch -p SLURM_PART --mincpus=1 -c 1 "
    "-J JOBNAME "
    "-o JOBNAME.out "
    "-e JOBNAME.err "
)

JOB_DESC_TEMPLATE = (
    "SCARAB_HOME/bin/scarab_launch.py "
    "--frontend=trace --trace=TRACE "
    "--params=PRM "
    "--simdir=SIMDIR "
    '--pintool_args="PINTOOL_ARGS" '
    '--scarab_args="SCARAB_ARGS" '
)


def main():
    parser = argparse.ArgumentParser(
        description="Process trace and experiment YAML files."
    )
    parser.add_argument(
        "--tlist", required=True, help="Path to the input trace YAML file"
    )
    parser.add_argument(
        "--exp", required=True, help="Path to the input experiment YAML file"
    )
    parser.add_argument(
        "--slurm_part",
        required=False,
        default="cpu_part",
        help="Slurm partition to run on",
    )

    args = parser.parse_args()

    trace_data = load_yaml(args.tlist)
    exp_data = load_yaml(args.exp)

    traces = create_traces(trace_data)
    experiments = create_experiments(exp_data)

    print(JOBFILE_PREAMBLE)
    print("# Traces:")
    for trace in traces:
        print("#\t{}".format(trace.name))
    print("#\n#\n#")
    print("# Experiments:")
    for exp in experiments:
        print(
            "#\t{}: params={}, pintool_args=\"{}\", scarab_args=\"{}\"".format(
                exp.name, exp.params, exp.pintool_args, exp.scarab_args
            )
        )
    print("#\n#\n#")
    
    for exp in experiments:
        for trace in traces:
            exp_trace_combo = f"{trace.name}.{exp.name}"
            cmd = f"{SLURM_PREAMBLE} {JOB_DESC_TEMPLATE}"
            cmd = cmd.replace("SLURM_PART", args.slurm_part)
            cmd = cmd.replace("JOBNAME", exp_trace_combo)
            cmd = cmd.replace("SCARAB_HOME", scarab_paths.sim_dir)
            cmd = cmd.replace("TRACE", trace.path)
            cmd = cmd.replace("SIMDIR", exp_trace_combo)
            cmd = cmd.replace("PRM", exp.params)
            cmd = cmd.replace("PINTOOL_ARGS", exp.pintool_args)
            cmd = cmd.replace("SCARAB_ARGS", exp.scarab_args)
            print(cmd)


if __name__ == "__main__":
    main()