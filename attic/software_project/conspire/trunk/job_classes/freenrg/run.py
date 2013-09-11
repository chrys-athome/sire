import os,re, sys, shutil

from Sire.IO import *
from Sire.Mol import *
from Sire.CAS import *
from Sire.System import *
from Sire.Move import *
from Sire.MM import *
from Sire.FF import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Maths import *
from Sire.Base import *
from Sire.Qt import *
from Sire.ID import *
from Sire.Config import *

import Sire.Stream

# import all parameters from the "conspire_defined_parameters.py" file
from conspire_defined_parameters import *

####### FUNCTIONS  ###############
def readProteinFlexibility(protein_flex_file):

    FILE = open(protein_flex_file,'r')
    buffer = FILE.readlines()

    sc_flex = []
    bb_flex = []

    scmode = False
    bbmode = False

    for line in buffer:
        if line.startswith("#"):
            continue
        if line.startswith("flexible sidechain"):
            scmode = True
            continue
        if line.startswith("flexible backbone"):
            scmode = False
            bbmode = True
            continue

        elems = line.split()

        for elem in elems:
            res_number = ResNum( int(elem) )
            if scmode:
                sc_flex.append(res_number)
            elif bbmode:
                bb_flex.append(res_number)

    return sc_flex, bb_flex

def createBBMoveGroup(protein, bbgroup, flex_list):

    hn_atoms = AtomName("N", CaseInsensitive) * \
               AtomName("HN", CaseInsensitive) * AtomName("HN1", CaseInsensitive) * \
               AtomName("HN2", CaseInsensitive) * AtomName("HN3", CaseInsensitive)

    for i in range(0, protein.nResidues()):
        residue = protein.select(ResIdx(i))
        # Create only if mentioned as flexible
        if (not residue.number() in flex_list):
            continue
        atoms = protein.select(ResIdx(i)).selection()

        if i < (protein.nResidues()-1):
           try:
                atoms.deselect( hn_atoms + ResIdx(i) )
           except:
               pass

        if i > 0:
           try:
               atoms.select( hn_atoms + ResIdx(i-1) )
           except:
               pass

        bbgroup.add( PartialMolecule(protein, atoms) )

    #return bbgroup

def adjustPerturbedDOFs( molecule ):
    
    perturbations = molecule.property("perturbations").perturbations()
    
    r0 = Symbol("r0")
    theta0 = Symbol("theta0")

    for pert in perturbations:
        if ( pert.what() == 'SireMM::TwoAtomPerturbation'):
            ri = pert.initialForms()[r0].toString().toDouble()
            rf = pert.finalForms()[r0].toString().toDouble()
            if (abs(ri-rf) > 0.001):
                #rint ri,rf
                r = (1-lam_val) * ri + lam_val * rf
                r = r * angstrom
                bond = BondID(pert.atom0(), pert.atom1() )
                mover = molecule.move()
                try:
                    mover.set(bond, r)
                except UserWarning as error:
                    # extract the type of the errror
                    error_type = re.search(r"(Sire\w*::\w*)", str(error)).group(0)
                    if error_type == "SireMol::ring_error":
                        continue
                molecule = mover.commit()
        elif ( pert.what() == 'SireMM::ThreeAtomPerturbation'):
            thetai = pert.initialForms()[theta0].toString().toDouble()
            thetaf = pert.finalForms()[theta0].toString().toDouble()
            if (abs(thetai-thetaf) > 0.001):
                #rint thetai,thetaf
                theta = (1-lam_val) * thetai + lam_val * thetaf
                theta = theta * radians
                angle = AngleID(pert.atom0(), pert.atom1(), pert.atom2() )
                mover = molecule.move()
                try:                
                    mover.set(angle, theta)
                except UserWarning as error:
                    # extract the type of the errror
                    error_type = re.search(r"(Sire\w*::\w*)", str(error)).group(0)
                    if error_type == "SireMol::ring_error":
                        continue
                molecule = mover.commit()
    return molecule

def getDummies(molecule):
    print "Selecting dummy groups"
    natoms = molecule.nAtoms()
    atoms = molecule.atoms()

    from_dummies = None
    to_dummies = None

    for x in range(0,natoms):
        atom = atoms[x]
        if atom.property("initial_ambertype") == "du":
            if from_dummies is None:
                from_dummies = molecule.selectAll( atom.index() )
            else:
                from_dummies += molecule.selectAll( atom.index() )
        elif atom.property("final_ambertype") == "du":
            if to_dummies is None:
                to_dummies = molecule.selectAll( atom.index() )
            else:
                to_dummies += molecule.selectAll( atom.index() )
    
    return to_dummies, from_dummies


