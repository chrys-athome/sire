#!/bin/env python
# -*- coding: utf-8 -*-

from Sire.IO import *
from Sire.System import *
from Sire.Mol import *
from Sire.MM import *
from Sire.FF import *
from Sire.CAS import *
from Sire.Maths import *
from Sire.System import *
from Sire.Base import *
from Sire.Units import *

import Sire.Config
import Sire.Stream

from Sire.Tools.AmberLoader import *

import os

wsrc_tools_dir = "%s/Tools/WSRC" % Sire.Config.share_directory

def getIdentityPoints(ligand):
    
    atoms = ligand.atoms()

    have_point = {}

    for atom in atoms:
        # skip small atoms
        if atom.property("element").nProtons() >= 6:
            have_point[str(atom.name().value())] = True

        else:
            have_point[str(atom.name().value())] = False

    if ligand.hasProperty("connectivity"):
        connectivity = ligand.property("connectivity")
    else:
        connectivity = Connectivity(ligand)

    for key in have_point.keys():
        if have_point[key]:
            # if this is bonded to 3+ atoms that also have 
            # identity points, then get rid of this point
            atom = ligand.atom( AtomName(key) )

            bonded = connectivity.connectionsTo(atom.name())

            if len(bonded) >=3:
                n = 0

                for b in bonded:
                    if have_point[ str(ligand.atom(b).name().value()) ]:
                        n += 1

                if n >= 3:
                    print "Skipping %s as it is bonded to 3 or more points..." % atom.name()
                    have_point[key] = False

    identity_points = []
    k2 = []

    # skip every 8th point
    iskip = 0

    for key in have_point.keys():
        if have_point[key]:
            iskip += 1

            if iskip == 8:
                iskip = 0
            else:
                k2.append(key)
                identity_points.append( ligand.atom( AtomName(key) ) )

    k2.sort()

    print "Using %d identity points: %s" % (len(k2), str(k2))

    return identity_points


def getMinimumDistance(mol0, mol1):
    space = Cartesian()
    return space.minimumDistance(CoordGroup(mol0.molecule().property("coordinates").array()), \
                                 CoordGroup(mol1.molecule().property("coordinates").array()))


def setCLJProperties(forcefield, params):
    cutoff_method = readConfig("cutoff method", params, "shift electrostatics")

    try:
        forcefield.setUseAtomisticCutoff(True)
    except:
        pass

    if cutoff_method.lower().find("shift electrostatics") != -1:
        forcefield.setShiftElectrostatics(True)

    elif cutoff_method.lower().find("reaction field") != -1:
        forcefield.setUseReactionField(True)
        rf_dielectric = readConfig("reaction field dielectric", params, 78.3)
        forcefield.setReactionFieldDielectric(rf_dielectric)

    forcefield.setSpace(Cartesian())
    forcefield.setSwitchingFunction(NoCutoff())

    return forcefield


def setFakeGridProperties(forcefield_cl, forcefield_lj, params):
    lj_cutoff = readConfig("LJ cutoff", params, 10*angstrom)
    coul_cutoff = readConfig("coulomb cutoff", params, 30*angstrom)

    forcefield_cl.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff) )
    forcefield_lj.setSwitchingFunction( HarmonicSwitchingFunction(lj_cutoff) )

    return (forcefield_cl, forcefield_lj)


def setGridProperties(forcefield, params, extra_buffer=0*angstrom):
    grid_spacing = readConfig("grid spacing", params, 0.5*angstrom)
    grid_buffer = readConfig("grid buffer", params, 2*angstrom)
    lj_cutoff = readConfig("LJ cutoff", params, 10*angstrom)
    coul_cutoff = readConfig("coulomb cutoff", params, 30*angstrom)

    forcefield.setGridSpacing(grid_spacing)
    forcefield.setBuffer(grid_buffer + extra_buffer)
    forcefield.setLJCutoff(lj_cutoff)
    forcefield.setCoulombCutoff(coul_cutoff)

    return forcefield


def setSoftCoreProperties(forcefield, params):
    forcefield.setCoulombPower(0)
    forcefield.setShiftDelta(0.25)

    return forcefield


def createWSRCMoves(system, params=None):
    temperature = readConfig("temperature", params, 25*celsius)
    random_seed = readConfig("random seed", params, None)

    # pull out all of the molecule groups for the mobile parts of the system
    mobile_solvent = system[MGName("mobile_solvent")]
    mobile_sidechains = system[MGName("mobile_sidechains")]
    mobile_backbones = system[MGName("mobile_backbones")]
    mobile_solutes = system[MGName("mobile_solutes")]
    mobile_ligand = system[MGName("mobile_ligand")]
    mobile_swap = system[MGName("mobile_swap_water")]

    print "Creating the Monte Carlo moves to sample the WSRC system..."

    # create the global set of moves that will be applied to
    # the system
    moves = WeightedMoves()

    # create zmatrix moves to move the protein sidechains
    if mobile_sidechains.nViews() > 0:
        sc_moves = ZMatMove(mobile_sidechains)
        moves.add( sc_moves, mobile_sidechains.nViews() )

    if mobile_backbones.nViews() > 0:
        bb_moves = RigidBodyMC(mobile_backbones)
        bb_moves.setCenterOfRotation( GetCOGPoint( AtomName("CA", CaseInsensitive),
                                                   AtomName("N", CaseInsensitive) ) )

        bb_moves.setMaximumTranslation(0.015*angstrom)
        bb_moves.setMaximumRotation(0.5*degrees)
        moves.add( bb_moves, mobile_backbones.nViews() )

    if mobile_ligand.nViews() > 0:
        # we do not translate or rotate the ligand - just move its internals
        intra_moves = InternalMove(mobile_ligand)
        moves.add( intra_moves, 10 * mobile_ligand.nViews() )

    if mobile_solutes.nViews() > 0:
        rb_moves = RigidBodyMC(mobile_solutes)

        if system.containsProperty("average solute translation delta"):
            translation_delta = float(str(system.property("average solute translation delta")))
        else:
            translation_delta = 0

        if system.containsProperty("average solute rotation delta"):
            rotation_delta = float(str(system.property("average solute rotation delta")))
        else:
            rotation_delta = 0

        if translation_delta > 0 and rotation_delta > 0:
            rb_moves.setMaximumTranslation(translation_delta * angstroms)
            rb_moves.setMaximumRotation(rotation_delta * degrees)
            moves.add(rb_moves, 4 * mobile_solutes.nViews())

        intra_moves = InternalMove(solute_group)
        moves.add(intra_moves, 4 * mobile_solutes.nViews())

    max_water_translation = 0.15 * angstroms
    max_water_rotation = 15 * degrees

    if mobile_swap.nViews() > 0:
        rb_moves = RigidBodyMC(mobile_swap)
        rb_moves.setMaximumTranslation(max_water_translation)
        rb_moves.setMaximumRotation(max_water_rotation)
        moves.add(rb_moves, 4 * mobile_swap.nViews())

    if mobile_solvent.nViews() > 0:
        rb_moves = RigidBodyMC(mobile_solvent)
        rb_moves.setMaximumTranslation(max_water_translation)
        rb_moves.setMaximumRotation(max_water_rotation)

        if system.containsProperty("reflection sphere radius"):
            reflection_radius = float(str(system.property("reflection sphere radius"))) * angstroms
            reflection_center = system.property("reflection center").toVector()[0]
            rb_moves.setReflectionSphere(reflection_center, reflection_radius)

        moves.add(rb_moves, 4 * mobile_solvent.nViews())

    moves.setTemperature(temperature)

    if random_seed is None:
        random_seed = RanGenerator().randInt(100000,1000000)
        print "Using generated random number seed %d" % random_seed
    else:
        print "Using supplied random number seed %d" % random_seed
    
    moves.setGenerator( RanGenerator(random_seed) )

    return moves    


def renumberMolecules(molgroup):
    newgroup = MoleculeGroup(molgroup.name().value())
    for molnum in molgroup.molNums():
        mol = molgroup[molnum]
        newmol = mol.molecule().edit().renumber().commit()
        newgroup.add( ViewsOfMol(newmol,mol.selections()) )

    return newgroup


