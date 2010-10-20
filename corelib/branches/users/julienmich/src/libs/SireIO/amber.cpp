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

#include "SireMol/element.h"

//#include "SireMol/atomcoords.h"
//#include "SireMol/atomelements.h"
#include "SireMol/atomcharges.h"
#include "SireMol/atommasses.h"
#include "SireMol/connectivity.h"
//#include "SireMol/mover.hpp"
#include "SireMol/selector.hpp"

#include "SireMol/molecule.h"
//#include "SireMol/moleculegroup.h"
//#include "SireMol/molidx.h"
//#include "SireMol/molidentifier.h"
#include "SireMol/moleditor.h"
//#include "SireMol/segeditor.h"
//#include "SireMol/chaineditor.h"
#include "SireMol/reseditor.h"
//#include "SireMol/cgeditor.h"
#include "SireMol/atomeditor.h"
//#include "SireMol/residue.h"
//#include "SireMol/chain.h"
//#include "SireMol/segment.h"
#include "SireMol/cgatomidx.h"
#include "SireMol/residuecutting.h"
//#include "SireBase/stringmangler.h"

#include "SireMM/ljparameter.h"
#include "SireMM/atomljs.h"
#include "SireMM/internalff.h"

#include "SireMaths/maths.h"
#include "SireUnits/units.h"

#include "SireBase/tempdir.h"
#include "SireBase/findexe.h"
#include "SireBase/process.h"

#include "SireError/errors.h"
#include "SireIO/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"
//#include "SireStream/sharestrings.h"

//#include "SireMol/atomcoords.h"
//#include "SireMol/atomelements.h"
//#include "SireMol/atomcharges.h"
//#include "SireMol/connectivity.h"

//#include "SireMol/molecule.h"
//#include "SireMol/molecules.h"
//#include "SireMol/moleditor.h"
//#include "SireMol/reseditor.h"
//#include "SireMol/atomeditor.h"
//#include "SireMol/residue.h"

//#include "SireError/errors.h"

//#include "SireStream/datastream.h"
//#include "SireStream/shareddatastream.h"

using namespace SireIO;
using namespace SireMol;
using namespace SireMM;
using namespace SireMaths;
using namespace SireUnits;
using namespace SireStream;
using namespace SireBase;

//
// Implementation of FortranFormat
//

FortranFormat::FortranFormat() : repeat(0), type("I"), size(0), decimal(0)
{}

FortranFormat::FortranFormat(const int repeat, const QString type, const int size, const int decimal )
{
  this->repeat = repeat;
  this->type = type;
  this->size = size;
  this->decimal = decimal;
}

