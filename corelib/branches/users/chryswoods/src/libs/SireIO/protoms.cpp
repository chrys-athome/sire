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

#include "SireMM/cljnbpairs.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QByteArray>

#include "protoms.h"

#include "pdb.h"

#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/moleditor.h"
#include "SireMol/reseditor.h"
#include "SireMol/atomeditor.h"
#include "SireMol/atomname.h"
#include "SireMol/resname.h"
#include "SireMol/resnum.h"
#include "SireMol/groupatomids.h"

#include "SireMol/mover.hpp"
#include "SireMol/selector.hpp"

#include "SireMol/atomcharges.h"

#include "SireMove/zmatrix.h"

#include "SireCAS/trigfuncs.h"

#include "SireMM/ljparameter.h"
#include "SireMM/atomljs.h"
#include "SireMM/internalff.h"

#include "SireUnits/units.h"

#include "SireBase/tempdir.h"
#include "SireBase/findexe.h"

#include "SireMove/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireIO;
using namespace SireMol;
using namespace SireMM;
using namespace SireMove;
using namespace SireID;
using namespace SireCAS;
using namespace SireUnits;
using namespace SireBase;
using namespace SireStream;

///////////
/////////// Implementation of ProtoMSParameters
///////////

ProtoMSParameters::ProtoMSParameters()
{}

ProtoMSParameters::~ProtoMSParameters()
{}

PropertyName ProtoMSParameters::charge_property( "charge" );
PropertyName ProtoMSParameters::lj_property( "LJ" );

PropertyName ProtoMSParameters::bond_property( "bond" );
PropertyName ProtoMSParameters::angle_property( "angle" );
PropertyName ProtoMSParameters::dihedral_property( "dihedral" );
PropertyName ProtoMSParameters::ub_property( "Urey-Bradley" );

PropertyName ProtoMSParameters::zmatrix_property( "z-matrix" );
PropertyName ProtoMSParameters::nb_property( "intrascale" );

///////////
/////////// Implementation of ProtoMS
///////////

static const RegisterMetaType<ProtoMS> r_protoms;

/** Serialise to a binary datastream */
QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds, const ProtoMS &protoms)
{
    writeHeader(ds, r_protoms, 1);
    
    SharedDataStream sds(ds);
    
    sds << protoms.paramfiles << protoms.protoms_exe;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, ProtoMS &protoms)
{
    VersionID v = readHeader(ds, r_protoms);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> protoms.paramfiles >> protoms.protoms_exe;
    }
    else
        throw version_error( v, "1", r_protoms, CODELOC );
        
    return ds;
}

ProtoMSParameters ProtoMS::protoms_parameters;

/** Constructor */
ProtoMS::ProtoMS()
{}

/** Constructor, specifying the location of ProtoMS */
ProtoMS::ProtoMS(const QString &protoms)
{
    this->setExecutable(protoms);
}

/** Destructor */
ProtoMS::~ProtoMS()
{}

/** Add a parameter file to the list of files which will be used
    to parameterise the molecules */
void ProtoMS::addParameterFile(const QString &paramfile)
{
    //get the absolute path to this file
    QFile f(paramfile);
    
    if (not f.open( QIODevice::ReadOnly)) 
    {
        throw SireError::file_error(f, CODELOC);
    }

    QString absfilepath = QFileInfo(f).absoluteFilePath();

    f.close();

    if (not paramfiles.contains(absfilepath))
        paramfiles.append(absfilepath);
}

/** Return the list of parameter files which will be used to 
    parameterise the molecules, in the order that they will
    be read */
QStringList ProtoMS::parameterFiles() const
{
    return paramfiles;
}

/** Set the path to the ProtoMS executable that will be used
    to parameterise the molecules */
void ProtoMS::setExecutable(const QString &protoms)
{
    protoms_exe = protoms;
}

