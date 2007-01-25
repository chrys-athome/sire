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

#include "molproprocessor.h"
#include "molprocalculator.h"

#include "molproff.h"

#include "SireFF/ffthreadworker.h"

#include "SireError/errors.h"

using namespace Squire;
using namespace Squire::detail;

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireCluster;

//////////
////////// Implementation of MolproProcessorPvt
//////////

/** Null constructor - the resulting worker will not be able to work! */
MolproProcessorPvt::MolproProcessorPvt()
                   : FFThreadProcessorPvt(),
                     molpro_exe("molpro"), tmpdir(QDir::temp())
{}

/** Construct with a specified forcefield (which must be derived from MolproFF),
    specified molpro executable, and specified temp directory

    \throw SireError::incompatible_error
*/
MolproProcessorPvt::MolproProcessorPvt(const ForceField &forcefield,
                                       const QString &molpro_executable,
                                       const QDir &temp_dir)
                   : FFThreadProcessorPvt(forcefield),
                     molpro_exe(molpro_executable),
                     tmpdir(temp_dir)
{
    if (not forcefield.isA<MolproFF>())
    {
        throw SireError::incompatible_error( QObject::tr(
                  "Cannot place a non-MolproFF derived forcefield (\"%1\") "
                  "onto a MolproProcessor!").arg(forcefield.what()), CODELOC );
    }
}

/** Destructor */
MolproProcessorPvt::~MolproProcessorPvt()
{}

/** Set the full name and path to the molpro executable */
void MolproProcessorPvt::setMolpro(const QString &molpro_executable)
{
    molpro_exe = molpro_executable;
}

/** Set the location of the temporary directory in which to run molpro */
void MolproProcessorPvt::setTempDir(const QDir &temp_dir)
{
    tmpdir = temp_dir;
}

/** Activate this processor, returning a pointer to the worker */
boost::shared_ptr<WorkerBase> MolproProcessorPvt::_pvt_activate()
{
    return boost::shared_ptr<WorkerBase>(
                  new FFThreadWorker(
                      new MolproCalculator(
                              forcefield(), molpro_exe, tmpdir
                                          )
                                    )
                                        );
}

//////////
////////// Implementation of MolproProcessor
//////////

/** Constructor */
MolproProcessor::MolproProcessor()
                : FFThreadProcessor( boost::shared_ptr<FFThreadProcessorPvt>(
                                                    new MolproProcessorPvt())
                                   )
{}

/** Construct for a specified forcefield (which must be derived from MolproFF),
    molpro executable and temp dir */
MolproProcessor::MolproProcessor(const ForceField &forcefield,
                                 const QString &molpro_executable,
                                 const QDir &temp_dir)
                : FFThreadProcessor( boost::shared_ptr<FFThreadProcessorPvt>(
                                               new MolproProcessorPvt(forcefield,
                                                                      molpro_executable,
                                                                      temp_dir)
                                                                            )
                                   )
{}

/** Construct from a MolproProcessorPvt pointer */
MolproProcessor::MolproProcessor(const boost::shared_ptr<MolproProcessorPvt> &data)
                : FFThreadProcessor(data)
{}

/** Copy constructor */
MolproProcessor::MolproProcessor(const MolproProcessor &other)
                : FFThreadProcessor(other)
{}

/** Destructor */
MolproProcessor::~MolproProcessor()
{}

/** Return the data of this processor */
MolproProcessorPvt& MolproProcessor::data()
{
    return dynamic_cast<MolproProcessorPvt&>( FFThreadProcessor::data() );
}

/** Return the data of this processor */
const MolproProcessorPvt& MolproProcessor::data() const
{
    return dynamic_cast<const MolproProcessorPvt&>( FFThreadProcessor::data() );
}

/** Set the full path and name of the molpro executable to use to
    evaluate QM energies */
void MolproProcessor::setMolpro(const QString &molpro_exe)
{
    data().setMolpro(molpro_exe);
}

/** Set the directory used as the temporary directory */
void MolproProcessor::setTempDir(const QDir &tmpdir)
{
    data().setTempDir(tmpdir);
}