def mergeSystems(protein_system, water_system, ligand_mol, params=None):
    
    print "Merging the protein box and water box to create the WSRC system..."

    system = System("WSRC system")

    if protein_system.containsProperty("reflection center"):
        prot_reflection_center = protein_system.property("reflection center").toVector()[0]
        prot_reflection_radius = float(str(protein_system.property("reflection sphere radius")))

        wat_reflection_center = water_system.property("reflection center").toVector()[0]
        wat_reflection_radius = float(str(water_system.property("reflection sphere radius")))

        if prot_reflection_center != wat_reflection_center or \
           prot_reflection_radius != wat_reflection_radius:
            print "Disagreement of the reflection sphere in the protein and water boxes!"
            print "Protein: %s and %s    Water: %s and %s" % \
                    (prot_reflection_center,prot_reflection_radius,
                     wat_reflection_center,wat_reflection_radius)

            sys.exit(-1)

        system.setProperty("reflection center", AtomCoords(CoordGroup(1,prot_reflection_center)))
        system.setProperty("reflection sphere radius", VariantProperty(prot_reflection_radius))

    if protein_system.containsProperty("average solute translation delta"):
        system.setProperty("average solute translation delta", \
                       protein_system.property("average solute translation delta"))

    if protein_system.containsProperty("average solute rotation delta"):
        system.setProperty("average solute rotation delta", \
                       protein_system.property("average solute rotation delta"))

    add_groups = []

    # create a molecule group for the ligand
    ligand_group = MoleculeGroup("ligand")
    ligand_group.add(ligand_mol)

    bound_leg = MoleculeGroup("bound_leg")
    free_leg = MoleculeGroup("free_leg")

    bound_leg.add(ligand_mol)
    free_leg.add(ligand_mol)

    # pull out the groups that we want from the two systems

    # create a group to hold all of the mobile water molecules in the free leg
    mobile_free_water_group = MoleculeGroup("mobile_free")
    water_mol = None
    if MGName("mobile_solvents") in water_system.mgNames():
        mols = water_system[MGName("mobile_solvents")].molecules()
        for molnum in mols.molNums():
            water_mol = mols[molnum].molecule().edit().renumber().commit()
            for j in range(0,water_mol.nResidues()):
                water_mol = water_mol.residue( ResIdx(j) ).edit() \
                                           .setProperty( PDB.parameters().pdbResidueName(), "FWT" ) \
                                           .commit().molecule()

            mobile_free_water_group.add(water_mol)

    # create a group to hold all of the fixed water molecules in the free leg
    fixed_free_water_group = MoleculeGroup("fixed_free")
    if MGName("fixed_molecules") in water_system.mgNames():
        mols = water_system[MGName("fixed_molecules")].molecules()
        for molnum in mols.molNums():
            fixed_free_water_group.add( mols[molnum].molecule().edit().renumber().commit() )

    # create a group to hold all of the fixed molecules in the bound leg
    fixed_bound_group = MoleculeGroup("fixed_bound")
    if MGName("fixed_molecules") in protein_system.mgNames():
        fixed_bound_group.add( protein_system[ MGName("fixed_molecules") ] )

    # create a group to hold all of the mobile solute molecules in the bound leg
    mobile_bound_solutes_group = MoleculeGroup("mobile_bound_solutes")
    if MGName("mobile_solutes") in protein_system.mgNames():
        mobile_bound_solutes_group.add( protein_system[MGName("mobile_solutes")] )
        mobile_bound_solutes_group.remove(ligand_mol)
        if mobile_bound_solutes_group.nMolecules() > 0:
            bound_leg.add(mobile_bound_solutes_group)
    
    # create a group to hold all of the mobile solvent molecules in the bound leg
    mobile_bound_solvents_group = MoleculeGroup("mobile_bound_solvents")
    mobile_bound_water_group = MoleculeGroup("mobile_bound_water")
    if MGName("mobile_solvents") in protein_system.mgNames():
        mols = protein_system[MGName("mobile_solvents")]
        for molnum in mols.molNums():
            solvent_mol = mols[molnum].molecule()

            try:
                # this is a water molecule if we can swap the coordinates with the 
                # water molecule from teh water box
                water_mol.edit().setProperty("coordinates", \
                                     solvent_mol.property("coordinates"))
                
                for j in range(0,solvent_mol.nResidues()):
                    solvent_mol = solvent_mol.residue( ResIdx(j) ).edit() \
                                             .setProperty( PDB.parameters().pdbResidueName(), "BWT" ) \
                                             .commit().molecule()
                
                mobile_bound_solvents_group.add(solvent_mol)
                mobile_bound_water_group.add(solvent_mol)
            except:
                # the test molecule is not compatible, so it is not 
                # compatible with the water in the water box
                mobile_bound_solvents_group.add(solvent_mol)

        print "The number of bound leg mobile solvent molecules is %d." % mobile_bound_solvents_group.nMolecules()
        print "The number of these which are compatible water molecules is %d." % mobile_bound_water_group.nMolecules()

    # create the groups to hold all of the protein molecules. We will use "extract" to 
    # pull out only those protein atoms that are in the mobile region
    bound_protein_intra_group = MoleculeGroup("bound_protein_intra_group")
    mobile_bound_proteins_group = MoleculeGroup("mobile_bound_proteins")
    mobile_bound_protein_sidechains_group = MoleculeGroup("mobile_bound_protein_sidechains")
    mobile_bound_protein_backbones_group = MoleculeGroup("mobile_bound_protein_backbones")

    if MGName("protein_sidechains") in protein_system.mgNames() or \
       MGName("protein_backbones") in protein_system.mgNames():

        all_proteins = Molecules()

        try:
            protein_sidechains = protein_system[MGName("protein_sidechains")]
            all_proteins.add(protein_sidechains.molecules())
        except:
            protein_sidechains = MoleculeGroup()

        try:
            protein_backbones = protein_system[MGName("protein_backbones")]
            all_proteins.add(protein_backbones.molecules())
        except:
            protein_backbones = MoleculeGroup()

        try:
            boundary_molecules = protein_system[MGName("boundary_molecules")]
            all_proteins.add(boundary_molecules.molecules())
        except:
            boundary_molecules = MoleculeGroup()

        for molnum in all_proteins.molNums():
            protein_mol = all_proteins[molnum].join()
            
            if protein_mol.selectedAll():
                bound_protein_intra_group.add(protein_mol)

                mobile_protein = None                

                try:
                    mobile_protein = protein_sidechains[molnum]
                    mobile_bound_protein_sidechains_group.add( mobile_protein )
                except:
                    pass

                try:
                    if mobile_protein is None:
                        mobile_protein = protein_backbones[molnum]
                        mobile_bound_protein_backbones_group.add( mobile_protein )
                    else:
                        mobile_protein.add( protein_backbones[molnum].selection() )
                        mobile_bound_protein_backbones_group.add( protein_backbones[molnum] )
                except:
                    pass

                if not (mobile_protein is None):
                    mobile_bound_proteins_group.add( mobile_protein.join() )
            else:
                # only some of the atoms have been selected. We will extract
                # the mobile atoms and will then update all of the other selections
                print "Extracting the mobile atoms of protein %s" % protein_mol
                new_protein_mol = protein_mol.extract()
                print "Extracted %d mobile atoms from %d total atoms..." % \
                                        (new_protein_mol.nAtoms(), protein_mol.molecule().nAtoms())

                bound_protein_intra_group.add(new_protein_mol)

                mobile_protein_view = new_protein_mol.selection()
                mobile_protein_view = mobile_protein_view.selectNone()

                try:
                    sidechains = protein_sidechains[molnum]

                    for i in range(0,sidechains.nViews()):
                        view = new_protein_mol.selection()
                        view = view.selectNone()

                        for atomid in sidechains.viewAt(i).selectedAtoms():
                            atom = protein_mol.atom(atomid)
                            resatomid = ResAtomID( atom.residue().number(), atom.name() )
                            view = view.select( resatomid )
                            mobile_protein_view = mobile_protein_view.select( resatomid )

                        if view.nSelected() > 0:
                            mobile_bound_protein_sidechains_group.add( PartialMolecule(new_protein_mol, view) )
                except:
                    pass

                try:
                    backbones = protein_backbones[molnum]

                    for i in range(0,backbones.nViews()):
                        view = new_protein_mol.selection()
                        view = view.selectNone()

                        for atomid in backbones.viewAt(i).selectedAtoms():
                            atom = protein_mol.atom(atomid)
                            resatomid = ResAtomID( atom.residue().number(), atom.name() )
                            view = view.select( resatomid )
                            mobile_protein_view = mobile_protein_view.select( resatomid )

                        if view.nSelected() > 0:
                            mobile_bound_protein_backbones_group.add( PartialMolecule(new_protein_mol, view) )
                except:
                    pass

                if mobile_protein_view.nSelected() > 0:
                    mobile_bound_proteins_group.add( PartialMolecule(new_protein_mol, mobile_protein_view) )
                    bound_leg.add( PartialMolecule(new_protein_mol, mobile_protein_view) )

    # finished adding in all of the protein groups

    # get the identity points for the ligand
    print "\nObtaining the identity points..."
    identity_atoms = readConfig("identity atoms", params, None)

    if identity_atoms is None:
        print "Auto-identifying the identiy atoms..."
        identity_points = getIdentityPoints(ligand_mol)
    else:
        identity_points = []
        for identity_atom in identity_atoms:
            identity_points.append( ligand_mol.atom( AtomName(identity_atom) ) )

    print "Using identity points:"
    print identity_points

    print "\nIdentifying the swap-water cluster..."
    swap_water_group = MoleculeGroup("swap water")
    mobile_free_water_group = IdentityConstraint.constrain( mobile_free_water_group, identity_points )

    # Rename the residues of the swap solvent so that they are easy
    # to find in the output PDBs
    for i in range(0,len(identity_points)):
        swap_water_mol = mobile_free_water_group.moleculeAt(i).molecule()
        mobile_free_water_group.remove(swap_water_mol)

        for j in range(0,swap_water_mol.nResidues()):
            swap_water_mol = swap_water_mol.residue( ResIdx(j) ).edit() \
                                           .setProperty( PDB.parameters().pdbResidueName(), "SWP" ) \
                                           .commit().molecule()

        swap_water_group.add(swap_water_mol)

    bound_leg.add(swap_water_group)
    bound_leg.add(mobile_bound_solvents_group)

    free_leg.add(swap_water_group)
    free_leg.add(mobile_free_water_group)

    system.add(bound_leg)
    system.add(free_leg)

    # now add in the forcefields for the system...
    print "Creating the forcefields for the WSRC system..."

    # first, group together the molecules grouped above into convenient
    # groups for the forcefields

    # group holding just the ligand
    ligand_mols = ligand_group.molecules()

    # group holding just the swap water cluster
    swap_water_mols = swap_water_group.molecules()

    # group holding all of the mobile atoms in the bound leg
    mobile_bound_mols = mobile_bound_solvents_group.molecules()
    mobile_bound_mols.add( mobile_bound_solutes_group.molecules() )
    mobile_bound_mols.add( bound_protein_intra_group.molecules() )

    # group holding all of the mobile atoms in the bound leg, excluding the 
    # buffer atoms that are fixed, but bonded to mobile atoms
    mobile_buffered_bound_mols = mobile_bound_solvents_group.molecules()
    mobile_buffered_bound_mols.add( mobile_bound_solutes_group.molecules() )
    mobile_buffered_bound_mols.add( mobile_bound_proteins_group.molecules() )

    # group holding all of the mobile water molecules in the free leg
    mobile_free_mols = mobile_free_water_group.molecules()

    # group holding all of the fixed water molecules in the free leg
    fixed_free_group = fixed_free_water_group

    # group holding all of the protein molecules that need intramolecular terms calculated
    bound_protein_intra_mols = bound_protein_intra_group.molecules()

    # group holding all of the solute molecules that nede intramolecular terms calculated
    bound_solute_intra_mols = mobile_bound_solutes_group.molecules()

    ###
    ### INTRA-ENERGY OF THE LIGAND AND CLUSTER
    ###
    
    # intramolecular energy of the ligand
    ligand_intraclj = IntraCLJFF("ligand:intraclj")
    ligand_intraclj = setCLJProperties(ligand_intraclj, params)
    ligand_intraclj.add(ligand_mols)

    ligand_intraff = InternalFF("ligand:intra")
    ligand_intraff.add(ligand_mols)

    # intramolecular energy of the swap water cluster
    swap_interclj = InterCLJFF("swap:interclj")
    swap_interclj = setCLJProperties(swap_interclj, params)
    swap_interclj.add(swap_water_mols)

    ###
    ### FORCEFIELDS INVOLVING THE LIGAND/CLUSTER BOUND LEG
    ###

    # forcefield holding the energy between the ligand and the mobile atoms in the
    # bound leg
    bound_ligand_mobile = InterGroupSoftCLJFF("bound:ligand-mobile")
    bound_ligand_mobile = setCLJProperties(bound_ligand_mobile, params)
    bound_ligand_mobile = setSoftCoreProperties(bound_ligand_mobile, params)

    bound_ligand_mobile.add(ligand_mols, MGIdx(0))
    bound_ligand_mobile.add(mobile_bound_mols, MGIdx(1))

    # forcefield holding the energy between the swap water cluster and the mobile
    # atoms in the bound leg
    bound_swap_mobile = InterGroupSoftCLJFF("bound:swap-mobile")
    bound_swap_mobile = setCLJProperties(bound_swap_mobile, params)
    bound_swap_mobile = setSoftCoreProperties(bound_swap_mobile, params)

    bound_swap_mobile.add(swap_water_mols, MGIdx(0))
    bound_swap_mobile.add(mobile_bound_mols, MGIdx(1))

    # Whether or not to disable the grid and calculate all energies atomisticly
    disable_grid = readConfig("disable grid", params, False)

    if disable_grid:
        # we need to renumber all of the fixed molecules so that they don't clash
        # with the mobile molecules
        print "Renumbering fixed molecules..."
        fixed_bound_group = renumberMolecules(fixed_bound_group)
        fixed_free_group = renumberMolecules(fixed_free_group)

    # forcefield holding the energy between the ligand and the fixed atoms in the bound leg
    if disable_grid:
        bound_ligand_fixed_cl = InterGroupCLJFF("bound:ligand-fixed-cl")
        bound_ligand_fixed_lj = InterGroupCLJFF("bound:ligand-fixed-lj")
        bound_ligand_fixed_cl = setCLJProperties(bound_ligand_fixed_cl, params)
        bound_ligand_fixed_lj = setCLJProperties(bound_ligand_fixed_lj, params)
        (bound_ligand_fixed_cl, bound_ligand_fixed_lj) \
            = setFakeGridProperties(bound_ligand_fixed_cl, bound_ligand_fixed_lj, params)

        bound_ligand_fixed_cl.add(ligand_mols, MGIdx(0))
        bound_ligand_fixed_lj.add(ligand_mols, MGIdx(0))
        bound_ligand_fixed_cl.add(fixed_bound_group, MGIdx(1))
        bound_ligand_fixed_lj.add(fixed_bound_group, MGIdx(1))
    else:
        bound_ligand_fixed = GridFF("bound:ligand-fixed")
        bound_ligand_fixed = setCLJProperties(bound_ligand_fixed, params)
        bound_ligand_fixed = setGridProperties(bound_ligand_fixed, params)

        bound_ligand_fixed.add(ligand_mols, MGIdx(0))
        bound_ligand_fixed.addFixedAtoms( fixed_bound_group )

    # forcefield holding the energy between the swap water cluster and the
    # fixed atoms in the bound leg
    if disable_grid:
        bound_swap_fixed_cl = InterGroupCLJFF("bound:swap-fixed_cl")
        bound_swap_fixed_lj = InterGroupCLJFF("bound:swap-fixed_lj")
        bound_swap_fixed_cl = setCLJProperties(bound_swap_fixed_cl, params)
        bound_swap_fixed_lj = setCLJProperties(bound_swap_fixed_lj, params)
        (bound_swap_fixed_cl, bound_swap_fixed_lj) = \
                setFakeGridProperties(bound_swap_fixed_cl, bound_swap_fixed_lj, params)

        bound_swap_fixed_cl.add(swap_water_mols, MGIdx(0))
        bound_swap_fixed_lj.add(swap_water_mols, MGIdx(0))
        bound_swap_fixed_cl.add( fixed_bound_group, MGIdx(1) )
        bound_swap_fixed_lj.add( fixed_bound_group, MGIdx(1) )
    else:
        bound_swap_fixed = GridFF("bound:swap-fixed")
        bound_swap_fixed = setCLJProperties(bound_swap_fixed, params)
        # The swap water cluster is more mobile, so needs a bigger grid buffer
        bound_swap_fixed = setGridProperties(bound_swap_fixed, params, 6*angstrom)

        bound_swap_fixed.add(swap_water_mols, MGIdx(0))
        bound_swap_fixed.addFixedAtoms(fixed_bound_group)

    ###
    ### FORCEFIELDS INVOLVING THE LIGAND/CLUSTER FREE LEG
    ###

    # forcefield holding the energy between the ligand and the mobile atoms
    # in the free leg
    free_ligand_mobile = InterGroupSoftCLJFF("free:ligand-mobile")
    free_ligand_mobile = setCLJProperties(free_ligand_mobile, params)
    free_ligand_mobile = setSoftCoreProperties(free_ligand_mobile, params)

    free_ligand_mobile.add(ligand_mols, MGIdx(0))
    free_ligand_mobile.add(mobile_free_mols, MGIdx(1))

    # forcefield holding the energy between the swap water cluster and the
    # mobile atoms of the free leg
    free_swap_mobile = InterGroupSoftCLJFF("free:swap-mobile")
    free_swap_mobile = setCLJProperties(free_swap_mobile, params)
    free_swap_mobile = setSoftCoreProperties(free_swap_mobile, params)

    free_swap_mobile.add(swap_water_mols, MGIdx(0))
    free_swap_mobile.add(mobile_free_mols, MGIdx(1))

    # forcefield holding the energy between the ligand and the fixed atoms
    # in the free leg
    if disable_grid:
        free_ligand_fixed_cl = InterGroupCLJFF("free:ligand_fixed-cl")
        free_ligand_fixed_lj = InterGroupCLJFF("free:ligand_fixed-lj")
        free_ligand_fixed_cl = setCLJProperties(free_ligand_fixed_cl, params)
        free_ligand_fixed_lj = setCLJProperties(free_ligand_fixed_lj, params)
        (free_ligand_fixed_cl,free_ligand_fixed_lj) = \
             setFakeGridProperties(free_ligand_fixed_cl,free_ligand_fixed_lj, params)

        free_ligand_fixed_cl.add(ligand_mols, MGIdx(0))
        free_ligand_fixed_lj.add(ligand_mols, MGIdx(0))
        free_ligand_fixed_cl.add(fixed_free_group, MGIdx(1))
        free_ligand_fixed_lj.add(fixed_free_group, MGIdx(1))
    else:
        free_ligand_fixed = GridFF("free:ligand-fixed")
        free_ligand_fixed = setCLJProperties(free_ligand_fixed, params)
        free_ligand_fixed = setGridProperties(free_ligand_fixed, params)

        free_ligand_fixed.add(ligand_mols, MGIdx(0))
        free_ligand_fixed.addFixedAtoms(fixed_free_group)

    # forcefield holding the energy between the swap water cluster and the 
    # fixed atoms in the free leg
    if disable_grid:
        free_swap_fixed_cl = InterGroupCLJFF("free:swap-fixed-cl")
        free_swap_fixed_lj = InterGroupCLJFF("free:swap-fixed-lj")
        free_swap_fixed_cl = setCLJProperties(free_swap_fixed_cl, params)
        free_swap_fixed_lj = setCLJProperties(free_swap_fixed_lj, params)
        (free_swap_fixed_cl,free_swap_fixed_lj) = \
            setFakeGridProperties(free_swap_fixed_cl,free_swap_fixed_lj,params)

        free_swap_fixed_cl.add(swap_water_mols, MGIdx(0))
        free_swap_fixed_lj.add(swap_water_mols, MGIdx(0))
        free_swap_fixed_cl.add(fixed_free_group, MGIdx(1))
        free_swap_fixed_lj.add(fixed_free_group, MGIdx(1))
    else:
        free_swap_fixed = GridFF("free:swap-fixed")
        free_swap_fixed = setCLJProperties(free_swap_fixed, params)
        # The swap water cluster is more mobile so needs a bigger grid buffer
        free_swap_fixed = setGridProperties(free_swap_fixed, params, 6*angstrom)

        free_swap_fixed.add(swap_water_mols, MGIdx(0))
        free_swap_fixed.addFixedAtoms(fixed_free_group)

    ###
    ### FORCEFIELDS LOCAL ONLY TO THE BOUND LEG
    ###
    bound_forcefields = []
    bound_forcefields_cl = []
    bound_forcefields_lj = []

    # forcefield holding the energy between the bound leg mobile atoms and  
    # the bound leg fixed atoms
    if disable_grid:
        bound_mobile_fixed_cl = InterGroupCLJFF("bound:mobile-fixed-cl")
        bound_mobile_fixed_lj = InterGroupCLJFF("bound:mobile-fixed-lj")
        bound_mobile_fixed_cl = setCLJProperties(bound_mobile_fixed_cl, params)
        bound_mobile_fixed_lj = setCLJProperties(bound_mobile_fixed_lj, params)
        (bound_mobile_fixed_cl,bound_mobile_fixed_lj) = \
           setFakeGridProperties(bound_mobile_fixed_cl,bound_mobile_fixed_lj,params)

        bound_mobile_fixed_cl.add(mobile_buffered_bound_mols, MGIdx(0))
        bound_mobile_fixed_lj.add(mobile_buffered_bound_mols, MGIdx(0))
        bound_mobile_fixed_cl.add(fixed_bound_group, MGIdx(1))
        bound_mobile_fixed_lj.add(fixed_bound_group, MGIdx(1))

        bound_forcefields_cl.append(bound_mobile_fixed_cl)
        bound_forcefields_lj.append(bound_mobile_fixed_lj)
    else:
        bound_mobile_fixed = GridFF("bound:mobile-fixed")
        bound_mobile_fixed = setCLJProperties(bound_mobile_fixed, params)
        bound_mobile_fixed = setGridProperties(bound_mobile_fixed, params)

        # we use mobile_buffered_bound_group as this group misses out atoms that are bonded
        # to fixed atoms (thus preventing large energies caused by incorrect non-bonded calculations)
        bound_mobile_fixed.add(mobile_buffered_bound_mols, MGIdx(0))
        bound_mobile_fixed.addFixedAtoms(fixed_bound_group)

        bound_forcefields.append(bound_mobile_fixed)

    # forcefield holding the intermolecular energy between all bound molecules
    bound_mobile_mobile = InterCLJFF("bound:mobile-mobile")
    bound_mobile_mobile = setCLJProperties(bound_mobile_mobile, params)

    bound_mobile_mobile.add(mobile_bound_mols)

    bound_forcefields.append(bound_mobile_mobile)

    # intramolecular energy of the protein
    if bound_protein_intra_mols.nMolecules() > 0:
        protein_intraclj = IntraCLJFF("bound:protein_intraclj")
        protein_intraclj = setCLJProperties(protein_intraclj, params)

        protein_intraff = InternalFF("bound:protein_intra")

        for molnum in bound_protein_intra_mols.molNums():
            protein_mol = bound_protein_intra_mols[molnum].join()
            protein_intraclj.add(protein_mol)
            protein_intraff.add(protein_mol)

        bound_forcefields.append(protein_intraclj)
        bound_forcefields.append(protein_intraff)

    # intramolecular energy of any other solutes
    if bound_solute_intra_mols.nMolecules() > 0:
        solute_intraclj = IntraCLJFF("bound:solute_intraclj")
        solute_intraclj = setCLJProperties(solute_intraclj, params)

        solute_intraff = InternalFF("bound:solute_intra")

        for molnum in bound_solute_intra_mols.molNums():
            solute_mol = bound_solute_intra_mols[molnum].join()
            solute_intraclj.add(solute_mol)
            solute_intraff.add(solute_mol)

        bound_forcefields.append(solute_intraclj)
        bound_forcefields.append(solute_intraff)

    ###
    ### FORCEFIELDS LOCAL ONLY TO THE FREE LEG
    ###
    free_forcefields = []
    free_forcefields_cl = []
    free_forcefields_lj = []    

    # forcefield holding the energy between the mobile free molecules and the
    # fixed free molecules
    if disable_grid:
        free_mobile_fixed_cl = InterGroupCLJFF("free:mobile-fixed-cl")
        free_mobile_fixed_lj = InterGroupCLJFF("free:mobile-fixed-lj")
        free_mobile_fixed_cl = setCLJProperties(free_mobile_fixed_cl, params)
        free_mobile_fixed_lj = setCLJProperties(free_mobile_fixed_lj, params)
        (free_mobile_fixed_cl,free_mobile_fixed_lj) = \
            setFakeGridProperties(free_mobile_fixed_cl,free_mobile_fixed_lj,params)

        free_mobile_fixed_cl.add(mobile_free_mols, MGIdx(0))
        free_mobile_fixed_lj.add(mobile_free_mols, MGIdx(0))
        free_mobile_fixed_cl.add(fixed_free_group, MGIdx(1))
        free_mobile_fixed_lj.add(fixed_free_group, MGIdx(1))

        free_forcefields_cl.append(free_mobile_fixed_cl)
        free_forcefields_lj.append(free_mobile_fixed_lj)
    else:
        free_mobile_fixed = GridFF("free:mobile-fixed")
        free_mobile_fixed = setCLJProperties(free_mobile_fixed, params)
        free_mobile_fixed = setGridProperties(free_mobile_fixed, params)

        free_mobile_fixed.add(mobile_free_mols, MGIdx(0))
        free_mobile_fixed.addFixedAtoms(fixed_free_group)

        free_forcefields.append(free_mobile_fixed)
    
    # forcefield holding the intermolecular energy between the mobile free molecules
    free_mobile_mobile = InterCLJFF("free:mobile-mobile")
    free_mobile_mobile = setCLJProperties(free_mobile_mobile, params)

    free_mobile_mobile.add(mobile_free_mols)

    free_forcefields.append(free_mobile_mobile)

    ###
    ### NOW ADD THE FORCEFIELDS TO THE SYSTEM
    ###
    ###
    ### SETTING THE FORCEFIELD EXPRESSIONS
    ###

    if disable_grid:
        bound_ligand_fixed_nrg = bound_ligand_fixed_cl.components().coulomb() + \
                                 bound_ligand_fixed_lj.components().lj()
        free_ligand_fixed_nrg = free_ligand_fixed_cl.components().coulomb() + \
                                free_ligand_fixed_lj.components().lj()
        bound_swap_fixed_nrg = bound_swap_fixed_cl.components().coulomb() + \
                               bound_swap_fixed_lj.components().lj()
        free_swap_fixed_nrg = free_swap_fixed_cl.components().coulomb() + \
                              free_swap_fixed_lj.components().lj()
    else:
        bound_ligand_fixed_nrg = bound_ligand_fixed.components().total()
        free_ligand_fixed_nrg = free_ligand_fixed.components().total()
        bound_swap_fixed_nrg = bound_swap_fixed.components().total()
        free_swap_fixed_nrg = free_swap_fixed.components().total()

    ligand_bound_nrg_sym = Symbol("E_{ligand:bound}")

    ligand_bound_nrg = ligand_intraclj.components().total() + \
                       ligand_intraff.components().total() + \
                       bound_ligand_mobile.components().total(0) + \
                       bound_ligand_fixed_nrg

    ligand_bound_nrg_f_sym = Symbol("E_{ligand:bound_{f}}")
    ligand_bound_nrg_f = ligand_intraclj.components().total() + \
                         ligand_intraff.components().total() + \
                         bound_ligand_mobile.components().total(1) + \
                         bound_ligand_fixed_nrg

    ligand_bound_nrg_b_sym = Symbol("E_{ligand:bound_{b}}")
    ligand_bound_nrg_b = ligand_intraclj.components().total() + \
                         ligand_intraff.components().total() + \
                         bound_ligand_mobile.components().total(2) + \
                         bound_ligand_fixed_nrg

    ligand_free_nrg_sym = Symbol("E_{ligand:free}")
    ligand_free_nrg = ligand_intraclj.components().total() + \
                      ligand_intraff.components().total() + \
                      free_ligand_mobile.components().total(0) + \
                      free_ligand_fixed_nrg

    ligand_free_nrg_f_sym = Symbol("E_{ligand:free_{f}}")
    ligand_free_nrg_f = ligand_intraclj.components().total() + \
                        ligand_intraff.components().total() + \
                        free_ligand_mobile.components().total(1) + \
                        free_ligand_fixed_nrg

    ligand_free_nrg_b_sym = Symbol("E_{ligand:free_{b}}")
    ligand_free_nrg_b = ligand_intraclj.components().total() + \
                        ligand_intraff.components().total() + \
                        free_ligand_mobile.components().total(2) + \
                        free_ligand_fixed_nrg

    lam = Symbol("lambda")
    lam_f = Symbol("lambda_{f}")
    lam_b = Symbol("lambda_{b}")

    S_sym = Symbol("S")
    S_scl = S_sym - 4*(S_sym-1)*(lam-0.5)**2
    S_scl_f = S_sym - 4*(S_sym-1)*(lam_f-0.5)**2
    S_scl_b = S_sym - 4*(S_sym-1)*(lam_b-0.5)**2

    swap_bound_nrg_sym = Symbol("E_{swap:bound}")
    swap_bound_nrg = ((S_scl) * swap_interclj.components().coulomb()) + \
                                 swap_interclj.components().lj() + \
                     bound_swap_mobile.components().total(0) + \
                     bound_swap_fixed_nrg

    swap_bound_nrg_f_sym = Symbol("E_{swap:bound_{f}}")
    swap_bound_nrg_f = ((S_scl_f) * swap_interclj.components().coulomb()) + \
                                     swap_interclj.components().lj() + \
                         bound_swap_mobile.components().total(1) + \
                         bound_swap_fixed_nrg

    swap_bound_nrg_b_sym = Symbol("E_{swap:bound_{b}}")
    swap_bound_nrg_b = ((S_scl_b) * swap_interclj.components().coulomb()) + \
                                     swap_interclj.components().lj() + \
                        bound_swap_mobile.components().total(2) + \
                        bound_swap_fixed_nrg

    swap_free_nrg_sym = Symbol("E_{swap:free}")
    swap_free_nrg = ((S_scl) * swap_interclj.components().coulomb()) + \
                                swap_interclj.components().lj() + \
                     free_swap_mobile.components().total(0) + \
                     free_swap_fixed_nrg

    swap_free_nrg_f_sym = Symbol("E_{swap:free_{f}}")
    swap_free_nrg_f = ((S_scl_f) * swap_interclj.components().coulomb()) + \
                                    swap_interclj.components().lj() + \
                         free_swap_mobile.components().total(1) + \
                         free_swap_fixed_nrg

    swap_free_nrg_b_sym = Symbol("E_{swap:free_{b}}")
    swap_free_nrg_b = ((S_scl_b) * swap_interclj.components().coulomb()) + \
                                    swap_interclj.components().lj() + \
                        free_swap_mobile.components().total(2) + \
                        free_swap_fixed_nrg

    system.add(ligand_intraclj)
    system.add(ligand_intraff)
    system.add(swap_interclj)
    system.add(bound_ligand_mobile)
    system.add(bound_swap_mobile)
    system.add(free_ligand_mobile)
    system.add(free_swap_mobile)

    if disable_grid:
        system.add(bound_ligand_fixed_cl)
        system.add(bound_swap_fixed_cl)
        system.add(free_ligand_fixed_cl)
        system.add(free_swap_fixed_cl)
        system.add(bound_ligand_fixed_lj)
        system.add(bound_swap_fixed_lj)
        system.add(free_ligand_fixed_lj)
        system.add(free_swap_fixed_lj)
    else:
        system.add(bound_ligand_fixed)
        system.add(bound_swap_fixed)
        system.add(free_ligand_fixed)
        system.add(free_swap_fixed)

    system.setConstant(lam, 0.0)
    system.setConstant(lam_f, 0.0)
    system.setConstant(lam_b, 0.0)

    system.setComponent(S_sym, 0)

    system.setComponent(ligand_bound_nrg_sym, ligand_bound_nrg)
    system.setComponent(ligand_bound_nrg_f_sym, ligand_bound_nrg_f)
    system.setComponent(ligand_bound_nrg_b_sym, ligand_bound_nrg_b)

    system.setComponent(ligand_free_nrg_sym, ligand_free_nrg)
    system.setComponent(ligand_free_nrg_f_sym, ligand_free_nrg_f)
    system.setComponent(ligand_free_nrg_b_sym, ligand_free_nrg_b)

    system.setComponent(swap_bound_nrg_sym, swap_bound_nrg)
    system.setComponent(swap_bound_nrg_f_sym, swap_bound_nrg_f)
    system.setComponent(swap_bound_nrg_b_sym, swap_bound_nrg_b)

    system.setComponent(swap_free_nrg_sym, swap_free_nrg)
    system.setComponent(swap_free_nrg_f_sym, swap_free_nrg_f)
    system.setComponent(swap_free_nrg_b_sym, swap_free_nrg_b)

    bound_bound_nrg_sym = Symbol("E_{bound-bound}")
    bound_bound_nrg = None
    
    for bound_forcefield in bound_forcefields:
        if bound_bound_nrg is None:
            bound_bound_nrg = bound_forcefield.components().total()
        else:
            bound_bound_nrg = bound_bound_nrg + bound_forcefield.components().total()

        system.add(bound_forcefield)

    for bound_forcefield in bound_forcefields_cl:
        bound_bound_nrg = bound_bound_nrg + bound_forcefield.components().coulomb()
        system.add(bound_forcefield)

    for bound_forcefield in bound_forcefields_lj:
        bound_bound_nrg = bound_bound_nrg + bound_forcefield.components().lj()
        system.add(bound_forcefield)

    system.setComponent(bound_bound_nrg_sym, bound_bound_nrg)

    free_free_nrg_sym = Symbol("E_{free-free}")
    free_free_nrg = None

    for free_forcefield in free_forcefields:
        if free_free_nrg is None:
            free_free_nrg = free_forcefield.components().total()
        else:
            free_free_nrg = free_free_nrg + free_forcefield.components().total()

        system.add(free_forcefield)

    for free_forcefield in free_forcefields_cl:
        free_free_nrg = free_free_nrg + free_forcefield.components().coulomb()
        system.add(free_forcefield)

    for free_forcefield in free_forcefields_lj:
        free_free_nrg = free_free_nrg + free_forcefield.components().lj()
        system.add(free_forcefield)

    system.setComponent(free_free_nrg_sym, free_free_nrg)

    bound_nrg_sym = Symbol("E_{bound}")
    bound_nrg = bound_bound_nrg_sym + ((1-lam) * ligand_bound_nrg_sym) + (lam * swap_bound_nrg_sym)

    bound_nrg_f_sym = Symbol("E_{bound_{f}}")
    bound_nrg_f = bound_bound_nrg_sym + ((1-lam) * ligand_bound_nrg_f_sym) + (lam * swap_bound_nrg_f_sym)

    bound_nrg_b_sym = Symbol("E_{bound_{b}}")
    bound_nrg_b = bound_bound_nrg_sym + ((1-lam) * ligand_bound_nrg_b_sym) + (lam * swap_bound_nrg_b_sym)
    
    free_nrg_sym = Symbol("E_{free}")
    free_nrg = free_free_nrg_sym + (lam * ligand_free_nrg_sym) + ((1-lam) * swap_free_nrg_sym)

    free_nrg_f_sym = Symbol("E_{free_{f}}")
    free_nrg_f = free_free_nrg_sym + (lam * ligand_free_nrg_f_sym) + ((1-lam) * swap_free_nrg_f_sym)

    free_nrg_b_sym = Symbol("E_{free_{b}}")
    free_nrg_b = free_free_nrg_sym + (lam * ligand_free_nrg_b_sym) + ((1-lam) * swap_free_nrg_b_sym)

    total_nrg_sym = system.totalComponent()
    total_nrg = bound_nrg_sym + free_nrg_sym

    total_nrg_f_sym = Symbol("E_{total_{f}}")
    total_nrg_f = bound_nrg_f_sym + free_nrg_f_sym

    total_nrg_b_sym = Symbol("E_{total_{b}}")
    total_nrg_b = bound_nrg_b_sym + free_nrg_b_sym

    system.setComponent(bound_nrg_sym, bound_nrg)
    system.setComponent(bound_nrg_f_sym, bound_nrg_f)
    system.setComponent(bound_nrg_b_sym, bound_nrg_b)

    system.setComponent(free_nrg_sym, free_nrg)
    system.setComponent(free_nrg_f_sym, free_nrg_f)
    system.setComponent(free_nrg_b_sym, free_nrg_b)

    system.setComponent(total_nrg_sym, total_nrg)
    system.setComponent(total_nrg_f_sym, total_nrg_f)
    system.setComponent(total_nrg_b_sym, total_nrg_b)

    system.setComponent( Symbol("delta_nrg^{F}"), (total_nrg_f_sym - total_nrg_sym) )
    system.setComponent( Symbol("delta_nrg^{B}"), (total_nrg_sym - total_nrg_b_sym) )

    # Now add constraints. These are used to keep the identity of the 
    # swap water, to keep all lambda values between 0 and 1, and to
    # map the alpha values of the softcore forcefields to lambda
    print "\nCreating WSRC system constraints...\n"

    # Add the constraint that lambda_f is lambda + delta_lambda and
    # lambda_b is lambda - delta_lambda (kept to between 0 and 1)
    delta_lambda = readConfig("delta_lambda", params, 0.001)

    if delta_lambda > 1 or delta_lambda < 0.0000001:
        print "Weird value of delta_lambda (%s). Setting it to 0.001" % delta_lambda
        delta_lambda = 0.001

    system.setConstant( Symbol("delta_lambda"), delta_lambda )
    system.add( ComponentConstraint( lam_f, Min( lam + delta_lambda, 1 ) ) )
    system.add( ComponentConstraint( lam_b, Max( lam - delta_lambda, 0 ) ) )

    # now add alpha variables that can be used by the EnergyMonitors
    alpha_on = Symbol("alpha_on")
    alpha_off = Symbol("alpha_off")

    system.setConstant(alpha_on, 0)
    system.setConstant(alpha_off, 1)

    alpha_scale = readConfig("alpha_scale", params, 1.0)
    system.setConstant( Symbol("alpha_scale"), alpha_scale )
    system.add( ComponentConstraint( alpha_on, alpha_scale * lam ) )
    system.add( ComponentConstraint( alpha_off, alpha_scale * (1-lam) ) )

    # Now constrain alpha to follow lambda
    # First, the reference state (alpha0)
    system.add( PropertyConstraint( "alpha0", FFName("free:swap-mobile"), alpha_scale * lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("bound:swap-mobile"), alpha_scale * (1 - lam) ) )

    system.add( PropertyConstraint( "alpha0", FFName("bound:ligand-mobile"), alpha_scale * lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("free:ligand-mobile"), alpha_scale * (1 - lam) ) )

    # Now the forwards perturbed state (alpha1)
    system.add( PropertyConstraint( "alpha1", FFName("free:swap-mobile"),  alpha_scale * lam_f ) )
    system.add( PropertyConstraint( "alpha1", FFName("bound:swap-mobile"),  alpha_scale * (1 - lam_f) ) )

    system.add( PropertyConstraint( "alpha1", FFName("bound:ligand-mobile"),  alpha_scale * lam_f ) )
    system.add( PropertyConstraint( "alpha1", FFName("free:ligand-mobile"),  alpha_scale * (1 - lam_f) ) )

    # Now the backwards perturbed state (alpha2)
    system.add( PropertyConstraint( "alpha2", FFName("free:swap-mobile"),  alpha_scale * lam_b ) )
    system.add( PropertyConstraint( "alpha2", FFName("bound:swap-mobile"),  alpha_scale * (1 - lam_b) ) )

    system.add( PropertyConstraint( "alpha2", FFName("bound:ligand-mobile"),  alpha_scale * lam_b ) )
    system.add( PropertyConstraint( "alpha2", FFName("free:ligand-mobile"),  alpha_scale * (1 - lam_b) ) )

    # Now lets create all of the groups for moves based on the above

    # All solvent molecules in the bound and free legs are moved together
    mobile_solvent = MoleculeGroup("mobile_solvent")
    mobile_solvent.add( mobile_bound_solvents_group.molecules() )
    mobile_solvent.add( mobile_free_water_group.molecules() )

    system.add( mobile_solvent )

    # All protein sidechains are moved together
    mobile_sidechains = MoleculeGroup("mobile_sidechains")
    mobile_sidechains.add(mobile_bound_protein_sidechains_group.molecules())
    
    system.add( mobile_sidechains )

    # All protein backbones are moved together
    mobile_backbones = MoleculeGroup("mobile_backbones")
    mobile_backbones.add(mobile_bound_protein_backbones_group.molecules())

    system.add( mobile_backbones )

    # All solute molecules are moved together
    mobile_solutes = MoleculeGroup("mobile_solutes")
    mobile_solutes.add(mobile_bound_solutes_group.molecules())

    system.add( mobile_solutes )

    # The ligand is moved in its own group
    mobile_ligand = MoleculeGroup("mobile_ligand")
    mobile_ligand.add(ligand_mol)

    system.add( mobile_ligand )

    # The swap water cluster is moved in its own group
    mobile_swap = MoleculeGroup("mobile_swap_water")
    mobile_swap.add(swap_water_group.molecules())

    system.add( mobile_swap )

    print "Adding the identity constraint..."

    # Now add the constraint that keeps the identities of the
    # swap molecules. The swap molecules are chosen from all available mobile
    # water molecules. We need to build a group of all mobile water molecules that
    # are waters (as opposed to ions, as other molecules may be in mobile_solvent)
    mobile_water = MoleculeGroup("mobile_water")

    # The mobile water *must* contain the swap waters, so that they can be swapped
    mobile_water.add(swap_water_group)
    mobile_water.add(mobile_free_water_group)
    mobile_water.add(mobile_bound_water_group)

    print "The number of candidates for the swap water equals: %d" % mobile_water.nMolecules()

    system.add(mobile_water)
    system.add( IdentityConstraint(identity_points, mobile_water, { "space" : Cartesian() } ) )

    # Apply all of the constraints
    system.applyConstraints()

    ###
    ### ADD THE SYSTEM MONITORS
    ###

    # Now we need to add the monitors...
    print "\nAdding system monitors..."
    temperature = readConfig(params, "temperature", 25*celsius)

    system.add( "delta_g^{F}", MonitorComponent( Symbol("delta_nrg^{F}"),
                                                 FreeEnergyAverage(temperature) ) )

    system.add( "delta_g^{B}", MonitorComponent( Symbol("delta_nrg^{B}"),
                                                 FreeEnergyAverage(temperature) ) )
    
    # we will monitor the average energy between the swap cluster/ligand and each
    # residue with mobile sidechain, and each mobile solute
    monitor_prosol = None

    if mobile_solutes.isEmpty():
        monitor_prosol = mobile_sidechains
    elif mobile_sidechains.isEmpty():
        monitor_prosol = mobile_solutes
    else:
        monitor_prosol = MoleculeGroup("monitored_protein_solute")
        monitor_prosol.add(mobile_sidechains)
        monitor_prosol.add(mobile_solutes)
        system.add(monitor_prosol)

    ligand_protein_nrgmon = EnergyMonitor(ligand_group, monitor_prosol)
    ligand_protein_nrgmon.setAlphaComponent(alpha_on)
    swapwater_protein_nrgmon = EnergyMonitor(swap_water_group, monitor_prosol)
    swapwater_protein_nrgmon.setAlphaComponent(alpha_off)

    nrgmons = {}
    nrgmons["ligand_protein_solute_nrgmon"] = ligand_protein_nrgmon
    nrgmons["swapwater_protein_solute_nrgmon"] = swapwater_protein_nrgmon
    
    # because the water molecules can diffuse, we find all waters within
    # a certain distance of the ligand, and then identify them using identity
    # points (placed at the center of the initial positions of the waters),
    # and then monitor those...
    boundwater_points = []
    freewater_points = []

    water_monitor_distance = readConfig("water monitor distance", params, 5.0)

    for molnum in mobile_bound_water_group.molNums():
        water_mol = mobile_bound_water_group[molnum].molecule()
        if getMinimumDistance(ligand_mol,water_mol) < water_monitor_distance:
            # we should monitor this water
            boundwater_points.append( VectorPoint(water_mol.evaluate().center()) )
    
    for molnum in mobile_free_water_group.molNums():
        #this is a mobile water, so a candidate for monitoring
        water_mol = mobile_free_water_group[molnum].molecule()
        if getMinimumDistance(ligand_mol,water_mol) < water_monitor_distance:
            # we should monitor this water
            freewater_points.append( VectorPoint(water_mol.evaluate().center()) )

    system.add(mobile_bound_water_group)
    system.add(mobile_free_water_group)

    boundwater_assigner = IDAssigner(boundwater_points, mobile_bound_water_group,
                                     {"space" : Cartesian()})

    boundwater_assigner.update(system)

    freewater_assigner = IDAssigner(freewater_points, mobile_free_water_group,
                                    {"space" : Cartesian()})

    freewater_assigner.update(system)

    ligand_boundwater_nrgmon = EnergyMonitor(ligand_group, boundwater_assigner)
    ligand_boundwater_nrgmon.setAlphaComponent(alpha_on)
    swapwater_boundwater_nrgmon = EnergyMonitor(swap_water_group, boundwater_assigner)
    swapwater_boundwater_nrgmon.setAlphaComponent(alpha_off)

    ligand_freewater_nrgmon = EnergyMonitor(ligand_group, freewater_assigner)
    ligand_freewater_nrgmon.setAlphaComponent(alpha_off)
    swapwater_freewater_nrgmon = EnergyMonitor(swap_water_group, freewater_assigner)
    swapwater_freewater_nrgmon.setAlphaComponent(alpha_on)

    nrgmons["ligand_boundwater_nrgmon"] = ligand_boundwater_nrgmon
    nrgmons["swapwater_boundwater_nrgmon"] = swapwater_boundwater_nrgmon
    nrgmons["ligand_freewater_nrgmon"] = ligand_freewater_nrgmon
    nrgmons["swapwater_freewater_nrgmon"] = swapwater_freewater_nrgmon

    coulomb_power = readConfig("coulomb power", params, 0)
    shift_delta = readConfig("shift delta", params, 1.2)
    nrgmon_frequency = readConfig("energy monitor frequency", params, 1000)

    for key in nrgmons.keys():
        nrgmons[key].setCoulombPower(coulomb_power)
        nrgmons[key].setShiftDelta(shift_delta)

        system.add(key, nrgmons[key], nrgmon_frequency)

    return system