FortranFormat::~FortranFormat()
{}

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

  // Read the contents of the top file
  QFile top_f(topfile);
  
  if ( not (top_f.exists() and top_f.open(QIODevice::ReadOnly) ) )
    {
      throw SireError::file_error(top_f, CODELOC);
    }

  QTextStream ts(&top_f);

  // TOP file format generated by sleap in Amber-tools 1.4
  // see amber11/AmberTools/src/gleap/mortsrc/ambfmt/prmtop.cpp

  // The following holds the data read from the top file
  int currentFlag = 0;
  FortranFormat currentFormat = FortranFormat(); 
  //QString currentVersion = " ";
  QString title = " ";
  QList<int> pointers;
  QStringList atomName;
  QList<double> charge;
  QList<double> mass;
  QList<int> atomTypeIndex;
  QList<int> numberExcludedAtoms;
  QList<int> nonbondedParmIndex;
  QStringList residueLabel;
  QList<int> residuePointer;
  QList<double> bondForceConstant;
  QList<double> bondEquilValue;
  QList<double> angleForceConstant;
  QList<double> angleEquilValue;
  QList<double> dihedralForceConstant;
  QList<double> dihedralPeriodicity;
  QList<double> dihedralPhase;
  QList<double> solty;
  QList<double> lennardJonesAcoef;
  QList<double> lennardJonesBcoef;
  QList<int> bondsIncHydrogen;
  QList<int> bondsWithoutHydrogen;
  QList<int> anglesIncHydrogen;
  QList<int> anglesWithoutHydrogen;
  QList<int> dihedralsIncHydrogen;
  QList<int> dihedralsWithoutHydrogen;
  QList<int> excludedAtomsList;
  QList<double> hbondAcoef;
  QList<double> hbondBcoef;
  QList<double> hbcut;
  QStringList amberAtomType;
  QStringList treeChainClassification;
  QList<int> joinArray;
  QList<int> irotat;
  QList<int> solventPointers;
  QList<int> atomsPerMolecule;
  QList<double> boxDimensions;
  QStringList radiusSet;
  QList<double> radii;
  QList<double> screen;

  QString line = ts.readLine();
  // The first line containts the version. 
  // Make sure we know how to read this version
  //this->processVersionLine(words, currentVersion);
  while (not line.isNull() )
    {
      //qDebug() << line;
      line = ts.readLine();

      if (line.startsWith("%"))
	{
	  // We are reading meta data, can be VERSION, FLAG, FORMAT
	  QStringList words = line.split(" ", QString::SkipEmptyParts);
	  if (line.startsWith("%FLAG"))
	    this->processFlagLine(words, currentFlag);
	  else if (line.startsWith("%FORMAT"))
	    this->processFormatLine(words, currentFormat);
	  else
	      throw SireError::program_bug( QObject::tr(
		  "Does not know what to do with a '%1' statement").arg(words[0]), CODELOC );
	}
      // if not we are reading data. Read it according to the latest FLAG/FORMAT values
      else
	{
	  //qDebug() << "FORMAT IS " << currentFormat.type << " FLAG IS " << currentFlag;
	  switch ( currentFlag ) 
	    {
	    case TITLE:
	      this->processTitleLine(line, currentFormat, title);
	      break;
	    case POINTERS:
	      this->processIntegerLine(line, currentFormat, pointers);
	      break;
	    case ATOM_NAME:
	      this->processStringLine(line, currentFormat, atomName);
	      break;
	    case CHARGE:
	      this->processDoubleLine(line, currentFormat, charge);
	      break;
	    case MASS:
	      this->processDoubleLine(line, currentFormat, mass);
	      break;
	    case ATOM_TYPE_INDEX:
	      this->processIntegerLine(line, currentFormat, atomTypeIndex);
	      break;
	    case NUMBER_EXCLUDED_ATOMS:
	      this->processIntegerLine(line, currentFormat, numberExcludedAtoms);
	      break;
	    case NONBONDED_PARM_INDEX:
	      this->processIntegerLine(line, currentFormat, nonbondedParmIndex);
	      break;
	    case RESIDUE_LABEL:
	      this->processStringLine(line, currentFormat, residueLabel);
	      break;
	    case RESIDUE_POINTER:
	      this->processIntegerLine(line, currentFormat, residuePointer);
	      break;
	    case BOND_FORCE_CONSTANT:
	      this->processDoubleLine(line, currentFormat, bondForceConstant);
	      break;
	    case BOND_EQUIL_VALUE:
	      this->processDoubleLine(line, currentFormat, bondEquilValue);
	      break;
	    case ANGLE_FORCE_CONSTANT:
	      this->processDoubleLine(line, currentFormat, angleForceConstant);
	      break;
	    case ANGLE_EQUIL_VALUE:
	      this->processDoubleLine(line, currentFormat, angleEquilValue);
	      break;
	    case DIHEDRAL_FORCE_CONSTANT:
	      this->processDoubleLine(line, currentFormat, dihedralForceConstant);
	      break;
	    case DIHEDRAL_PERIODICITY:
	      this->processDoubleLine(line, currentFormat, dihedralPeriodicity);
	      break;
	    case DIHEDRAL_PHASE:
	      this->processDoubleLine(line, currentFormat, dihedralPhase);
	      break;
	    case SOLTY:
	      this->processDoubleLine(line, currentFormat, solty);
	      break; 
	    case LENNARD_JONES_ACOEF:
	      this->processDoubleLine(line, currentFormat, lennardJonesAcoef);
	      break; 
	    case LENNARD_JONES_BCOEF:
	      this->processDoubleLine(line, currentFormat, lennardJonesBcoef);
	      break; 
	    case BONDS_INC_HYDROGEN:
	      this->processIntegerLine(line, currentFormat, bondsIncHydrogen);
	      break;
	    case BONDS_WITHOUT_HYDROGEN:
	      this->processIntegerLine(line, currentFormat, bondsWithoutHydrogen);
	      break;
	    case ANGLES_INC_HYDROGEN:
	      this->processIntegerLine(line, currentFormat, anglesIncHydrogen);
	      break;
	    case ANGLES_WITHOUT_HYDROGEN:
	      this->processIntegerLine(line, currentFormat, anglesWithoutHydrogen);
	      break;
	    case DIHEDRALS_INC_HYDROGEN:
	      this->processIntegerLine(line, currentFormat, dihedralsIncHydrogen);
	      break;
	    case DIHEDRALS_WITHOUT_HYDROGEN:
	      this->processIntegerLine(line, currentFormat, dihedralsWithoutHydrogen);
	      break;
	    case EXCLUDED_ATOMS_LIST:
	      this->processIntegerLine(line, currentFormat, excludedAtomsList);
	      break;
	    case HBOND_ACOEF:
	      this->processDoubleLine(line, currentFormat, hbondAcoef);
	      break; 
	    case HBOND_BCOEF:
	      this->processDoubleLine(line, currentFormat, hbondBcoef);
	      break; 
	    case HBCUT:
	      this->processDoubleLine(line, currentFormat, hbcut);
	      break; 
	    case AMBER_ATOM_TYPE:
	      this->processStringLine(line, currentFormat, amberAtomType);
	      break; 
	    case TREE_CHAIN_CLASSIFICATION:
	      this->processStringLine(line, currentFormat, treeChainClassification);
	      break; 
	    case JOIN_ARRAY:
	      this->processIntegerLine(line, currentFormat, joinArray);
	      break;
 	    case IROTAT:
	      this->processIntegerLine(line, currentFormat, irotat);
	      break;
	    case SOLVENT_POINTERS:
	      this->processIntegerLine(line, currentFormat, solventPointers);
	      break;
	    case ATOMS_PER_MOLECULE:
	      this->processIntegerLine(line, currentFormat, atomsPerMolecule);
	      break;
	    case BOX_DIMENSIONS:
	      this->processDoubleLine(line, currentFormat, boxDimensions);
	      break;
	    case RADIUS_SET:
	      this->processStringLine(line, currentFormat, radiusSet);
	      break;
	    case RADII:
	      this->processDoubleLine(line, currentFormat, radii);
	      break;
	    case SCREEN:
	      this->processDoubleLine(line, currentFormat, screen);
	      break;
	    default:
	      throw SireError::program_bug( QObject::tr(
	      "Serious problem with the value of the variable currentFlag, '%1'").arg(currentFlag), CODELOC);
	    }
	}
    }
  //DEBUG STUFF
  //qDebug() << " POINTERS " << pointers << " ATOMNAME " << atomName.size() << " CHARGE " << charge.size() << " MASS " << mass.size() << " ATOM_TYPE_INDEX " << atomTypeIndex.size() << " SCREEN " << screen.size();

  // Now read the contents of the crd file to get the coordinates
  QFile crd_f(crdfile);
  
  if ( not (crd_f.exists() and crd_f.open(QIODevice::ReadOnly) ) )
    {
      throw SireError::file_error(crd_f, CODELOC);
    }

  QTextStream ts2(&crd_f);
  // the first line contains the title
  line = ts2.readLine();
  // the second line contains the number of atoms, read as 1I6
  line = ts2.readLine();
  QList<int> temp;
  FortranFormat crdIntegerFormat = FortranFormat(1, "I", 6, 0);
  this->processIntegerLine(line, crdIntegerFormat,temp);
  int crdAtoms = temp[0];
  //qDebug() << " THERE ARE " << crdAtoms << " ATOMS IN THE CRD FILE";
  // Check that this number of atoms is compatible with what is in the top file
  if (pointers[NATOM] != crdAtoms)
    throw SireError::incompatible_error( QObject::tr(
                        "The number of atoms in the crd file (%1) does not equal the number "
                "of atoms in the top file (%2)!")
		     .arg(crdAtoms).arg(pointers[NATOM]), CODELOC );
  QList<double> crdCoords;
  FortranFormat crdDoubleFormat = FortranFormat(6,"E",12,7);

  for (int i=0; i < crdAtoms/2 ; i++ )
    {
      line = ts2.readLine();
      this->processDoubleLine(line, crdDoubleFormat, crdCoords);
    }
  //qDebug() << " THE COORDS ARE " << crdCoords;
  // And now the box dimensions
  line = ts2.readLine();
  QList<double> crdBox;
  this->processDoubleLine(line, crdDoubleFormat, crdBox);
  //qDebug() << " THE BOX DIMENSIONS ARE " << crdBox;

  // Now create the atoms and molecules etc..

  PropertyName coords_property = PropertyName("coordinates");
  PropertyName charge_property = PropertyName("charge");
  PropertyName mass_property = PropertyName("mass");
  PropertyName lj_property = PropertyName("LJ");
  PropertyName ambertype_property = PropertyName("ambertype");

  PropertyName connectivity_property = PropertyName("connectivity");
  PropertyName bond_property = PropertyName("bond");
  PropertyName angle_property = PropertyName("angle");
  PropertyName dihedral_property = PropertyName("dihedral");
  PropertyName improper_property = PropertyName("improper");

  Molecules molecules;

  int moleculeNumber = 1;
  int residueNumber = 1;
  for (int i=0; i < solventPointers[NSPM] ; i++)
  // for each molecule. Use solventPointers[NSPM] or ATOMS_PER_MOLECULE to find out the number of molecules
    {
      /** First pass, use StructureEditors to build the layout of the molecule*/
      MolStructureEditor molstructeditor;
      int atomsInMolecule = 0;
      while (atomsInMolecule < atomsPerMolecule[i])
	{
	  int atomStart = residuePointer[residueNumber - 1];
	  // Be careful not to overflow 
	  int atomEnd;
	  if ( residueNumber < ( pointers[NRES] ) )
	    atomEnd = residuePointer[residueNumber - 1 + 1 ] - 1;
	  else
	    atomEnd = pointers[NATOM] ;

	  //qDebug() << "Residue " << residueNumber << " start " << atomStart << " end " << atomEnd;
	  //       create an empty residue. Use RESIDUE_LABEL for the name
	  ResStructureEditor resstructeditor = molstructeditor.add( ResNum(residueNumber) );
	  resstructeditor.rename( ResName( residueLabel[residueNumber - 1]) );
	  for (int j=atomStart; j <= atomEnd; j++)
	    {
	      AtomStructureEditor atomstructeditor = molstructeditor.add( AtomNum(j) );
	      atomstructeditor.rename( AtomName(atomName[j -1]) );
	      atomstructeditor.reparent( ResNum(residueNumber) );
	    }
	  atomsInMolecule += ( atomEnd - atomStart ) + 1 ;
	  residueNumber++;
	}
      // Create cut groups using a per residue scheme
      ResidueCutting residue_cutfunc = ResidueCutting();

      molstructeditor = residue_cutfunc(molstructeditor);

      Molecule molecule = molstructeditor.commit();

      MolEditor editmol = molecule.edit();

      ConnectivityEditor connectivity = Connectivity(editmol.data()).edit();
      TwoAtomFunctions bondfuncs(editmol);
      ThreeAtomFunctions anglefuncs(editmol);
      FourAtomFunctions dihedralfuncs(editmol);
      FourAtomFunctions improperfuncs(editmol);

      for (int i=0; i < editmol.nAtoms() ; ++i)
	{
	  // Now that the structure of the molecule has been built, we assign the following 
	  // atom properties: coordinates, charge, mass, lj , amber_atom_type
	  AtomEditor editatom = editmol.atom(AtomIdx(i)); 

	  this->setAtomParameters( editatom, editmol, crdCoords, coords_property, 
				   charge, charge_property,
				   mass, mass_property, atomTypeIndex, 
				   nonbondedParmIndex, lennardJonesAcoef, 
				   lennardJonesBcoef, lj_property,
				   amberAtomType, ambertype_property, pointers);
	}
      
      // Find all the bonds that involve an atom in this molecule. 
      // Because bonds are indexed in two arrays in the top file 
      // (those without hydrogen atoms and those with hydrogen atoms) 
      // we have to look in both. Also note how the atom index is x / 3 + 1 

      this->setConnectivity(editmol, pointers, 
			    bondsIncHydrogen, bondsWithoutHydrogen,
			    connectivity, connectivity_property);

      // Next all the forcefield terms
      this->setBonds(editmol, pointers[NBONH],
		     bondsIncHydrogen, 
		     bondForceConstant, bondEquilValue,
		     bondfuncs, bond_property);

      this->setBonds(editmol, pointers[MBONA],
		     bondsWithoutHydrogen, 
		     bondForceConstant, bondEquilValue,
		     bondfuncs, bond_property);
      
      this->setAngles(editmol, pointers[NTHETH], 
		      anglesIncHydrogen, 
		      angleForceConstant, angleEquilValue,
		      anglefuncs, angle_property);

      this->setAngles(editmol, pointers[MTHETA], 
		      anglesWithoutHydrogen, 
		      angleForceConstant, angleEquilValue,
		      anglefuncs, angle_property);
      
      this->setDihedrals(editmol, pointers[NTHETH],
			 dihedralsIncHydrogen, 
			 dihedralForceConstant, dihedralPeriodicity, dihedralPhase,
			 dihedralfuncs, dihedral_property,
			 improperfuncs, improper_property);

      this->setDihedrals(editmol, pointers[MTHETA],
			 dihedralsWithoutHydrogen, 
			 dihedralForceConstant, dihedralPeriodicity, dihedralPhase,
			 dihedralfuncs, dihedral_property,
			 improperfuncs, improper_property);
      // Needs to deal with the excluded atoms too
      
      molecule = editmol.commit();

      molecules.add(molecule);
      moleculeNumber++;
    }
  // Finally, box information

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
  
  
  //Molecules molmols = Molecules::Molecules(); 
  return molecules;
}

