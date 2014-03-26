
from Sire.IO import *
from Sire.Mol import *
from Sire.Maths import *
from Sire.Base import *

import Sire.Config

import argparse
import os
import sys

parser = argparse.ArgumentParser(description="Align ligands/small molecules/fragments against each other.",
                                 epilog="align is built using Sire and is distributed "
                                        "under the GPL. For more information please visit "
                                        "http://siremol.org/align",
                                 prog="align")

parser.add_argument('-l0', '--ligand0', nargs=1,
                    help="Supply the name of the ligand in the first PDB file against "
                         "which the ligand in the second PDB file will be aligned.")

parser.add_argument('-l1', '--ligand1', nargs=1,
                    help="Supply the name of the ligand in the second PDB file that will "
                         "be aligned against the ligand in the first PDB file.")

parser.add_argument('-p0', '--pdb0', nargs=1,
                    help="Supply the PDB file containing the first ligand against which "
                         "the second ligand will be aligned.")

parser.add_argument('-p1', '--pdb1', nargs=1,
                    help="Supply the PDB file containing the second ligand, which will be "
                         "aligned against the first ligand.")

parser.add_argument('-o', '--output', nargs=1,
                    help="Name of the PDB file in which to output the aligned copy of the "
                         "second ligand.")

parser.add_argument('--author', action="store_true",
                    help="Get information about the authors of this script.")

parser.add_argument('--version', action="store_true",
                    help="Get version information about this script.")

sys.stdout.write("\n")
args = parser.parse_args()

must_exit = False

if args.author:
    print("\nalign was written by Christopher Woods (C) 2014")
    print("It is based on the Molecule::move().align() function distributed in Sire.")
    must_exit = True

if args.version:
    print("\nalign version 0.1")
    must_exit = True

if must_exit:
    sys.exit(0)

pdb0 = args.pdb0
pdb1 = args.pdb1
ligname0 = args.ligand0
ligname1 = args.ligand1
outfile = args.output

if pdb0 is None or pdb1 is None or ligname0 is None or ligname1 is None or outfile is None:
    parser.print_help()
    sys.exit(0)

pdb0 = args.pdb0[0]
pdb1 = args.pdb1[0]
ligname0 = args.ligand0[0]
ligname1 = args.ligand1[0]
outfile = args.output[0]

print("\nAligning ligand %s from PDB file %s against ligand %s in PDB file %s." % \
            (ligname1,pdb1,ligname0,pdb0))
print("Aligned coordinates of ligand %s will be written to file %s." % (ligname1,outfile))

mols0 = PDB().read(pdb0)
mols1 = PDB().read(pdb1)

def getResidueNames(molecule):
    nres = molecule.nResidues()

    resnams = []

    for i in range(0, nres):
        resnams.append( str( molecule.residue(ResIdx(i)).name().value()).upper() )

    return resnams

def findMolecule(molecules, molname):
    molname = molname.upper()

    for molnum in molecules.molNums():
        molecule = molecules[molnum].molecule()

        if str(molecule.name().value()).upper() == molname:
            return molecule

        resnams = getResidueNames(molecule)

        for resnam in resnams:
            if resnam == molname:
                return molecule

    return None

lig0 = findMolecule(mols0, ligname0)
lig1 = findMolecule(mols1, ligname1)

can_align = True

if lig0 is None:
    print("\nWARNING: Cannot find the first ligand (%s) in PDB file %s." % (ligname0,pdb0))
    can_align = False

if lig1 is None:
    print("\nWARNING: Cannot find the second ligand (%s) in PDB file %s." % (ligname1,pdb1))
    can_align = False

if not can_align:
    print("Cannot align the molecule! Exiting...")
    sys.exit(-1)

print("Looking for the maximum common substructure of the two ligands...")
atommap = AtomMCSMatcher().match(lig0, PropertyMap(), lig1, PropertyMap())

print("Number of matched atoms == %s" % len(atommap))

print("Mapping from the first to second ligand: %s" % atommap)

print("\nRMSD before alignment == %s A" % lig0.evaluate().rmsd(lig1, AtomResultMatcher(atommap)))

print("\nAligning the molecule...")
lig1 = lig1.move().align(lig0, AtomMCSMatcher()).commit()

print("\nRMSD after alignment == %s A" % lig0.evaluate().rmsd(lig1, AtomResultMatcher(atommap)))

PDB().write(lig1, outfile)