def createSystem(molecules):
    print "Applying flexibility and zmatrix templates..."

    moleculeNumbers = molecules.molNums()
    moleculeNumbers.sort()
    moleculeList = []

    for moleculeNumber in moleculeNumbers:
        molecule = molecules.molecule(moleculeNumber).molecule()
        moleculeList.append(molecule)

    # Create molecule groups and system

    system = System()

    all = MoleculeGroup("all")

    solutes = MoleculeGroup("solutes")
    perturbed_solutes = MoleculeGroup("perturbed_solutes")    

    protein = MoleculeGroup("protein")
    bbgroup = MoleculeGroup("bbresidues")
    residues = MoleculeGroup("residues")

    solvent = MoleculeGroup("solvent")
    water = MoleculeGroup( "water")
    ion = MoleculeGroup( "ion")

    # We will need those to set properties
    zmat_maker = ZmatrixMaker()
    zmat_maker.loadTemplates( protein_zmatrices )
    
    flexibility_lib = FlexibilityLibrary(ligand_flex_file)
    perturbations_lib = PerturbationsLibrary(ligand_pert_file)

    # Read the list of flexible sidechains and backbones
    sc_flex , bb_flex = readProteinFlexibility(protein_flex_file)

    for molecule in moleculeList:
        if molecule.residues()[0].name().value() in SOLUTE_RESNAMES:
            #
            # SOLUTE SETUP
            #
            if not solutes.isEmpty():
                print "This simulation script only supports one solute molecule."
                sys.exit(-1)
            # Solute...
            solute = molecule
            # If ligname has not been defined, and there is a single residue,
            # use the residue name
            global lig_name
            # print lig_name
            try:
                lig_name
            except NameError:
                if ( solute.nResidues() == 1 ):
                    lig_name = solute.residue( ResIdx(0) ).name().value()
                else:
                    lig_name = "ligand" # Likely not good...
            # print lig_name
            solute = solute.edit().rename(lig_name).commit()
            # print solute
            # This will add the property "flexibility" to the solute

            flexibility = flexibility_lib.getFlexibility(solute)
            solute = solute.edit().setProperty("flexibility", flexibility).commit()

            solute = perturbations_lib.applyTemplate(solute)

            perturbations = solute.property("perturbations")

            # print lam_val
            lam = Symbol("lambda")
            lam_fwd = Symbol("lambda_{fwd}")
            lam_bwd = Symbol("lambda_{bwd}")
    
            initial = Perturbation.symbols().initial()
            final = Perturbation.symbols().final()

            solute = solute.edit().setProperty("perturbations",
                       perturbations.recreate( (1-lam)*initial + lam*final ) ).commit()

            # Set the geometry of perturbed bonds/angles to match the corresponding equilibrium value 
            solute = adjustPerturbedDOFs( solute )

            solute_fwd = solute.edit().renumber().setProperty("perturbations",
                            perturbations.substitute( lam, lam_fwd ) ).commit()
            solute_bwd = solute.edit().renumber().setProperty("perturbations",
                            perturbations.substitute( lam, lam_bwd ) ).commit()

            # print solute
            # print solute_fwd
            # print solute_bwd

            # We put atoms in three groups depending on what happens in the perturbation
            # non dummy to non dummy --> the hard group, uses a normal intermolecular FF
            # non dummy to dummy --> the todummy group, uses SoftFF with alpha = Lambda
            # dummy to non dummy --> the fromdummy group, uses SoftFF with alpha = 1 - Lambda
            # We start assuming all atoms are hard atoms. Then we call getDummies to find which atoms 
            # start/end as dummies and update the hard, todummy and fromdummy groups accordingly

            solute_grp_ref = MoleculeGroup("solute_ref", solute)
            solute_grp_ref_hard = MoleculeGroup("solute_ref_hard")
            solute_grp_ref_todummy = MoleculeGroup("solute_ref_todummy")
            solute_grp_ref_fromdummy = MoleculeGroup("solute_ref_fromdummy")

            solute_grp_fwd = MoleculeGroup("solute_fwd", solute_fwd)
            solute_grp_fwd_hard = MoleculeGroup("solute_fwd_hard")
            solute_grp_fwd_todummy = MoleculeGroup("solute_fwd_todummy")
            solute_grp_fwd_fromdummy = MoleculeGroup("solute_fwd_fromdummy")

            solute_grp_bwd = MoleculeGroup("solute_bwd", solute_bwd)
            solute_grp_bwd_hard = MoleculeGroup("solute_bwd_hard")
            solute_grp_bwd_todummy = MoleculeGroup("solute_bwd_todummy")
            solute_grp_bwd_fromdummy = MoleculeGroup("solute_bwd_fromdummy")
    
            solute_ref_hard = solute.selectAllAtoms()
            solute_ref_todummy = solute_ref_hard.invert()
            solute_ref_fromdummy = solute_ref_hard.invert()
            solute_fwd_hard = solute_fwd.selectAllAtoms()
            solute_fwd_todummy = solute_fwd_hard.invert()
            solute_fwd_fromdummy = solute_fwd_hard.invert()
            solute_bwd_hard = solute_bwd.selectAllAtoms()
            solute_bwd_todummy = solute_bwd_hard.invert()
            solute_bwd_fromdummy = solute_bwd_hard.invert()

            to_dummies, from_dummies = getDummies(solute)

            # print to_dummies
            # print from_dummies

            if to_dummies is not None:
                ndummies = to_dummies.count()
                dummies = to_dummies.atoms()

                for x in range(0,ndummies):
                    dummy_index = dummies[x].index()
                    solute_ref_hard = solute_ref_hard.subtract( solute.select( dummy_index ) )
                    solute_fwd_hard = solute_fwd_hard.subtract( solute_fwd.select( dummy_index ) )
                    solute_bwd_hard = solute_bwd_hard.subtract( solute_bwd.select( dummy_index ) )
                    solute_ref_todummy = solute_ref_todummy.add( solute.select( dummy_index ) )
                    solute_fwd_todummy = solute_fwd_todummy.add( solute_fwd.select( dummy_index ) )
                    solute_bwd_todummy = solute_bwd_todummy.add( solute_bwd.select( dummy_index ) )

            if from_dummies is not None:
                ndummies = from_dummies.count()
                dummies = from_dummies.atoms()

                for x in range(0,ndummies):
                    dummy_index = dummies[x].index()
                    solute_ref_hard = solute_ref_hard.subtract( solute.select( dummy_index ) )
                    solute_fwd_hard = solute_fwd_hard.subtract( solute_fwd.select( dummy_index ) )
                    solute_bwd_hard = solute_bwd_hard.subtract( solute_bwd.select( dummy_index ) )
                    solute_ref_fromdummy = solute_ref_fromdummy.add( solute.select( dummy_index ) )
                    solute_fwd_fromdummy = solute_fwd_fromdummy.add( solute_fwd.select( dummy_index ) )
                    solute_bwd_fromdummy = solute_bwd_fromdummy.add( solute_bwd.select( dummy_index ) )

            solute_grp_ref_hard.add(solute_ref_hard)
            solute_grp_fwd_hard.add(solute_fwd_hard)
            solute_grp_bwd_hard.add(solute_bwd_hard)
            
            solute_grp_ref_todummy.add(solute_ref_todummy)
            solute_grp_fwd_todummy.add(solute_fwd_todummy)
            solute_grp_bwd_todummy.add(solute_bwd_todummy)

            solute_grp_ref_fromdummy.add(solute_ref_fromdummy)
            solute_grp_fwd_fromdummy.add(solute_fwd_fromdummy)
            solute_grp_bwd_fromdummy.add(solute_bwd_fromdummy)
 
            solutes.add(solute)
            solutes.add(solute_fwd)
            solutes.add(solute_bwd)

            perturbed_solutes.add(solute_fwd)
            perturbed_solutes.add(solute_bwd)

        elif molecule.residues()[0].name().value() in PROTEIN_RESNAMES:
            # This will add the property "z-matrix" to the protein

            molecule = zmat_maker.applyTemplates( molecule )

            # Update the MoleculeGroup used to perform backbone moves
            # on the protein
            createBBMoveGroup(molecule, bbgroup, bb_flex)

            for i in range(0, molecule.nResidues()):
                res = molecule.residue( ResIdx(i) )
                # Skip residues that have not been flagged as flexible
                if ( not res.number() in sc_flex ):
                    continue
                residues.add(res)

            protein.add(molecule)

        elif molecule.residues()[0].name().value() in SOLVENT_RESNAMES:
            # Separate water from ions
            water.add(molecule)
            solvent.add(molecule)
        elif molecule.residues()[0].name().value() in IONS_RESNAMES:
            ion.add(molecule)
            solvent.add(molecule)
        else:
            print "Oops, script does not know in what kind of molecule this residue is %s " % (molecule.residues()[0].name().value())
            sys.exit(-1)

    ligand = solutes[MolIdx(0)].molecule()
    
    sphere_center = ligand.evaluate().center()

    # Select mobile/fix water
    water_molnums = water.molecules().molNums()

    mobilewater = MoleculeGroup("mobilewater")
    fixwater = MoleculeGroup("fixwater")

    for water_molnum in water_molnums:
        wat = water.molecule(water_molnum).molecule()

        if Vector.distance(sphere_center, wat.evaluate().center() ) < water_sphere_radius.value():
            mobilewater.add(wat)
        else:
            fixwater.add(wat)

    print "The number of mobile water molecules is %s " % mobilewater.nMolecules()
    #print "Fix wat %s " % fixwater.nMolecules()
    #sys.exit(-1)

    # Select mobile/fix ions
    ions_molnums = ion.molecules().molNums()
    mobileion = MoleculeGroup("mobileion")
    fixion = MoleculeGroup("fixion")
    
    for ion_molnum in ions_molnums:
        io = ion.molecule(ion_molnum).molecule()
        if Vector.distance(sphere_center, io.evaluate().center() ) < water_sphere_radius.value():
            mobileion.add(io)
        else:
            fixion.add(io)

    #print "Mobile ion %s " % mobileion.nMolecules()
    #print "Fix ion %s " % fixion.nMolecules()

    all.add(solutes)
    
    all.add(perturbed_solutes)

    all.add(solute_grp_ref)
    all.add(solute_grp_fwd)
    all.add(solute_grp_bwd)

    all.add(solute_grp_ref_hard)
    all.add(solute_grp_ref_todummy)
    all.add(solute_grp_ref_fromdummy)

    all.add(solute_grp_fwd_hard)
    all.add(solute_grp_fwd_todummy)
    all.add(solute_grp_fwd_fromdummy)

    all.add(solute_grp_bwd_hard)
    all.add(solute_grp_bwd_todummy)
    all.add(solute_grp_bwd_fromdummy)

    all.add(solvent)
    all.add(water)
    all.add(ion) 

    all.add(protein)

    # NOT SURE NEED TO ADD REDUNDANT GRPS TO ALL...

    traj = MoleculeGroup("traj")
    traj.add(solute)
    traj.add(protein)
    traj.add(solvent)

    # Add these groups to the System
    system.add(solutes)

    system.add(perturbed_solutes)

    system.add(solute_grp_ref)
    system.add(solute_grp_fwd)
    system.add(solute_grp_bwd)

    system.add(solute_grp_ref_hard)
    system.add(solute_grp_ref_todummy)
    system.add(solute_grp_ref_fromdummy)

    system.add(solute_grp_fwd_hard)
    system.add(solute_grp_fwd_todummy)
    system.add(solute_grp_fwd_fromdummy)

    system.add(solute_grp_bwd_hard)
    system.add(solute_grp_bwd_todummy)
    system.add(solute_grp_bwd_fromdummy)

    system.add(solvent)
    system.add(water)
    system.add(ion)

    system.add(mobilewater)
    system.add(fixwater)

    system.add(mobileion)
    system.add(fixion)
    
    system.add(protein)
    system.add(residues)
    system.add(bbgroup)

    system.add(all)

    system.add(traj)

    return system

