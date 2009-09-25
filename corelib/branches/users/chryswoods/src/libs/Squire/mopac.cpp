/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "mopac.h"
#include "qmpotential.h"

#include "SireMol/element.h"

#include "SireBase/tempdir.h"
#include "SireBase/findexe.h"
#include "SireBase/process.h"

#include "SireUnits/units.h"

#include "tostring.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace Squire;
using namespace SireFF;
using namespace SireMol;
using namespace SireVol;
using namespace SireBase;
using namespace SireUnits;
using namespace SireStream;
using namespace SireStream;

static const RegisterMetaType<Mopac> r_mopac;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const Mopac &mopac)
{
    writeHeader(ds, r_mopac, 1);
    
    SharedDataStream sds(ds);
    
    sds << mopac.env_variables
        << mopac.mopac_exe << mopac.qm_method
        << mopac.energy_template << mopac.force_template
        << mopac.total_charge;
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, Mopac &mopac)
{
    VersionID v = readHeader(ds, r_mopac);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> mopac.env_variables
            >> mopac.mopac_exe >> mopac.qm_method
            >> mopac.energy_template >> mopac.force_template
            >> mopac.total_charge;
    }
    else
        throw version_error(v, "1", r_mopac, CODELOC);
        
    return ds;
}

static const QString default_energy_template =
       "@QM_METHOD@ 1SCF CHARGE=@QM_CHARGE@ GEO-OK MMOK PRECISE\n"
       "created by Sire for mopac6\n\n"
       "@QM_COORDS@\n";

static const QString default_force_template = "! NEEDS TO BE WRITTEN";

/** Constructor */
Mopac::Mopac() 
      : ConcreteProperty<Mopac,QMProgram>(),
        qm_method("AM1"),
        energy_template(default_energy_template),
        force_template(default_force_template),
        total_charge(0)
{}

/** Construct, passing in the location of the Mopac executable */
Mopac::Mopac(const QString &mopac)
       : ConcreteProperty<Mopac,QMProgram>(),
         qm_method("AM1"),
         energy_template(default_energy_template),
         force_template(default_force_template),
         total_charge(0)
{
    this->setExecutable(mopac);
}

/** Copy constructor */
Mopac::Mopac(const Mopac &other)
       : ConcreteProperty<Mopac,QMProgram>(other),
         env_variables(other.env_variables), mopac_exe(other.mopac_exe),
         qm_method(other.qm_method),
         energy_template(other.energy_template),
         force_template(other.force_template),
         total_charge(other.total_charge)
{}

/** Destructor */
Mopac::~Mopac()
{}

/** Copy assignment operator */
Mopac& Mopac::operator=(const Mopac &other)
{
    if (this != &other)
    {
        env_variables = other.env_variables;
        mopac_exe = other.mopac_exe;
        qm_method = other.qm_method;
        energy_template = other.energy_template;
        force_template = other.force_template;
        total_charge = other.total_charge;
    }
    
    return *this;
}

/** Comparison operator */
bool Mopac::operator==(const Mopac &other) const
{
    return this == &other or 
           (env_variables == other.env_variables and
            mopac_exe == other.mopac_exe and
            qm_method == other.qm_method and
            energy_template == other.energy_template and
            force_template == other.force_template and
            total_charge == other.total_charge);
}

/** Comparison operator */
bool Mopac::operator!=(const Mopac &other) const
{
    return not this->operator==(other);
}

/** Set the Mopac executable (full path and also arguments) to be used */
void Mopac::setExecutable(const QString &mopac_executable)
{
    mopac_exe = mopac_executable;
}

/** Return the executable (full path and also arguments) to be used. This
    is null if the executable is searched for in the path */
QString Mopac::executable() const
{
    return mopac_exe;
}

/** Set the environmental variable 'variable' to have the value 'value'
    when the Mopac executable is run. This replaces any existing
    value of this environmental variable */
void Mopac::setEnvironment(const QString &variable, const QString &value)
{
    env_variables[variable] = value;
}

/** Return all of the environmental variables that are to be set explicitly
    when Mopac is run. This does not include any environmental variables
    that have not been explicitly set, but do have values */
const QHash<QString,QString>& Mopac::environment() const
{
    return env_variables;
}

/** Return the value of the explicitly set environmental variable 'variable'.
    A null string is returned if this variable has not been set 
    explicitly (this does not mean the variable doesn't exist - merely
    that a specific value has not been set) */
QString Mopac::environment(const QString &variable) const
{
    return env_variables.value(variable);
}

/** Set the QM method to be used to calculate the energy or
    force (e.g. AM1, PM3). This will substitute for 
    @QM_METHOD@ in the energy and force command file templates */
void Mopac::setMethod(const QString &method)
{
    qm_method = method;
}

/** Return the QM method to be used to calculate the energy or
    force (e.g. AM1, PM3). This will substitute for 
    @QM_METHOD@ in the energy and force command file templates */
const QString& Mopac::method() const
{
    return qm_method;
}

/** Set the total charge of the system (in unit charges) */
void Mopac::setTotalCharge(int charge)
{
    total_charge = charge;
}

