#!/bin/bash

PYTHONPATH=$PYTHONPATH:/home/rahbera/scarab/bin

echo "Running in $HOSTNAME..."
echo "Original cmd: python /home/rahbera/scarab/bin/gen_trace/gen_trace.py --fastfwd $1 --tracelen $2 --bmkname $3 --bmkdir $4 --bmkcmd \"$5\""

# Actual run
python /home/rahbera/scarab/bin/gen_trace/gen_trace.py --fastfwd $1 --tracelen $2 --bmkname $3 --bmkdir $4 --bmkcmd "$5"