def setupForcefields(system, space):

    print "Creating force fields... "

    solutes = system[ MGName("solutes") ]

    solute = system[ MGName("solute_ref") ]
    solute_hard = system[ MGName("solute_ref_hard") ]
    solute_todummy = system[ MGName("solute_ref_todummy") ]
    solute_fromdummy = system[ MGName("solute_ref_fromdummy") ]

    solute_fwd = system[ MGName("solute_fwd") ]
    solute_fwd_hard = system[ MGName("solute_fwd_hard") ]
    solute_fwd_todummy = system[ MGName("solute_fwd_todummy") ]
    solute_fwd_fromdummy = system[ MGName("solute_fwd_fromdummy") ]

    solute_bwd = system[ MGName("solute_bwd") ]
    solute_bwd_hard = system[ MGName("solute_bwd_hard") ]
    solute_bwd_todummy = system[ MGName("solute_bwd_todummy") ]
    solute_bwd_fromdummy = system[ MGName("solute_bwd_fromdummy") ]

    solvent = system[ MGName("solvent") ]

    protein = system[ MGName("protein") ]

    residues = system[ MGName("residues") ]

    all = system[ MGName("all") ]

    # - first solvent-solvent coulomb/LJ (CLJ) energy
    solventff = InterCLJFF("solvent:solvent")
    solventff.add(solvent)
    #solventff.setUseReactionField(True)
    #solventff.setReactionFieldDielectric(rfdielectric)

    # The protein bond, angle, dihedral energy
    protein_intraff = InternalFF("protein_intraff")
    protein_intraff.add(protein)

    # The protein intramolecular CLJ energy
    protein_intraclj = IntraCLJFF("protein_intraclj")
    protein_intraclj.add(protein)
    #protein_intraclj.setUseReactionField(True)
    #protein_intraclj.setReactionFieldDielectric(rfdielectric)

    # The protein-solvent energy 
    protein_solventff = InterGroupCLJFF("protein:solvent")
    protein_solventff.add(protein, MGIdx(0))
    protein_solventff.add(solvent, MGIdx(1))
    #protein_solventff.setUseReactionField(True)
    #protein_solventff.setReactionFieldDielectric(rfdielectric)

    # Now solute bond, angle, dihedral energy
    solute_intraff = InternalFF("solute_intraff")
    solute_intraff.add(solute)
    
    solute_fwd_intraff = InternalFF("solute_fwd_intraff")
    solute_fwd_intraff.add(solute_fwd)

    solute_bwd_intraff = InternalFF("solute_bwd_intraff")
    solute_bwd_intraff.add(solute_bwd)

    # Now solute intramolecular CLJ energy
    solute_hard_intraclj = IntraCLJFF("solute_hard_intraclj")
    solute_hard_intraclj.add(solute_hard)
    #solute_hard_intraclj.setUseReactionField(True)
    #solute_hard_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_todummy_intraclj = IntraSoftCLJFF("solute_todummy_intraclj")
    solute_todummy_intraclj.add(solute_todummy)
    #solute_todummy_intraclj.setUseReactionField(True)
    #solute_todummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_fromdummy_intraclj = IntraSoftCLJFF("solute_fromdummy_intraclj")
    solute_fromdummy_intraclj.add(solute_fromdummy)
    #solute_fromdummy_intraclj.setUseReactionField(True)
    #solute_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_hard_todummy_intraclj = IntraGroupSoftCLJFF("solute_hard:todummy_intraclj")
    solute_hard_todummy_intraclj.add(solute_hard, MGIdx(0))
    solute_hard_todummy_intraclj.add(solute_todummy, MGIdx(1))
    #solute_hard_todummy_intraclj.setUseReactionField(True)
    #solute_hard_todummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_hard_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_hard:fromdummy_intraclj")
    solute_hard_fromdummy_intraclj.add(solute_hard, MGIdx(0))
    solute_hard_fromdummy_intraclj.add(solute_fromdummy, MGIdx(1))
    #solute_hard_fromdummy_intraclj.setUseReactionField(True)
    #solute_hard_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_todummy_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_todummy:fromdummy_intraclj")
    solute_todummy_fromdummy_intraclj.add(solute_todummy, MGIdx(0))
    solute_todummy_fromdummy_intraclj.add(solute_fromdummy, MGIdx(1))
    #solute_todummy_fromdummy_intraclj.setUseReactionField(True)
    #solute_todummy_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    # The forwards intramolecular CLJ energy

    solute_fwd_hard_intraclj = IntraCLJFF("solute_fwd_hard_intraclj")
    solute_fwd_hard_intraclj.add(solute_fwd_hard)
    #solute_fwd_hard_intraclj.setUseReactionField(True)
    #solute_fwd_hard_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_fwd_todummy_intraclj = IntraSoftCLJFF("solute_fwd_todummy_intraclj")
    solute_fwd_todummy_intraclj.add(solute_fwd_todummy)
    #solute_fwd_todummy_intraclj.setUseReactionField(True)
    #solute_fwd_todummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_fwd_fromdummy_intraclj = IntraSoftCLJFF("solute_fwd_fromdummy_intraclj")
    solute_fwd_fromdummy_intraclj.add(solute_fwd_fromdummy)
    #solute_fwd_fromdummy_intraclj.setUseReactionField(True)
    #solute_fwd_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_fwd_hard_todummy_intraclj = IntraGroupSoftCLJFF("solute_fwd_hard:todummy_intraclj")
    solute_fwd_hard_todummy_intraclj.add(solute_fwd_hard, MGIdx(0))
    solute_fwd_hard_todummy_intraclj.add(solute_fwd_todummy, MGIdx(1))
    #solute_fwd_hard_todummy_intraclj.setUseReactionField(True)
    #solute_fwd_hard_todummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_fwd_hard_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_fwd_hard:fromdummy_intraclj")
    solute_fwd_hard_fromdummy_intraclj.add(solute_fwd_hard, MGIdx(0))
    solute_fwd_hard_fromdummy_intraclj.add(solute_fwd_fromdummy, MGIdx(1))
    #solute_fwd_hard_fromdummy_intraclj.setUseReactionField(True)
    #solute_fwd_hard_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_fwd_todummy_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_fwd_todummy:fromdummy_intraclj")
    solute_fwd_todummy_fromdummy_intraclj.add(solute_fwd_todummy, MGIdx(0))
    solute_fwd_todummy_fromdummy_intraclj.add(solute_fwd_fromdummy, MGIdx(1))
    #solute_fwd_todummy_fromdummy_intraclj.setUseReactionField(True)
    #solute_fwd_todummy_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    # The backwards intramolecular CLJ energy

    solute_bwd_hard_intraclj = IntraCLJFF("solute_bwd_hard_intraclj")
    solute_bwd_hard_intraclj.add(solute_bwd_hard)
    #solute_bwd_hard_intraclj.setUseReactionField(True)
    #solute_bwd_hard_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_bwd_todummy_intraclj = IntraSoftCLJFF("solute_bwd_todummy_intraclj")
    solute_bwd_todummy_intraclj.add(solute_bwd_todummy)
    #solute_bwd_todummy_intraclj.setUseReactionField(True)
    #solute_bwd_todummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_bwd_fromdummy_intraclj = IntraSoftCLJFF("solute_bwd_fromdummy_intraclj")
    solute_bwd_fromdummy_intraclj.add(solute_bwd_fromdummy)
    #solute_bwd_fromdummy_intraclj.setUseReactionField(True)
    #solute_bwd_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_bwd_hard_todummy_intraclj = IntraGroupSoftCLJFF("solute_bwd_hard:todummy_intraclj")
    solute_bwd_hard_todummy_intraclj.add(solute_bwd_hard, MGIdx(0))
    solute_bwd_hard_todummy_intraclj.add(solute_bwd_todummy, MGIdx(1))
    #solute_bwd_hard_todummy_intraclj.setUseReactionField(True)
    #solute_bwd_hard_todummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_bwd_hard_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_bwd_hard:fromdummy_intraclj")
    solute_bwd_hard_fromdummy_intraclj.add(solute_bwd_hard, MGIdx(0))
    solute_bwd_hard_fromdummy_intraclj.add(solute_bwd_fromdummy, MGIdx(1))
    #solute_bwd_hard_fromdummy_intraclj.setUseReactionField(True)
    #solute_bwd_hard_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    solute_bwd_todummy_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_bwd_todummy:fromdummy_intraclj")
    solute_bwd_todummy_fromdummy_intraclj.add(solute_bwd_todummy, MGIdx(0))
    solute_bwd_todummy_fromdummy_intraclj.add(solute_bwd_fromdummy, MGIdx(1))
    #solute_bwd_todummy_fromdummy_intraclj.setUseReactionField(True)
    #solute_bwd_todummy_fromdummy_intraclj.setReactionFieldDielectric(rfdielectric)

    # Now the solute-solvent CLJ energy
    solute_hard_solventff = InterGroupCLJFF("solute_hard:solvent")
    solute_hard_solventff.add(solute_hard, MGIdx(0))
    solute_hard_solventff.add(solvent, MGIdx(1))
    #solute_hard_solventff.setUseReactionField(True)
    #solute_hard_solventff.setReactionFieldDielectric(rfdielectric)

    solute_todummy_solventff = InterGroupSoftCLJFF("solute_todummy:solvent")
    solute_todummy_solventff.add(solute_todummy, MGIdx(0))
    solute_todummy_solventff.add(solvent, MGIdx(1))
    #solute_todummy_solventff.setUseReactionField(True)
    #solute_todummy_solventff.setReactionFieldDielectric(rfdielectric)

    solute_fromdummy_solventff = InterGroupSoftCLJFF("solute_fromdummy:solvent")
    solute_fromdummy_solventff.add(solute_fromdummy, MGIdx(0))
    solute_fromdummy_solventff.add(solvent, MGIdx(1))
    #solute_fromdummy_solventff.setUseReactionField(True)
    #solute_fromdummy_solventff.setReactionFieldDielectric(rfdielectric)

    # Now the forwards solute-solvent CLJ energy
    solute_fwd_hard_solventff = InterGroupCLJFF("solute_fwd_hard:solvent")
    solute_fwd_hard_solventff.add(solute_fwd_hard, MGIdx(0))
    solute_fwd_hard_solventff.add(solvent, MGIdx(1))
    #solute_fwd_hard_solventff.setUseReactionField(True)
    #solute_fwd_hard_solventff.setReactionFieldDielectric(rfdielectric)

    solute_fwd_todummy_solventff = InterGroupSoftCLJFF("solute_fwd_todummy:solvent")
    solute_fwd_todummy_solventff.add(solute_fwd_todummy, MGIdx(0))
    solute_fwd_todummy_solventff.add(solvent, MGIdx(1))
    #solute_fwd_todummy_solventff.setUseReactionField(True)
    #solute_fwd_todummy_solventff.setReactionFieldDielectric(rfdielectric)

    solute_fwd_fromdummy_solventff = InterGroupSoftCLJFF("solute_fwd_fromdummy:solvent")
    solute_fwd_fromdummy_solventff.add(solute_fwd_fromdummy, MGIdx(0))
    solute_fwd_fromdummy_solventff.add(solvent, MGIdx(1))
    #solute_fwd_fromdummy_solventff.setUseReactionField(True)
    #solute_fwd_fromdummy_solventff.setReactionFieldDielectric(rfdielectric)

    # Now the backwards solute-solvent CLJ energy
    solute_bwd_hard_solventff = InterGroupCLJFF("solute_bwd_hard:solvent")
    solute_bwd_hard_solventff.add(solute_bwd_hard, MGIdx(0))
    solute_bwd_hard_solventff.add(solvent, MGIdx(1))
    #solute_bwd_hard_solventff.setUseReactionField(True)
    #solute_bwd_hard_solventff.setReactionFieldDielectric(rfdielectric)

    solute_bwd_todummy_solventff = InterGroupSoftCLJFF("solute_bwd_todummy:solvent")
    solute_bwd_todummy_solventff.add(solute_bwd_todummy, MGIdx(0))
    solute_bwd_todummy_solventff.add(solvent, MGIdx(1))
    #solute_bwd_todummy_solventff.setUseReactionField(True)
    #solute_bwd_todummy_solventff.setReactionFieldDielectric(rfdielectric)

    solute_bwd_fromdummy_solventff = InterGroupSoftCLJFF("solute_bwd_fromdummy:solvent")
    solute_bwd_fromdummy_solventff.add(solute_bwd_fromdummy, MGIdx(0))
    solute_bwd_fromdummy_solventff.add(solvent, MGIdx(1))
    #solute_bwd_fromdummy_solventff.setUseReactionField(True)
    #solute_bwd_fromdummy_solventff.setReactionFieldDielectric(rfdielectric)

    # Now the solute-protein ( soft-core ) CLJ energy
    solute_hard_proteinff = InterGroupCLJFF("solute_hard:protein")
    solute_hard_proteinff.add(solute_hard, MGIdx(0))
    solute_hard_proteinff.add(protein, MGIdx(1))
    #solute_hard_proteinff.setUseReactionField(True)
    #solute_hard_proteinff.setReactionFieldDielectric(rfdielectric)

    solute_todummy_proteinff = InterGroupSoftCLJFF("solute_todummy:protein")
    solute_todummy_proteinff.add(solute_todummy, MGIdx(0))
    solute_todummy_proteinff.add(protein, MGIdx(1))
    #solute_todummy_proteinff.setUseReactionField(True)
    #solute_todummy_proteinff.setReactionFieldDielectric(rfdielectric)

    solute_fromdummy_proteinff = InterGroupSoftCLJFF("solute_fromdummy:protein")
    solute_fromdummy_proteinff.add(solute_fromdummy, MGIdx(0))
    solute_fromdummy_proteinff.add(protein, MGIdx(1))
    #solute_fromdummy_proteinff.setUseReactionField(True)
    #solute_fromdummy_proteinff.setReactionFieldDielectric(rfdielectric)

    # Now the forwards solute-protein CLJ energy
    solute_fwd_hard_proteinff = InterGroupCLJFF("solute_fwd_hard:protein")
    solute_fwd_hard_proteinff.add(solute_fwd_hard, MGIdx(0))
    solute_fwd_hard_proteinff.add(protein, MGIdx(1))
    #solute_fwd_hard_proteinff.setUseReactionField(True)
    #solute_fwd_hard_proteinff.setReactionFieldDielectric(rfdielectric)

    solute_fwd_todummy_proteinff = InterGroupSoftCLJFF("solute_fwd_todummy:protein")
    solute_fwd_todummy_proteinff.add(solute_fwd_todummy, MGIdx(0))
    solute_fwd_todummy_proteinff.add(protein, MGIdx(1))
    #solute_fwd_todummy_proteinff.setUseReactionField(True)
    #solute_fwd_todummy_proteinff.setReactionFieldDielectric(rfdielectric)

    solute_fwd_fromdummy_proteinff = InterGroupSoftCLJFF("solute_fwd_fromdummy:protein")
    solute_fwd_fromdummy_proteinff.add(solute_fwd_fromdummy, MGIdx(0))
    solute_fwd_fromdummy_proteinff.add(protein, MGIdx(1))
    #solute_fwd_fromdummy_proteinff.setUseReactionField(True)
    #solute_fwd_fromdummy_proteinff.setReactionFieldDielectric(rfdielectric)

    # Now the backwards solute-protein CLJ energy
    solute_bwd_hard_proteinff = InterGroupCLJFF("solute_bwd_hard:protein")
    solute_bwd_hard_proteinff.add(solute_bwd_hard, MGIdx(0))
    solute_bwd_hard_proteinff.add(protein, MGIdx(1))
    #solute_bwd_hard_proteinff.setUseReactionField(True)
    #solute_bwd_hard_proteinff.setReactionFieldDielectric(rfdielectric)

    solute_bwd_todummy_proteinff = InterGroupSoftCLJFF("solute_bwd_todummy:protein")
    solute_bwd_todummy_proteinff.add(solute_bwd_todummy, MGIdx(0))
    solute_bwd_todummy_proteinff.add(protein, MGIdx(1))
    #solute_bwd_todummy_proteinff.setUseReactionField(True)
    #solute_bwd_todummy_proteinff.setReactionFieldDielectric(rfdielectric)

    solute_bwd_fromdummy_proteinff = InterGroupSoftCLJFF("solute_bwd_fromdummy:protein")
    solute_bwd_fromdummy_proteinff.add(solute_bwd_fromdummy, MGIdx(0))
    solute_bwd_fromdummy_proteinff.add(protein, MGIdx(1))
    #solute_bwd_fromdummy_proteinff.setUseReactionField(True)
    #solute_bwd_fromdummy_proteinff.setReactionFieldDielectric(rfdielectric)

    # Here is the list of all forcefields
    forcefields = [ solute_intraff, solute_fwd_intraff, solute_bwd_intraff,
                    solute_hard_intraclj, solute_todummy_intraclj, solute_fromdummy_intraclj,
                    solute_hard_todummy_intraclj, solute_hard_fromdummy_intraclj, 
                    solute_todummy_fromdummy_intraclj,
                    solute_fwd_hard_intraclj, solute_fwd_todummy_intraclj, solute_fwd_fromdummy_intraclj,
                    solute_fwd_hard_todummy_intraclj, solute_fwd_hard_fromdummy_intraclj, 
                    solute_fwd_todummy_fromdummy_intraclj,
                    solute_bwd_hard_intraclj, solute_bwd_todummy_intraclj, solute_bwd_fromdummy_intraclj,
                    solute_bwd_hard_todummy_intraclj, solute_bwd_hard_fromdummy_intraclj, 
                    solute_bwd_todummy_fromdummy_intraclj,
                    solventff, 
                    solute_hard_solventff, solute_todummy_solventff, solute_fromdummy_solventff,
                    solute_fwd_hard_solventff, solute_fwd_todummy_solventff, solute_fwd_fromdummy_solventff,
                    solute_bwd_hard_solventff, solute_bwd_todummy_solventff, solute_bwd_fromdummy_solventff,
                    protein_intraff, protein_intraclj, protein_solventff, 
                    solute_hard_proteinff, solute_todummy_proteinff, solute_fromdummy_proteinff,
                    solute_fwd_hard_proteinff, solute_fwd_todummy_proteinff, solute_fwd_fromdummy_proteinff,
                    solute_bwd_hard_proteinff, solute_bwd_todummy_proteinff, solute_bwd_fromdummy_proteinff ]
    
    for forcefield in forcefields:
        system.add(forcefield)

    system.setProperty( "space", space )
    system.setProperty( "switchingFunction", 
                        CHARMMSwitchingFunction(coulomb_cutoff, coulomb_feather, lj_cutoff, lj_feather) )
    system.setProperty( "useReactionField", VariantProperty(True) )
    system.setProperty( "reactionFieldDielectric", VariantProperty(rfdielectric) )
    system.setProperty( "combiningRules", VariantProperty(combining_rules) )
    system.setProperty( "coulombPower", VariantProperty(coulomb_power) )
    system.setProperty( "shiftDelta", VariantProperty(shift_delta) )
    
    total_nrg = solute_intraff.components().total() + solute_hard_intraclj.components().total() +\
        solute_todummy_intraclj.components().total(0) + solute_fromdummy_intraclj.components().total(0) +\
        solute_hard_todummy_intraclj.components().total(0) + solute_hard_fromdummy_intraclj.components().total(0) +\
        solute_todummy_fromdummy_intraclj.components().total(0) +\
        solventff.components().total() +\
        solute_hard_solventff.components().total() +\
        solute_todummy_solventff.components().total(0) +\
        solute_fromdummy_solventff.components().total(0) +\
        protein_intraff.components().total() + protein_intraclj.components().total() + protein_solventff.components().total() +\
        solute_hard_proteinff.components().total() +\
        solute_todummy_proteinff.components().total(0) +\
        solute_fromdummy_proteinff.components().total(0) 

    fwd_nrg = solute_fwd_intraff.components().total() + solute_fwd_hard_intraclj.components().total() +\
        solute_fwd_todummy_intraclj.components().total(0) + solute_fwd_fromdummy_intraclj.components().total(0) +\
        solute_fwd_hard_todummy_intraclj.components().total(0) + solute_fwd_hard_fromdummy_intraclj.components().total(0) +\
        solute_fwd_todummy_fromdummy_intraclj.components().total(0) +\
        solventff.components().total() +\
        solute_fwd_hard_solventff.components().total() +\
        solute_fwd_todummy_solventff.components().total(0) +\
        solute_fwd_fromdummy_solventff.components().total(0) +\
        protein_intraff.components().total() + protein_intraclj.components().total() + protein_solventff.components().total() +\
        solute_fwd_hard_proteinff.components().total() +\
        solute_fwd_todummy_proteinff.components().total(0) +\
        solute_fwd_fromdummy_proteinff.components().total(0) 
        
    bwd_nrg = solute_bwd_intraff.components().total() + solute_bwd_hard_intraclj.components().total() +\
        solute_bwd_todummy_intraclj.components().total(0) + solute_bwd_fromdummy_intraclj.components().total(0) +\
        solute_bwd_hard_todummy_intraclj.components().total(0) + solute_bwd_hard_fromdummy_intraclj.components().total(0) +\
        solute_bwd_todummy_fromdummy_intraclj.components().total(0) +\
        solventff.components().total() +\
        solute_bwd_hard_solventff.components().total() +\
        solute_bwd_todummy_solventff.components().total(0) +\
        solute_bwd_fromdummy_solventff.components().total(0) +\
        protein_intraff.components().total() + protein_intraclj.components().total() + protein_solventff.components().total() +\
        solute_bwd_hard_proteinff.components().total() +\
        solute_bwd_todummy_proteinff.components().total(0) +\
        solute_bwd_fromdummy_proteinff.components().total(0) 


    e_total = system.totalComponent()
    e_fwd = Symbol("E_{fwd}")
    e_bwd = Symbol("E_{bwd}")

    lam = Symbol("lambda")
    lam_fwd = Symbol("lambda_{fwd}")
    lam_bwd = Symbol("lambda_{bwd}")

    system.setComponent( e_total, total_nrg )
    system.setComponent( e_fwd, fwd_nrg )
    system.setComponent( e_bwd, bwd_nrg )

    system.setConstant(lam, 0.0)
    system.setConstant(lam_fwd, 0.0)
    system.setConstant(lam_bwd, 0.0)

    de_fwd = Symbol("dE_{fwd}")
    de_bwd = Symbol("dE_{bwd}")

    system.setComponent( de_fwd, fwd_nrg - total_nrg )
    system.setComponent( de_bwd, total_nrg - bwd_nrg )

    # Add a space wrapper that wraps all molecules into the box centered at (0,0,0)
    # system.add( SpaceWrapper(Vector(0,0,0), all) )

    # Add a monitor that calculates the average total energy and average energy
    # deltas - we will collect both a mean average and an zwanzig average
    system.add( "total_energy", MonitorComponent(e_total, Average()) )
    system.add( "dg_fwd", MonitorComponent(de_fwd, FreeEnergyAverage(temperature)) )
    system.add( "dg_bwd", MonitorComponent(de_bwd, FreeEnergyAverage(temperature)) )

    system.add( PerturbationConstraint(solutes) )

    system.add( ComponentConstraint( lam_fwd, Min( lam + delta_lambda, 1 ) ) )
    system.add( ComponentConstraint( lam_bwd, Max( lam - delta_lambda, 0 ) ) )

    # Add a monitor that records the value of all energy components
    system.add( "energies", MonitorComponents(RecordValues()), nmoves_per_energy )
    
    # Add a monitor that records the coordinates of the system
    if (lam_val < 0.001 or lam_val > 0.999):
        system.add( "trajectory", TrajectoryMonitor(MGName("traj")), nmoves_per_pdb )
    else:
        system.add( "trajectory", TrajectoryMonitor(MGName("traj")), nmoves_per_pdb_intermediates )

    # Alpha constraints for the soft force fields

    system.add( PropertyConstraint( "alpha0", FFName("solute_todummy_intraclj"), lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fromdummy_intraclj"), 1 - lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_hard:todummy_intraclj"), lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_hard:fromdummy_intraclj"), 1 - lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_todummy:fromdummy_intraclj"), Min( lam, 1 - lam )  ) ) 
    system.add( PropertyConstraint( "alpha0", FFName("solute_todummy:solvent"), lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fromdummy:solvent"), 1 - lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_todummy:protein"), lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fromdummy:protein"), 1 - lam ) )

    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_todummy_intraclj"), lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_fromdummy_intraclj"), 1 - lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_hard:todummy_intraclj"), lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_hard:fromdummy_intraclj"), 1 - lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_todummy:fromdummy_intraclj"), Min( lam_fwd, 1 - lam_fwd ) ) ) 
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_todummy:solvent"), lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_fromdummy:solvent"), 1 - lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_todummy:protein"), lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_fromdummy:protein"), 1 - lam_fwd ) )

    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_todummy_intraclj"), lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_fromdummy_intraclj"), 1 - lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_hard:todummy_intraclj"), lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_hard:fromdummy_intraclj"), 1 - lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_todummy:fromdummy_intraclj"), Min( lam_bwd, 1 - lam_bwd ) ) ) 
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_todummy:solvent"), lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_fromdummy:solvent"), 1 - lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_todummy:protein"), lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_fromdummy:protein"), 1 - lam_bwd ) )

    system.setComponent( lam, lam_val )

    return system

