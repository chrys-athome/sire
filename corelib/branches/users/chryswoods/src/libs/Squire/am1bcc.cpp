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

#include <cstdlib>

#include "am1bcc.h"

#include "SireMol/moleculedata.h"
#include "SireMol/moleculeinfodata.h"
#include "SireMol/atomname.h"
#include "SireMol/resname.h"
#include "SireMol/groupatomids.h"

#include "SireUnits/units.h"
#include "SireUnits/dimensions.h"

#include "SireBase/tempdir.h"
#include "SireBase/process.h"

#include "SireIO/pdb.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireMol;
using namespace SireIO;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<AM1BCC> r_am1bcc;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const AM1BCC &am1bcc)
{
    writeHeader(ds, r_am1bcc, 1);
    
    SharedDataStream sds(ds);
    
    sds << am1bcc.mopac
        << static_cast<const QMChargeCalculator&>(am1bcc);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, AM1BCC &am1bcc)
{
    VersionID v = readHeader(ds, r_am1bcc);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> am1bcc.mopac
            >> static_cast<QMChargeCalculator&>(am1bcc);
    }
    else
        throw version_error(v, "1", r_am1bcc, CODELOC);
        
    return ds;
}

/** Constructor */
AM1BCC::AM1BCC() : ConcreteProperty<AM1BCC,QMChargeCalculator>()
{}

/** Copy constructor */
AM1BCC::AM1BCC(const AM1BCC &other)
       : ConcreteProperty<AM1BCC,QMChargeCalculator>(other),
         mopac(other.mopac)
{}

/** Destructor */
AM1BCC::~AM1BCC()
{}

const char* AM1BCC::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AM1BCC>() );
}

