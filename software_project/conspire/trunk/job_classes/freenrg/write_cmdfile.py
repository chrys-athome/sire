#!/bin/env python
# -*- coding: utf-8 -*-

import sys
import string

# get the config file as the first argument of the script
config_file = sys.argv[1]

# the name of the command file to write is the second argument
cmd_file = sys.argv[2]

def readConfig(file):
    lines = open(file, "r").readlines()

    params = {}

    for line in lines:
        line = line.lstrip().rstrip()

        if not line.startswith("#"):
            words = line.split("=")

            if len(words) > 1:
                key = words[0].lstrip().rstrip()
                value = string.join(words[1:], "=").lstrip().rstrip()

                params[key] = value

    return params

# read in all of the keys from this file
params = readConfig(config_file)

# ok - now we have all of the configuration options, let us write
# the Sire input file

def getBool(value):
    value = value.lower()
    if (value == "true" or value == "on" or value == "yes"):
        return "on"
    else:
        return "no"

def getAngstroms(value):
    words = value.split()
    return words[0]

def getCelsius(value):
    words = value.split()
    return words[0]

def getAtmosphere(value):
    words = value.split()
    return words[0]

def getCount(params, key):

    if key in params:
        return 1

    n = 1
    while True:
        numkey = "%s[%d]" % (key, n)
        if not numkey in params:
            return (n-1)
        else:
            n += 1