/** Return the total charge of the system */
int Mopac::totalCharge() const
{
    return total_charge;
}

/** Set the template for the command file to be used to get
    Mopac to calculate an energy. The following tags will
    be substituted in the template;
    
    @QM_METHOD@          - the desired QM method (e.g. HF)
    @QM_COORDS@          - the list of elements and coordinates of QM atoms
    @QM_CHARGE@          - the total charge of the system
*/
void Mopac::setEnergyTemplate(const QString &cmd_template)
{
    energy_template = cmd_template;
}

/** Return the template for the command file to be used to get Mopac
    to calculate the energy. */
const QString& Mopac::energyTemplate() const
{
    return energy_template;
}

/** Set the template for the command file to be used to get
    Mopac to calculate the forces. The following tags will
    be substituted in the template;
    
    @QM_METHOD@          - the desired QM method (e.g. HF)
    @QM_COORDS@          - the list of elements and coordinates of QM atoms
    @QM_CHARGE@          - the total charge of the system
*/
void Mopac::setForceTemplate(const QString &cmd_template)
{
    force_template = cmd_template;
}

/** Return the template for the command file to be used to get Mopac
    to calculate the forces. */
const QString& Mopac::forceTemplate() const
{
    return force_template;
}

/** Function used to substitute in the atom and lattice coordinates
    into the provided mopac command template */
QString Mopac::createCommandFile(QString cmd_template,
                                 const QMPotential::Molecule &molecule) const
{
    cmd_template.replace( QLatin1String("@QM_METHOD@"),
                          qm_method, Qt::CaseInsensitive );

    cmd_template.replace( QLatin1String("@QM_CHARGE@"),
                          QString::number(total_charge), Qt::CaseInsensitive );
                  
    //now build the list of all of the atoms
    QStringList atom_coords;
    
    //loop through the atoms...
    const CoordGroupArray &coords = molecule.coordinates();
    const PackedArray2D<Element> &elements = molecule.parameters().atomicParameters();
        
    int natoms = coords.nCoords();
        
    BOOST_ASSERT( natoms == elements.nValues() );
        
    const Vector *coords_array = coords.constCoordsData();
    const Element *elements_array = elements.constValueData();
        
    QString coords_string;
    QTextStream ts(&coords_string);
        
    for (int i=0; i<natoms; ++i)
    {
        const Element &element = elements_array[i];
        
        if (element.nProtons() > 0)
        {
            //this is not a dummy atom!
            const Vector &c = coords_array[i];

            //000000000011111111112222222222333333333344444444445555
            //012345678901234567890123456789012345678901234567890123
            //    O      5.9450  0       -4.8230  0       11.3560  0

            ts.setFieldAlignment(QTextStream::AlignRight);
            ts.setRealNumberNotation(QTextStream::FixedNotation);
            ts.setRealNumberPrecision(4);
            
            ts.setFieldWidth(5);
            ts << element.symbol();
            
            ts.setFieldWidth(12);
            ts << c.x();
            
            ts.setFieldWidth(0);
            ts << "  0  ";
            
            ts.setFieldWidth(12);
            ts << c.y();
            
            ts.setFieldWidth(0);
            ts << "  0  ";
            
            ts.setFieldWidth(12);
            ts << c.z();

            ts.setFieldWidth(0);
            ts << "  0\n";
        }
    }
                          
    cmd_template.replace( QLatin1String("@QM_COORDS@"),
                          coords_string, Qt::CaseInsensitive );
                                       
    return cmd_template;
}

/** Return the command file that will be used to calculate the energy of the 
    molecules in 'molecules' */
QString Mopac::energyCommandFile(const QMPotential::Molecules &molecules) const
{
    QStringList cmdfiles;

    for (int i=0; i<molecules.count(); ++i)
    {
        cmdfiles.append( createCommandFile(energy_template, 
                                           molecules.moleculesByIndex().at(i)) );
    }

    return cmdfiles.join("\n\n");
}

/** Return the command files that will be used to calculate the forces on the  
    atoms of the molecules in 'molecules' */
QString Mopac::forceCommandFile(const QMPotential::Molecules &molecules) const
{
    QStringList cmdfiles;

    for (int i=0; i<molecules.count(); ++i)
    {
        cmdfiles.append( createCommandFile(force_template, 
                                           molecules.moleculesByIndex().at(i)) );
    }

    return cmdfiles.join("\n\n");
}

/** Extract the energy from the mopac output in 'mopac_output' */
double Mopac::extractEnergy(QFile &mopac_output) const
{
    return 0;
}

