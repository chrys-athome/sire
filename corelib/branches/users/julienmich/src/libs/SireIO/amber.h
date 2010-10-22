/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREIO_AMBER_H
#define SIREIO_AMBER_H

#include "iobase.h"
#include "SireVol/space.h"

SIRE_BEGIN_HEADER

namespace SireIO
{
  class Amber;
}

QDataStream& operator<<(QDataStream&, const SireIO::Amber&);
QDataStream& operator>>(QDataStream&, SireIO::Amber&);

class QTextStream;

namespace SireMol
{
  class Molecules;
  class MolEditor;
  class AtomEditor;
  class ConnectivityEditor;
  class AtomNum;
}

namespace SireMM
{
  class TwoAtomFunctions;
  class ThreeAtomFunctions;
  class FourAtomFunctions;
  class CLJNBPairs;
}

namespace SireVol
{
  class Space;
}

namespace SireIO
{
using boost::tuple;

using SireMol::Molecules;
using SireMol::MolEditor;
using SireMol::AtomEditor;
using SireMol::ConnectivityEditor;
using SireMol::AtomNum;

using SireMM::TwoAtomFunctions;
using SireMM::ThreeAtomFunctions;
using SireMM::FourAtomFunctions;
using SireMM::CLJNBPairs;

using SireVol::SpacePtr;

///////////
/////////// FortranFormat is an internal class that holds the information about a format used in a top file entry
/////////// 

class FortranFormat
{
public:
  FortranFormat();
  FortranFormat( const int repeat, const QString type, const int size, const int decimal);
  ~FortranFormat();
  /// 10I8 'repeat' 'type' 'size', 'decimal' is 0
  // 5E16.8 'repeat' is 5, 'type' is 'E' 'size' is 16 'decimal' is 8
  int repeat;
  QString type;
  int size;
  int decimal;
};

/** This class is used to read in an AMBER top file and crd file 
    
    @author Julien Michel
*/
class SIREIO_EXPORT Amber
{
  friend QDataStream& ::operator<<(QDataStream&, const SireIO::Amber&);
  friend QDataStream& ::operator>>(QDataStream&, SireIO::Amber&);
  
 public:
  Amber();
  ~Amber();
  
  static const char* typeName();
  
  const char* what() const
  {
    return Amber::typeName();
  }
  
 tuple<Molecules,SpacePtr> readcrdtop(const QString &crdfile, const QString &topfile);

 private:
  /** enumerates the FLAGS in a TOP file*/
  enum { TITLE = 1, //a TITLE flag in a top file
	 POINTERS = 2, // a POINTERS flag in a top file
	 ATOM_NAME = 3, //a ATOM_NAME flag in a top file
	 CHARGE = 4, //a CHARGE flag in a top file
	 MASS = 5, // a MASS flag
	 ATOM_TYPE_INDEX = 6, //
	 NUMBER_EXCLUDED_ATOMS = 7,
	 NONBONDED_PARM_INDEX = 8,
	 RESIDUE_LABEL = 9,
	 RESIDUE_POINTER = 10,
	 BOND_FORCE_CONSTANT = 11,
	 BOND_EQUIL_VALUE = 12,
	 ANGLE_FORCE_CONSTANT = 13,
	 ANGLE_EQUIL_VALUE = 14,
	 DIHEDRAL_FORCE_CONSTANT = 15,
	 DIHEDRAL_PERIODICITY = 16,
	 DIHEDRAL_PHASE = 17,
	 SOLTY = 18,
	 LENNARD_JONES_ACOEF = 19,
	 LENNARD_JONES_BCOEF = 20,
	 BONDS_INC_HYDROGEN = 21,
	 BONDS_WITHOUT_HYDROGEN = 22,
	 ANGLES_INC_HYDROGEN = 23,
	 ANGLES_WITHOUT_HYDROGEN = 24,	
	 DIHEDRALS_INC_HYDROGEN = 25,
	 DIHEDRALS_WITHOUT_HYDROGEN = 26,
	 EXCLUDED_ATOMS_LIST = 27,
	 HBOND_ACOEF = 28,
	 HBOND_BCOEF = 29,
	 HBCUT = 30,
	 AMBER_ATOM_TYPE = 31,
	 TREE_CHAIN_CLASSIFICATION = 32,
	 JOIN_ARRAY = 33,
	 IROTAT = 34,
	 SOLVENT_POINTERS = 35,
	 ATOMS_PER_MOLECULE = 36,
	 BOX_DIMENSIONS = 37,
	 RADIUS_SET = 38,
	 RADII = 39,
	 SCREEN = 40
  };
  /** enumerates the POINTERS in a TOP file */
  enum { NATOM = 0, // total number of atoms
	 NTYPES = 1, // total number of distinct atom types
	 NBONH = 2, // number of bonds containing hydrogen
	 MBONA = 3, // number of bonds not containing hydrogen
	 NTHETH = 4, // number of angles containing hydrogen
	 MTHETA = 5, // number of angles not containing hydrogen
	 NPHIH = 6, // number of dihedrals containing hydrogen
	 MPHIA = 7, // number of dihedrals not containing hydrogen
	 NHPARM = 8, // currently not used
	 NPARM = 9, // currently  not used
	 NEXT = 10, // number of excluded atoms
	 NRES = 11, // number of residues
	 NBONA = 12, // MBONA + number of constraint bonds
	 NTHETA = 13, // MTHETA + number of constraint angles
	 NPHIA = 14, // MPHIA + number of constraint dihedrals
	 NUMBND = 15, // number of unique bond types
	 NUMANG = 16, // number of unique angle types
	 NPTRA = 17, // number of unique dihedral types
	 NATYP = 18, // number of atom types in parameter file, see SOLTY below
	 NPHB = 19, // number of distinct 10-12 hydrogen bond pair types
	 IFPERT = 20, // set to 1 if perturbation info is to be read in
	 NBPER = 21, // number of bonds to be perturbed
	 NGPER = 22, // number of angles to be perturbed
	 NDPER = 23, // number of dihedrals to be perturbed
	 MBPER = 24, // number of bonds with atoms completely in perturbed group
	 MGPER = 25, // number of agnles with atoms completely in perturbed group
	 MDPER = 26, // number of dihedrals with atoms completely in perturbed groups
	 IFBOX = 27, // set to 1 if standard periodic box, 2 when truncated octahedral
	 NMXRS = 28, // number of atoms in the largest residue
	 IFCAP = 29 // set to 1 if the CAP option from edit was specified
  };
  /** enumerate SOLVENT pointers*/
  enum { IPTRES = 0, //final residue that is considered part of the solute, reset in sander and gibbs
	 NSPM = 1,   //total number of molecules
	 NSPOL = 2   //the first solvent "molecule"
  };
  void processFlagLine(const QStringList &words, int &flag);
  void processFormatLine(const QStringList &words, FortranFormat &format);
  void processVersionLine(const QStringList &words, QString &version);
 
