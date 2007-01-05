#ifndef SQUIRE_MOLPROSESSION_H
#define SQUIRE_MOLPROSESSION_H

#include <MolproClient/molproclient.h>

#include <QMutex>
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QVector>

#include <boost/noncopyable.hpp>

#include "SireBase/version.h"

SIRE_BEGIN_HEADER

namespace Squire
{

class MolproFF;

using SireBase::Version;

/** This class represents a complete molpro session. This class
    is the interface to an actual instance of the running Molpro
    program. This interface caches the current state of the Molpro
    program and is responsible for the actual communication with
    that program.

    As this represents a single interface to a single running program,
    this class is obviously non-copyable.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproSession : public boost::noncopyable
{
public:
    MolproSession( const QFileInfo &molpro_executable,
                   const MolproFF &molproff,
                   const QDir &tmpdir = QDir::temp() );

    ~MolproSession();

    void setArrays(const QVector<double> &qm_array,
                   const QVector<double> &mm_array);

    double calculateEnergy(const char *cmds);

    bool incompatibleWith(const MolproFF &molproff);
    void assertCompatibleWith(const MolproFF &molproff) const;

    void assertMolproIsRunning() const;

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
    quint32 ffid;

    /** The version number of the MolproFF that has been loaded
        onto this session */
    Version ff_version;

    /** The unique run directory for the molpro process */
    QDir rundir;

    /** The current QM array that has been loaded into molpro */
    QVector<double> current_qm;

    /** The current MM array that has been loaded into molpro */
    QVector<double> current_mm;

    /** The new QM array that needs to be loaded into molpro before
        the next evaluation */
    QVector<double> new_qm;

    /** The new MM array that needs to be loaded into molpro before
        the next evaluation */
    QVector<double> new_mm;
};

}

SIRE_END_HEADER

#endif
