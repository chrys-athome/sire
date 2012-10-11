#!/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import glob
import re
import tempfile
import shutil
import datetime

try:
    ncpus = int(sys.argv[1])
except:
    ncpus = 1

try:
    ngpus = int(sys.argv[2])
except:
    ngpus = 0

# redirect all standard output and error to "run_md.log" in this directory
sys.stdout = open("run_md.log", "w", 0)
sys.stderr = open("run_md.err", "w", 0)

# get the full, absolute path to the run directory
rundir = os.getcwd()

username = os.getenv("USER")

print ("User %s is running a simulation in directory %s" % (username, rundir))
print (datetime.datetime.now())

def readConfig(config_file):
    """Read the configuration file - this returns a dictionary 
       of key-value pairs"""
    lines = open(config_file, "r").readlines()

    d = {}

    for line in lines:
        line = line.strip()

        if line.startswith("#"):
            continue

        parts = line.split("=")

        try:
            d[parts[0].strip()] = parts[1].strip()
        except:
            pass

    return d

def getOption(key, options, defaultVal=None):
    try:
        return options[key]
    except:
        return defaultVal

options = readConfig("config")

def getFemtoseconds(time):
    try:
        words = time.split()
        time = float(words[0])

        if len(words) > 1:
            units = words[1].lower()

            if units.find("pico") != -1 or units == "ps":
                return int( time * 1000 )

            elif units.find("nano") != -1 or units == "ns":
                return int( time * 1000000 )

            elif units.find("micro") != -1:
                return int( time * 1000000000 )

            else:
                return int(time)
    except:
        return None

def getKelvin(temperature):
    try:
        words = temperature.split()
        temperature = float(words[0])
        
        if len(words) > 1:
            units = words[1].lower()
            
            if units.find("c") != -1:
                #assume celsius
                return temperature + 273.15

            elif units.find("f") != -1:
                #assume fahrenheit
                return 273.15 + (5.0/9.0)*(temperature - 32)

            else:
                return temperature
        else:
            return temperature + 273.15

    except:
        return None

def getAtmosphere(pressure):
    try:
        words = pressure.split()
        pressure = float(words[0])

        if len(words) > 1:
            units = words[1].lower()

            if units.find("a") == -1:
                print "WARNING - PRESSURE UNITS NOT ATMOSPHERES????"

        return pressure
    except:
        return None

top_file = getOption("structure file", options, "molecules.top")
restart_file = getOption("coordinate file", options, "molecules.rst")
output_name = getOption("output name", options, "output")
nblocks = int( getOption("number of blocks", options, 1) )
mdin_file = getOption("command file", options)
length_dynamics = getFemtoseconds(getOption("length of dynamics", options, "1 ps"))

try:
    num_moves = int(length_dynamics / 2)
except:
    num_moves = 1

temperature = getKelvin(getOption("temperature", options, "25"))
pressure = getAtmosphere(getOption("pressure", options))

if not mdin_file:
    # we need to write the mdin file ourselves using the user's options
    mdin_file = "mdin"
    FILE = open(mdin_file, "w")

    if pressure:
        print >>FILE,"MD system -- Constant Temperature and Pressure"
    else:
        print >>FILE,"MD system -- Constant Temperature"

    print >>FILE,"&cntrl"
    print >>FILE," imin=0, irest=1, ntx=5, ntpr=10000, ntwr=0, ntwx=5000,"
    print >>FILE," nstlim=%d, dt=0.002," % num_moves
    print >>FILE," tempi=%.2f, temp0=%.2f, ntt=3, gamma_ln=5, ig=-1," % (temperature, temperature)
    print >>FILE," ntb=2, cut=10.0,"

    if pressure:
        print >>FILE," ntp=1, pres0=%.2f," % pressure

    print >>FILE," ntc=2, ntf=2,"  
    print >>FILE," iwrap=1,"
    print >>FILE,"&end"

    print >>FILE,"&ewald"
    print >>FILE,"  nfft1 = 128, nfft2 = 128, nfft3 = 128"
    print >>FILE,"&end"

    FILE.close()


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

# Uncompress the files if needed
if top_file.endswith(".bz2"):
    os.system("bunzip2 %s" % top_file)
    top_file = top_file[0:-4]

if top_file.endswith(".gz"):
    os.system("gunzip %s" % top_file)
    top_file = top_file[0:-3]

if top_file.endswith(".zip"):
    os.system("unzip %s" % top_file)
    top_file = top_file[0:-3]

if restart_file.endswith(".bz2"):
    os.system("bunzip2 %s" % restart_file)
    restart_file = restart_file[0:-4]

if restart_file.endswith(".gz"):
    os.system("gunzip %s" % restart_file)
    restart_file = restart_file[0:-3]

if restart_file.endswith(".zip"):
    os.system("unzip %s" % restart_file)
    restart_file = restart_file[0:-4]

if top_file.endswith(".bz2"):
    os.system("bunzip2 %s" % top_file)
    top_file = top_file[0:-4]

if top_file.endswith(".gz"):
    os.system("gunzip %s" % top_file)
    top_file = top_file[0:-3]

if top_file.endswith(".zip"):
    os.system("unzip %s"  % top_file)
    top_file = top_file[0:-4]

# Now run the iterations one after another until the requested number
# have been completed
try:
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

        elif ngpus > 1:
            cmd = "mpirun -1sided -np %d $AMBERHOME/bin/pmemd.cuda_SPDP.MPI -O -i %s -o %s -p %s -c %s -ref %s -r %s -x %s -v mdvel" % \
                (ngpus, mdin_file, mdout, top_file, restart_file, restart_file, \
                 new_restart, traj)

        elif ncpus == 1:
            cmd = "$AMBERHOME/bin/pmemd -O -i %s -o %s -p %s -c %s -ref %s -r %s -x %s -v mdvel" % \
                (mdin_file, mdout, top_file, restart_file, restart_file, \
                 new_restart, traj)

        else:
            cmd = "mpirun -np %d $AMBERHOME/bin/pmemd.MPI -O -i %s -o %s -p %s -c %s -ref %s -r %s -x %s -v mdvel" % \
                (ncpus, mdin_file, mdout, top_file, restart_file, restart_file, \
                 new_restart, traj)

        print "(command = \"%s\")" % cmd
        exitval = os.system(cmd)
        print "Finished :-)"
        print datetime.datetime.now()

        if exitval != 0:
            print "WARNING: SOMETHING WENT WRONG WITH THE SIMULATION"
            print "MOVING THE OUTPUT FILES TO filename_BROKEN"
            os.system("mv %s %s_BROKEN" % (mdout, mdout))
            os.system("mv %s %s_BROKEN" % (new_restart, new_restart))
            os.system("mv %s %s_BROKEN" % (traj, traj))

            print "EXITING!!!"
            break

        # compress the old restart file
        os.system("bzip2 %s &" % restart_file)

        # jump to the next restart file for the next iteration
        restart_file = new_restart

        # compress the output files to save space
        os.system("bzip2 %s %s" % (mdout, traj))
   
    # compress the last output restart file 
    os.system("bzip2 %s" % restart_file)

    print "All iterations completed :-)"

except:
    print "SOMETHING WEIRD HAPPENED!!!"
    print "PLEASE LOOK AT THE OUTPUT AND LET US KNOW WHAT WENT WRONG"