  void processTitleLine(const QString &line, const FortranFormat &format, QString &title);
  void processIntegerLine(const QString &line, const FortranFormat &format, QList<int> &intarray);
  void processStringLine(const QString &line, const FortranFormat &format, QStringList &stringarray);
  void processDoubleLine(const QString &line, const FortranFormat &format, QList<double> &doublearray);

  void setAtomParameters(AtomEditor &editatom, MolEditor &editmol, QList<double> &crdCoords, 
			 PropertyName &coords_property, QList<double> &charge, PropertyName &charge_property, 
			 QList<double> &mass, PropertyName &mass_property, QList<int> &atomTypeIndex, 
			 QList<int> &nonbondedParmIndex, QList<double> &lennardJonesAcoef, 
			 QList<double> &lennardJonesBcoef, PropertyName &lj_property, 
			 QStringList &amberAtomType, PropertyName &ambertype_property, QList<int> &pointers);

  void setConnectivity(MolEditor &editmol, int pointer, 
		       QList<int> &bondsArray, 
		       ConnectivityEditor &connectivity, PropertyName &connectivity_property);

  void setBonds(MolEditor &editmol, int pointer,
		QList<int> &bondsArray, 
		QList<double> &bondForceConstant, QList<double> &bondEquilValue,
		TwoAtomFunctions &bondfuncs, PropertyName &bond_property);
  
  void setAngles(MolEditor &editmol, int pointer,
		 QList<int> &anglesArray, 
		 QList<double> &angleForceConstant, QList<double> &angleEquilValue,
		 ThreeAtomFunctions &anglefuncs, PropertyName &angle_property);

  void setDihedrals(MolEditor &editmol, int pointer,
		    QList<int> &dihedralsArray, 
		    QList<double> &dihedralForceConstant, QList<double> &dihedralPeriodicity, QList<double> &dihedralPhase,
		    FourAtomFunctions &dihedralfuncs, PropertyName &dihedral_property,
		    FourAtomFunctions &improperfuncs, PropertyName &improper_property,
		    QHash<AtomNum, QList<AtomNum> > &atoms14);

  void setNonBondedPairs(MolEditor &editmol, int pointer,
			 QList<int> &numberExcludedAtoms, QList<int> &excludedAtomsList,
			 CLJNBPairs &nbpairs, PropertyName &nb_property,
			 QHash<AtomNum, QList<AtomNum> > &atoms14);

  QSet<AtomNum> _pvt_selectAtomsbyNumber(const MolEditor &editmol);

  static const double AMBERCHARGECONV = 18.2223;// The partial charges in the top file are not in electrons
  static const double AMBER14COUL = 1.0 / 1.2 ;
  static const double AMBER14LJ = 0.50 ;
};

}

Q_DECLARE_METATYPE( SireIO::Amber )

SIRE_EXPOSE_CLASS( SireIO::Amber )

SIRE_END_HEADER

#endif