/** Processes a line that starts with %FLAG*/
void Amber::processFlagLine(const QStringList &words, int &flag)
{
  //%FLAG TITLE
  if (words[1] == "TITLE")
    flag = TITLE;
  else if (words[1] == "POINTERS")
    flag = POINTERS;
  else if (words[1] == "ATOM_NAME")
    flag = ATOM_NAME;
  else if (words[1] == "CHARGE")
    flag = CHARGE;
  else if (words[1] == "MASS")
    flag = MASS;
  else if (words[1] == "ATOM_TYPE_INDEX")
    flag = ATOM_TYPE_INDEX;
  else if (words[1] == "NUMBER_EXCLUDED_ATOMS")
    flag = NUMBER_EXCLUDED_ATOMS;
  else if (words[1] == "NONBONDED_PARM_INDEX")
    flag = NONBONDED_PARM_INDEX;
  else if (words[1] == "RESIDUE_LABEL")
    flag = RESIDUE_LABEL;
  else if (words[1] == "RESIDUE_POINTER")
    flag = RESIDUE_POINTER;
  else if (words[1] == "BOND_FORCE_CONSTANT")
    flag = BOND_FORCE_CONSTANT;
  else if (words[1] == "BOND_EQUIL_VALUE")
    flag = BOND_EQUIL_VALUE;
  else if (words[1] == "ANGLE_FORCE_CONSTANT")
    flag = ANGLE_FORCE_CONSTANT;
  else if (words[1] == "ANGLE_EQUIL_VALUE")
    flag = ANGLE_EQUIL_VALUE;
  else if (words[1] == "DIHEDRAL_FORCE_CONSTANT")
    flag = DIHEDRAL_FORCE_CONSTANT;
  else if (words[1] == "DIHEDRAL_PERIODICITY")
    flag = DIHEDRAL_PERIODICITY;
  else if (words[1] == "DIHEDRAL_PHASE")
    flag = DIHEDRAL_PHASE;
  else if (words[1] == "SOLTY")
    flag = SOLTY;
  else if (words[1] == "LENNARD_JONES_ACOEF")
    flag = LENNARD_JONES_ACOEF;
  else if (words[1] == "LENNARD_JONES_BCOEF")
    flag = LENNARD_JONES_BCOEF;
  else if (words[1] == "BONDS_INC_HYDROGEN")
    flag = BONDS_INC_HYDROGEN;
  else if (words[1] == "BONDS_WITHOUT_HYDROGEN")
    flag = BONDS_WITHOUT_HYDROGEN;
  else if (words[1] == "ANGLES_INC_HYDROGEN")
    flag = ANGLES_INC_HYDROGEN;
  else if (words[1] == "ANGLES_WITHOUT_HYDROGEN")
    flag = ANGLES_WITHOUT_HYDROGEN;
  else if (words[1] == "DIHEDRALS_INC_HYDROGEN")
    flag = DIHEDRALS_INC_HYDROGEN;
  else if (words[1] == "DIHEDRALS_WITHOUT_HYDROGEN")
    flag = DIHEDRALS_WITHOUT_HYDROGEN;
  else if (words[1] == "EXCLUDED_ATOMS_LIST")
    flag = EXCLUDED_ATOMS_LIST;
  else if (words[1] == "HBOND_ACOEF")
    flag = HBOND_ACOEF;
  else if (words[1] == "HBOND_BCOEF")
    flag = HBOND_BCOEF;
  else if (words[1] == "HBCUT")
    flag = HBCUT;
  else if (words[1] == "AMBER_ATOM_TYPE")
    flag = AMBER_ATOM_TYPE;
  else if (words[1] == "TREE_CHAIN_CLASSIFICATION")
    flag = TREE_CHAIN_CLASSIFICATION;
  else if (words[1] == "JOIN_ARRAY")
      flag = JOIN_ARRAY;
  else if (words[1] == "IROTAT")
      flag = IROTAT;
  else if (words[1] == "SOLVENT_POINTERS")
      flag = SOLVENT_POINTERS;
  else if (words[1] == "ATOMS_PER_MOLECULE")
      flag = ATOMS_PER_MOLECULE;
  else if (words[1] == "BOX_DIMENSIONS")
     flag = BOX_DIMENSIONS;
  else if (words[1] == "RADIUS_SET")
     flag = RADIUS_SET;
  else if (words[1] == "RADII")
    flag = RADII;
  else if (words[1] == "SCREEN")
    flag = SCREEN;
  else
    throw SireError::program_bug( QObject::tr(
      "Does not know what to do with this flag: '%1'").arg(words[1]), CODELOC );
}

