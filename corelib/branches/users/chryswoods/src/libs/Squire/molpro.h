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

#ifndef SQUIRE_MOLPRO_H
#define SQUIRE_MOLPRO_H

#include <QHash>
#include <QString>
#include <QProcess>

#include "qmprogram.h"
#include "latticecharges.h"

SIRE_BEGIN_HEADER

namespace Squire 
{
class Molpro;
}

QDataStream& operator<<(QDataStream&, const Squire::Molpro&);
QDataStream& operator>>(QDataStream&, Squire::Molpro&);

class QFile;

namespace SireBase
{
class TempDir;
}

namespace Squire
{

/** This is a wrapper that allows Molpro to be used to calculate
    QM and QM/MM energies
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT Molpro : public SireBase::ConcreteProperty<Molpro,QMProgram>
{

friend QDataStream& ::operator<<(QDataStream&, const Molpro&);
friend QDataStream& ::operator>>(QDataStream&, Molpro&);

public:
    Molpro();
    Molpro(const QString &molpro);
    
    Molpro(const Molpro &other);
    
    ~Molpro();
    
    static const char* typeName();
    
    Molpro& operator=(const Molpro &other);
    
    bool operator==(const Molpro &other) const;
    bool operator!=(const Molpro &other) const;
    
    void setExecutable(const QString &molpro_exe);
    void setEnvironment(const QString &variable, const QString &value);
    
    void setMemoryRequirement(int nbytes);
    
    int memoryRequirement() const;
    
    QString executable() const;
    
    const QHash<QString,QString>& environment() const;
    
    QString environment(const QString &variable) const;
    
    void setBasisSet(const QString &basis_set);
    
    const QString& basisSet() const;
    
    void setMethod(const QString &method);
    
    const QString& method() const;
    
    void setTotalCharge(int charge);
    int totalCharge() const;
    
    void setEnergyTemplate(const QString &energy_template);
    
    const QString& energyTemplate() const;
    
    void setForceTemplate(const QString &force_template);
    
    const QString& forceTemplate() const;

    bool supportsLatticeCharges() const
    {
        return true;
    }

protected:
    double calculateEnergy(const QMPotential::Molecules &molecules,
                           int ntries = 5) const;
    double calculateEnergy(const QMPotential::Molecules &molecules,
                           const LatticeCharges &lattice_charges,
                           int ntries = 5) const;

    QString energyCommandFile(const QMPotential::Molecules &molecules) const;
    QString energyCommandFile(const QMPotential::Molecules &molecules,
                              const LatticeCharges &lattice_charges) const;
    
    QString forceCommandFile(const QMPotential::Molecules &molecules) const;
    QString forceCommandFile(const QMPotential::Molecules &molecules,
                             const LatticeCharges &lattice_charges) const;

private:
    QString createCommandFile(QString cmd_template,
                        const QMPotential::Molecules &molecules,
                        const LatticeCharges &lattice_charges = LatticeCharges()) const;

    QString writeShellFile(const SireBase::TempDir &tempdir) const;

    double extractEnergy(QFile &molpro_output) const;

    double calculateEnergy(const QString &cmd_file, int ntries) const;

    /** The environmental variables to hold when running Molpro */
    QHash<QString,QString> env_variables;
    
    /** The full path to the molpro executable to run (including
        any necessary command line arguments) */
    QString molpro_exe;
    
    /** The basis set to use during this calculation */
    QString basis_set;
    
    /** The QM method to use to calculate the energy */
    QString qm_method;
    
    /** The template command file used for the energy calculations.
        The basis set, QM method and atom coordinates are substituted
        into this template */
    QString energy_template;
    
    /** The template command file used for the force calculations.
        The basis set, QM method and atom coordinates are substituted
        into this template */
    QString force_template;
    
    /** The total charge of the system */
    qint32 total_charge;
    
    /** The amount of memory (in bytes) to reserve for the 
        QM calculation */
    quint32 memory_requirement;
};

}

Q_DECLARE_METATYPE( Squire::Molpro )

SIRE_EXPOSE_CLASS( Squire::Molpro )

SIRE_END_HEADER

#endif