def makeRETI(system, moves, params):
    """This function replicates 'system' over each of the supplied lambda values
       and uses 'moves' to sample each of the replicated systems. This uses RETI
       to perform replica exchange moves across lambda"""

    lam = Symbol("lambda")
    lambda_values = readConfig("lambda values", params, [0.001, 0.999])
    nsubmoves = readConfig("nsubmoves", params, 50000)

    replicas = Replicas( len(lambda_values) )

    replicas.setSubSystem(system)
    replicas.setSubMoves(moves)
    replicas.setNSubMoves(nsubmoves)
    replicas.setLambdaComponent(lam)
    replicas.setRecordAllStatistics(True)        

    random_seed = readConfig("random seed", params)
    
    if random_seed is None:
        random_seed = RanGenerator().randInt(100000,1000000)
        print "RETI system using generated random number seed %d" % random_seed
    else:
        print "RETI system using supplied random number seed %d" % random_seed
    
    replicas.setGenerator( RanGenerator(random_seed+5) )

    for i in range(0, len(lambda_values)):
        # set the initial lambda value for this replica
        replicas.setLambdaValue(i, lambda_values[i])

    # Now add monitors for each replica that will copy back
    nrgmons = [ "delta_g^{F}", "delta_g^{B}",
                "ligand_protein_solute_nrgmon", "swapwater_protein_solute_nrgmon",
                "ligand_boundwater_nrgmon", "swapwater_boundwater_nrgmon",
                "ligand_freewater_nrgmon", "swapwater_freewater_nrgmon" ]

    for nrgmon in nrgmons:
        replicas.add( nrgmon, MonitorMonitor(MonitorName(nrgmon), True) )

    # now create the replica exchange moves for the replicas
    replica_moves = RepExMove()
    replica_moves.setGenerator( RanGenerator(random_seed+7) )

    print "\nReturning the WSRC RETI replicas and moves..."
    return (replicas, replica_moves)