/** Processes a line that starts with %FORMAT*/
void Amber::processFormatLine(const QStringList &words, FortranFormat &format)
{
  //%FORMAT(20a4)
  QString tmp = words[0];
  tmp = tmp.remove( QString("%FORMAT(") );
  tmp = tmp.remove( QChar(')') );
  // QRegExp must match either (a,I,E)
  QString match = " ";
  QRegExp rx("[aIE]");
  
  if (tmp.contains(rx) )
    match = rx.cap(0);
  else
    throw SireError::program_bug( QObject::tr(
      "The format line '%1' does not contain a supported format type").arg(tmp), CODELOC);

  QStringList elements = tmp.split(match);
  bool ok;
  int repeat = elements[0].toInt(&ok);
  QStringList elem2;
  int size;
  int decimal;
  if (elements[1].contains(".")) 
    {
      elem2 =  elements[1].split(".");
      size = elem2[0].toInt(&ok);
      decimal = elem2[1].toInt(&ok);
    }
  else
    {
      size = elements[1].toInt(&ok);
      decimal = 0;
    }
  format.repeat = repeat;
  format.type = match;
  format.size = size;
  format.decimal = decimal;
  //qDebug() << "MY FORMAT " << format.repeat << " " << format.type << " " << format.size << " " << format.decimal;
}