/** Internal function used to write the ProtoMS command file */
QString ProtoMS::writeCommandFile(const TempDir &tmpdir, 
                                  const Molecule &molecule, int type) const
{
    //write a PDB of the molecule to the TMPDIR for ProtoMS to read
    // ProtoMS solutes can be named by setting the name of the
    // PDB file
    QString name = molecule.name();
    
    if (name.isEmpty())
        name = "molecule";
    
    {
        QFile f( QString("%1/%2").arg(tmpdir.path(),name) );
        f.open(QIODevice::WriteOnly);

        f.write( QString("header %1\n").arg(name).toAscii().constData() );
        
        PDB().write( molecule, f );
        
        f.close();
    }

    QString cmdfile = QString("%1/protoms_input").arg(tmpdir.path());
    
    QFile f(cmdfile);
    f.open(QIODevice::WriteOnly);
    
    QTextStream ts(&f);
    
    for (int i=0; i<paramfiles.count(); ++i)
    {
        ts << QString("parfile%1 %2\n").arg(i+1).arg(paramfiles[i]);
    }
    
    switch (type)
    {
    case PROTEIN:
        ts << "protein1 " << name << "\n";
        break;
    case SOLUTE:
        ts << "solute1 " << name << "\n";
        break;
    case SOLVENT:
        ts << "solvent1 " << name << "\n";
        break;
    
    default:
        throw SireError::program_bug( QObject::tr(
                "Unrecognised ProtoMS molecule type (%1)").arg(type), CODELOC );
    }
    
    ts << "streamheader off\n"
          "streaminfo stdout\n" 
          "streamwarning stdout\n"
          "streamfatal stdout\n"
          "streamparameters stdout\n\n"
          "chunk1 printparameters\n";
          
    f.close();
    
    return cmdfile;
}

/** Internal function used to write a shell file used to run ProtoMS */
QString ProtoMS::writeShellFile(const TempDir &tempdir, 
                                const QString &cmdfile) const
{
    QString shellfile = QString("%1/run_protoms.cmd").arg(tempdir.path());
    
    QFile f(shellfile);
    f.open(QIODevice::WriteOnly);
    
    QTextStream ts(&f);

    //set the script to change into the run directory of the job
    ts << QString("\ncd %1").arg(tempdir.path()) << "\n\n";

    //write the line used to run ProtoMS
    if (protoms_exe.isEmpty())
    {
        //the user hasn't specified a ProtoMS executable - try to find one
        QString found_protoms = SireBase::findExe("protoms2").absoluteFilePath();
        ts << QString("%1 %2 > protoms_output\n")
                    .arg(found_protoms, cmdfile);
    }
    else
        ts << QString("%1 %2 > protoms_output\n")
                        .arg(protoms_exe, cmdfile);
    
    f.close();
    
    return shellfile;
}

static QByteArray readAll(const QString &file)
{
    QFile f(file);
    
    if (f.open(QIODevice::ReadOnly))
    {
        return f.readAll();
    }
    else
        return QByteArray();
}

static Atom getSoluteAtom(const Molecule &molecule,
                          const QString &atomname, const QString &resname)
{
    return molecule.atom( AtomName(atomname, CaseInsensitive) +
                          ResName(resname, CaseInsensitive) );
}

static Atom getProteinAtom(const Molecule &molecule,
                           const QString &atomname, const QString &resnum)
{
    bool ok;
    
    ResNum rnum( resnum.toInt(&ok) );
    
    if (not ok)
        throw SireError::program_bug( QObject::tr(
            "The string '%1' should contain a residue number!").arg(resnum),
                CODELOC );
                
    return molecule.atom( AtomName(atomname, CaseInsensitive) + rnum);
}

static AtomEditor getSolventAtom(const MolEditor &molecule, const QString &atomname)
{
    return molecule.atom( AtomName(atomname, CaseInsensitive) );
}

static AtomEditor getSoluteAtom(const MolEditor &molecule,
                                const QString &atomname, const QString &resname)
{
    return molecule.atom( AtomName(atomname, CaseInsensitive) +
                          ResName(resname, CaseInsensitive) );
}

