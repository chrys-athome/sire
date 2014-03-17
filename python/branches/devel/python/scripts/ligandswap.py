
from Sire.Tools import LSRC
from Sire.Tools import readParams

import Sire.Config

import argparse
import os
import sys

parser = argparse.ArgumentParser(description="Calculate relative binding free "
                                             "energies using ligandswap",
                                 epilog="ligandswap is built using Sire and is distributed "
                                        "under the GPL. For more information please visit "
                                        "http://siremol.org/ligandswap",
                                 prog="ligandswap")

parser.add_argument('-H', '--help-config', action="store_true",
                    help="Get additional help regarding all of the parameters "
                         "(and their default values) that can be "
                         "set in the optionally-supplied CONFIG file")

parser.add_argument('-l0', '--ligand0', nargs="?",
                    help="Supply the name of one of the residues in ligand 0 whose "
                         "binding free energy is to be calculated. By default, the ligand "
                         "will be the first non-protein, non-solvent molecule in the "
                         "input topology file. ligandswap calculates the relative binding "
                         "free energy of ligand0 and ligand1 (dG{ligand1} - dG{ligand0}).")

parser.add_argument('-l1', '--ligand1', nargs="?",
                    help="Supply the name of one of the residues in ligand 1 whose "
                         "binding free energy is to be calculated. By default, the ligand "
                         "will be the first non-protein, non-solvent molecule in the "
                         "input topology file. ligandswap calculates the relative binding "
                         "free energy of ligand0 and ligand1 (dG{ligand1} - dG{ligand0}).")

parser.add_argument('-t0', '--topology_file0', nargs="?",
                    help="The Amber topology file containing the solvated, equilbrated protein-ligand0 complex.")

parser.add_argument('-t1', '--topology_file1', nargs="?",
                    help="The Amber topology file containing the solvated, equilbrated protein-ligand1 complex.")

parser.add_argument('-c0', '--coordinate_file0', nargs="?",
                    help="The Amber coordinate file (with periodic box) giving the coordinates "
                         "of all of the atoms in the passed topology file of the protein-ligand0 complex.")

parser.add_argument('-c1', '--coordinate_file1', nargs="?",
                    help="The Amber coordinate file (with periodic box) giving the coordinates "
                         "of all of the atoms in the passed topology file of the protein-ligand1 complex.")

parser.add_argument('--lambda_values', type=float, nargs='+',
                    help='Lambda values for the windows used in the free energy calculation')

parser.add_argument('-C', '--config', nargs="?", 
                    help='Supply an optional CONFIG file to control the calculation.')

parser.add_argument('--author', action="store_true",
                    help="Get information about the authors of this script.")

parser.add_argument('--version', action="store_true",
                    help="Get version information about this script.")

sys.stdout.write("\n")
args = parser.parse_args()

must_exit = False

if args.author:
    print("\nligandswap was written by Christopher Woods (C) 2013")
    print("It is based on the LSRC module distributed in Sire.")
    must_exit = True

if args.version:
    print("\ligandswap version 0.1")
    print(Sire.Config.versionString())
    must_exit = True

if args.help_config:
    LSRC.Parameter.printAll(True)
    must_exit = True

if must_exit:
    sys.exit(0)

# If we have been given a CONFIG file, read it now
params = {}

if args.config:
    print("Loading configuration information from file %s" % args.config)
    params = readParams(args.config)

if args.coordinate_file0:
    coord_file0 = args.coordinate_file0
    params["crdfile0"] = coord_file0
elif "protein crdfile0" in params:
    coord_file0 = params["crdfile0"]
else:
    coord_file0 = "complex0.crd"
    params["crdfile0"] = coord_file0

if args.coordinate_file1:
    coord_file1 = args.coordinate_file1
    params["crdfile1"] = coord_file1
elif "protein crdfile1" in params:
    coord_file1 = params["crdfile1"]
else:
    coord_file1 = "complex1.crd"
    params["crdfile1"] = coord_file1

if args.topology_file0:
    top_file0 = args.topology_file0
    params["topfile0"] = top_file0
elif "topfile0" in params:
    top_file0 = params["topfile0"]
else:
    top_file0 = "complex0.top"
    params["topfile0"] = top_file0

if args.topology_file1:
    top_file1 = args.topology_file1
    params["topfile1"] = top_file1
elif "topfile1" in params:
    top_file1 = params["topfile1"]
else:
    top_file1 = "complex1.top"
    params["topfile1"] = top_file1

if not (os.path.exists(coord_file0) and os.path.exists(top_file0) and
        os.path.exists(coord_file1) and os.path.exists(top_file1)):
    parser.print_help()
    print("\nPlease supply the name of an existing topology and coordinate files.")
    if not os.path.exists(coord_file0):
        print("(cannot find coordinate file %s)" % coord_file0)
    if not os.path.exists(top_file0):
        print("(cannot find topology file %s)" % top_file0)
    if not os.path.exists(coord_file1):
        print("(cannot find coordinate file %s)" % coord_file1)
    if not os.path.exists(top_file1):
        print("(cannot find topology file %s)" % top_file1)

    sys.exit(-1)

ligand0 = None
if args.ligand0:
    ligand0 = args.ligand0
    params["ligand0"] = ligand0
elif "ligand0" in params:
    ligand0 = params["ligand0"]

if ligand0:
    print("Ligand 0 will be located by finding the first molecule containing residue %s" % ligand0)
    
else:
    print("Ligand 0 will be the first non-protein, non-solvent molecule found in system.")

ligand1 = None
if args.ligand1:
    ligand1 = args.ligand1
    params["ligand1"] = ligand1
elif "ligand1" in params:
    ligand1 = params["ligand1"]

if ligand1:
    print("Ligand 1 will be located by finding the first molecule containing residue %s" % ligand1)
    
else:
    print("Ligand 1 will be the first non-protein, non-solvent molecule found in system.")

print("\nRunning a ligandswap calculation calculating the difference in free energy between"
      "\nligands 0 and 1 using files %s|%s and %s|%s." % (top_file0,coord_file0,top_file1,coord_file1))

lambda_values = args.lambda_values

if lambda_values:
    print("Using lambda windows %s\n" % lambda_values)
    params["lambda values"] = lambda_values

# Now lets run the LSRC calculation
LSRC.run(params)
