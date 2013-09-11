#!/bin/sh

set NTHREADS = 8

# first, write the input file based on the options
# supplied by the user
python write_cmdfile.py config conspire_defined_parameters.py

# we now have to run the "run_jobs.py" script that manages all of the 
# different jobs for different lambda values
python run_jobs.py ${NTHREADS} > run_jobs.log