/** Comparison operator */
AM1BCC& AM1BCC::operator=(const AM1BCC &other)
{
    if (this != &other)
    {
        mopac = other.mopac;
    
        QMChargeCalculator::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool AM1BCC::operator==(const AM1BCC &other) const
{
    return this == &other or
           (mopac == other.mopac and QMChargeCalculator::operator==(other));
}

/** Comparison operator */
bool AM1BCC::operator!=(const AM1BCC &other) const
{
    return not AM1BCC::operator==(other);
}

/** Set the environmental variable 'variable' to 'value'. It is 
    important that "AMBERHOME" is set, as this is needed by antechamber
    to generate the charges */
void AM1BCC::setEnvironment(const QString &variable, const QString &value)
{
    mopac.setEnvironment(variable, value);
}

/** Return the environmental variables that will be overridden when
    the charges are calculated */
const QHash<QString,QString>& AM1BCC::environment() const
{
    return mopac.environment();
}

/** Return the value of the environmental variable 'variable' */
QString AM1BCC::environment(const QString &variable) const
{
    return mopac.environment(variable);
}
    
/** Set the total charge of the molecule whose charges are being generated */
void AM1BCC::setTotalCharge(int charge)
{
    mopac.setTotalCharge(charge);
}

/** Return the total charge of the molecule whose charges are being generated */
int AM1BCC::totalCharge() const
{
    return mopac.totalCharge();
}

static QString getOutput(const QString &filename)
{
    QFile f(filename);
    
    if (not f.open(QIODevice::ReadOnly))
    {
        return QObject::tr("Unable to open file %1.").arg(filename);
    }
    else
    {
        QTextStream ts(&f);
        
        return ts.readAll();
    }
}

static void runProcess(const QString &cmd, const QString &path,
               const QHash<QString,QString> &environment = QHash<QString,QString>())
{
    QString shellfile = QString("%1/shell_cmd_file").arg(path);

    //write the shell file to run the command
    {
        QFile f( shellfile );
    
        if (not f.open(QIODevice::WriteOnly))
            throw SireError::file_error(f, CODELOC);
    
        QTextStream ts(&f);
    
        //set the environmental variables of the job
        for (QHash<QString,QString>::const_iterator it = environment.constBegin();
             it != environment.constEnd();
             ++it)
        {
            ts << "export " << it.key() << "=\"" << it.value() << "\"\n";
        }

        //set the script to change into the run directory of the job
        ts << QString("\ncd %1").arg(path) << "\n\n";
    
        //set the command to run
        ts << cmd << " > runcommand.log";
    
        f.close();
    }
    
    //now run the command
    {
        Process p = Process::run("sh", shellfile);
        
        //wait until the job has finished
        p.wait();
        
        if (p.wasKilled())
        {
            throw SireError::process_error( QObject::tr(
                "The command {%1} was killed.").arg(cmd), CODELOC );
        }
        
        if (p.isError())
        {
            throw SireError::process_error( QObject::tr(
                "There was an error running the command {%1}. Here is the output "
                "from running the command:\n%2")
                    .arg(cmd, getOutput(QString("%1/runcommand.log").arg(path))), 
                        CODELOC );
        }
    }
}

static void addChargesToAC(const AtomCharges &mulliken_charges,
                           const QString &infilename, const QString &outfilename,
                           const Molecule &molecule)
{
    QFile infile( infilename );
    
    if (not infile.open(QIODevice::ReadOnly))
        throw SireError::file_error(infile, CODELOC);
    
    QFile outfile( outfilename );
    
    if (not outfile.open(QIODevice::WriteOnly))
        throw SireError::file_error(outfile, CODELOC);
        
    QTextStream instream( &infile );
    QTextStream outstream( &outfile );

    outstream.setFieldAlignment( QTextStream::AlignRight );
    outstream.setRealNumberNotation( QTextStream::FixedNotation );
    outstream.setRealNumberPrecision(6);
    
    const MoleculeInfoData &molinfo = molecule.data().info();
    
    while (not instream.atEnd())
    {
        QString line = instream.readLine();
        
        if (line.startsWith("ATOM", Qt::CaseInsensitive))
        {
            // found an antechamber AC ATOM line
            //ATOM      1  C01 MEO     1       0.000   0.000   0.000  0.000000        c3
        
            QStringList words = line.split(" ",QString::SkipEmptyParts);
            
            if (words.count() < 10)
                throw SireError::file_error( QObject::tr(   
                    "The AC file line \"%1\" does not look like a valid antechamber "
                    "format ATOM line.").arg(line), CODELOC );
                    
            AtomName atmnam( words[2] );
            ResName resnam( words[3] );
            
            //get the CGAtomIdx of the atom with this atom and residue name
            CGAtomIdx cgatomidx = molinfo.cgAtomIdx( atmnam + resnam );
            
            //get the mulliken charge of this atom
            Charge atmchg = mulliken_charges[cgatomidx];
            
            //substitute that into the AC file
            outstream << line.left(54);
            
            outstream.setFieldWidth(10);
            outstream << atmchg.to( mod_electron );
            
            outstream.setFieldWidth(0);
            outstream << line.mid(62,-1) << "\n";
        }
        else
        {
            outstream << line << "\n";
        }
    }
}

/** Internal function used to convert AM1 mulliken charges to AM1-BCC charges */
AtomCharges AM1BCC::convertAM1MullikenToAM1BCC(const AtomCharges &mulliken_charges,
                                               const Molecule &molecule,
                                               const PropertyMap &map,
                                               const QString &amberhome) const
{
    //we will use antechamber to create an AC file for this
    //molecule (with zero charges). We will then copy the 
    //AM1 mulliken charges into this file, run "am1bcc" to
    //get the AM1-BCC charges, and will then parse the output
    //file to extract those charges. 

    //antechamber needs the AMBERHOME environmental variable to be set
    QHash<QString,QString> env = this->environment();
    env.insert( "AMBERHOME", amberhome );
    
    //all of this will be performed in a temporary directory!
    TempDir tmpdir;
    
    //first we need a PDB of the molecule to input to antechamber
    PDB().write(molecule, QString("%1/molecule.pdb").arg(tmpdir.path()), map);

    qDebug() << getOutput( QString("%1/molecule.pdb").arg(tmpdir.path()) );

    //now we run antechamber on this PDB to create the AC file (needed by am1bcc)
    ::runProcess( QString("%1/bin/antechamber -i molecule.pdb -fi pdb "
                          "-o molecule.AC -j 4 -fo ac -nc %2")
                  .arg(amberhome).arg(mopac.totalCharge()),
            tmpdir.path(), env );

    qDebug() << getOutput( QString("%1/molecule.AC").arg(tmpdir.path()) );
    
    //now we edit the resulting AC file to insert the AM1 mulliken charges...
    ::addChargesToAC( mulliken_charges,
                      QString("%1/molecule.AC").arg(tmpdir.path()),
                      QString("%1/am1mulliken.AC").arg(tmpdir.path()),
                      molecule );

    qDebug() << getOutput( QString("%1/am1mulliken.AC").arg(tmpdir.path()) );
    
    //use "am1bcc" to convert the charges to AM1-BCC charges
    ::runProcess(
            QString("%1/bin/am1bcc -i am1mulliken.AC -o am1bcc.AC -f ac "
                    "-p %1/dat/antechamber/BCCPARM.DAT -j 4")
                    .arg(amberhome),
            tmpdir.path(), env);

    qDebug() << getOutput( QString("%1/am1bcc.AC").arg(tmpdir.path()) );

    //finally(!) read the output AC file and extract all of the AM1-BCC charges

    return mulliken_charges;
}

/** Return the amber directory (AMBERHOME) */
QString AM1BCC::getAmberHome() const
{
    if (this->environment().contains("AMBERHOME"))
        return this->environment().value("AMBERHOME");
    else
    {
        const char *amberhome = ::getenv("AMBERHOME");
        
        if (not amberhome)
            throw SireError::process_error( QObject::tr(
                    "It is not possible to run the mopac and antechamber "
                    "programs required to generate charges unless the "
                    "AMBERHOME environmental variable is set (this must point "
                    "to the location of the amber directory, e.g. "
                    "$HOME/ambertools/amber10"), CODELOC );
                    
        return QString(amberhome);
    }
}

/** This calculates and returns the AM1BCC charges for the atoms in 
    molecule 'molecule'. This only calculates the charges for the selected
    atoms in the molecule. Either default (0) charges, or the original
    charges are use for atoms that are not selected */
AtomCharges AM1BCC::operator()(const PartialMolecule &molecule,
                               const PropertyMap &map) const
{
    //we use the tools provided by antechamber - all in AMBERHOME
    QString amberhome = this->getAmberHome();

    //we use mopac to calculate all of the AM1 mulliken charges for the entire molecule
    Molecule whole_mol( molecule );
    
    Mopac my_mopac( mopac );
    my_mopac.setExecutable( QString("%1/bin/mopac").arg(amberhome) );
    
    AtomCharges mopac_chgs = my_mopac.calculateCharges(whole_mol, map);

    //we now need to convert these AM1 charges to AM1-BCC charges
    //using the am1bcc program from antechamber
    AtomCharges am1bcc_chgs = convertAM1MullikenToAM1BCC(mopac_chgs, whole_mol, map,
                                                         amberhome);

    if (not molecule.selectedAll())
    {
        //merge any original charges for unselected atoms
        //with the new charges for selected atoms - note that this may
        //(probably will!) make the total charge on the molecule
        //non-integer
        
        const PropertyName &charge_property = map["charge"];
        
        AtomCharges old_charges;
        bool got_charges = false;
        
        if (whole_mol.hasProperty(charge_property))
        {
            const Property &prop = whole_mol.property(charge_property);
            
            if (prop.isA<AtomCharges>())
            {
                old_charges = prop.asA<AtomCharges>();
                got_charges = true;
            }
        }
        
        const MoleculeInfoData &molinfo = whole_mol.data().info();
        
        if (not got_charges)
        {
            old_charges = AtomCharges(molinfo);
        }
        
        if (molecule.selection().nSelected() > molecule.nAtoms()/2)
        {
            for (CGIdx i(0); i < molinfo.nCutGroups(); ++i)
            { 
                for (Index j(0); j < molinfo.nAtoms(i); ++j)
                {
                    CGAtomIdx cgatomidx(i,j);
            
                    if (not molecule.selection().selected(cgatomidx))
                    {
                        am1bcc_chgs.set(cgatomidx, old_charges[cgatomidx]);
                    }
                }
            }
        }
        else
        {
            for (CGIdx i(0); i<molinfo.nCutGroups(); ++i)
            {
                for (Index j(0); j<molinfo.nAtoms(i); ++j)
                {
                    CGAtomIdx cgatomidx(i,j);
            
                    if (molecule.selection().selected(cgatomidx))
                    {
                        old_charges.set(cgatomidx, am1bcc_chgs[cgatomidx]);
                    }
                }
            }
            
            am1bcc_chgs = old_charges;
        }
    }
    
    return am1bcc_chgs;
}

/** This returns whether or not the charges will change when going
    from 'oldmol' to 'newmol' - note that this assumes that the 
    charges in 'oldmol' are already AM1BCC charges! If they are
    not, then this will give the wrong answer! */
bool AM1BCC::mayChangeCharges(const PartialMolecule &oldmol,
                              const PartialMolecule &newmol,
                              const PropertyMap &map) const
{
    return true;
}