/** Processes the line that contains the VERSION of the top file*/
void Amber::processVersionLine(const QStringList &words, QString &version)
{
  //%VERSION VERSION_STAMP = V0001.000 DATE = 05/22/06  12:10:21
  version = words[3];
}

/** Processes the title of the top file*/
void Amber::processTitleLine(const QString &line, const FortranFormat &format, QString &title)
{
  //FORMAT(20a4) reads up to 20 times 4 characters and treat them as a string
  // However, FORMAT is ignored in the TOP file, so we merely set title to line
  //qDebug() << "TITLE LINE IS " << line;
  title = line;
}

/** Processes a line of INTEGER in the top file */
void Amber::processIntegerLine(const QString &line, const FortranFormat &format, QList<int> &intarray)
{
  //FORMAT(10i8) reads up to 10 times 8 characters and treat them as an integer
  if ( format.type != "I") 
    throw SireError::program_bug( QObject::tr(
	      "Format '%1' is not supported, should be I").arg(format.type), CODELOC);
  int count = 0;
  for (int i= 0 ; i < line.size() ; i = i + format.size )
    {
      if ( ++count > format.repeat || (i + format.size) > line.size() )
	break;
      QString element = line.mid(i, format.size );
      bool ok;
      int data = element.toInt(&ok);
      intarray.append(data);
    }
}
/** Processes a line of STRINGS in the top file */
void Amber::processStringLine(const QString &line, const FortranFormat &format, QStringList &stringarray)
{
  //FORMAT(20a4) 
  if ( format.type != "a") 
    throw SireError::program_bug( QObject::tr(
	      "Format '%1' is not supported, should be a").arg(format.type), CODELOC);
  int count = 0;
  for (int i= 0 ; i < line.size() ; i = i + format.size )
    {
      if ( ++count > format.repeat || (i + format.size) > line.size() )
	break;
      QString str = line.mid(i, format.size );
      // Should we remove trailing spaces?
      str = str.trimmed();
      // Terminate if empty string
      if (str.size() == 0)
	break;
      stringarray.append(str);
    }
}
/** Processes a line of type DOUBLE in the top file*/
void Amber::processDoubleLine(const QString &line, const FortranFormat &format, QList<double> &doublearray)
{
  //FORMAT(5E16.8) 
  if ( format.type != "E") 
    throw SireError::program_bug( QObject::tr(
	      "Format '%1' is not supported, should be E").arg(format.type), CODELOC);
  int count = 0;
  for (int i= 0 ; i < line.size() ; i = i + format.size )
    {
      if ( ++count > format.repeat || (i + format.size) > line.size() )
	break;
      QString element = line.mid(i, format.size );
      bool ok;
      double data = element.toDouble(&ok);
      //qDebug() << "element " << element << "Data " << data;
      //printf("%16.8f\n", data);
      doublearray.append(data);
    }
}


