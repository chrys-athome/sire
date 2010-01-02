
from Sire.IO import *
from Sire.Mol import *
from Sire.CAS import *
from Sire.System import *
from Sire.Move import *
from Sire.MM import *

import os

protomsdir = "%s/Work/ProtoMS" % os.getenv("HOME")

protoms = ProtoMS( "%s/protoms2" % protomsdir )

protoms.addParameterFile( "%s/parameter/amber99.ff" % protomsdir )
protoms.addParameterFile( "%s/parameter/solvents.ff" % protomsdir )
protoms.addParameterFile( "%s/parameter/gaff.ff" % protomsdir )
protoms.addParameterFile( "ethane2methanol.par" )

solute = PDB().readMolecule("ethane2methanol.pdb")
solute = solute.edit().rename("ethane2methanol").commit()
solute = protoms.parameterise(solute, ProtoMS.SOLUTE)

solute_fwd = solute.edit().renumber().commit()
solute_bwd = solute.edit().renumber().commit()

solvent = PDB().read("boxT4P.pdb")

tip4p = solvent.moleculeAt(0).molecule()
tip4p = tip4p.edit().rename("T4P").commit()
tip4p = protoms.parameterise(tip4p, ProtoMS.SOLVENT)

tip4p_chgs = tip4p.property("charge")
tip4p_ljs = tip4p.property("LJ")

for i in range(0,solvent.nMolecules()):
    tip4p = solvent.moleculeAt(i).molecule()
    tip4p = tip4p.edit().rename("T4P") \
                        .setProperty("charge", tip4p_chgs) \
                        .setProperty("LJ", tip4p_ljs) \
                        .commit()

    solvent.update(tip4p)

system = System()

solute = MoleculeGroup("solute", solute)
solute_fwd = MoleculeGroup("solute_fwd", solute_fwd)
solute_bwd = MoleculeGroup("solute_bwd", solute_bwd)

solutes = MoleculeGroup("solutes")
solutes.add(solute)
solutes.add(solute_fwd)
solutes.add(solute_bwd)

solvent = MoleculeGroup("solvent", solvent)

system.add(solute)
system.add(solute_fwd)
system.add(solute_bwd)
system.add(solutes)
system.add(solvent)

solventff = InterCLJFF("solvent:solvent")
solventff.add(solvent)

solute_intraff = InternalFF("solute_intraff")
solute_intraff.add(solute)

solute_fwd_intraff = InternalFF("solute_fwd_intraff")
solute_fwd_intraff.add(solute_fwd)

solute_bwd_intraff = InternalFF("solute_bwd_intraff")
solute_bwd_intraff.add(solute_bwd)

solute_intraclj = IntraCLJFF("solute_intraclj")
solute_intraclj.add(solute)

solute_fwd_intraclj = IntraCLJFF("solute_fwd_intraclj")
solute_fwd_intraclj.add(solute_fwd)

solute_bwd_intraclj = IntraCLJFF("solute_bwd_intraclj")
solute_bwd_intraclj.add(solute_bwd)

solute_solventff = InterGroupCLJFF("solute:solvent")
solute_solventff.add(solute, MGIdx(0))
solute_solventff.add(solvent, MGIdx(1))

solute_fwd_solventff = InterGroupCLJFF("solute_fwd:solvent")
solute_fwd_solventff.add(solute_fwd, MGIdx(0))
solute_fwd_solventff.add(solvent, MGIdx(1))

solute_bwd_solventff = InterGroupCLJFF("solute_bwd:solvent")
solute_bwd_solventff.add(solute_bwd, MGIdx(0))
solute_bwd_solventff.add(solvent, MGIdx(1))

forcefields = [ solventff, solute_intraff, solute_intraclj, solute_solventff,
                           solute_fwd_intraff, solute_fwd_intraclj, solute_fwd_solventff,
                           solute_bwd_intraff, solute_bwd_intraclj, solute_bwd_solventff ]

for forcefield in forcefields:
    system.add(forcefield)

e_total = system.totalComponent()
e_fwd = Symbol("E_{fwd}")
e_bwd = Symbol("E_{bwd}")

total_nrg = solventff.components().total() + \
            solute_intraff.components().total() + solute_intraff.components().total() + \
            solute_solventff.components().total()

fwd_nrg = solventff.components().total() + \
          solute_fwd_intraff.components().total() + solute_fwd_intraff.components().total() + \
          solute_fwd_solventff.components().total()

bwd_nrg = solventff.components().total() + \
          solute_bwd_intraff.components().total() + solute_bwd_intraff.components().total() + \
          solute_bwd_solventff.components().total()

system.setComponent( e_total, total_nrg )
system.setComponent( e_fwd, fwd_nrg )
system.setComponent( e_bwd, bwd_nrg )

lam = Symbol("lambda")
lam_fwd = Symbol("lambda_{fwd}")
lam_bwd = Symbol("lambda_{bwd}")

system.setConstant(lam, 0.0)
system.setConstant(lam_fwd, 0.0)
system.setConstant(lam_bwd, 0.0)

initial = Perturbation.symbols().initial()
final = Perturbation.symbols().final()

system.add( PerturbationConstraint(solute, lam*initial + (1-lam)*final) )
system.add( PerturbationConstraint(solute_fwd, lam_fwd*initial + (1-lam_fwd)*final) )
system.add( PerturbationConstraint(solute_bwd, lam_bwd*initial + (1-lam_bwd)*final) )

delta_lambda = 0.001

system.add( ComponentConstraint( lam_fwd, Min( lam + delta_lambda, 1 ) ) )
system.add( ComponentConstraint( lam_bwd, Max( lam - delta_lambda, 0 ) ) )

def printComponents(energies):
    """Use this function to print out the energies that are in 'energies'"""

    components = energies.symbols()

    components.sort( key=str )

    for component in components:
        print "%s == %f kcal mol-1" % (component, energies[component])

    print "\n",

printComponents( system.energies() )