static AtomEditor getProteinAtom(const MolEditor &molecule,
                                 const QString &atomname, const QString &resnum)
{
    bool ok;
    
    ResNum rnum( resnum.toInt(&ok) );
    
    if (not ok)
        throw SireError::program_bug( QObject::tr(
            "The string '%1' should contain a residue number!").arg(resnum),
                CODELOC );
                
    return molecule.atom( AtomName(atomname, CaseInsensitive) + rnum);
}

/** This processes the output line that contains the z-matrix */
void ProtoMS::processZMatrixLine(const QStringList &words, const Molecule &mol,
                                 int type, ZMatrix &zmatrix) const
{
    Atom atom, bond, angle, dihedral;
    
    //skip lines involving dummy atoms
    if ( words[4] == "DUM" or words[9] == "DUM" or
         words[14] == "DUM" or words[19] == "DUM" )
    {
        return;
    }
    
    if (type == SOLVENT)
        return;
        
    else if (type == SOLUTE)
    {
        atom = getSoluteAtom(mol, words[2], words[4]);
        bond = getSoluteAtom(mol, words[7], words[9]);
        angle = getSoluteAtom(mol, words[12], words[14]);
        dihedral = getSoluteAtom(mol, words[17], words[19]);
    }
    else if (type == PROTEIN)
    {
        atom = getProteinAtom(mol, words[2], words[4]);
        bond = getProteinAtom(mol, words[7], words[9]);
        angle = getProteinAtom(mol, words[12], words[14]); 
        dihedral = getProteinAtom(mol, words[17], words[19]);
    }
    
    zmatrix.add( atom.index(), bond.index(), 
                 angle.index(), dihedral.index() );
}

void ProtoMS::processBondDeltaLine(const QStringList &words, const Molecule &mol,
                                   int type, ZMatrix &zmatrix) const
{
    //skip lines involving dummy atoms
    if ( words[4] == "DUM" or words[9] == "DUM" )
    {
        return;
    }

    Atom atom, bond;
    
    if (type == SOLVENT)
        return;
        
    else if (type == SOLUTE)
    {
        atom = getSoluteAtom(mol, words[2], words[4]);
        bond = getSoluteAtom(mol, words[7], words[9]);
    }
    else if (type == PROTEIN)
    {
        atom = getProteinAtom(mol, words[2], words[4]);
        bond = getProteinAtom(mol, words[7], words[9]);
    }

    try
    {
        zmatrix.setBondDelta( atom.index(), bond.index(),
                              words[12].toDouble() * angstrom );
    }
    catch(const SireMove::zmatrix_error&)
    {
        //skip z-matrix errors...
        return;
    }
}

void ProtoMS::processAngleDeltaLine(const QStringList &words, const Molecule &mol,
                                    int type, ZMatrix &zmatrix) const
{
    //skip lines involving dummy atoms
    if ( words[4] == "DUM" or words[9] == "DUM" or words[14] == "DUM" )
    {
        return;
    }

    Atom atom, bond, angle;
    
    if (type == SOLVENT)
        return;
        
    else if (type == SOLUTE)
    {
        atom = getSoluteAtom(mol, words[2], words[4]);
        bond = getSoluteAtom(mol, words[7], words[9]);
        angle = getSoluteAtom(mol, words[12], words[14]);
    }
    else if (type == PROTEIN)
    {
        atom = getProteinAtom(mol, words[2], words[4]);
        bond = getProteinAtom(mol, words[7], words[9]);
        angle = getProteinAtom(mol, words[12], words[14]);
    }

    try
    {
        zmatrix.setAngleDelta( atom.index(), bond.index(), angle.index(),
                               words[17].toDouble() * radians );
    }
    catch(const SireMove::zmatrix_error&)
    {
        //skip zmatrix errors
        return;
    }
}