void Amber::setAtomParameters(AtomEditor &editatom, MolEditor &editmol, QList<double> &crdCoords, 
				  PropertyName &coords_property, QList<double> &charge, PropertyName &charge_property, 
				  QList<double> &mass, PropertyName &mass_property, QList<int> &atomTypeIndex, 
				  QList<int> &nonbondedParmIndex, QList<double> &lennardJonesAcoef, 
				  QList<double> &lennardJonesBcoef, PropertyName &lj_property, 
				  QStringList &amberAtomType, PropertyName &ambertype_property, QList<int> &pointers)
{
  //AtomEditor editatom = editmol.atom(AtomIdx(atomIndex));
  int atomNumber = editatom.number().value();
  
  // set the coordinates. index into coordinates array is idx = 3 * ( atom.number().value() -1)
  int array_index = 3 * ( atomNumber - 1 );
  Vector coords = Vector(crdCoords[array_index], crdCoords[array_index+1],crdCoords[array_index+2] );
  editatom.setProperty( coords_property.source(), coords);

  // set the charges
  SireUnits::Dimension::Charge chg = ( charge[atomNumber - 1] / this->AMBERCHARGECONV ) * mod_electron;
  editatom.setProperty( charge_property, chg);
  
  // set the masses
  SireUnits::Dimension::MolarMass ms = mass[atomNumber - 1] * g_per_mol; 
  editatom.setProperty( mass_property.source(), ms);
  
  // set the LJ parameters
  // For atom 'i' first get the 'itype' from atomTypeIndex
  // Then lookup the index 'inbparams' for an interaction of 'itype' with 'itype' in nonbondedParmIndex
  // which is calculated as 'inbparams' = pointers[NTYPES]*(itype-1)+itype
  // Then lookup the values of A & B in lennardJonesAcoef and lennardJonesBcoef
  // iAcoef = lennardJonesAcoef[inbparams] iBcoef =  lennardJonesBcoef[inbparams]
  
  int itype = atomTypeIndex[ atomNumber - 1 ];
  int inbparams = nonbondedParmIndex[ pointers[NTYPES] * (itype - 1) + itype - 1 ];
  double iAcoef = lennardJonesAcoef[ inbparams - 1 ];
  double iBcoef = lennardJonesBcoef[ inbparams - 1 ];
  double sigma, epsilon, rstar;
  // is there a SMALL?
  if (iAcoef < 0.000001)
    {
      sigma = 0.0;
      epsilon = 0.0;
      rstar = 0.0;
    }
  else
    {
      // and convert lennardJonesAcoef & lennardJonesBcoef into angstroms and kcal/mol-1
      sigma = std::pow( iAcoef / iBcoef ,  1/6. );
      epsilon = pow_2( iBcoef ) / (4*iAcoef);
      rstar = (sigma/2.)* std::pow(2.0, 1/6. ) ;
    }
  //qDebug() << " Atom " << atomNumber << " itype " << itype << " inbparams " << inbparams << " iAcoef " << iAcoef << " iBcoef " << iBcoef << " sigma " << sigma << " epsilon " << epsilon << " rstar " << rstar ;
  // Note that the amber par files give rstar=(sigma/2)*(2**(1/6.)) instead of sigma
  LJParameter lj( sigma * angstrom, epsilon * kcal_per_mol);
  editatom.setProperty( lj_property.source(), lj);
  
  // set the Amber atom type
  QString ambertype = amberAtomType[ atomNumber - 1 ];
  editatom.setProperty( ambertype_property.source(), ambertype);

  editmol = editatom.molecule();
}


