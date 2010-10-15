/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <QFile>
#include <QTextStream>

#include "amber.h"

#include "SireMol/molecules.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireIO;
using namespace SireMol;
using namespace SireStream;

///////////
/////////// Implementation of Amber
///////////

static const RegisterMetaType<Amber> r_amber;

/** Serialise to a binary datastream */
QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds, const Amber &amber)
{
    writeHeader(ds, r_amber, 1);
    
    SharedDataStream sds(ds);
    
    //sds << protoms.paramfiles << protoms.protoms_exe;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, Amber &amber)
{
    VersionID v = readHeader(ds, r_amber);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        //sds >> protoms.paramfiles >> protoms.protoms_exe;
    }
    else
        throw version_error( v, "1", r_amber, CODELOC );
        
    return ds;
}

/** Constructor */
Amber::Amber()
{
}
/** Destructor */
Amber::~Amber()
{
}
/** Reads the contents of a topfile and associated crdfile and returns a molecule group */
Molecules Amber::readcrdtop(const QString &crdfile, const QString &topfile)
{
  Molecules molecules = Molecules::Molecules();

  QFile top_f(topfile);
  
  if ( not (top_f.exists() and top_f.open(QIODevice::ReadOnly) ) )
    {
      throw SireError::file_error(top_f, CODELOC);
    }

  QTextStream ts(&top_f);

  /** 
      See http://ambermd.org/formats.html

      FORMAT(20a4)  (ITITL(i), i=1,20)
  ITITL  : title

FORMAT(12i6)  NATOM,  NTYPES, NBONH,  MBONA,  NTHETH, MTHETA,
              NPHIH,  MPHIA,  NHPARM, NPARM,  NEXT,   NRES,
              NBONA,  NTHETA, NPHIA,  NUMBND, NUMANG, NPTRA,
              NATYP,  NPHB,   IFPERT, NBPER,  NGPER,  NDPER,
              MBPER,  MGPER,  MDPER,  IFBOX,  NMXRS,  IFCAP
  NATOM  : total number of atoms 
  NTYPES : total number of distinct atom types
  NBONH  : number of bonds containing hydrogen
  MBONA  : number of bonds not containing hydrogen
  NTHETH : number of angles containing hydrogen
  MTHETA : number of angles not containing hydrogen
  NPHIH  : number of dihedrals containing hydrogen
  MPHIA  : number of dihedrals not containing hydrogen
  NHPARM : currently not used
  NPARM  : currently not used
  NEXT   : number of excluded atoms
  NRES   : number of residues
  NBONA  : MBONA + number of constraint bonds
  NTHETA : MTHETA + number of constraint angles
  NPHIA  : MPHIA + number of constraint dihedrals
  NUMBND : number of unique bond types
  NUMANG : number of unique angle types
  NPTRA  : number of unique dihedral types
  NATYP  : number of atom types in parameter file, see SOLTY below
  NPHB   : number of distinct 10-12 hydrogen bond pair types
  IFPERT : set to 1 if perturbation info is to be read in
  NBPER  : number of bonds to be perturbed
  NGPER  : number of angles to be perturbed
  NDPER  : number of dihedrals to be perturbed
  MBPER  : number of bonds with atoms completely in perturbed group
  MGPER  : number of angles with atoms completely in perturbed group
  MDPER  : number of dihedrals with atoms completely in perturbed groups
  IFBOX  : set to 1 if standard periodic box, 2 when truncated octahedral
  NMXRS  : number of atoms in the largest residue
  IFCAP  : set to 1 if the CAP option from edit was specified

FORMAT(20a4)  (IGRAPH(i), i=1,NATOM)
  IGRAPH : the user atoms names

FORMAT(5E16.8)  (CHRG(i), i=1,NATOM)
  CHRG   : the atom charges.  (Divide by 18.2223 to convert to charge
           in units of the electron charge)

FORMAT(5E16.8)  (AMASS(i), i=1,NATOM)
  AMASS  : the atom masses

FORMAT(12I6)  (IAC(i), i=1,NATOM)
  IAC    : index for the atom types involved in Lennard Jones (6-12) 
           interactions.  See ICO below.

FORMAT(12I6)  (NUMEX(i), i=1,NATOM)
  NUMEX  : total number of excluded atoms for atom "i".  See
           NATEX below.

FORMAT(12I6)  (ICO(i), i=1,NTYPES*NTYPES)
  ICO    : provides the index to the nonbon parameter
           arrays CN1, CN2 and ASOL, BSOL.  All possible 6-12
           or 10-12 atoms type interactions are represented.
           NOTE: A particular atom type can have either a 10-12
           or a 6-12 interaction, but not both.  The index is
           calculated as follows:
             index = ICO(NTYPES*(IAC(i)-1)+IAC(j))
           If index is positive, this is an index into the
           6-12 parameter arrays (CN1 and CN2) otherwise it
           is an index into the 10-12 parameter arrays (ASOL
           and BSOL).
           
FORMAT(20A4)  (LABRES(i), i=1,NRES)
  LABRES : the residue labels

FORMAT(12I6)  (IPRES(i), i=1,NRES)
  IPRES  : atoms in each residue are listed for atom "i" in
           IPRES(i) to IPRES(i+1)-1

FORMAT(5E16.8)  (RK(i), i=1,NUMBND)
  RK     : force constant for the bonds of each type, kcal/mol

FORMAT(5E16.8)  (REQ(i), i=1,NUMBND)
  REQ    : the equilibrium bond length for the bonds of each type, angstroms

FORMAT(5E16.8)  (TK(i), i=1,NUMANG)
  TK     : force constant for the angles of each type, kcal/mol A**2

FORMAT(5E16.8)  (TEQ(i), i=1,NUMANG)
  TEQ    : the equilibrium angle for the angles of each type, radians

FORMAT(5E16.8)  (PK(i), i=1,NPTRA)
  PK     : force constant for the dihedrals of each type, kcal/mol

FORMAT(5E16.8)  (PN(i), i=1,NPTRA)
  PN     : periodicity of the dihedral of a given type

FORMAT(5E16.8)  (PHASE(i), i=1,NPTRA)
  PHASE  : phase of the dihedral of a given type, radians

FORMAT(5E16.8)  (SOLTY(i), i=1,NATYP)
  SOLTY  : currently unused (reserved for future use)

FORMAT(5E16.8)  (CN1(i), i=1,NTYPES*(NTYPES+1)/2)
  LENNARD_JONES_ACOEF    : Lennard Jones r**12 terms for all possible
           atom type interactions, indexed by ICO and IAC; for atom i
           and j where i < j, the index into this array is as follows
           (assuming the value of ICO(index) is positive):
           CN1(ICO(NTYPES*(IAC(i)-1)+IAC(j))).

FORMAT(5E16.8)  (CN2(i), i=1,NTYPES*(NTYPES+1)/2)
  LENNARD_JONES_BCOEF    : Lennard Jones r**6 terms for all possible
           atom type interactions.  Indexed like CN1 above.

NOTE: the atom numbers in the following arrays that describe bonds, angles, and dihedrals are 
coordinate array indexes for runtime speed. The true atom number equals the absolute value of 
the number divided by three, plus one. In the case of the dihedrals, if the fourth atom is negative, 
this implies that the dihedral is an improper. If the third atom is negative, this implies that the 
end group interations are to be ignored. End group interactions are ignored, for example, in 
dihedrals of various ring systems (to prevent double counting of 1-4 interactions) and in 
multiterm dihedrals.

FORMAT(12I6)  (IBH(i),JBH(i),ICBH(i), i=1,NBONH)
  IBH    : atom involved in bond "i", bond contains hydrogen
  JBH    : atom involved in bond "i", bond contains hydrogen
  ICBH   : index into parameter arrays RK and REQ

FORMAT(12I6)  (IB(i),JB(i),ICB(i), i=1,NBONA)
  IB     : atom involved in bond "i", bond does not contain hydrogen
  JB     : atom involved in bond "i", bond does not contain hydrogen
  ICB    : index into parameter arrays RK and REQ

FORMAT(12I6)  (ITH(i),JTH(i),KTH(i),ICTH(i), i=1,NTHETH)
  ITH    : atom involved in angle "i", angle contains hydrogen
  JTH    : atom involved in angle "i", angle contains hydrogen
  KTH    : atom involved in angle "i", angle contains hydrogen
  ICTH   : index into parameter arrays TK and TEQ for angle
           ITH(i)-JTH(i)-KTH(i)

FORMAT(12I6)  (IT(i),JT(i),KT(i),ICT(i), i=1,NTHETA)
  IT     : atom involved in angle "i", angle does not contain hydrogen
  JT     : atom involved in angle "i", angle does not contain hydrogen
  KT     : atom involved in angle "i", angle does not contain hydrogen
  ICT    : index into parameter arrays TK and TEQ for angle
           IT(i)-JT(i)-KT(i)

FORMAT(12I6)  (IPH(i),JPH(i),KPH(i),LPH(i),ICPH(i), i=1,NPHIH)
  IPH    : atom involved in dihedral "i", dihedral contains hydrogen
  JPH    : atom involved in dihedral "i", dihedral contains hydrogen
  KPH    : atom involved in dihedral "i", dihedral contains hydrogen
  LPH    : atom involved in dihedral "i", dihedral contains hydrogen
  ICPH   : index into parameter arrays PK, PN, and PHASE for
           dihedral IPH(i)-JPH(i)-KPH(i)-LPH(i)

FORMAT(12I6)  (IP(i),JP(i),KP(i),LP(i),ICP(i), i=1,NPHIA)
  IP     : atom involved in dihedral "i", dihedral does not contain hydrogen
  JP     : atom involved in dihedral "i", dihedral does not contain hydrogen
  KP     : atom involved in dihedral "i", dihedral does not contain hydrogen
  LP     : atom involved in dihedral "i", dihedral does not contain hydrogen
  ICP    : index into parameter arrays PK, PN, and PHASE for
           dihedral IPH(i)-JPH(i)-KPH(i)-LPH(i).  Note, if the
           periodicity is negative, this implies the following entry
           in the PK, PN, and PHASE arrays is another term in a
           multitermed dihedral.  

FORMAT(12I6)  (NATEX(i), i=1,NEXT)
  NATEX  : the excluded atom list.  To get the excluded list for atom 
           "i" you need to traverse the NUMEX list, adding up all
           the previous NUMEX values, since NUMEX(i) holds the number
           of excluded atoms for atom "i", not the index into the 
           NATEX list.  Let IEXCL = SUM(NUMEX(j), j=1,i-1), then
           excluded atoms are NATEX(IEXCL) to NATEX(IEXCL+NUMEX(i)).

FORMAT(5E16.8)  (ASOL(i), i=1,NPHB)
  ASOL   : the value for the r**12 term for hydrogen bonds of all
           possible types.  Index into these arrays is equivalent
           to the CN1 and CN2 arrays, however the index is negative.
           For example, for atoms i and j, with i < j, the index is
           -ICO(NTYPES*(IAC(i)-1+IAC(j)).

FORMAT(5E16.8)  (BSOL(i), i=1,NPHB)
  BSOL   : the value for the r**10 term for hydrogen bonds of all
           possible types.  Indexed like ASOL.

FORMAT(5E16.8)  (HBCUT(i), i=1,NPHB)
  HBCUT  : no longer in use

FORMAT(20A4)  (ISYMBL(i), i=1,NATOM)
  ISYMBL : the AMBER atom types for each atom

FORMAT(20A4)  (ITREE(i), i=1,NATOM)
  ITREE  : the list of tree joining information, classified into five
           types.  M -- main chain, S -- side chain, B -- branch point, 
           3 -- branch into three chains, E -- end of the chain

FORMAT(12I6)  (JOIN(i), i=1,NATOM)
  JOIN   : tree joining information, potentially used in ancient
           analysis programs.  Currently unused in sander or gibbs.

FORMAT(12I6)  (IROTAT(i), i = 1, NATOM)
  IROTAT : apparently the last atom that would move if atom i was
           rotated, however the meaning has been lost over time.
           Currently unused in sander or gibbs.

The following are only present if IFBOX .gt. 0

FORMAT(12I6)  IPTRES, NSPM, NSPSOL
  IPTRES : final residue that is considered part of the solute,
           reset in sander and gibbs
  NSPM   : total number of molecules
  NSPSOL : the first solvent "molecule"

FORMAT(12I6)  (NSP(i), i=1,NSPM)
  NSP    : the total number of atoms in each molecule,
           necessary to correctly perform the pressure
           scaling.

FORMAT(5E16.8)  BETA, BOX(1), BOX(2), BOX(3)
  BETA   : periodic box, angle between the XY and YZ planes in
           degrees.
  BOX    : the periodic box lengths in the X, Y, and Z directions

The following are only present if IFCAP .gt. 0

FORMAT(12I6)  NATCAP
  NATCAP : last atom before the start of the cap of waters
           placed by edit

FORMAT(5E16.8)  CUTCAP, XCAP, YCAP, ZCAP
  CUTCAP : the distance from the center of the cap to the outside
  XCAP   : X coordinate for the center of the cap
  YCAP   : Y coordinate for the center of the cap
  ZCAP   : Z coordinate for the center of the cap

The following is only present if IFPERT .gt. 0
Note that the initial state, or equivalently the prep/link/edit state, is represented by lambda=1 and the 
perturbed state, or final state specified in parm, is the lambda=0 state.

FORMAT(12I6)  (IBPER(i), JBPER(i), i=1,NBPER)
  IBPER  : atoms involved in perturbed bonds
  JBPER  : atoms involved in perturbed bonds

FORMAT(12I6)  (ICBPER(i), i=1,2*NBPER)
  ICBPER : pointer into the bond parameter arrays RK and REQ for the
           perturbed bonds.  ICBPER(i) represents lambda=1 and 
           ICBPER(i+NBPER) represents lambda=0.

FORMAT(12I6)  (ITPER(i), JTPER(i), KTPER(i), i=1,NGPER)
  IPTER  : atoms involved in perturbed angles
  JTPER  : atoms involved in perturbed angles
  KTPER  : atoms involved in perturbed angles

FORMAT(12I6)  (ICTPER(i), i=1,2*NGPER)
  ICTPER : pointer into the angle parameter arrays TK and TEQ for 
           the perturbed angles.  ICTPER(i) represents lambda=0 and 
           ICTPER(i+NGPER) represents lambda=1.

FORMAT(12I6)  (IPPER(i), JPPER(i), KPPER(i), LPPER(i), i=1,NDPER)
  IPPER  : atoms involved in perturbed dihedrals
  JPPER  : atoms involved in perturbed dihedrals
  KPPER  : atoms involved in perturbed dihedrals
  LPPER  : atoms involved in pertrubed dihedrals

FORMAT(12I6)  (ICPPER(i), i=1,2*NDPER)
  ICPPER : pointer into the dihedral parameter arrays PK, PN and
           PHASE for the perturbed dihedrals.  ICPPER(i) represents 
           lambda=1 and ICPPER(i+NGPER) represents lambda=0.

FORMAT(20A4)  (LABRES(i), i=1,NRES)
  LABRES : residue names at lambda=0

FORMAT(20A4)  (IGRPER(i), i=1,NATOM)
  IGRPER : atomic names at lambda=0

FORMAT(20A4)  (ISMPER(i), i=1,NATOM)
  ISMPER : atomic symbols at lambda=0

FORMAT(5E16.8)  (ALMPER(i), i=1,NATOM)
  ALMPER : unused currently in gibbs

FORMAT(12I6)  (IAPER(i), i=1,NATOM)
  IAPER  : IAPER(i) = 1 if the atom is being perturbed

FORMAT(12I6)  (IACPER(i), i=1,NATOM)
  IACPER : index for the atom types involved in Lennard Jones
           interactions at lambda=0.  Similar to IAC above.  
           See ICO above.

FORMAT(5E16.8)  (CGPER(i), i=1,NATOM)
  CGPER  : atomic charges at lambda=0

The following is only present if IPOL .eq. 1

FORMAT(5E18.8) (ATPOL(i), i=1,NATOM)
  ATPOL  : atomic polarizabilities

The following is only present if IPOL .eq. 1 .and. IFPERT .eq. 1

FORMAT(5E18.8) (ATPOL1(i), i=1,NATOM)
  ATPOL1 : atomic polarizabilities at lambda = 1 (above is at lambda = 0)
  */

  // TOP file format generated by sleap in Amber-tools 1.4
  // see amber11/AmberTools/src/gleap/mortsrc/ambfmt/prmtop.cpp
  QString line = ts.readline();
  //%VERSION
  while (not line.isNull() )
    {
      qDebug() << line;
      line = ts.readline()
    }


 
  //%FLAG TITLE
  //%FORMAT(20a4)
  // (...)
  //%FLAG POINTERS
  //%FORMAT (10i8)
  // NATOMS NTYPES NBONH MBONA NTHETH MTHETA NPHIH MPHIA NHPARM NPARM
  // NNB NRES NBONA NTHETA NPHIA NUMBND NUMANG NPTRA NATYP NPHB
  // 0    0     0     0     0      0       0    IFBOX NMXRS IFCAP
  //%FLAG ATOM_NAME --> use NATOM the user atoms names
  //%FORMAT(20a4) 
  // (...)
  //%FLAG CHARGE --> use NATOM 
  //%FORMAT(5E16.8)
  // (...)
  //%FLAG MASS --> use NATOM 
  //%FORMAT(5E16.8)
  //%FLAG ATOM_TYPE_INDEX --> use NATOM 
  //%FORMAT(10I8)
  // (...)
  //%FLAG NUMBER_EXCLUDED_ATOMS --> use NATOM 
  //%FORMAT(10I8)
  //(...)
  //%FLAG NONBONDED_PARM_INDEX --> use NTYPES * NTYPES 
  //%FORMAT(10I8)
  //(...)
  //%FLAG RESIDUE_LABEL --> use NRES 
  //%FORMAT(20a4)
  //(...)
  //%FLAG RESIDUE_POINTER --> use NRES 
  //%FORMAT(10I8)
  //(...)
  //%FLAG BOND_FORCE_CONSTANT --> use NUMBND 
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG BOND_EQUIL_VALUE --> use NUMBND 
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG ANGLE_FORCE_CONSTANT --> use NUMANG 
  //%FORMAT(5E16.8)
  //(...) 
  //%FLAG ANGLE_EQUIL_VALUE --> use NUMANG 
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG DIHEDRAL_FORCE_CONSTANT --> use NPTRA 
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG DIHEDRAL_PERIODICITY --> use NPTRA 
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG DIHEDRAL_PHASE --> use NPTRA 
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG SOLTY currently unused (reserved for future use) --> Use NATYP
  //%FORMAT(5E16.8)
  // (...)
  //%FLAG LENNARD_JONES_ACOEF --> Use NTYPES*(NTYPES+1)/2
  //%FORMAT(5E16.8)
  // (...)
  //%FLAG LENNARD_JONES_BCOEF --> Use NTYPES*(NTYPES+1)/2
  //%FORMAT(5E16.8)
  // (...)
  //%FLAG BONDS_INC_HYDROGEN --> Use NBONH
  //%FORMAT(10I8)
  // (...)
  //%FLAG BONDS_WITHOUT_HYDROGEN --> Use NBONA
  //%FORMAT(10I8)
  // (...)
  //  %FLAG ANGLES_INC_HYDROGEN --> NTHETH
  //%FORMAT(10I8)
  // (...)
  //  %FLAG ANGLES_WITHOUT_HYDROGEN --> NTHETA
  //%FORMAT(10I8)
  // (...)
  //  %FLAG DIHEDRALS_INC_HYDROGEN --> NPHIH
  //%FORMAT(10I8)
  // (...)
  //  %FLAG DIHEDRALS_WITHOUT_HYDROGEN --> NPHIA
  //%FORMAT(10I8)
  // (...)
  //  %FLAG EXCLUDED_ATOMS_LIST --> NEXT
  //%FORMAT(10I8)
  // (...)
  //%FLAG HBOND_ACOEF --> UNUSED
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG HBOND_BCOEF --> UNUSED
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG HBCUT --> UNUSED
  //%FORMAT(5E16.8)
  //(...)
  //%FLAG AMBER_ATOM_TYPE --> use NATOM
  //%FORMAT(20a4)
  // (...)
  //%FLAG TREE_CHAIN_CLASSIFICATION --> use NATOM
  //%FORMAT(20a4)
  // (...)
  //%FLAG JOIN_ARRAY --> use NATOM
  //%FORMAT(10I8)
  // (...)
  //%FLAG IROTAT --> use NATOM
  //%FORMAT(10I8)
  // (...)
  //%FLAG SOLVENT_POINTERS
  //%FORMAT(3I8)
  // IPTRES NSPM NSPSOL
  //%FLAG ATOMS_PER_MOLECULE --> Use NSPM
  //%FORMAT(10I8)
  // (...)
  //%FLAG BOX_DIMENSIONS
  //%FORMAT(5E16.8)
  // BETA BOX(1) BOX(2) BOX(3)
  //  %FLAG RADIUS_SET
  /** HERE MUST ADD SUPPORT TO PARSE CAP SETUP */
  //%FORMAT(1a80)
  //modified Bondi radii (mbondi)
  //%FLAG RADII
  //%FORMAT(5E16.8) --> use NATOM
  // (...)
  // %FLAG SCREEN --> use NATOM
  //%FORMAT(5E16.8)
  // (...)
  
  
  
  return molecules;
}

const char* Amber::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Amber>() );
}


