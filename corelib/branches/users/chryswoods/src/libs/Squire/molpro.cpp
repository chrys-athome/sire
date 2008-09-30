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

#include <QProcess>

#include "molpro.h"

#include "qmpotential.h"

#include "SireMol/element.h"

#include "SireBase/tempdir.h"
#include "SireBase/findexe.h"

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

static const RegisterMetaType<Molpro> r_molpro;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const Molpro &molpro)
{
    writeHeader(ds, r_molpro, 1);
    
    SharedDataStream sds(ds);
    
    sds << molpro.env_variables
        << molpro.molpro_exe << molpro.basis_set << molpro.qm_method
        << molpro.energy_template << molpro.force_template;
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, Molpro &molpro)
{
    VersionID v = readHeader(ds, r_molpro);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> molpro.env_variables
            >> molpro.molpro_exe >> molpro.basis_set >> molpro.qm_method
            >> molpro.energy_template >> molpro.force_template;
    }
    else
        throw version_error(v, "1", r_molpro, CODELOC);
        
    return ds;
}

static const QString default_energy_template =
       "geomtyp=xyz\n"
       "geometry={ NOSYM, NOORIENT,\n"
       "@NUM_QM_ATOMS@  ! number of atoms\n"
       "This is an auto-generated molpro command file generated using Sire\n"
       "@QM_COORDS@\n"
       "}\n\n"
       "lattice, NUCONLY\n"
       "BEGIN_DATA\n"
       "@LATTICE_POINTS@\n"
       "END\n\n"
       "basis=@BASIS_SET@\n\n"
       "@QM_METHOD@;SIRE_FINAL_ENERGY=energy\n";

static const QString default_force_template = "! NEEDS TO BE WRITTEN";

/** Constructor */
Molpro::Molpro() 
       : ConcreteProperty<Molpro,QMProg>(),
         basis_set("vdz"), qm_method("HF"),
         energy_template(default_energy_template),
         force_template(default_force_template)
{}

/** Copy constructor */
Molpro::Molpro(const Molpro &other)
       : ConcreteProperty<Molpro,QMProg>(other),
         env_variables(other.env_variables), molpro_exe(other.molpro_exe),
         basis_set(other.basis_set), qm_method(other.qm_method),
         energy_template(other.energy_template),
         force_template(other.force_template)
{}

/** Destructor */
Molpro::~Molpro()
{}

/** Copy assignment operator */
Molpro& Molpro::operator=(const Molpro &other)
{
    if (this != &other)
    {
        env_variables = other.env_variables;
        molpro_exe = other.molpro_exe;
        basis_set = other.basis_set;
        qm_method = other.qm_method;
        energy_template = other.energy_template;
        force_template = other.force_template;
    }
    
    return *this;
}

/** Comparison operator */
bool Molpro::operator==(const Molpro &other) const
{
    return this == &other or 
           (env_variables == other.env_variables and
            molpro_exe == other.molpro_exe and
            basis_set == other.basis_set and
            qm_method == other.qm_method and
            energy_template == other.energy_template and
            force_template == other.force_template);
}

/** Comparison operator */
bool Molpro::operator!=(const Molpro &other) const
{
    return not this->operator==(other);
}

/** Set the Molpro executable (full path and also arguments) to be used */
void Molpro::setExecutable(const QString &molpro_executable)
{
    molpro_exe = molpro_executable;
}

/** Set the environmental variable 'variable' to have the value 'value'
    when the Molpro executable is run. This replaces any existing
    value of this environmental variable */
void Molpro::setEnvironment(const QString &variable, const QString &value)
{
    env_variables[variable] = value;
}

/** Return all of the environmental variables that are to be set explicitly
    when Molpro is run. This does not include any environmental variables
    that have not been explicitly set, but do have values */
const QHash<QString,QString>& Molpro::environment() const
{
    return env_variables;
}

/** Return the value of the explicitly set environmental variable 'variable'.
    A null string is returned if this variable has not been set 
    explicitly (this does not mean the variable doesn't exist - merely
    that a specific value has not been set) */
QString Molpro::environment(const QString &variable) const
{
    return env_variables.value(variable);
}

/** Set the string representation of the basis set to be used
    during the calculation. This will substitute for @BASIS_SET@ in 
    the energy and force command file templates. */
void Molpro::setBasisSet(const QString &basis)
{
    basis_set = basis;
}

/** Return the basis set to be used during the calculation. This
    substitutes for @BASIS_SET@ in the energy and force command
    file templates */
