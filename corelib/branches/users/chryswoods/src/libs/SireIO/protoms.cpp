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

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QByteArray>

#include "protoms.h"

#include "pdb.h"

#include "SireMol/molecule.h"
#include "SireMol/molecules.h"

#include "SireBase/tempdir.h"
#include "SireBase/findexe.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireIO;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

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
        //the user hasn't specified a molpro executable - try to find one
        QString found_protoms = SireBase::findExe("protoms2").absoluteFilePath();
        ts << QString("%1 %2 > protoms_output\n")
                    .arg(found_protoms, cmdfile);
    }
    else
        ts << QString("%1 %3 > protoms_output\n")
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

/** Internal function used to run ProtoMS to get it to 
    parameterise a molecule */
Molecule ProtoMS::runProtoMS(const Molecule &molecule, int type) const
{
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
    
    while (not line.isNull())
    {
        qDebug() << line;
        
        line = ts.readLine();
    }
    
    return molecule;
}

/** Parameterise the molecule 'molecule' as a 'type' type of
    molecule (PROTEIN, SOLUTE or SOLVENT) */
Molecule ProtoMS::parameterise(const Molecule &molecule, int type)
{
    if (type != PROTEIN and type != SOLUTE and type != SOLVENT)
        throw SireError::invalid_arg( QObject::tr(
            "Unrecognised ProtoMS molecule type (%1). Only "
            "ProtoMS::PROTEIN, ProtoMS::SOLUTE and ProtoMS::SOLVENT "
            "are supported.").arg(type), CODELOC );

    return this->runProtoMS(molecule, type);
}

/** Parameterise the molecules 'molecules' as 'type' type of
    molecules (PROTEIN, SOLUTE or SOLVENT) */
Molecules ProtoMS::parameterise(const Molecules &molecules, int type)
{
    Molecules new_molecules = molecules;
    
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        new_molecules.update( this->parameterise(it->molecule(), type) );
    }

    return new_molecules;
}