def calculatePMF(gradients):
    """This function calculates and return the PMF given the passed series
       of lambda values and gradients"""

    pmf = {}

    lamvals = gradients.keys()
    lamvals.sort()

    if lamvals[0] != 0:
        #we need to start from 0
        gradients[0] = gradients[lamvals[0]]
        lamvals.insert(0, 0)

    if lamvals[-1] != 1:
        #we need to end with 1
        gradients[1] = gradients[lamvals[-1]]
        lamvals.append(1)

    #start at 0
    pmf[ lamvals[0] ] = 0.0

    for i in range(1,len(lamvals)):
        last_lam = lamvals[i-1]
        this_lam = lamvals[i]

        delta_lam = this_lam - last_lam

        pmf[this_lam] = pmf[last_lam] + (delta_lam * 0.5 * (gradients[this_lam] + \
                                                            gradients[last_lam]))

    return pmf


def calculatePMFs(gradients):
    
    pmfs = []

    lamvals = gradients.keys()

    npmfs = len(gradients[lamvals[0]])

    for i in range(0,npmfs):
        grads = {}

        for lamval in lamvals:
            grads[lamval] = gradients[lamval][i]

        pmfs.append( calculatePMF(grads) )

    return pmfs


def extractEnergies(protein_nrgmon, boundwater_nrgmon, freewater_nrgmon):

    nrgs = []

    nrgmons = [protein_nrgmon, boundwater_nrgmon, freewater_nrgmon]

    group_cnrgs = []
    group_ljnrgs = []
    group_totalnrgs = []

    for i in range(0,3):
        nrgmon = nrgmons[i]

        if i == 2:
            nrgs.append( (group_cnrgs, group_ljnrgs, group_totalnrgs) )

            group_cnrgs = []
            group_ljnrgs = []
            group_totalnrgs = []

        cnrgs = nrgmon.coulombEnergies()
        ljnrgs = nrgmon.ljEnergies()

        for j in range(0,cnrgs.nColumns()):
            total_cnrg = 0
            total_ljnrg = 0

            for i in range(0,cnrgs.nRows()):
                total_cnrg += cnrgs(i,j).average()
                total_ljnrg += ljnrgs(i,j).average()

            group_cnrgs.append(total_cnrg)
            group_ljnrgs.append(total_ljnrg)
            group_totalnrgs.append(total_cnrg+total_ljnrg)

    nrgs.append( (group_cnrgs, group_ljnrgs, group_totalnrgs) )

    return nrgs