def getAtomNearCOG( molecule ):

    mol_centre = molecule.evaluate().center()
    mindist = 99999.0

    for x in range(0, molecule.nAtoms()):
        atom = molecule.atoms()[x]
        at_coords = atom.property('coordinates')
        dist = Vector().distance2(at_coords, mol_centre)
        if dist < mindist:
            mindist = dist
            nearest_atom = atom

    return nearest_atom

def setupMoves(system, random_seed):

    solutes = system[ MGName("solutes") ]
    solute_ref = system[ MGName("solute_ref") ]

    solvent = system[ MGName("solvent") ]
    water = system[ MGName("water") ]
    ion = system[ MGName("ion") ]

    mobilewater = system[ MGName("mobilewater") ]
    fixwater = system[ MGName("fixwater") ]

    mobileion = system[ MGName("mobileion") ]
    fixion = system[ MGName("fixion") ]

    protein = system[ MGName("protein") ]
    residues = system[ MGName("residues") ]
    bbresidues = system[ MGName("bbresidues") ]

    all = system[ MGName("all") ]
    
    print "Setting up moves..."
    # Setup Moves
    max_volume_change = 0.10 * solvent.nMolecules() * angstrom3

    solute_moves = RigidBodyMC( solutes ) 
    solute_moves.setMaximumTranslation(solutes[MolIdx(0)].molecule().property('flexibility').translation() )
    solute_moves.setMaximumRotation(solutes[MolIdx(0)].molecule().property('flexibility').rotation() )
    # Find solute atom nearest to the center of geometry
    nearestcog_atom = getAtomNearCOG( solutes[MolIdx(0)].molecule() )
    #print nearestcog_atom
    solute_moves.setCenterOfRotation( GetCOGPoint( nearestcog_atom.name() ) )
    solute_moves.setSynchronisedTranslation(True)
    solute_moves.setSynchronisedRotation(True)
    #solute_moves.setSharedRotationCenter(True)

    solute_intra_moves = InternalMoveSingle( solute_ref )

    perturbed_solutes = system[ MGName("perturbed_solutes") ]
    # Each molecule in perturbed_solutes will have its coordinates set to those 
    # of solute_ref after the move
    solute_intra_moves.setSynchronisedCoordinates(perturbed_solutes)
    
    # Setup a reflection sphere, so only waters within a sphere are actually sampled
    ligand = solute_ref[MolIdx(0)].molecule()
    sphere_center = ligand.evaluate().center()

    # Solvent moves, split in water and ions
    #water_moves = RigidBodyMC( PrefSampler(solute_ref[MolIdx(0)], water, pref_constant) )
    water_moves = RigidBodyMC( PrefSampler(solute_ref[MolIdx(0)], mobilewater, pref_constant) )    
    water_moves.setMaximumTranslation(max_solvent_translation)
    water_moves.setMaximumRotation(max_solvent_rotation)
    water_moves.setCenterOfRotation(GetCOGPoint( AtomName("O") ) )
    water_moves.setReflectionSphere(sphere_center, water_sphere_radius)

    #ion_moves = RigidBodyMC( PrefSampler(solute_ref[MolIdx(0)], ion, pref_constant) )
    ion_moves = RigidBodyMC( PrefSampler(solute_ref[MolIdx(0)], mobileion, pref_constant) )
    ion_moves.setMaximumTranslation(max_solvent_translation)
    ion_moves.setMaximumRotation(max_solvent_rotation)
    ion_moves.setCenterOfRotation(GetCOGPoint( AtomName("Cl-"), AtomName("Na+") ) )
    ion_moves.setReflectionSphere(sphere_center, water_sphere_radius)

    # Protein intra moves
    protein_intra_moves = ZMatMove( PrefSampler(solute_ref[MolIdx(0)], residues, pref_constant) ) 
    
    # Now add protein backbone moves
    bbmoves = RigidBodyMC(bbresidues)
    bbmoves.setMaximumTranslation(0.025*angstrom)
    bbmoves.setMaximumRotation(1*degrees)
    bbmoves.setCenterOfRotation( GetCOGPoint( AtomName("CA", CaseInsensitive),
                                              AtomName("N", CaseInsensitive) ) )
    
    # Volume moves
    #volume_moves = VolumeMove(all)
    #volume_moves.setMaximumVolumeChange(max_volume_change)

    moves = WeightedMoves()
    moves.add( solute_moves, solute_mc_weight / 2 )
    moves.add( solute_intra_moves, solute_mc_weight / 2)
    moves.add( protein_intra_moves, protein_mc_weight / 2)
    moves.add( bbmoves, protein_mc_weight / 2 )

    if mobilewater.nMolecules() > 0:
        moves.add( water_moves, mobilewater.nMolecules()*water_mc_weight_factor )
    if mobileion.nMolecules() > 0:
        moves.add( ion_moves, mobileion.nMolecules() )

    #moves.add( volume_moves, volume_mc_weight )

    moves.setTemperature(temperature)
    #moves.setPressure(pressure)

    if (not random_seed):
	random_seed = RanGenerator().randInt(100000,1000000)
	print "Generated random seed number %d " % random_seed

    moves.setGenerator( RanGenerator(random_seed) )
    
    return moves