const QString& Molpro::basisSet() const
{
    return basis_set;
}

/** Set the QM method to be used to calculate the energy or
    force (e.g. HF, df-ks,b3lyp). This will substitute for 
    @QM_METHOD@ in the energy and force command file templates */
void Molpro::setMethod(const QString &method)
{
    qm_method = method;
}

/** Return the QM method to be used to calculate the energy or
    force (e.g. HF, df-ks,b3lyp). This will substitute for 
    @QM_METHOD@ in the energy and force command file templates */
const QString& Molpro::method() const
{
    return qm_method;
}

/** Set the template for the command file to be used to get
    Molpro to calculate an energy. The following tags will
    be substituted in the template;
    
    @BASIS_SET@          - the desired basis set
    @QM_METHOD@          - the desired QM method (e.g. HF)
    @QM_COORDS@          - the list of elements and coordinates of QM atoms
    @NUM_QM_ATOMS@       - the number of QM atoms
    @LATTICE_POINTS@     - the coordinates and charges of the lattice points (MM atoms)
    @NUM_LATTICE_POINTS@ - the number of lattice points (MM atoms)
*/
void Molpro::setEnergyTemplate(const QString &cmd_template)
{
    energy_template = cmd_template;
}

/** Return the template for the command file to be used to get Molpro
    to calculate the energy. */
const QString& Molpro::energyTemplate() const
{
    return energy_template;
}

/** Set the template for the command file to be used to get
    Molpro to calculate the forces. The following tags will
    be substituted in the template;
    
    @BASIS_SET@          - the desired basis set
    @QM_METHOD@          - the desired QM method (e.g. HF)
    @QM_COORDS@          - the list of elements and coordinates of QM atoms
    @NUM_QM_ATOMS@       - the number of QM atoms
    @LATTICE_POINTS@     - the coordinates and charges of the lattice points (MM atoms)
    @NUM_LATTICE_POINTS@ - the number of lattice points (MM atoms)
*/
void Molpro::setForceTemplate(const QString &cmd_template)
{
    force_template = cmd_template;
}

/** Return the template for the command file to be used to get Molpro
    to calculate the forces. */
const QString& Molpro::forceTemplate() const
{
    return force_template;
}

/** Internal function used to fix the environmental variables of the child
    Molpro process - this is used to allow the user to override default
    variables (without requiring them to mess around with bashrc files or the like)
*/
void Molpro::fixEnvironment(QProcess &p) const
{
    //get the current environment
    QStringList system_environment = QProcess::systemEnvironment();
    
    for (QHash<QString,QString>::const_iterator it = env_variables.constBegin();
         it != env_variables.constEnd();
         ++it)
    {
        QString envvar = QString("%1=%2").arg(it.key(), it.value());
        QRegExp regexp( QString("^%1=(.*)").arg(it.key()),
                        Qt::CaseInsensitive );

        bool found_variable = false;
        
        for (QStringList::iterator it2 = system_environment.begin();
             it2 != system_environment.end();
             ++it2)
        {
            if (it2->contains(regexp))
            {
                it2->replace(regexp, envvar);
                found_variable = true;
                break;
            }
        }

        if (not found_variable)
            system_environment << QString("%1=%2").arg(it.key(), it.value());
    }
    
    //system_environment is now correct - pass it to the process
    p.setEnvironment(system_environment);
}

/** Function used to substitute in the atom and lattice coordinates
    into the provided molpro command template */