void ProtoMS::processDihedralDeltaLine(const QStringList &words, const Molecule &mol,
                                       int type, ZMatrix &zmatrix) const
{
    //skip lines involving dummy atoms
    if ( words[4] == "DUM" or words[9] == "DUM" or
         words[14] == "DUM" or words[19] == "DUM" )
    {
        return;
    }

    Atom atom, bond, angle, dihedral;
    
    if (type == SOLVENT)
        return;
        
    else if (type == SOLUTE)
    {
        atom = getSoluteAtom(mol, words[2], words[4]);
        bond = getSoluteAtom(mol, words[7], words[9]);
        angle = getSoluteAtom(mol, words[12], words[14]);
        dihedral = getSoluteAtom(mol, words[17], words[19]);
    }
    else if (type == PROTEIN)
    {
        atom = getProteinAtom(mol, words[2], words[4]);
        bond = getProteinAtom(mol, words[7], words[9]);
        angle = getProteinAtom(mol, words[12], words[14]);
        dihedral = getProteinAtom(mol, words[17], words[19]);
    }

    try
    {
        zmatrix.setDihedralDelta( atom.index(), bond.index(), 
                                  angle.index(), dihedral.index(),
                                  words[22].toDouble() * radians );
    }
    catch(const SireMove::zmatrix_error &e)
    {
        qDebug() << e.toString();
    
        //ignore zmatrix errors
        return;
    }
}

/** This processes the output line that contains the atom parameters */
void ProtoMS::processAtomLine(const QStringList &words, MolEditor &editmol,
                              int type, const QString &charge_property,
                              const QString &lj_property) const
{
    AtomEditor atom;
    
    if (type == SOLUTE)
        atom = getSoluteAtom(editmol, words[2], words[4]);
                             
    else if (type == SOLVENT)
        atom = getSolventAtom(editmol, words[2]);
        
    else if (type == PROTEIN)
        atom = getProteinAtom(editmol, words[2], words[4]);

    atom.setProperty( charge_property, words[7].toDouble() * mod_electron );
    atom.setProperty( lj_property, LJParameter( words[9].toDouble() * angstrom,
                                                words[11].toDouble() * kcal_per_mol ) );

    editmol = atom.molecule();
}

/** This processes the output line that contains the bond parameters */
void ProtoMS::processBondLine(const QStringList &words, const Molecule &mol,
                              int type, TwoAtomFunctions &bondfuncs) const
{
    if (type == SOLVENT)
        return;

    Symbol r = InternalPotential::symbols().bond().r();
    
    Expression bondfunc = words[12].toDouble() 
                                * SireMaths::pow_2( r - words[14].toDouble() );

    Atom atom0, atom1;

    if (type == SOLUTE)
    {
        atom0 = getSoluteAtom(mol, words[2], words[4]);
        atom1 = getSoluteAtom(mol, words[7], words[9]);
    }
    else if (type == PROTEIN)
    {
        atom0 = getProteinAtom(mol, words[2], words[4]);
        atom1 = getProteinAtom(mol, words[7], words[9]);
    }
    
    bondfuncs.set( atom0.index(), atom1.index(), bondfunc );
}

/** This processes the output line that contains the angle parameters */
void ProtoMS::processAngleLine(const QStringList &words, const Molecule &mol,
                               int type, ThreeAtomFunctions &anglefuncs) const
{
    if (type == SOLVENT)
        return;

    Symbol theta = InternalPotential::symbols().angle().theta();
    
    Expression anglefunc = words[17].toDouble() 
                    * SireMaths::pow_2( theta - (words[19].toDouble()*radians).value() );

    Atom atom0, atom1, atom2;

    if (type == SOLUTE)
    {
        atom0 = getSoluteAtom(mol, words[2], words[4]);
        atom1 = getSoluteAtom(mol, words[7], words[9]);
        atom2 = getSoluteAtom(mol, words[12], words[14]);
    }
    else if (type == PROTEIN)
    {
        atom0 = getProteinAtom(mol, words[2], words[4]);
        atom1 = getProteinAtom(mol, words[7], words[9]);
        atom2 = getProteinAtom(mol, words[12], words[14]);
    }
    
    anglefuncs.set( atom0.index(), atom1.index(), atom2.index(), anglefunc );
}

