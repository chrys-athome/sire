#ifndef SQUIRE_MOLPROCALCULATOR_H
#define SQUIRE_MOLPROCALCULATOR_H

#include <MolproClient/molproclient.h>
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QMutex>

#include "SireFF/ffcalculator.h"

#include "molproff.h"

SIRE_BEGIN_HEADER

namespace Squire
{

using SireFF::ForceField;

using SireCAS::Values;

/** This class represents a complete molpro session */
class SQUIRE_EXPORT MolproSession
{
public:
    MolproSession( const QFileInfo &molpro_executable,
                   const MolproFF &molproff,
                   const QDir &tmpdir = QDir::temp() );

    ~MolproSession();

    MolproConnection& connection();

    bool incompatibleWith(const MolproFF &molproff);

private:

    /** Mutex used to ensure that no two molpro jobs are started simultaeneously
        (so they don't trash each others tmp directories, or get in each others
        way when creating ports) */
    static QMutex starter_mutex;

    /** The full name and path to the molpro executable that is
        running in this session */
    QFileInfo molpro_exe;

    /** The RPC connection to the molpro process */
    MolproConnection molpro_rpc;

    /** The QProcess in which the molpro process is started */
    QProcess molpro_process;

    /** The current ID number of the MolproFF that has been loaded
        onto this session */
    int ff_id;

    /** The version number of the MolproFF that has been loaded
        onto this session */
    VersionID ff_version;

    /** The unique run directory for the molpro process */
    QDir rundir;
};

/** Return the molpro RPC connection used to communicate with the running process */
inline MolproConnection& MolproSession::connection()
{
    return molpro_rpc;
}

/** This is the calculator used to calculate energies and forces
    via an external Molpro process.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproCalculator : public SireFF::FFCalculatorBase
{
public:
    MolproCalculator(const ForceField &forcefield,
                     const QFileInfo &molpro_exe = "molpro",
                     const QDir &temp_dir = QDir::temp());

    ~MolproCalculator();

    double getEnergies(Values &values);

    const Molecule& molecule(MoleculeID molid) const;

    void calculateEnergy();

    bool move(const Molecule &molecule);

    bool move(const Residue &residue);

    bool setForceField(const ForceField &forcefield);

    ForceField forcefield() const;

private:

    void restartMolpro();

    /** The Molpro forcefield being evaluated */
    SireBase::SharedPolyPointer<MolproFF> molproff;

    /** The path to the molpro executable */
    QFileInfo molpro_exe;

    /** The directory that will be used to run the calculation (tmpdir) */
    QDir temp_dir;

    /** The total energy of the forcefield */
    double total_nrg;

    /** The values of the energy components */
    Values nrg_components;

    /** Shared pointer to the Molpro session that is
        used to calculate the energies */
    boost::shared_ptr<MolproSession> molpro_session;
};

}

SIRE_END_HEADER

#endif
