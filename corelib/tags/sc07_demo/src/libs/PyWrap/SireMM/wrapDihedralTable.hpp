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

#ifndef PYWRAP_SIREMM_WRAP_DIHEDRALTABLE_HPP
#define PYWRAP_SIREMM_WRAP_DIHEDRALTABLE_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/dihedraltable.hpp"

using namespace boost::python;

SIRE_BEGIN_HEADER

namespace SireMM
{

template<class Param>
void wrap_DihedralTable(const char *wrapname)
{
    typedef DihedralTableT<Param> DT;
    
    class_<DT, bases<InternalTable<MolDihedralInfo,Param> >, boost::noncopyable>
                    (wrapname, no_init)
    
        .def( "addDihedral", (void (DT::*)(const Dihedral&))
                    &DT::addDihedral )
        .def( "addDihedral", (void (DT::*)(const Dihedral&, const Param&))
                    &DT::addDihedral )
    
        .def( "addDihedrals", (void (DT::*)(const QSet<Dihedral>&))
                    &DT::addDihedrals )
        .def( "addDihedrals", (void (DT::*)(const QList<Dihedral>&))
                    &DT::addDihedrals )

        .def( "assignedDihedralParameter", (bool (DT::*)(const Dihedral&) const)
                    &DT::assignedDihedralParameter )
        .def( "assignedDihedralParameter", (bool (DT::*)(const GroupIndexID&) const)
                    &DT::assignedDihedralParameter )
    
        .def( "dihedrals", (QVector<Dihedral> (DT::*)() const)
                    &DT::dihedrals )
        .def( "dihedrals", (QVector<Dihedral> (DT::*)(ResNum) const)
                    &DT::dihedrals )
        .def( "dihedrals", (QVector<Dihedral> (DT::*)(GroupID) const)
                    &DT::dihedrals )
    
        .def( "dihedralsByGroup", (GroupedVector<GroupIndexID,Dihedral> (DT::*)() const)
                    &DT::dihedralsByGroup )
        .def( "dihedralsByGroup", (GroupedVector<GroupIndexID,Dihedral> (DT::*)(
                                                        const QSet<GroupID>&) const)
                    &DT::dihedralsByGroup )
    
        .def( "dihedralsByResidue", (GroupedVector<ResNumIndexID,Dihedral> (DT::*)() const)
                    &DT::dihedralsByResidue )
        .def( "dihedralsByResidue", (GroupedVector<ResNumIndexID,Dihedral> (DT::*)(
                                                        const QSet<ResNum>&) const)
                    &DT::dihedralsByResidue )
    
        .def( "dihedralParameters", (QVector<Param> (DT::*)() const)
                    &DT::dihedralParameters )
        .def( "dihedralParameters", (QVector<Param> (DT::*)(ResNum) const)
                    &DT::dihedralParameters )
        .def( "dihedralParameters", (QVector<Param> (DT::*)(GroupID) const)
                    &DT::dihedralParameters )
    
        .def( "dihedralParametersByGroup", (GroupedVector<GroupIndexID,Param> (DT::*)() const)
                    &DT::dihedralParametersByGroup )
        .def( "dihedralParametersByGroup", (GroupedVector<GroupIndexID,Param> (DT::*)(
                                                            const QSet<GroupID>&) const)
                    &DT::dihedralParametersByGroup )
        
        .def( "dihedralParametersByResidue", (GroupedVector<ResNumIndexID,Param> (DT::*)() const)
                    &DT::dihedralParametersByResidue )
        .def( "dihedralParametersByResidue", (GroupedVector<ResNumIndexID,Param> (DT::*)(
                                                            const QSet<ResNum>&) const)
                    &DT::dihedralParametersByResidue )
    
        .def( "clearDihedral", (void (DT::*)(const Dihedral&))
                    &DT::clearDihedral )
        
        .def( "clearDihedrals", (void (DT::*)())
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(ResNum))
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(GroupID))
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(const QSet<Dihedral>&))
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(const QList<Dihedral>&))
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(const QSet<ResNum>&))
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(const QList<ResNum>&))
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(const QSet<GroupID>&))
                    &DT::clearDihedrals )
        .def( "clearDihedrals", (void (DT::*)(const QList<GroupID>&))
                    &DT::clearDihedrals )
    
        .def( "clearIntraDihedrals", (void (DT::*)())
                    &DT::clearIntraDihedrals )
        .def( "clearIntraDihedrals", (void (DT::*)(ResNum))
                    &DT::clearIntraDihedrals )
        .def( "clearIntraDihedrals", (void (DT::*)(const QSet<ResNum>&))
                    &DT::clearIntraDihedrals )
        .def( "clearIntraDihedrals", (void (DT::*)(const QList<ResNum>&))
                    &DT::clearIntraDihedrals )
    
        .def( "clearInterDihedrals", (void (DT::*)())
                    &DT::clearInterDihedrals )
        .def( "clearInterDihedrals", (void (DT::*)(ResNum))
                    &DT::clearInterDihedrals )
        .def( "clearInterDihedrals", (void (DT::*)(const QSet<ResNum>&))
                    &DT::clearInterDihedrals )
        .def( "clearInterDihedrals", (void (DT::*)(const QList<ResNum>&))
                    &DT::clearInterDihedrals )
    
        .def( "getDihedralParameter", (const Param& (DT::*)(const Dihedral&) const)
                    &DT::getDihedralParameter, return_value_policy<copy_const_reference>() )
        .def( "getDihedralParameter", (const Param& (DT::*)(const GroupIndexID&) const)
                    &DT::getDihedralParameter, return_value_policy<copy_const_reference>() )
    
        .def( "hasMissingDihedralParameters", (bool (DT::*)() const)
                    &DT::hasMissingDihedralParameters )
        .def( "hasMissingDihedralParameters", (bool (DT::*)(ResNum) const)
                    &DT::hasMissingDihedralParameters )
        .def( "hasMissingDihedralParameters", (bool (DT::*)(GroupID) const)
                    &DT::hasMissingDihedralParameters )
    
        .def( "hasMissingIntraDihedralParameters", (bool (DT::*)() const)
                    &DT::hasMissingIntraDihedralParameters )
        .def( "hasMissingIntraDihedralParameters", (bool (DT::*)(ResNum) const)
                    &DT::hasMissingIntraDihedralParameters )
    
        .def( "hasMissingInterDihedralParameters", (bool (DT::*)() const)
                    &DT::hasMissingInterDihedralParameters )
        .def( "hasMissingInterDihedralParameters", (bool (DT::*)(ResNum) const)
                    &DT::hasMissingInterDihedralParameters )
    
        .def( "intraDihedrals", (QVector<Dihedral> (DT::*)() const)
                    &DT::intraDihedrals )
        .def( "intraDihedrals", (QVector<Dihedral> (DT::*)(ResNum) const)
                    &DT::intraDihedrals )
    
        .def( "interDihedrals", (QVector<Dihedral> (DT::*)() const)
                    &DT::interDihedrals )
        .def( "interDihedrals", (QVector<Dihedral> (DT::*)(ResNum) const)
                    &DT::interDihedrals )
    
        .def( "intraDihedralsByResidue", (GroupedVector<ResNumIndexID,Dihedral> (DT::*)() const)
                    &DT::intraDihedralsByResidue )
        .def( "intraDihedralsByResidue", (GroupedVector<ResNumIndexID,Dihedral> (DT::*)(
                                                        const QSet<ResNum>&) const)
                    &DT::intraDihedralsByResidue )
    
        .def( "interDihedralsByResidue", (GroupedVector<ResNumIndexID,Dihedral> (DT::*)() const)
                    &DT::interDihedralsByResidue )
        .def( "interDihedralsByResidue", (GroupedVector<ResNumIndexID,Dihedral> (DT::*)(
                                                        const QSet<ResNum>&) const)
                    &DT::interDihedralsByResidue )
    
        .def( "intraDihedralParameters", (QVector<Param> (DT::*)() const)
                    &DT::intraDihedralParameters )
        .def( "intraDihedralParameters", (QVector<Param> (DT::*)(ResNum) const)
                    &DT::intraDihedralParameters )
    
        .def( "interDihedralParameters", (QVector<Param> (DT::*)() const)
                    &DT::interDihedralParameters )
        .def( "interDihedralParameters", (QVector<Param> (DT::*)(ResNum) const)
                    &DT::interDihedralParameters )
                                                    
        .def( "intraDihedralParametersByResidue", (GroupedVector<ResNumIndexID,Param> (DT::*)() const)
                    &DT::intraDihedralParametersByResidue )
        .def( "intraDihedralParametersByResidue", (GroupedVector<ResNumIndexID,Param> (DT::*)(
                                                            const QSet<ResNum>&) const)
                    &DT::intraDihedralParametersByResidue )
                                                    
        .def( "interDihedralParametersByResidue", (GroupedVector<ResNumIndexID,Param> (DT::*)() const)
                    &DT::interDihedralParametersByResidue )
        .def( "interDihedralParametersByResidue", (GroupedVector<ResNumIndexID,Param> (DT::*)(
                                                            const QSet<ResNum>&) const)
                    &DT::interDihedralParametersByResidue )
    
        .def( "missingDihedralParameters", (QSet<Dihedral> (DT::*)() const)
                    &DT::missingDihedralParameters )
        .def( "missingDihedralParameters", (QSet<Dihedral> (DT::*)(ResNum) const)
                    &DT::missingDihedralParameters )
        .def( "missingDihedralParameters", (QSet<Dihedral> (DT::*)(const QSet<GroupID>&) const)
                    &DT::missingDihedralParameters )
    
        .def( "missingIntraDihedralParameters", (QSet<Dihedral> (DT::*)() const)
                    &DT::missingIntraDihedralParameters )
        .def( "missingIntraDihedralParameters", (QSet<Dihedral> (DT::*)(ResNum) const)
                    &DT::missingIntraDihedralParameters )
    
        .def( "missingInterDihedralParameters", (QSet<Dihedral> (DT::*)() const)
                    &DT::missingInterDihedralParameters )
        .def( "missingInterDihedralParameters", (QSet<Dihedral> (DT::*)(ResNum) const)
                    &DT::missingInterDihedralParameters )
                    
        .def( "nDihedrals", (int (DT::*)() const)
                    &DT::nDihedrals )
        .def( "nDihedrals", (int (DT::*)(ResNum) const)
                    &DT::nDihedrals )
        .def( "nDihedrals", (int (DT::*)(GroupID) const)
                    &DT::nDihedrals )
    
        .def( "nIntraDihedrals", (int (DT::*)() const)
                    &DT::nIntraDihedrals )
        .def( "nIntraDihedrals", (int (DT::*)(ResNum) const)
                    &DT::nIntraDihedrals )
    
        .def( "nInterDihedrals", (int (DT::*)() const)
                    &DT::nInterDihedrals )
        .def( "nInterDihedrals", (int (DT::*)(ResNum) const)
                    &DT::nInterDihedrals )
                    
        .def( "nDihedralParameters", (int (DT::*)() const)
                    &DT::nDihedralParameters )
        .def( "nDihedralParameters", (int (DT::*)(ResNum) const)
                    &DT::nDihedralParameters )
        .def( "nDihedralParameters", (int (DT::*)(GroupID) const)
                    &DT::nDihedralParameters )
    
        .def( "nIntraDihedralParameters", (int (DT::*)() const)
                    &DT::nIntraDihedralParameters )
        .def( "nIntraDihedralParameters", (int (DT::*)(ResNum) const)
                    &DT::nIntraDihedralParameters )
    
        .def( "nInterDihedralParameters", (int (DT::*)() const)
                    &DT::nInterDihedralParameters )
        .def( "nInterDihedralParameters", (int (DT::*)(ResNum) const)
                    &DT::nInterDihedralParameters )
    
        .def( "removeDihedral", (void (DT::*)(const Dihedral&))
                    &DT::removeDihedral )
    
        .def( "removeDihedrals", (void (DT::*)())
                    &DT::removeDihedrals )
        .def( "removeDihedrals", (void (DT::*)(ResNum))
                    &DT::removeDihedrals )
        .def( "removeDihedrals", (void (DT::*)(GroupID))
                    &DT::removeDihedrals )
        .def( "removeDihedrals", (void (DT::*)(const QSet<GroupID>&))
                    &DT::removeDihedrals )
        .def( "removeDihedrals", (void (DT::*)(const QSet<Dihedral>&))
                    &DT::removeDihedrals )
        .def( "removeDihedrals", (void (DT::*)(const QList<Dihedral>&))
                    &DT::removeDihedrals )
        
        .def( "removeIntraDihedrals", (void (DT::*)())
                    &DT::removeIntraDihedrals )
        .def( "removeIntraDihedrals", (void (DT::*)(ResNum))
                    &DT::removeIntraDihedrals )
    
        .def( "removeInterDihedrals", (void (DT::*)())
                    &DT::removeInterDihedrals )
        .def( "removeInterDihedrals", (void (DT::*)(ResNum))
                    &DT::removeInterDihedrals )
    
        .def( "removeMissingDihedrals", (void (DT::*)())
                    &DT::removeMissingDihedrals )
        .def( "removeMissingDihedrals", (void (DT::*)(ResNum))
                    &DT::removeMissingDihedrals )
        .def( "removeMissingDihedrals", (void (DT::*)(GroupID))
                    &DT::removeMissingDihedrals )
        .def( "removeMissingDihedrals", (void (DT::*)(const QSet<GroupID>&))
                    &DT::removeMissingDihedrals )
    
        .def( "removeMissingIntraDihedrals", (void (DT::*)())
                    &DT::removeMissingIntraDihedrals )
        .def( "removeMissingIntraDihedrals", (void (DT::*)(ResNum))
                    &DT::removeMissingIntraDihedrals )
    
        .def( "removeMissingInterDihedrals", (void (DT::*)())
                    &DT::removeMissingInterDihedrals )
        .def( "removeMissingInterDihedrals", (void (DT::*)(ResNum))
                    &DT::removeMissingInterDihedrals )
    
        .def( "setDihedralParameter", (void (DT::*)(const Dihedral&, const Param&))
                    &DT::setDihedralParameter )
        .def( "setDihedralParameter", (void (DT::*)(const GroupIndexID&, const Param&))
                    &DT::setDihedralParameter )
    ;

}

}

SIRE_END_HEADER

#endif