/** This processes the output line that contains the bond parameters */
void ProtoMS::processUBLine(const QStringList &words, const Molecule &mol,
                            int type, TwoAtomFunctions &ubfuncs) const
{
    if (type == SOLVENT)
        return;

    Symbol r = InternalPotential::symbols().ureyBradley().r();
    
    Expression ubfunc = words[12].toDouble() 
                                * SireMaths::pow_2( r - words[14].toDouble() );

    Atom atom0, atom1;

    if (type == SOLUTE)
    {
        atom0 = getSoluteAtom(mol, words[2], words[4]);
        atom1 = getSoluteAtom(mol, words[7], words[9]);
    }
    else if (type == PROTEIN)
    {
        atom0 = getProteinAtom(mol, words[2], words[4]);
        atom1 = getProteinAtom(mol, words[7], words[9]);
    }
    
    ubfuncs.set( atom0.index(), atom1.index(), ubfunc );
}

/** This processes the lines that contain the dihedral parameters */
QString ProtoMS::processDihedralLine(QTextStream &ts, const QStringList &words,
                                     const Molecule &mol, int type,
                                     FourAtomFunctions &dihedralfuncs) const
{
    //read in the parameter - each cosine term is on a separate line
    QString line = ts.readLine();
    
    Expression dihedralfunc;
    
    Symbol phi = InternalPotential::symbols().dihedral().phi();
    
    while (not line.isNull())
    {
        QStringList words = line.split(" ", QString::SkipEmptyParts);
    
        if (words[1] != "DihedralParameter")
            break;
        
        //there are four parameters, k0, k1, k2, k3
        // The cosine function is;
        //  k0 { 1 + k1 [ cos(k2*phi + k3) ] }
        double k0 = words[3].toDouble();
        double k1 = words[5].toDouble();
        double k2 = (words[7].toDouble() * radians).value();
        double k3 = words[9].toDouble();
        
        dihedralfunc += k0 * ( 1 + k1*( Cos(k2*phi + k3) ) );
        
        line = ts.readLine();
    }

    Atom atom0, atom1, atom2, atom3;
    
    if (type == SOLVENT)
        return line;
        
    else if (type == SOLUTE)
    {
        atom0 = getSoluteAtom(mol, words[2], words[4]);
        atom1 = getSoluteAtom(mol, words[7], words[9]);
        atom2 = getSoluteAtom(mol, words[12], words[14]);
        atom3 = getSoluteAtom(mol, words[17], words[19]);
    }
    else if (type == PROTEIN)
    {
        atom0 = getProteinAtom(mol, words[2], words[4]);
        atom1 = getProteinAtom(mol, words[7], words[9]);
        atom2 = getProteinAtom(mol, words[12], words[14]);
        atom3 = getProteinAtom(mol, words[17], words[19]);
    }
    
    dihedralfuncs.set( atom0.index(), atom1.index(), atom2.index(),
                       atom3.index(), dihedralfunc );

    return line;
}

/** Process the line that contains information about the 
    intramolecular non-bonded pairs */
void ProtoMS::processNBLine(const QStringList &words, const Molecule &mol, int type,
                            CLJNBPairs &cljpairs) const
{
    Atom atom0, atom1;
    
    if (type == SOLVENT)
        return;
        
    else if (type == SOLUTE)
    {
        atom0 = getSoluteAtom(mol, words[2], words[4]);
        atom1 = getSoluteAtom(mol, words[7], words[9]);
    }
    else if (type == PROTEIN)
    {
        atom0 = getProteinAtom(mol, words[2], words[4]);
        atom1 = getProteinAtom(mol, words[7], words[9]);
    }

    double cscl = words[12].toDouble();
    double ljscl = words[14].toDouble();

    cljpairs.set( atom0.cgAtomIdx(), atom1.cgAtomIdx(),
                  CLJScaleFactor(cscl, ljscl) );
}

/** Internal function used to run ProtoMS to get it to 
    parameterise a molecule */