void Amber::setConnectivity(MolEditor &editmol, QList<int> &pointers, 
			    QList<int> &bondsIncHydrogen, QList<int> &bondsWithoutHydrogen,
			    ConnectivityEditor &connectivity, PropertyName &connectivity_property)
{
  
  QSet<AtomNum> moleculeAtomNumbers = this->_pvt_selectAtomsbyNumber(editmol);
 
  for (int i=0 ; i < 3 * pointers[NBONH] ; i = i + 3)
    {
      int index0 = bondsIncHydrogen[ i ] / 3 + 1 ;
      AtomNum number0 = AtomNum( index0 );
      int index1 = bondsIncHydrogen[ i + 1] / 3 + 1 ;
      AtomNum number1 = AtomNum( index1 );
      //int bondParamIndex = bondsIncHydrogen[ i + 2 ];
      if ( moleculeAtomNumbers.contains(number0) || moleculeAtomNumbers.contains(number1) ) 
	{
	  AtomIdx atom0 = editmol.select( number0 ).index();
	  AtomIdx atom1 = editmol.select( number1 ).index();
	  //qDebug() << " Connect " << bondedAtom0Number << " and " << bondedAtom1Number;
	  connectivity.connect( atom0, atom1 );
	}
    }
  for (int i=0 ; i < 3 * pointers[MBONA] ; i = i + 3)
    {
      int index0 = bondsWithoutHydrogen[ i ] / 3 + 1 ;
      AtomNum number0 = AtomNum( index0 );
      int index1 = bondsWithoutHydrogen[ i + 1] / 3 + 1 ;
      AtomNum number1 = AtomNum( index1 );
      //int bondParamIndex = bondsWithoutHydrogen[ i + 2 ];
      if ( moleculeAtomNumbers.contains(number0) || moleculeAtomNumbers.contains(number1) ) 
	{
	  AtomIdx atom0 = editmol.select( number0 ).index();
	  AtomIdx atom1 = editmol.select( number1 ).index();
	  //qDebug() << " Connect " << bondedAtom0Number << " and " << bondedAtom1Number;
	  connectivity.connect( atom0, atom1 );
	}
    }
  editmol.setProperty( connectivity_property.source(), connectivity.commit() );
}

void Amber::setBonds(MolEditor &editmol, int pointer,
		     QList<int> &bondsArray, 
		     QList<double> &bondForceConstant, QList<double> &bondEquilValue,
		     TwoAtomFunctions &bondfuncs, PropertyName &bond_property)
{
  QSet<AtomNum> moleculeAtomNumbers = this->_pvt_selectAtomsbyNumber(editmol);

  for (int i=0 ; i < 3 * pointer ; i = i + 3)
    {
      int index0 = bondsArray[ i ] / 3 + 1 ;
      AtomNum number0 = AtomNum( index0 );
      int index1 = bondsArray[ i + 1] / 3 + 1 ;
      AtomNum number1 = AtomNum( index1 );
      int paramIndex = bondsArray[ i + 2 ];
      if ( moleculeAtomNumbers.contains(number0) || moleculeAtomNumbers.contains(number1) ) 
	{
	  AtomIdx atom0 = editmol.select( number0 ).index();
	  AtomIdx atom1 = editmol.select( number1 ).index();

	  Symbol r = InternalPotential::symbols().bond().r();

	  double k = bondForceConstant[ paramIndex -1 ];
	  double r0 = bondEquilValue[ paramIndex - 1 ];

	  Expression bondfunc = k * SireMaths::pow_2(r - r0);
	  // IS IT CORRECT ? AM I NOT GOING TO COUNT MORE THAN ONCE BONDS ACROSS RESIDUES?
	  bondfuncs.set( atom0, atom1, bondfunc );
	}
    }
  editmol.setProperty( bond_property.source(), bondfuncs );
}

