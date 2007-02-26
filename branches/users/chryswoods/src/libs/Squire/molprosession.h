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
    MolproSession(const MolproFF &molproff);

    ~MolproSession();

    void setArrays(const QVector<double> &qm_array,
                   const QVector<double> &mm_array);

    double calculateEnergy(const char *cmds);

    bool incompatibleWith(const MolproFF &molproff) const;
    void assertCompatibleWith(const MolproFF &molproff) const;

    void assertMolproIsRunning() const;

private:

    static void removeDirectory(QDir dir);

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

    /** The QM version number of the MolproFF that has been loaded
        onto this session */
    quint32 qm_version;

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