def subtractEnergies(nrgs0, nrgs1):

    nrgs = []

    for j in range(0,2):
        delta_cnrgs = []
        delta_ljnrgs = []
        delta_total = []

        for i in range(0, len(nrgs0[j][0])):
            delta_cnrgs.append( nrgs0[j][0][i] - nrgs1[j][0][i] )
            delta_ljnrgs.append( nrgs0[j][1][i] - nrgs1[j][1][i] )
            delta_total.append( nrgs0[j][2][i] - nrgs1[j][2][i] )

        nrgs.append( (delta_cnrgs, delta_ljnrgs, delta_total) )

    return nrgs


def getName(view):
   try:
       residue = view.residue()
       return "%s:%s" % (residue.name().value(), residue.number().value())
   except:
       return "%s:%s" % (view.name().value(), view.number().value())


def writeMonitoredGroups(replica, filename):

    pdbgroup = MoleculeGroup("pdb")

    system = replica.subSystem()
    ligand_protein_nrgmon = system[MonitorName("ligand_protein_nrgmon")]
    ligand_boundwater_nrgmon = system[MonitorName("ligand_boundwater_nrgmon")]
    swapwater_freewater_nrgmon = system[MonitorName("swapwater_freewater_nrgmon")]
    system = None

    for view in ligand_protein_nrgmon.views0():
        pdbgroup.add(view)
                                                 
    for view in swapwater_freewater_nrgmon.views0():
        pdbgroup.add(view)

    for view in ligand_protein_nrgmon.views1():
        pdbgroup.add(view)

    for view in ligand_boundwater_nrgmon.views1():
        view = view.residue().edit().setProperty("PDB-residue-name", "PRT").commit()
        pdbgroup.add(view)

    ligand_protein_nrgmon = None
    ligand_boundwater_nrgmon = None

    for view in swapwater_freewater_nrgmon.views1():
        pdbgroup.add(view)

    swapwater_freewater_nrgmon = None

    PDB().write(pdbgroup, filename)

    pdbgroup = None

    print "Written the monitored group to file %s" % filename


