/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef PYWRAP_SIREMM_WRAP_ANGLETABLE_HPP
#define PYWRAP_SIREMM_WRAP_ANGLETABLE_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/angletable.hpp"

using namespace boost::python;

SIRE_BEGIN_HEADER

namespace SireMM
{

template<class Param>
void wrap_AngleTable(const char *wrapname)
{
    typedef AngleTableT<Param> AT;
    
    class_<AT, bases<InternalTable<MolAngleInfo,Param> >, boost::noncopyable>
                    (wrapname, no_init)
    
        .def( "addAngle", (void (AT::*)(const Angle&))
                    &AT::addAngle )
        .def( "addAngle", (void (AT::*)(const Angle&, const Param&))
                    &AT::addAngle )
    
        .def( "addAngles", (void (AT::*)(const QSet<Angle>&))
                    &AT::addAngles )
        .def( "addAngles", (void (AT::*)(const QList<Angle>&))
                    &AT::addAngles )

        .def( "assignedAngleParameter", (bool (AT::*)(const Angle&) const)
                    &AT::assignedAngleParameter )
        .def( "assignedAngleParameter", (bool (AT::*)(const GroupIndexID&) const)
                    &AT::assignedAngleParameter )
    
        .def( "angles", (QVector<Angle> (AT::*)() const)
                    &AT::angles )
        .def( "angles", (QVector<Angle> (AT::*)(ResNum) const)
                    &AT::angles )
        .def( "angles", (QVector<Angle> (AT::*)(GroupID) const)
                    &AT::angles )
    
        .def( "anglesByGroup", (GroupedVector<GroupIndexID,Angle> (AT::*)() const)
                    &AT::anglesByGroup )
        .def( "anglesByGroup", (GroupedVector<GroupIndexID,Angle> (AT::*)(
                                                        const QSet<GroupID>&) const)
                    &AT::anglesByGroup )
    
        .def( "anglesByResidue", (GroupedVector<ResNumIndexID,Angle> (AT::*)() const)
                    &AT::anglesByResidue )
        .def( "anglesByResidue", (GroupedVector<ResNumIndexID,Angle> (AT::*)(
                                                        const QSet<ResNum>&) const)
                    &AT::anglesByResidue )
    
        .def( "angleParameters", (QVector<Param> (AT::*)() const)
                    &AT::angleParameters )
        .def( "angleParameters", (QVector<Param> (AT::*)(ResNum) const)
                    &AT::angleParameters )
        .def( "angleParameters", (QVector<Param> (AT::*)(GroupID) const)
                    &AT::angleParameters )
    
        .def( "angleParametersByGroup", (GroupedVector<GroupIndexID,Param> (AT::*)() const)
                    &AT::angleParametersByGroup )
        .def( "angleParametersByGroup", (GroupedVector<GroupIndexID,Param> (AT::*)(
                                                            const QSet<GroupID>&) const)
                    &AT::angleParametersByGroup )
        
        .def( "angleParametersByResidue", (GroupedVector<ResNumIndexID,Param> (AT::*)() const)
                    &AT::angleParametersByResidue )
        .def( "angleParametersByResidue", (GroupedVector<ResNumIndexID,Param> (AT::*)(
                                                            const QSet<ResNum>&) const)
                    &AT::angleParametersByResidue )
    
        .def( "clearAngle", (void (AT::*)(const Angle&))
                    &AT::clearAngle )
        
        .def( "clearAngles", (void (AT::*)())
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(ResNum))
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(GroupID))
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(const QSet<Angle>&))
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(const QList<Angle>&))
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(const QSet<ResNum>&))
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(const QList<ResNum>&))
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(const QSet<GroupID>&))
                    &AT::clearAngles )
        .def( "clearAngles", (void (AT::*)(const QList<GroupID>&))
                    &AT::clearAngles )
    
        .def( "clearIntraAngles", (void (AT::*)())
                    &AT::clearIntraAngles )
        .def( "clearIntraAngles", (void (AT::*)(ResNum))
                    &AT::clearIntraAngles )
        .def( "clearIntraAngles", (void (AT::*)(const QSet<ResNum>&))
                    &AT::clearIntraAngles )
        .def( "clearIntraAngles", (void (AT::*)(const QList<ResNum>&))
                    &AT::clearIntraAngles )
    
        .def( "clearInterAngles", (void (AT::*)())
                    &AT::clearInterAngles )
        .def( "clearInterAngles", (void (AT::*)(ResNum))
                    &AT::clearInterAngles )
        .def( "clearInterAngles", (void (AT::*)(const QSet<ResNum>&))
                    &AT::clearInterAngles )
        .def( "clearInterAngles", (void (AT::*)(const QList<ResNum>&))
                    &AT::clearInterAngles )
    
        .def( "getAngleParameter", (const Param& (AT::*)(const Angle&) const)
                    &AT::getAngleParameter, return_value_policy<copy_const_reference>() )
        .def( "getAngleParameter", (const Param& (AT::*)(const GroupIndexID&) const)
                    &AT::getAngleParameter, return_value_policy<copy_const_reference>() )
    
        .def( "hasMissingAngleParameters", (bool (AT::*)() const)
                    &AT::hasMissingAngleParameters )
        .def( "hasMissingAngleParameters", (bool (AT::*)(ResNum) const)
                    &AT::hasMissingAngleParameters )
        .def( "hasMissingAngleParameters", (bool (AT::*)(GroupID) const)
                    &AT::hasMissingAngleParameters )
    
        .def( "hasMissingIntraAngleParameters", (bool (AT::*)() const)
                    &AT::hasMissingIntraAngleParameters )
        .def( "hasMissingIntraAngleParameters", (bool (AT::*)(ResNum) const)
                    &AT::hasMissingIntraAngleParameters )
    
        .def( "hasMissingInterAngleParameters", (bool (AT::*)() const)
                    &AT::hasMissingInterAngleParameters )
        .def( "hasMissingInterAngleParameters", (bool (AT::*)(ResNum) const)
                    &AT::hasMissingInterAngleParameters )
    
        .def( "intraAngles", (QVector<Angle> (AT::*)() const)
                    &AT::intraAngles )
        .def( "intraAngles", (QVector<Angle> (AT::*)(ResNum) const)
                    &AT::intraAngles )
    
        .def( "interAngles", (QVector<Angle> (AT::*)() const)
                    &AT::interAngles )
        .def( "interAngles", (QVector<Angle> (AT::*)(ResNum) const)
                    &AT::interAngles )
    
        .def( "intraAnglesByResidue", (GroupedVector<ResNumIndexID,Angle> (AT::*)() const)
                    &AT::intraAnglesByResidue )
        .def( "intraAnglesByResidue", (GroupedVector<ResNumIndexID,Angle> (AT::*)(
                                                        const QSet<ResNum>&) const)
                    &AT::intraAnglesByResidue )
    
        .def( "interAnglesByResidue", (GroupedVector<ResNumIndexID,Angle> (AT::*)() const)
                    &AT::interAnglesByResidue )
        .def( "interAnglesByResidue", (GroupedVector<ResNumIndexID,Angle> (AT::*)(
                                                        const QSet<ResNum>&) const)
                    &AT::interAnglesByResidue )
    
        .def( "intraAngleParameters", (QVector<Param> (AT::*)() const)
                    &AT::intraAngleParameters )
        .def( "intraAngleParameters", (QVector<Param> (AT::*)(ResNum) const)
                    &AT::intraAngleParameters )
    
        .def( "interAngleParameters", (QVector<Param> (AT::*)() const)
                    &AT::interAngleParameters )
        .def( "interAngleParameters", (QVector<Param> (AT::*)(ResNum) const)
                    &AT::interAngleParameters )
                                                    
        .def( "intraAngleParametersByResidue", (GroupedVector<ResNumIndexID,Param> (AT::*)() const)
                    &AT::intraAngleParametersByResidue )
        .def( "intraAngleParametersByResidue", (GroupedVector<ResNumIndexID,Param> (AT::*)(
                                                            const QSet<ResNum>&) const)
                    &AT::intraAngleParametersByResidue )
                                                    
        .def( "interAngleParametersByResidue", (GroupedVector<ResNumIndexID,Param> (AT::*)() const)
                    &AT::interAngleParametersByResidue )
        .def( "interAngleParametersByResidue", (GroupedVector<ResNumIndexID,Param> (AT::*)(
                                                            const QSet<ResNum>&) const)
                    &AT::interAngleParametersByResidue )
    
        .def( "missingAngleParameters", (QSet<Angle> (AT::*)() const)
                    &AT::missingAngleParameters )
        .def( "missingAngleParameters", (QSet<Angle> (AT::*)(ResNum) const)
                    &AT::missingAngleParameters )
        .def( "missingAngleParameters", (QSet<Angle> (AT::*)(const QSet<GroupID>&) const)
                    &AT::missingAngleParameters )
    
        .def( "missingIntraAngleParameters", (QSet<Angle> (AT::*)() const)
                    &AT::missingIntraAngleParameters )
        .def( "missingIntraAngleParameters", (QSet<Angle> (AT::*)(ResNum) const)
                    &AT::missingIntraAngleParameters )
    
        .def( "missingInterAngleParameters", (QSet<Angle> (AT::*)() const)
                    &AT::missingInterAngleParameters )
        .def( "missingInterAngleParameters", (QSet<Angle> (AT::*)(ResNum) const)
                    &AT::missingInterAngleParameters )
                    
        .def( "nAngles", (int (AT::*)() const)
                    &AT::nAngles )
        .def( "nAngles", (int (AT::*)(ResNum) const)
                    &AT::nAngles )
        .def( "nAngles", (int (AT::*)(GroupID) const)
                    &AT::nAngles )
    
        .def( "nIntraAngles", (int (AT::*)() const)
                    &AT::nIntraAngles )
        .def( "nIntraAngles", (int (AT::*)(ResNum) const)
                    &AT::nIntraAngles )
    
        .def( "nInterAngles", (int (AT::*)() const)
                    &AT::nInterAngles )
        .def( "nInterAngles", (int (AT::*)(ResNum) const)
                    &AT::nInterAngles )
                    
        .def( "nAngleParameters", (int (AT::*)() const)
                    &AT::nAngleParameters )
        .def( "nAngleParameters", (int (AT::*)(ResNum) const)
                    &AT::nAngleParameters )
        .def( "nAngleParameters", (int (AT::*)(GroupID) const)
                    &AT::nAngleParameters )
    
        .def( "nIntraAngleParameters", (int (AT::*)() const)
                    &AT::nIntraAngleParameters )
        .def( "nIntraAngleParameters", (int (AT::*)(ResNum) const)
                    &AT::nIntraAngleParameters )
    
        .def( "nInterAngleParameters", (int (AT::*)() const)
                    &AT::nInterAngleParameters )
        .def( "nInterAngleParameters", (int (AT::*)(ResNum) const)
                    &AT::nInterAngleParameters )
    
        .def( "removeAngle", (void (AT::*)(const Angle&))
                    &AT::removeAngle )
    
        .def( "removeAngles", (void (AT::*)())
                    &AT::removeAngles )
        .def( "removeAngles", (void (AT::*)(ResNum))
                    &AT::removeAngles )
        .def( "removeAngles", (void (AT::*)(GroupID))
                    &AT::removeAngles )
        .def( "removeAngles", (void (AT::*)(const QSet<GroupID>&))
                    &AT::removeAngles )
        .def( "removeAngles", (void (AT::*)(const QSet<Angle>&))
                    &AT::removeAngles )
        .def( "removeAngles", (void (AT::*)(const QList<Angle>&))
                    &AT::removeAngles )
        
        .def( "removeIntraAngles", (void (AT::*)())
                    &AT::removeIntraAngles )
        .def( "removeIntraAngles", (void (AT::*)(ResNum))
                    &AT::removeIntraAngles )
    
        .def( "removeInterAngles", (void (AT::*)())
                    &AT::removeInterAngles )
        .def( "removeInterAngles", (void (AT::*)(ResNum))
                    &AT::removeInterAngles )
    
        .def( "removeMissingAngles", (void (AT::*)())
                    &AT::removeMissingAngles )
        .def( "removeMissingAngles", (void (AT::*)(ResNum))
                    &AT::removeMissingAngles )
        .def( "removeMissingAngles", (void (AT::*)(GroupID))
                    &AT::removeMissingAngles )
        .def( "removeMissingAngles", (void (AT::*)(const QSet<GroupID>&))
                    &AT::removeMissingAngles )
    
        .def( "removeMissingIntraAngles", (void (AT::*)())
                    &AT::removeMissingIntraAngles )
        .def( "removeMissingIntraAngles", (void (AT::*)(ResNum))
                    &AT::removeMissingIntraAngles )
    
        .def( "removeMissingInterAngles", (void (AT::*)())
                    &AT::removeMissingInterAngles )
        .def( "removeMissingInterAngles", (void (AT::*)(ResNum))
                    &AT::removeMissingInterAngles )
    
        .def( "setAngleParameter", (void (AT::*)(const Angle&, const Param&))
                    &AT::setAngleParameter )
        .def( "setAngleParameter", (void (AT::*)(const GroupIndexID&, const Param&))
                    &AT::setAngleParameter )
    ;

}

}

SIRE_END_HEADER

#endif
