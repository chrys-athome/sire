
from Sire.IO import *
from Sire.Mol import *
from Sire.System import *
from Sire.MM import *
from Sire.FF import *
from Sire.Base import *
from Sire.Units import *

import os

print "Loading the water molecules..."
waters = PDB().read("test/io/water.pdb")

tip4p = waters.moleculeAt(0).molecule()

protoms_dir = "%s/Work/ProtoMS" % os.getenv("HOME")

tip4p = tip4p.edit().rename("T4P").commit()

print "Parameterising the water as TIP4P..."
protoms = ProtoMS("%s/protoms2" % protoms_dir)
protoms.addParameterFile("%s/parameter/solvents.ff" % protoms_dir)

tip4p = protoms.parameterise(tip4p, ProtoMS.SOLVENT)

# set the polarisability of the atoms
tip4p = tip4p.edit() \
             .atom( AtomName("O00") ) \
                  .setProperty("polarisability", 0.0*angstrom3) \
                  .molecule() \
             .atom( AtomName("H01") ) \
                  .setProperty("polarisability", 0.0*angstrom3) \
                  .molecule() \
             .atom( AtomName("H02") ) \
                  .setProperty("polarisability", 0.0*angstrom3) \
                  .molecule() \
             .atom( AtomName("M03") ) \
                  .setProperty("polarisability", 1.45*angstrom3) \
                  .molecule() \
             .commit()

# Connect the atoms together...
connectivity = Connectivity(tip4p)
connectivity = connectivity.edit() \
                           .connect(AtomName("O00"), AtomName("H01")) \
                           .connect(AtomName("O00"), AtomName("H02")) \
                           .connect(AtomName("O00"), AtomName("M03")) \
                           .connect(AtomName("M03"), AtomName("H01")) \
                           .connect(AtomName("M03"), AtomName("H02")) \
                           .commit()

tip4p_chgs = tip4p.property("charge")
tip4p_ljs = tip4p.property("LJ")
tip4p_pol = tip4p.property("polarisability")

for i in range(0, waters.nMolecules()):
    water = waters.moleculeAt(i).molecule()
    water = water.edit().setProperty("charge", tip4p_chgs) \
                        .setProperty("LJ", tip4p_ljs) \
                        .setProperty("polarisability", tip4p_pol) \
                        .setProperty("connectivity", connectivity) \
                 .commit()

    waters.update(water)

print "Constructing the forcefields..."

pol_tip4p = waters.moleculeAt(0).molecule()
waters.remove(pol_tip4p)

cljff = InterGroupCLJFF("pol_tip4p-water")
cljff.add(pol_tip4p, MGIdx(0))
cljff.add(waters, MGIdx(1))

print cljff.energy()

system = System()
system.add(cljff)

print system.energy()

polchgs = PolariseCharges(cljff[MGIdx(0)], cljff.components().coulomb(),
                          CoulombProbe(1*mod_electron))

system.add(polchgs)

print "Applying the polarisation constraint..."
system.applyConstraints()

print system.energy()