def loadWSRC(params):
    # Load the WSRC system and moves using the passed parameters
    # This returns (wsrc_system, wsrc_moves), ready for simulation

    ligand_name = readConfig("ligand name", params, "LIG")
    reflection_radius = readConfig("reflection radius", params, None)
    protein_topfile = readConfig("protein topfile", params, "proteinbox.top")
    protein_crdfile = readConfig("protein crdfile", params, "proteinbox.crd")
    protein_s3file = readConfig("protein s3file", params, "proteinbox.s3")
    water_topfile = readConfig("water topfile", params, "%s/waterbox.top" % wsrc_tools_dir)
    water_crdfile = readConfig("water crdfile", params, "%s/waterbox.crd" % wsrc_tools_dir)
    water_s3file = readConfig("water s3file", params, "waterbox.s3")

    print "Loading the protein box system..."

    if os.path.exists(protein_s3file):
        print "Loading existing s3 file %s..." % protein_s3file
        proteinsys = Sire.Stream.load(protein_s3file)

    else:
        print "Loading from Amber files %s / %s..." % (protein_topfile, protein_crdfile)
        # Add the name of the ligand to the list of solute molecules
        proteinsys_scheme = NamingScheme()
        proteinsys_scheme.addSoluteResidueName(ligand_name)

        # Load up the system. This will automatically find the protein, solute, water, solvent
        # and ion molecules and assign them to different groups
        proteinsys = createSystem(protein_topfile, protein_crdfile, proteinsys_scheme)
        ligand_mol = findMolecule(proteinsys, ligand_name)

        if ligand_mol is None:
            print "Cannot find the ligand (%s) in the set of loaded molecules!" % ligand_name
            sys.exit(-1)

        # Center the system with the ligand at (0,0,0)
        proteinsys = centerSystem(proteinsys, ligand_mol)
        ligand_mol = proteinsys[ligand_mol.number()].molecule()

        proteinsys = addFlexibility(proteinsys, Vector(0,0,0), reflection_radius, proteinsys_scheme )
        Sire.Stream.save(proteinsys, protein_s3file)

    ligand_mol = findMolecule(proteinsys, ligand_name)

    if ligand_mol is None:
        print "Cannot find the ligand (%s) in the set of loaded molecules!" % ligand_name
        sys.exit(-1)

    print "Loading the water box system..."

    if os.path.exists(water_s3file):
        print "Loading from existing s3 file %s..." % water_s3file
        watersys = Sire.Stream.load(water_s3file)
    else:
        print "Loading from Amber files %s / %s..." % (water_topfile, water_crdfile)
        watersys = createSystem(water_topfile, water_crdfile)
        watersys = addFlexibility(watersys, Vector(0,0,0), reflection_radius)
        Sire.Stream.save(watersys, water_s3file)

    print "Building the WSRC forcefields"

    wsrc_system = mergeSystems(proteinsys, watersys, ligand_mol, params)
    wsrc_moves = createWSRCMoves(wsrc_system, params)

    return (wsrc_system, wsrc_moves)