void Amber::setAngles(MolEditor &editmol, int pointer,
		      QList<int> &anglesArray, 
		      QList<double> &angleForceConstant, QList<double> &angleEquilValue,
		      ThreeAtomFunctions &anglefuncs, PropertyName &angle_property)
{
  QSet<AtomNum> moleculeAtomNumbers = this->_pvt_selectAtomsbyNumber(editmol);

  for (int i=0 ; i < 4 * pointer ; i = i + 4)
    {
      int index0 = anglesArray[ i ] / 3 + 1 ;
      AtomNum number0 = AtomNum( index0 );
      int index1 = anglesArray[ i + 1] / 3 + 1 ;
      AtomNum number1 = AtomNum( index1 );
      int index2 = anglesArray[ i + 2] / 3 + 1 ;
      AtomNum number2 = AtomNum( index2 );
      int paramIndex = anglesArray[ i + 3 ];

      if ( moleculeAtomNumbers.contains(number0) || 
	   moleculeAtomNumbers.contains(number1) || 
	   moleculeAtomNumbers.contains(number2)  ) 
	{
	  AtomIdx atom0 = editmol.select( number0 ).index();
	  AtomIdx atom1 = editmol.select( number1 ).index();
	  AtomIdx atom2 = editmol.select( number2 ).index();

	  Symbol theta = InternalPotential::symbols().angle().theta();

	  double k = angleForceConstant[ paramIndex - 1 ];
	  double theta0 = angleEquilValue[ paramIndex - 1 ];// radians

	  Expression anglefunc = k * SireMaths::pow_2(theta - theta0);
	  // IS IT CORRECT ? AM I NOT GOING TO COUNT MORE THAN ONCE BONDS ACROSS RESIDUES?
	  anglefuncs.set( atom0, atom1, atom2, anglefunc );
	}
    }
  editmol.setProperty( angle_property.source(), anglefuncs );
}

void Amber::setDihedrals(MolEditor &editmol, int pointer,
			 QList<int> &dihedralsArray, 
			 QList<double> &dihedralForceConstant, QList<double> &dihedralPeriodicity, QList<double> &dihedralPhase,
			 FourAtomFunctions &dihedralfuncs, PropertyName &dihedral_property,
			 FourAtomFunctions &improperfuncs, PropertyName &improper_property)
{
  QSet<AtomNum> moleculeAtomNumbers = this->_pvt_selectAtomsbyNumber(editmol);
  
  // Don't bother since no dihedrals can be defined
  if (moleculeAtomNumbers.size() < 4)
    return;

  for (int i= 0 ; i < 5 * pointer ; i = i + 5)
    {
      bool ignored = false;
      bool improper = false;

      int index0 = dihedralsArray[ i ] / 3 + 1 ;
      int index1 = dihedralsArray[ i + 1 ] / 3 + 1 ;
      int index2 = dihedralsArray[ i + 2 ] / 3 + 1 ;
      int index3 = dihedralsArray[ i + 3 ] / 3 + 1 ;
      int paramIndex = dihedralsArray[ i + 4 ];
      
      AtomNum number0 = AtomNum( index0 );
      AtomNum number1 = AtomNum( index1 );
 
      // Note that if index2 is negative it indicates that end group interactions are ignored
      if (index2 < 0)
	{
	  ignored = true;
	  index2 = - index2 ;
	}
      AtomNum number2 = AtomNum( index2 );

      // Note that if index3 is negative, it indicates that the dihedral is improper
      if (index3 < 0)
	{
	  improper = true;
	  index3 = -index3;
	}
      AtomNum number3 = AtomNum( index3 );
      
      if ( moleculeAtomNumbers.contains(number0) || 
	   moleculeAtomNumbers.contains(number1) || 
	   moleculeAtomNumbers.contains(number2) || 
	   moleculeAtomNumbers.contains(number3) ) 
	{
	  qDebug() << "indices " << index0 << index1 << index2 << index3 << paramIndex;
	  Atom atom0 = editmol.select( number0 );
	  Atom atom1 = editmol.select( number1 );
	  Atom atom2 = editmol.select( number2 );
	  Atom atom3 = editmol.select( number3 );
	  if ( ignored )
	    {
	      qDebug() << " IGNORING " << atom0.name().value() << atom1.name().value() << atom2.name().value() << atom3.name().value() ;
	    }
	  else if ( improper )
	    {
	      qDebug() << " IMPROPER " << atom0.name().value() << atom1.name().value() << atom2.name().value() << atom3.name().value() ;
	    }
	  else
	    {
	      qDebug() << " REGULAR " << atom0.name().value() << atom1.name().value() << atom2.name().value() << atom3.name().value() ;
	    }
	}
    }
  editmol.setProperty( dihedral_property.source(), dihedralfuncs ); 
  editmol.setProperty( improper_property.source(), improperfuncs );  
}

const char* Amber::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Amber>() );
}

/** Internal function to create a set of atom numbers in an editmol*/
QSet<AtomNum> Amber::_pvt_selectAtomsbyNumber(const MolEditor &editmol)
{
  Selector<Atom> atoms = editmol.selectAllAtoms();
  AtomSelection moleculeAtoms = atoms.selection();
  QVector<AtomIdx> moleculeAtomIdxs = moleculeAtoms.selectedAtoms();
  QSet<AtomNum> moleculeAtomNumbers;
  foreach (AtomIdx atomIdx, moleculeAtomIdxs)
    {
      Atom atom = editmol.select(atomIdx);
      moleculeAtomNumbers.insert(atom.number());
    }
  return moleculeAtomNumbers;
}