def writeComponents(components, filename):
    """This function writes the energy components to the file 'filename'"""

    symbols = components.monitoredComponents()

    if len(symbols) == 0:
        return

    newrun = False
    if not os.path.exists(filename):
        newrun = True

    FILE = open(filename, "a")

    nrgs = {}

    for symbol in symbols:
        nrgs[str(symbol)] = components.accumulator(symbol).values()

    symbols = nrgs.keys()
    symbols.sort()

    if newrun:
        print >>FILE,"#step   ",

        for symbol in symbols:
            print >>FILE,"%s   " % symbol,

        print >>FILE,"\n",

    for i in range(0, len(nrgs[symbols[0]])):
        print >>FILE,"%d   " % i,

        for symbol in symbols:
            print >>FILE,"%f   " % nrgs[symbol][i],

        print >>FILE,"\n",


def writeSystemData( system, moves, block):

    nmoves = moves.nMoves()
    monitors = system.monitors()

    pdb = monitors[MonitorName("trajectory")]
    energies = monitors[MonitorName("energies")]
    total_energy = monitors[MonitorName("total_energy")]
    
    dg_fwd = monitors[MonitorName("dg_fwd")]
    dg_bwd = monitors[MonitorName("dg_bwd")]
    
    dg_fwd = dg_fwd.accumulator().average() / delta_lambda
    dg_bwd = dg_bwd.accumulator().average() / delta_lambda

    system.clearStatistics()
    
    pdb.writeToDisk("output%0009d.pdb" % block )

    if os.path.exists("energies.dat.bz2"):
        os.system("bunzip2 -f energies.dat.bz2")
    writeComponents( energies, "energies.dat" )

    # Ugly
    lam = system.constantExpression(Symbol("lambda")).toString().toDouble()    
    #print dg_bwd, dg_fwd, lam

    if lam < 0.0001:
        dg_bwd = dg_fwd
    elif lam > 0.9999:
        dg_fwd = dg_bwd

    dg_avg = 0.5 * ( dg_fwd + dg_bwd )

    #print dg_avg

    FILE = open("gradients.dat" , 'a')
    print >>FILE, "%9d %12.8f " % ( block, dg_avg)
   
    FILE = open("moves.dat", "w")
    print >>FILE, "%s" % moves