def writeConfig(file, params):
    print >>file,"# Sire config written from Conspire input"
    print >>file,"from Sire.Units import *"
    print >>file,"print \"Importing parameters from Conspire parameter file...\""

    print >>file,"top_file = \"%s\"" % params["input files.structure file"]
    print >>file,"crd_file = \"%s\"" % params["input files.coordinate file"]
    print >>file,"ligand_flex_file = \"%s\"" % params["input files.solute flexibility"]
    print >>file,"ligand_pert_file = \"%s\"" % params["input files.perturbation file"]
    print >>file,"protein_flex_file = \"%s\"" % params["input files.protein flexibility"]
    
    if "input files.protein z-matrices" in params:
        print >>file,"protein_zmatrices = \"%s\"" % params["input files.protein_zmatrices"]
    else:
        print >>file,"protein_zmatrices = \"default_amber.zmatrices\""

    if "naming scheme.solute residue names.flexibility name" in params:
        print >>file,"lig_name = \"%s\"" % params["naming scheme.solute residue names.flexibility name"]
    else:
        print >>file,"lig_name = \"MORPH\""

    print >>file,"restart_file = \"sim_restart.s3\""

    if "parameters.random seed" in params:
        print >>file,"random_seed = %s" % params["parameters.random seed"]
    else:
        print >>file,"random_seed = 0"

    print >>file,"nmoves = %s" % params["moves.number of moves"]

    if "moves.moves per energy" in params:
        print >>file,"nmoves_per_energy = %s" % params["moves.moves per energy"]
    else:
        print >>file,"nmoves_per_energy = nmoves"

    if "moves.moves per snapshot" in params:
        print >>file,"nmoves_per_pdb = %s" % params["moves.moves per snapshot"]
    else:
        print >>file,"nmoves_per_pdb = nmoves * 10"

    print >>file,"nmoves_per_pdb_intermediates = nmoves_per_pdb * 10"

    print >>file,"temperature = %s * celsius" % getCelsius(params["parameters.temperature"])

    if "parameters.pressure" in params:
        print >>file,"pressure = %s * atm" % getAtmosphere(params["parameters.pressure"])

    cutoff = float( getAngstroms(params["parameters.cutoff"]) )    

    print >>file,"coulomb_cutoff = %f * angstrom" % cutoff
    print >>file,"coulomb_feather = %f * angstrom" % (cutoff - 0.5)
    print >>file,"lj_cutoff = %f * angstrom" % cutoff
    print >>file,"lj_feather = %f * angstrom" % (cutoff - 0.5)

    print >>file,"rfdielectric = %s" % params["parameters.dielectric constant"]
    print >>file,"coulomb_power = %s" % params["parameters.soft-core.coulomb power"]
    print >>file,"shift_delta = %s" % params["parameters.soft-core.shift delta"]

    print >>file,"combining_rules = \"arithmetic\""
    print >>file,"pref_constant = %s * angstrom2" % params["moves.preferential constant"]
    print >>file,"max_solvent_translation = %s * angstrom" % \
                             getAngstroms(params["moves.maximum solvent translation"])
    print >>file,"max_solvent_rotation = %s * degrees" % params["moves.maximum solvent rotation"]

    print >>file,"water_sphere_radius = %s * angstrom" % getAngstroms(params["moves.water sphere radius"])

    print >>file,"water_mc_weight_factor = %s" % params["moves.move weights.solvent weight factor"]
    print >>file,"protein_mc_weight = %s" % params["moves.move weights.protein weight"]
    print >>file,"solute_mc_weight = %s" % params["moves.move weights.solute weight"]

    if "moves.move weights.volume weight" in params:
        print >>file,"volume_mc_weight = %s" % params["moves.move weights.volume weight"]
    elif "parameters.pressure" in params:
        print >>file,"volume_mc_weight = 1"

    print >>file,"delta_lambda = 0.001"
    print >>file,"compress = \"bzip2 -f \""

    num_names = getCount(params, "naming scheme.solute residue names.name")

    if num_names == 0:
        print >>file,"SOLUTE_RESNAMES = []"
    elif num_names == 1:
        print >>file,"SOLUTE_RESNAMES = [ \"%s\" ]" % params["naming scheme.solute residue names.name"]
    else:
        print >>file,"SOLUTE_RESNAMES = [",

        for i in range(1,num_names+1):
            print >>file," \"%s\"" % params["naming scheme.solute residue names.name[%d]" % i],
            if i != num_names:
                print >>file,", ",

        print >>file," ]"

    num_names = getCount(params, "naming scheme.protein residue names.name")

    if num_names == 0:
        print >>file,"PROTEIN_RESNAMES = []"
    elif num_names == 1:
        print >>file,"PROTEIN_RESNAMES = [ \"%s\" ]" % params["naming scheme.protein residue names.name"]
    else:
        print >>file,"PROTEIN_RESNAMES = [",
                             
        for i in range(1,num_names+1):
            print >>file," \"%s\"" % params["naming scheme.protein residue names.name[%d]" % i],
            if i != num_names:
                print >>file,", ",
    
        print >>file," ]"

    num_names = getCount(params, "naming scheme.solvent residue names.name")

    if num_names == 0:
        print >>file,"SOLVENT_RESNAMES = []"
    elif num_names == 1:
        print >>file,"SOLVENT_RESNAMES = [ \"%s\" ]" % params["naming scheme.solvent residue names.name"]
    else:
        print >>file,"SOLVENT_RESNAMES = [",

        for i in range(1,num_names+1):
            print >>file," \"%s\"" % params["naming scheme.solvent residue names.name[%d]" % i],
            if i != num_names:
                print >>file,", ",

        print >>file," ]"

    num_names = getCount(params, "naming scheme.ion residue names.name")

    if num_names == 0:
        print >>file,"ION_RESNAMES = []"
    elif num_names == 1:
        print >>file,"ION_RESNAMES = [ \"%s\" ]" % params["naming scheme.ion residue names.name"]
    else:
        print >>file,"ION_RESNAMES = [",

        for i in range(1,num_names+1):
            print >>file," \"%s\"" % params["naming scheme.ion residue names.name[%d]" % i],
            if i != num_names:
                print >>file,", ",

        print >>file," ]"

    num_lam = getCount(params, "lambda values.lambda")

    if num_lam == 0:
        print >>file,"lambda_values = [ 0 ]"
    elif num_lam == 1:
        print >>file,"lambda_values = [ %s ]" % params["lambda values.lambda"]
    else:
        print >>file,"lambda_values = [ %s" % params["lambda values.lambda[1]"],

        for i in range(2,num_lam+1):
            print >>file,", %s" % params["lambda values.lambda[%d]" % i],

        print >>file," ]"

FILE = open(cmd_file, "w")

writeConfig(FILE, params)

FILE.close()