def analyseWSRC(replicas, iteration, params):
    """This function is used to perform all analysis of iteration 'it' of the passed WSRC system"""

    print "Analysing iteration %d..." % iteration
    outdir = readConfig("output directory", params, "output")

    if not os.path.exists(outdir):
        os.makedirs(outdir)

    # read the value of delta_lambda from the first system
    system = replicas[0].subSystem()
    delta_lambda = system.constant(Symbol("delta_lambda"))

    logfile = "%s/results_%0004d.log" % (outdir, iteration)

    FILE = open(logfile, "w")

    print >>FILE,"==========================="
    print >>FILE," Results for iteration %d" % iteration
    print >>FILE,"==========================="

    nreplicas = replicas.nReplicas()

    # extract all of the monitors from the replicas
    lambda_values = []

    dg_f = {}
    dg_b = {}

    proteinbox_dg = {}
    proteinbox_dg_coul = {}
    proteinbox_dg_lj = {}

    waterbox_dg = {}
    waterbox_dg_coul = {}
    waterbox_dg_lj = {}

    proteinbox_views = None
    waterbox_views = None

    for i in range(0, nreplicas):
        replica = replicas[i]
        monitors = replica.monitors()
        lamval = replica.lambdaValue()
        lambda_values.append(lamval)

        dg = monitors[MonitorName("delta_g^{F}")][-1]
        dg_f[lamval] = dg.accumulator().average() / delta_lambda
 
        dg = monitors[MonitorName("delta_g^{B}")][-1]
        dg_b[lamval] = dg.accumulator().average() / delta_lambda

        ligand_protein_nrgmon = monitors[MonitorName("ligand_protein_solute_nrgmon")][-1]
        ligand_boundwater_nrgmon = monitors[MonitorName("ligand_boundwater_nrgmon")][-1]
        ligand_freewater_nrgmon = monitors[MonitorName("ligand_freewater_nrgmon")][-1]

        swapwater_protein_nrgmon = monitors[MonitorName("swapwater_protein_solute_nrgmon")][-1]
        swapwater_boundwater_nrgmon = monitors[MonitorName("swapwater_boundwater_nrgmon")][-1]
        swapwater_freewater_nrgmon = monitors[MonitorName("swapwater_freewater_nrgmon")][-1]

        # the ligand and swapwater monitors use the same views
        # and are the same for all lambda values
        if proteinbox_views is None:
            proteinbox_views = []
            waterbox_views = []

            for view in ligand_protein_nrgmon.views1():
                proteinbox_views.append(view)

            for view in ligand_boundwater_nrgmon.views1():
                proteinbox_views.append(view)

            for view in ligand_freewater_nrgmon.views1():
                waterbox_views.append(view)

        ligand_nrgs = extractEnergies(ligand_protein_nrgmon, 
                                      ligand_boundwater_nrgmon,
                                      ligand_freewater_nrgmon)

        swapwater_nrgs = extractEnergies(swapwater_protein_nrgmon, 
                                         swapwater_boundwater_nrgmon,
                                         swapwater_freewater_nrgmon)

        delta_nrgs = subtractEnergies( swapwater_nrgs, ligand_nrgs)

        proteinbox_dg_coul[lamval] = delta_nrgs[0][0]
        proteinbox_dg_lj[lamval] = delta_nrgs[0][1]
        proteinbox_dg[lamval] = delta_nrgs[0][2]

        waterbox_dg_coul[lamval] = delta_nrgs[1][0]
        waterbox_dg_lj[lamval] = delta_nrgs[1][1]
        waterbox_dg[lamval] = delta_nrgs[1][2]

    pmf_f = calculatePMF(dg_f)
    pmf_b = calculatePMF(dg_b)

    proteinbox_pmfs = calculatePMFs(proteinbox_dg)
    proteinbox_pmfs_coul = calculatePMFs(proteinbox_dg_coul)
    proteinbox_pmfs_lj = calculatePMFs(proteinbox_dg_lj)

    waterbox_pmfs = calculatePMFs(waterbox_dg)
    waterbox_pmfs_coul = calculatePMFs(waterbox_dg_coul)
    waterbox_pmfs_lj = calculatePMFs(waterbox_dg_lj)

    # First, output the potential of mean force along the WSRC
    print >>FILE,"\nPotential of mean force (binding free energy)"

    lamvals = pmf_f.keys()
    lamvals.sort()

    print >>FILE,"Lambda    Forwards    Backwards"

    for lamval in lamvals:
        print >>FILE,"%f   %f   %f" % (lamval, pmf_f[lamval], pmf_b[lamval])

    bind_f = pmf_f[lamvals[-1]]
    bind_b = pmf_b[lamvals[-1]]

    proteinbox_bind = []
    proteinbox_bind_coul = []
    proteinbox_bind_lj = []

    waterbox_bind = []
    waterbox_bind_coul = []
    waterbox_bind_lj = []

    print >>FILE,"\n==============================="
    print >>FILE,"PROTEIN BOX FREE ENERGIES"
    print >>FILE,"===============================\n"

    # Now output the group-decomposed RDFs
    for i in range(0,len(proteinbox_views)):
        print >>FILE,"\nPotential of mean force for protein box group %d | %s" \
                                    % (i+1, getName(proteinbox_views[i]))

        print >>FILE,"Lambda   Total   Coulomb   LJ"

        lamvals = proteinbox_pmfs[i].keys()
        lamvals.sort()

        for lamval in lamvals:
            print >>FILE,"%f    %f    %f    %f" % \
               (lamval, proteinbox_pmfs[i][lamval], proteinbox_pmfs_coul[i][lamval],
                        proteinbox_pmfs_lj[i][lamval])

        proteinbox_bind.append( proteinbox_pmfs[i][lamvals[-1]] )
        proteinbox_bind_coul.append( proteinbox_pmfs_coul[i][lamvals[-1]] )
        proteinbox_bind_lj.append( proteinbox_pmfs_lj[i][lamvals[-1]] )

    print >>FILE,"\n==============================="
    print >>FILE,"WATER BOX FREE ENERGIES"
    print >>FILE,"===============================\n"

    # Now output the group-decomposed RDFs
    for i in range(0,len(waterbox_views)):
        print >>FILE,"\nPotential of mean force for water box group %d | %s" \
                                    % (i+1, getName(waterbox_views[i]))

        print >>FILE,"Lambda   Total   Coulomb   LJ"

        lamvals = waterbox_pmfs[i].keys()
        lamvals.sort()

        for lamval in lamvals:
            print >>FILE,"%f    %f    %f    %f" % \
               (lamval, waterbox_pmfs[i][lamval], waterbox_pmfs_coul[i][lamval],
                        waterbox_pmfs_lj[i][lamval])

        waterbox_bind.append( waterbox_pmfs[i][lamvals[-1]] )
        waterbox_bind_coul.append( waterbox_pmfs_coul[i][lamvals[-1]] )
        waterbox_bind_lj.append( waterbox_pmfs_lj[i][lamvals[-1]] )


    # Now write out the final binding free energies
    print >>FILE,"\n==================================="
    print >>FILE,"  BINDING FREE ENERGIES"
    print >>FILE,"===================================\n"       

    print >>FILE,"Protein box binding free energies\n"

    for i in range(0,len(proteinbox_views)):
        print >>FILE,"%s : %f kcal mol-1 (Coulomb = %f , LJ = %f )" \
                 % (getName(proteinbox_views[i]), -proteinbox_bind[i], \
                    -proteinbox_bind_coul[i], -proteinbox_bind_lj[i])

    print >>FILE,"\nWater box binding free energies\n"

    for i in range(0,len(waterbox_views)):
        print >>FILE,"%s : %f kcal mol-1 (Coulomb = %f , LJ = %f )" \
                 % (getName(waterbox_views[i]), -waterbox_bind[i], \
                    -waterbox_bind_coul[i], -waterbox_bind_lj[i])

    bind = -0.5 * (bind_f + bind_b)
    error = abs( 0.5*(bind_f - bind_b) )

    print >>FILE,"\nTotal Binding Free Energy = %f +/- %f kcal mol-1" \
                 % (bind, error)

    print >>FILE,"\n==================================="