######## MAIN SCRIPT  #############

if __name__ == '__main__':
    print " ### Starting python script ### " 

    timer = QTime()
    timer.start()

    # Setup the system from scratch if no restart file is available

    if not os.path.exists(restart_file):
        print "New run. Loading input and creating restart"
        try:
            lam_index = int( sys.argv[1] ) - 1
            if lam_index < 0:
                print "Sorry - lambda must be between 1 and len(lambda_values)"
                sys.exit(-1)
        except:
            print "Sorry, for a new run, lambda must be specified as a command line argument"
            sys.exit(-1)

        try:
            lam_val = lambda_values[lam_index]
        except:
            print "Sorry - you are asking for an invalid lambda index (%d) - lambda_values = %s" % \
                       (lam_index, str(lambda_values))
            sys.exit(-1)

        print "Lambda is %5.3f" % lam_val       

        amber = Amber()

        molecules, space = amber.readCrdTop(crd_file, top_file)

        system = createSystem(molecules)

        system = setupForcefields(system, space)

        moves = setupMoves(system, random_seed)
        print "Saving restart"
        Sire.Stream.save( [system, moves], restart_file )


    system, moves = Sire.Stream.load( restart_file )
    print "Loaded a restart file on wich we have performed %d moves." % moves.nMoves()
    block_number = moves.nMoves() / nmoves  + 1
    s1 = timer.elapsed()/1000.
    print "Setup took %d s " % ( s1 )

    # Run a short simulation
    #sys.exit(-1)
    print "Performing simulation for block number %d " % block_number
    
    system = moves.move(system, nmoves, True)

    s2 = timer.elapsed()/1000.
    print "Simulation took %d s " % ( s2 - s1)

    # Update statistics and save restart
    writeSystemData(system, moves, block_number) 

    Sire.Stream.save( [system, moves], restart_file )

    # Compress some output files
    outpdb = "./output%0009d.pdb" % block_number
    if os.path.exists(outpdb):
        os.system( "%s output%0009d*" % (compress, block_number ) )
    if os.path.exists("energies.dat"):
        os.system(" %s energies.dat" % compress)
