#!/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import glob
import re
import tempfile
import shutil
import datetime

# directory containing the input files. This should also
# contain a file called "config" that is used to run the 
# simulation
try:
    rundir = sys.argv[1]
except:
    rundir = "."

try:
    ngpus = int(sys.argv[2])
except:
    ngpus = 1

os.chdir(rundir)

# redirect all standard output and error to "run_md.log" in this directory
sys.stdout = open("run_md.log", "w", 0)
sys.stderr = open("run_md.err", "w", 0)

# get the full, absolute path to the run directory
rundir = os.getcwd()

username = os.getenv("USER")

print "User %s is running a simulation in directory %s" % (username, rundir)
print datetime.datetime.now()

def readConfig(config_file):
    """Read the configuration file - this returns a dictionary 
       of key-value pairs"""
    lines = open(config_file, "r").readlines()

    d = {}

    for line in lines:
        line = line.strip()

        if line.startswith("#"):
            next

        parts = line.split("=")

        try:
            d[parts[0].strip()] = parts[1].strip()
        except:
            pass

    return d

options = readConfig("config")

top_file = options["topology file"]
restart_file = options["restart file"]
output_name = options["output name"]
nblocks = int(options["number of blocks"])
mdin_file = options["command file"]

def getLastIteration(output_name):
    """Find the last iteration of the simulation using the name 'output_name'.
       Each iteration produces a restart file that is named 'output_name_%dddddd.rst'.
       This function finds the last numbered file in the current directory"""

    rstfiles = glob.glob("%s_*.rst*" % output_name)

    # numbering convention means that the last file is alphabetically last
    rstfiles.sort()

    try:
        return int( re.search("%s_(\d+).rst" % output_name, rstfiles[-1]).groups()[0] )
    except:
        return 0


def getRestartName(output_name, iteration):
    """Return the name of the restart file for the iteration 'iteration'"""
    return "%s_%000006d.rst" % (output_name,iteration) 


def getOutputName(output_name, iteration):
    """Return the name of the mdout file for the iteration 'iteration'"""
    return "%s_%000006d.out" % (output_name,iteration)


def getTrajectoryName(output_name, iteration):
    """Return the name of the output trajectory for iteration 'iteration'"""
    return "%s_%000006d.traj" % (output_name,iteration)


# now find out how many iterations have been run
last_iteration = getLastIteration(output_name)

if last_iteration == 0:
   print "No previous iterations have been run... Starting from block 1..."

elif last_iteration >= nblocks:
   print "All requested blocks have finished! Simulation is complete!"
   sys.exit(0)

else:
   print "Continuing the simulation from iteration %d..." % last_iteration

   # update the restart file to be the one from the last iteration
   restart_file = getRestartName(output_name, last_iteration)

# now create a temporary directory based on the output name of the simulation
# and the name of the user
tempdir = tempfile.mkdtemp(suffix="_%s_%s_" % (username,output_name), \
                           dir="/tmp")

try:
    # Copy the topology file, restart file and mdin file to the temporary directory
    print "Copying input files to temporary run directory \"%s\"..." % tempdir

    try:
        shutil.copyfile(top_file, "%s/%s" % (tempdir,top_file))
    except:
        shutil.copyfile("%s.bz2" % top_file, "%s/%s.bz2" % (tempdir,top_file))
        top_file = "%s.bz2" % top_file

    try:
        shutil.copyfile(restart_file, "%s/%s" % (tempdir,restart_file))
    except:
        shutil.copyfile("%s.bz2" % restart_file, "%s/%s.bz2" % (tempdir,restart_file))
        restart_file = "%s.bz2" % restart_file

    shutil.copyfile(mdin_file, "%s/%s" % (tempdir,mdin_file))

    print "...all files copied :-)"

    # Change into the temp directory - writing files here should be quicker
    # than writing to the home directory
    os.chdir(tempdir)

    # Uncompress the files if needed
    if restart_file.endswith(".bz2"):
        os.system("bunzip2 %s" % restart_file)
        restart_file = restart_file[0:-4]

    if top_file.endswith(".bz2"):
        os.system("bunzip2 %s" % top_file)
        top_file = top_file[0:-4]

    # Now run the iterations one after another until the requested number
    # have been completed
    for i in range(last_iteration+1, nblocks+1):
        print "Running iteration %d of %d..." % (i, nblocks)
        print datetime.datetime.now()

        mdout = getOutputName(output_name, i)
        new_restart = getRestartName(output_name, i)
        traj = getTrajectoryName(output_name, i)

        if ngpus == 1:
            cmd = "$AMBERHOME/bin/pmemd.cuda_SPDP -O -i %s -o %s -p %s -c %s -ref %s -r %s -x %s -v mdvel" % \
                (mdin_file, mdout, top_file, restart_file, restart_file, \
                 new_restart, traj)

        else:
            cmd = "mpirun -1sided -np %d $AMBERHOME/bin/pmemd.cuda_SPDP.MPI -O -i %s -o %s -p %s -c %s -ref %s -r %s -x %s -v mdvel" % \
                (ngpus, mdin_file, mdout, top_file, restart_file, restart_file, \
       	       	 new_restart, traj)

        print "(command = \"%s\")" % cmd
        exitval = os.system(cmd)
        print "Finished :-)"
        print datetime.datetime.now()

        if exitval != 0:
            print "WARNING: SOMETHING WENT WRONG WITH THE SIMULATION"
            print "COPYING THE OUTPUT FILES TO filename_BROKEN"
            os.system("cp %s %s/%s_BROKEN" % (mdout, rundir, mdout))
            os.system("cp %s %s/%s_BROKEN" % (new_restart, rundir, new_restart))
            os.system("cp %s %s/%s_BROKEN" % (traj, rundir, traj))

            print "EXITING!!!"
            break

        # We don't need the old restart file any more
        os.system("rm %s &" % restart_file)

        # jump to the next restart file for the next iteration
        restart_file = new_restart

        # compress the output files to save space
        os.system("bzip2 %s %s" % (mdout, traj))
        os.system("bzip2 -k %s" % (new_restart))  # we still need the uncompressed version

        # copy the output files back to the shared directory
        copy_cmd = "cp %s.bz2 %s.bz2 %s.bz2 %s/" % (new_restart, mdout, traj, rundir)

        # (do this in the background unless this is the last iteration)
        if i == nblocks:
            os.system(copy_cmd)
        else:
            os.system("%s &" % copy_cmd)

    # Change back to the run directory and remove the temporary directory
    os.chdir(rundir)

    os.system("rm -rf %s" % tempdir)

    print "All iterations completed :-)"

except:
    print "SOMETHING WEIRD HAPPENED!!!"
    os.system("rm -rf %s" % tempdir)
