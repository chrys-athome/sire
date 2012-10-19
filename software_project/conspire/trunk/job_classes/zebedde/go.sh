#!/bin/sh

# first, write the input file based on the options
# supplied by the user
python write_cmdfile.py config run.dat

# now run zebedde using the generated "run.dat" file
./zebedde run.dat > run.log