def runSimulation(params):
    """This is a very high level function that does everything to run a WSRC simulation"""

    restart_file = readConfig("restart file", params, "wsrc_restart.s3")

    if os.path.exists(restart_file):
        (wsrc_system, wsrc_moves) = Sire.Stream.load(restart_file)
    else:
        # Load the WSRC protein and water boxes from the topology and coordinate
        # files and merge together into the WSRC system and moves object
        sysmoves_file = readConfig("sysmoves file", params, "wsrc_sysmoves.s3")

        if os.path.exists(sysmoves_file):
            (wsrc_system, wsrc_moves) = Sire.Stream.load(sysmoves_file)
        else:
            (wsrc_system, wsrc_moves) = loadWSRC(params)

            Sire.Stream.save( (wsrc_system, wsrc_moves), "pre_equil.s3")

            # Should add in some equilibration here...
            nequilmoves = readConfig("nequilmoves", params, 5000)
            print "Equilibrating the system (number of moves: %d)..." % nequilmoves
            wsrc_system = wsrc_moves.move(wsrc_system, nequilmoves, False)
            print "...equilibration complete"
            Sire.Stream.save( (wsrc_system, wsrc_moves), sysmoves_file)

        # Now replicate the WSRC system across all lambda values so that we
        # can run a RETI simulation
        (wsrc_system, wsrc_moves) = makeRETI(wsrc_system, wsrc_moves, params)

        Sire.Stream.save( (wsrc_system, wsrc_moves), restart_file )

    # see how many blocks of moves we still need to perform...
    nattempted = wsrc_moves.nMoves()
    nmoves = readConfig("nmoves", params, 300)

    print "Number of iterations to perform: %d. Number of iterations completed: %d." % (nmoves, nattempted)

    for i in range(nattempted+1, nmoves+1):
        print "Performing iteration %d..." % i
        sim = SupraSim.run( wsrc_system, wsrc_moves, 1, True )
        sim.wait()

        wsrc_system = sim.system()
        wsrc_moves = sim.moves()

        print "...iteration complete"
        analyseWSRC(wsrc_system, i, params)
        wsrc_system.clearAllStatistics()

        # write a restart file every 5 moves in case of crash or run out of time
        if i % 5 == 0:
            print "Saving the restart file from iteration %d." % i
            Sire.Stream.save( (wsrc_system, wsrc_moves), restart_file )

    print "All iterations complete. Saving the final state restart file."
    Sire.Stream.save( (wsrc_system, wsrc_moves), restart_file )