QString Molpro::createCommandFile(QString cmd_template,
                                  const QMPotential::Molecules &molecules) const
{
    //replace the easy things...
    cmd_template.replace( QLatin1String("@BASIS_SET@"), 
                          basis_set, Qt::CaseInsensitive );
    
    cmd_template.replace( QLatin1String("@QM_METHOD@"),
                          qm_method, Qt::CaseInsensitive );
                          
    cmd_template.replace( QLatin1String("@NUM_LATTICE_POINTS@"),
                          QString::number(0), Qt::CaseInsensitive );
                          
    cmd_template.replace( QLatin1String("@LATTICE_POINTS@"),
                          QLatin1String(" "), Qt::CaseInsensitive );
                          
    //now build the list of all of the atoms
    QStringList atom_coords;
    
    int nmols = molecules.count();
    const QMPotential::Molecule *molecules_array = molecules.moleculesByIndex()
                                                            .constData();
                                                            
    for (int i=0; i<nmols; ++i)
    {
        const QMPotential::Molecule &molecule = molecules_array[i];
        
        //loop through the atoms...
        const CoordGroupArray &coords = molecule.coordinates();
        const PackedArray2D<Element> &elements = molecule.parameters().atomicParameters();
        
        int natoms = coords.nCoords();
        
        BOOST_ASSERT( natoms == elements.nValues() );
        
        const Vector *coords_array = coords.constCoordsData();
        const Element *elements_array = elements.constValueData();
        
        for (int j=0; j<natoms; ++j)
        {
            const Element &element = elements_array[j];
            
            if (element.nProtons() > 0)
            {
                //this is not a dummy atom!
                const Vector &c = coords_array[j];
    
                atom_coords.append( QString("%1,%2,%3,%4")
                                        .arg(element.symbol(),
                                             QString::number(c.x(), 'f', 6),
                                             QString::number(c.y(), 'f', 6),
                                             QString::number(c.z(), 'f', 6) ) );
            }
        }
    }
    
    cmd_template.replace( QLatin1String("@NUM_QM_ATOMS@"),
                          QString::number(atom_coords.count()), Qt::CaseInsensitive );
                          
    cmd_template.replace( QLatin1String("@QM_COORDS@"),
                          atom_coords.join("\n"), Qt::CaseInsensitive );
                          
    return cmd_template;
}

/** Return the command file that will be used to calculate the energy of the 
    molecules in 'molecules' */
QString Molpro::energyCommandFile(const QMPotential::Molecules &molecules) const
{
    return this->createCommandFile(energy_template, molecules);
}

/** Return the command files that will be used to calculate the forces on the  
    atoms of the molecules in 'molecules' */
QString Molpro::forceCommandFile(const QMPotential::Molecules &molecules) const
{
    return this->createCommandFile(force_template, molecules);
}

/** Extract the energy from the molpro output in 'molpro_output' */
double Molpro::extractEnergy(const QByteArray &molpro_output) const
{
    QRegExp regexp("SIRE_FINAL_ENERGY\\s*=\\s*([-\\d\\.]+)");
    
    if (regexp.indexIn( QLatin1String(molpro_output.constData()) ) == -1)
        throw SireError::process_error( QObject::tr(
            "Could not find the total energy in the molpro output!\n"
            "%1").arg( QLatin1String(molpro_output.constData()) ), CODELOC );
    
    QString num = regexp.cap(1);
        
    bool ok;
        
    double nrg = num.toDouble(&ok);
        
    if (not ok)
        throw SireError::process_error( QObject::tr(
            "The energy obtained from Molpro is garbled (%1) - %2.")
                .arg(regexp.cap(1), regexp.cap(0)), CODELOC );
        
    //the energy is in hartrees - convert it to kcal per mol
    return nrg * hartree;
}

/** Run Molpro and use it to calculate the energy of the molecules in 
    'molecules'. This blocks until Molpro has completed */
double Molpro::calculateEnergy(const QMPotential::Molecules &molecules) const
{
    //create the command file to be used by Molpro
    QString cmdfile = this->energyCommandFile(molecules);
    
    //now set up the Molpro process
    QProcess p;
    this->fixEnvironment(p);
    
    //merge the stdout and stderr channels
    p.setProcessChannelMode(QProcess::MergedChannels);

    //create a temporary directory in which to run Molpro
    QString tmppath = env_variables.value("TMPDIR");
    
    if (tmppath.isEmpty())
        tmppath = QDir::temp().absolutePath();

    TempDir tmpdir(tmppath);
    p.setWorkingDirectory(tmpdir.path());
    
    //now run Molpro!
    if (molpro_exe.isEmpty())
    {
        //the user hasn't specified a molpro executable - try to find one
        QString found_molpro = SireBase::findExe("molpro").absoluteFilePath();
        p.start(QString("%1 -d %2").arg(found_molpro, tmpdir.path()), 
                QIODevice::ReadWrite);
    }
    else
        p.start(QString("%1 -d %2").arg(molpro_exe, tmpdir.path()), 
                QIODevice::ReadWrite);
    
    if (not p.waitForStarted())
        throw SireError::process_error(molpro_exe, p, CODELOC);
    
    //write in the command file
    p.write( cmdfile.toAscii() );
    p.closeWriteChannel();

    //wait until Molpro has finished
    if (not p.waitForFinished(-1))
        throw SireError::process_error(molpro_exe, p, CODELOC);
        
    //parse the output to get the energy
    QByteArray molpro_output = p.readAllStandardOutput();
    
    return this->extractEnergy(molpro_output);
}