/** Internal function used to write the shell script that is used to
    run the mopac job and collect the output
*/
QString Mopac::writeShellFile(const TempDir &tempdir) const
{
    QString cmdfile = QString("%1/run_mopac.cmd").arg(tempdir.path());
    
    QFile f(cmdfile);
    f.open(QIODevice::WriteOnly);
    
    QTextStream ts(&f);

    //set the environmental variables of the job
    for (QHash<QString,QString>::const_iterator it = env_variables.constBegin();
         it != env_variables.constEnd();
         ++it)
    {
        ts << "export " << it.key() << "=\"" << it.value() << "\"\n";
    }

    //set the script to change into the run directory of the job
    ts << QString("\ncd %1").arg(tempdir.path()) << "\n\n";

    //write the line used to run mopac
    if (mopac_exe.isEmpty())
    {
        //the user hasn't specified a mopac executable - try to find one
        QString found_mopac = SireBase::findExe("mopac").absoluteFilePath();
        ts << found_mopac << "\n";
    }
    else
        ts << mopac_exe << "\n";

    ts << "sync\n";
    
    f.close();
    
    return cmdfile;
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

/** Return the energy calculate according to the Mopac command
    file 'cmd_file' (this is the contents of the file, not
    the path to the file) */
double Mopac::calculateEnergy(const QString &cmdfile, int ntries) const
{
    //create a temporary directory in which to run Mopac
    QString tmppath = env_variables.value("TMPDIR");
    
    if (tmppath.isEmpty())
        tmppath = QDir::temp().absolutePath();

    TempDir tmpdir(tmppath);
    tmpdir.doNotDelete();

    //write the file processed by the shell used to run the job
    QString shellfile = this->writeShellFile(tmpdir);

    {
        QFile f( QString("%1/FOR005").arg(tmpdir.path()) );
        f.open( QIODevice::WriteOnly );
   
        //write the command file
        f.write( cmdfile.toAscii() );
        f.close();
    }

    //run the shell file...
    Process p = Process::run( "sh", shellfile );

    //wait until the job has finished
    p.wait();
    
    if (p.wasKilled())
    {
        throw SireError::process_error( QObject::tr(
            "The Mopac job was killed."), CODELOC );
    }
    
    if (p.isError())
    {
        QByteArray shellcontents = ::readAll(shellfile);
        QByteArray cmdcontents = ::readAll(QString("%1/mopac_input").arg(tmpdir.path()));
        QByteArray outputcontents = ::readAll(QString("%1/mopac_output")
                                                    .arg(tmpdir.path()));

        throw SireError::process_error( QObject::tr(
            "There was an error running the Mopac job.\n"
            "The shell script used to run the job was;\n"
            "*****************************************\n"
            "%1\n"
            "*****************************************\n"
            "The mopac input used to run the job was;\n"
            "*****************************************\n"
            "%2\n"
            "*****************************************\n"
            "The mopac output was;\n"
            "*****************************************\n"
            "%3\n"
            "*****************************************\n"
            )
                .arg( QLatin1String(shellcontents),
                      QLatin1String(cmdcontents),
                      QLatin1String(outputcontents) ), CODELOC );
    }

    //read all of the output
    QFile f( QString("%1/mopac_output").arg(tmpdir.path()) );
    
    if ( not (f.exists() and f.open(QIODevice::ReadOnly)) )
    {
        QByteArray shellcontents = ::readAll(shellfile);
        QByteArray cmdcontents = ::readAll(QString("%1/mopac_input").arg(tmpdir.path()));
    
        throw SireError::process_error( QObject::tr(
            "There was an error running the Mopac job - no output was created.\n"
            "The shell script used to run the job was;\n"
            "*****************************************\n"
            "%1\n"
            "*****************************************\n"
            "The mopac input used to run the job was;\n"
            "*****************************************\n"
            "%2\n"
            "*****************************************\n")
                .arg( QLatin1String(shellcontents),
                      QLatin1String(cmdcontents) ), CODELOC );
    }


    try
    {
        //parse the output to get the energy
        return this->extractEnergy(f);
    }
    catch(...)
    {
        qDebug() << "Mopac process error. Number of remaining attempts = " << ntries;

        //print out the last twenty lines of output
        const int nlines_to_print = 20;

        qDebug() << "Printing out the last" << nlines_to_print << "lines of output...";

        QFile f( QString("%1/mopac_output").arg(tmpdir.path()) );

        if ( not (f.exists() and f.open(QIODevice::ReadOnly)) )    
            qDebug() << "Could not read the file" << tmpdir.path() << "/mopac_output";
        else
        {
            QStringList lines;
 
            QTextStream ts(&f);

            while (not ts.atEnd())
            {
                lines.append( ts.readLine() );

                if (lines.count() > nlines_to_print)
                    lines.removeFirst();
            }

            foreach (QString line, lines){ qDebug() << qPrintable(line); }
        }

        if (ntries <= 0)
            //don't bother trying again - it's not going to work!
            throw;
            
        //give it one more go - you never know, it may work
        return this->calculateEnergy(cmdfile, ntries-1);
    }
}

/** Run Mopac and use it to calculate the energy of the molecules in 
    'molecules'. This blocks until Mopac has completed */
double Mopac::calculateEnergy(const QMPotential::Molecules &molecules,
                               int ntries) const
{
    //create the command file to be used by Mopac
    QString cmdfile = this->energyCommandFile(molecules);
    
    return this->calculateEnergy(cmdfile, ntries);
}

const char* Mopac::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Mopac>() );
}