Molecule ProtoMS::runProtoMS(const Molecule &molecule, int type,
                             const PropertyMap &map) const
{
    //get the names of the properties that we need
    QString charge_property = map[ parameters().charge() ].source();
    QString lj_property = map[ parameters().lj() ].source();
    
    QString bond_property = map[ parameters().bond() ].source();
    QString angle_property = map[ parameters().angle() ].source();
    QString dihedral_property = map[ parameters().dihedral() ].source();
    QString ub_property = map[ parameters().ureyBradley() ].source();
    QString nb_property = map[ parameters().nonBonded() ].source();
    
    QString zmatrix_property = map[ parameters().zmatrix() ].source();

    //create a temporary directory in which to run ProtoMS
    QString tmppath = QDir::temp().absolutePath();
    
    if (tmppath.isEmpty())
        tmppath = QDir::temp().absolutePath();

    TempDir tmpdir(tmppath);

    //write the ProtoMS command file
    QString cmdfile = this->writeCommandFile(tmpdir, molecule, type);

    //write the file processed by the shell used to run the job
    QString shellfile = this->writeShellFile(tmpdir, cmdfile);

    //now run the command
    if (std::system(0))
    {
        std::system( QString("source %1").arg(shellfile).toAscii().constData() );
    }
    else
        throw SireError::unsupported( QObject::tr(
            "The operating system does not support the use of std::system(). "
            "It is not possible to use ProtoMS to parameterise molecules."),
                CODELOC );


    QFile f( QString("%1/protoms_output").arg(tmpdir.path()) );
    
    if ( not (f.exists() and f.open(QIODevice::ReadOnly)) )
    {
        QByteArray shellcontents = ::readAll(shellfile);
        QByteArray cmdcontents = ::readAll(QString("%1/protoms_input").arg(tmpdir.path()));
    
        throw SireError::process_error( QObject::tr(
            "There was an error running the ProtoMS - no output was created.\n"
            "The shell script used to run the job was;\n"
            "*****************************************\n"
            "%1\n"
            "*****************************************\n"
            "The ProtoMS input used to run the job was;\n"
            "*****************************************\n"
            "%2\n"
            "*****************************************\n")
                .arg( QLatin1String(shellcontents),
                      QLatin1String(cmdcontents) ), CODELOC );
    }

    //now read the output to parameterise the molecule
    QTextStream ts(&f);
    
    QString line = ts.readLine();
    
    MolEditor editmol = molecule.edit();
    
    ZMatrix zmatrix( editmol );
    
    TwoAtomFunctions bondfuncs(molecule);
    ThreeAtomFunctions anglefuncs(molecule);
    FourAtomFunctions dihedralfuncs(molecule);
    TwoAtomFunctions ubfuncs(molecule);
    
    CLJNBPairs nbpairs;
    
    if (type == SOLUTE)
    {
        //by default, say that all atom pairs are bonded
        nbpairs = CLJNBPairs( molecule.data().info(), CLJScaleFactor(0,0) );
    }
    else if (type == PROTEIN)
    {
        //by default, say that all atom pairs are non-bonded...
        nbpairs = CLJNBPairs( molecule.data().info(), CLJScaleFactor(1,1) );
        
        //...except for intra-residue pairs
        int nres = molecule.nResidues();
        
        for (ResIdx i(0); i<nres; ++i)
        {
            Residue residue = molecule.residue(i);
        
            int nats = residue.nAtoms();
            
            for (Index j(0); j<nats-1; ++j)
            {
                Atom atom0 = residue.atom(j);
                
                nbpairs.set( atom0.cgAtomIdx(), atom0.cgAtomIdx(), 
                             CLJScaleFactor(0,0) );
                
                for (Index k(j+1); k<nats; ++k)
                {
                    Atom atom1 = residue.atom(k);
                
                    nbpairs.set( atom0.cgAtomIdx(), atom1.cgAtomIdx(), 
                                 CLJScaleFactor(0,0) );
                    nbpairs.set( atom1.cgAtomIdx(), atom1.cgAtomIdx(), 
                                 CLJScaleFactor(0,0) );
                }
            }
        }
    }
    
    QStringList fatal_errors;
    
    while (not line.isNull())
    {
        if (line.startsWith("PARAMS "))
        {
            QStringList words = line.split(" ", QString::SkipEmptyParts);
            
            if (words[1] == "ZMATRIX")
                this->processZMatrixLine(words, editmol, type, zmatrix);

            else if (words[1] == "Atom")
                this->processAtomLine(words, editmol, type,
                                      charge_property, lj_property);
            
            else if (words[1] == "Bond")
                this->processBondLine(words, molecule, type, bondfuncs);
                
            else if (words[1] == "BondDelta")
                this->processBondDeltaLine(words, molecule, type, zmatrix);
            
            else if (words[1] == "Angle")
                this->processAngleLine(words, molecule, type, anglefuncs);
                
            else if (words[1] == "AngleDelta")
                this->processAngleDeltaLine(words, molecule, type, zmatrix);
                
            else if (words[1] == "UreyBradley")
                this->processUBLine(words, molecule, type, ubfuncs);
            
            else if (words[1] == "Dihedral")
            {
                line = this->processDihedralLine(ts, words, molecule, 
                                                 type, dihedralfuncs);
                continue;
            }

            else if (words[1] == "DihedralDelta")
                this->processDihedralDeltaLine(words, molecule, type, zmatrix);

            else if (words[1] == "NB")
                this->processNBLine(words, molecule, type, nbpairs);
        }
        else if (line.startsWith("FATAL"))
        {
            fatal_errors.append(line);
        }

        line = ts.readLine();
    }
    
    if (not fatal_errors.isEmpty())
    {
        //something went wrong in ProtoMS
        throw SireError::process_error( QObject::tr(
            "Something went wrong in ProtoMS when parameterising the molecule. "
            "Here is the error output from ProtoMS.\n%1")
                .arg(fatal_errors.join("\n")), CODELOC );
    }

    if ( not (editmol.hasProperty(charge_property) and
             editmol.hasProperty(lj_property)) )
    {
        QStringList errors;
        errors.append( QObject::tr("The molecule is missing either the %1 (charge) "
                 "or the %2 (LJ) properties. The available properties are %3.\n"
                 "Here's the output from ProtoMS. Can you see what's gone wrong?\n")
                    .arg(charge_property, lj_property)
                    .arg(Sire::toString(editmol.propertyKeys())) );
                    
        QTextStream ts2(&f);
        
        QString line2 = ts2.readLine();
        
        while (not line2.isNull())
        {
            errors.append(line2);
            line2 = ts2.readLine();
        }
        
        throw SireError::process_error( errors.join("\n"), CODELOC );
    }

    if (type == SOLUTE or type == PROTEIN)
    {
        editmol.setProperty( zmatrix_property, zmatrix );
        editmol.setProperty( bond_property, bondfuncs );
        editmol.setProperty( angle_property, anglefuncs );
        editmol.setProperty( dihedral_property, dihedralfuncs );
        editmol.setProperty( ub_property, ubfuncs );
        editmol.setProperty( nb_property, nbpairs );
    }
    
    return editmol.commit();
}

/** Parameterise the molecule 'molecule' as a 'type' type of
    molecule (PROTEIN, SOLUTE or SOLVENT) */
Molecule ProtoMS::parameterise(const Molecule &molecule, int type,
                               const PropertyMap &map)
{
    if (type != PROTEIN and type != SOLUTE and type != SOLVENT)
        throw SireError::invalid_arg( QObject::tr(
            "Unrecognised ProtoMS molecule type (%1). Only "
            "ProtoMS::PROTEIN, ProtoMS::SOLUTE and ProtoMS::SOLVENT "
            "are supported.").arg(type), CODELOC );

    return this->runProtoMS(molecule, type, map);
}

/** Parameterise the molecules 'molecules' as 'type' type of
    molecules (PROTEIN, SOLUTE or SOLVENT) */
Molecules ProtoMS::parameterise(const Molecules &molecules, int type,
                                const PropertyMap &map)
{
    Molecules new_molecules = molecules;
    
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        new_molecules.update( this->parameterise(it->molecule(), type, map) );
    }

    return new_molecules;
}
