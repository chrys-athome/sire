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

/**
  * This file contains the boost::python wrapping of MatchAtomType
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/matchdata.h"
#include "SireDB/matchatom.h"
#include "SireDB/matchres.h"
#include "SireDB/matchmol.h"
#include "SireDB/matchatomtype.h"
#include "SireDB/matchatomtypedata.h"
#include "SireDB/matchmr.h"
#include "SireDB/matchmrdata.h"
#include "SireDB/matchmra.h"
#include "SireDB/matchmradata.h"

using namespace boost::python;

namespace SireDB
{

MatchAtomTypeData typ_and_res(const MatchAtomType &typ, const MatchRes &res)
{
    return typ and res;
}

MatchAtomTypeData typ_and_mol(const MatchAtomType &typ, const MatchMol &mol)
{
    return typ and mol;
}

MatchAtomTypeData typ_and_mr(const MatchAtomType &typ, const MatchMR &mr)
{
    return typ and mr;
}

MatchAtomTypeData typ_and_mrdata(const MatchAtomType &typ, const MatchMRData &mrdata)
{
    return typ and mrdata;
}

MatchAtomTypeData res_and_typ(const MatchAtomType &typ, const MatchRes &res)
{
    return res and typ;
}

MatchAtomTypeData mol_and_typ(const MatchAtomType &typ, const MatchMol &mol)
{
    return mol and typ;
}

MatchAtomTypeData mr_and_typ(const MatchAtomType &typ, const MatchMR &mr)
{
    return mr and typ;
}

MatchAtomTypeData mrdata_and_typ(const MatchAtomType &typ, const MatchMRData &mrdata)
{
    return mrdata and typ;
}

MatchAtomTypeData typdata_and_typ(const MatchAtomTypeData &typdata, 
                                  const MatchAtomType &typ)
{
    return typdata and typ;
}

MatchAtomTypeData typdata_and_res(const MatchAtomTypeData &typdata, const MatchRes &res)
{
    return typdata and res;
}

MatchAtomTypeData typdata_and_mol(const MatchAtomTypeData &typdata, const MatchMol &mol)
{
    return typdata and mol;
}

MatchAtomTypeData typdata_and_mr(const MatchAtomTypeData &typdata, const MatchMR &mr)
{
    return typdata and mr;
}

MatchAtomTypeData typdata_and_mrdata(const MatchAtomTypeData &typdata, 
                                     const MatchMRData &mrdata)
{
    return typdata and mrdata;
}


MatchAtomTypeData typ_and_typdata(const MatchAtomTypeData &typdata, 
                                  const MatchAtomType &typ)
{
    return typ and typdata;
}

MatchAtomTypeData res_and_typdata(const MatchAtomTypeData &typdata, const MatchRes &res)
{
    return res and typdata;
}

MatchAtomTypeData mol_and_typdata(const MatchAtomTypeData &typdata, const MatchMol &mol)
{
    return mol and typdata;
}

MatchAtomTypeData mr_and_typdata(const MatchAtomTypeData &typdata, const MatchMR &mr)
{
    return mr and typdata;
}

MatchAtomTypeData mrdata_and_typdata(const MatchAtomTypeData &typdata, 
                                     const MatchMRData &mrdata)
{
    return mrdata and typdata;
}

void  
SIREDB_EXPORT
export_MatchAtomType()
{
    QString (MatchAtomType::*wrap_MatchAtomType_toString)(uint) const 
                                                  = &MatchAtomType::toString;
    
    void (MatchAtomType::*wrap_MatchAtomType_setType1)(const QString&) 
                                                  = &MatchAtomType::setType;
    void (MatchAtomType::*wrap_MatchAtomType_setType2)(const AtomType&) 
                                                  = &MatchAtomType::setType;
    
    class_<MatchAtomType, bases<MatchData> >("MatchAtomType", init<>())
        .def( init<const QString&, const Element&>() )
        .def( init<const AtomType&>() )
        .def( init<const Element&>() )
        .def( self == self )
        .def( self != self )
        .def( "__and__", &MatchAtomType::operator&& )
        .def( "__and__", &typ_and_res )
        .def( "__and__", &typ_and_mol )
        .def( "__and__", &typ_and_mr )
        .def( "__and__", &typ_and_mrdata )
        .def( "__rand__", &res_and_typ )
        .def( "__rand__", &mol_and_typ )
        .def( "__rand__", &mr_and_typ )
        .def( "__rand__", &mrdata_and_typ )
        .def( "toString", wrap_MatchAtomType_toString )
        .def( "isWild", &MatchAtomType::isWild )
        .def( "isWildType", &MatchAtomType::isWildType )
        .def( "isWildElement", &MatchAtomType::isWildElement )
        .def( "setType", wrap_MatchAtomType_setType1 )
        .def( "setType", wrap_MatchAtomType_setType2 )
        .def( "type", &MatchAtomType::type, return_value_policy<copy_const_reference>() )
        .def( "setElement", &MatchAtomType::setElement )
        .def( "element", &MatchAtomType::element, return_value_policy<copy_const_reference>() )
    ;
    
    implicitly_convertible<MatchAtomType, MatchAtomTypeData>();
    
    void (MatchAtomTypeData::*wrap_setAtomType1)(const AtomType&)
                                                = &MatchAtomTypeData::setAtomType;
    void (MatchAtomTypeData::*wrap_setAtomType2)(const QString&)
                                                = &MatchAtomTypeData::setAtomType;
    void (MatchAtomTypeData::*wrap_setAtomType3)(uint, const AtomType&)
                                                = &MatchAtomTypeData::setAtomType;
    void (MatchAtomTypeData::*wrap_setAtomType4)(uint, const QString&)
                                                = &MatchAtomTypeData::setAtomType;
    
    void (MatchAtomTypeData::*wrap_setElement1)(const Element&)
                                                = &MatchAtomTypeData::setElement;
    void (MatchAtomTypeData::*wrap_setElement2)(uint)
                                                = &MatchAtomTypeData::setElement;
    void (MatchAtomTypeData::*wrap_setElement3)(const QString&)
                                                = &MatchAtomTypeData::setElement;
    void (MatchAtomTypeData::*wrap_setElement4)(uint, const Element&)
                                                = &MatchAtomTypeData::setElement;
    void (MatchAtomTypeData::*wrap_setElement5)(uint, uint)
                                                = &MatchAtomTypeData::setElement;
    void (MatchAtomTypeData::*wrap_setElement6)(uint, const QString&)
                                                = &MatchAtomTypeData::setElement;
    
    class_<MatchAtomTypeData, bases<MatchData> >("MatchAtomTypeData", init<>())
        .def( init<const MatchMol&>() )
        .def( init<const MatchRes&>() )
        .def( init<const MatchAtomType&>() )
        .def( init<const MatchMR&>() )
        .def( init<const MatchMRData&>() )
        .def( init<uint, const MatchRes&>() )
        .def( init<uint, const MatchAtomType&>() )
        .def( "__and__", &MatchAtomTypeData::operator&& )
        .def( "__and__", typdata_and_typ )
        .def( "__and__", typdata_and_res )
        .def( "__and__", typdata_and_mol )
        .def( "__and__", typdata_and_mr )
        .def( "__and__", typdata_and_mrdata )
        .def( "__rand__", typ_and_typdata )
        .def( "__rand__", res_and_typdata )
        .def( "__rand__", mol_and_typdata )
        .def( "__rand__", mr_and_typdata )
        .def( "__rand__", mrdata_and_typdata )
        .def( "getMatch", &MatchAtomTypeData::getMatch )
        .def( "hasMoleculeCriteria", &MatchAtomTypeData::hasMoleculeCriteria )
        .def( "hasResidueCriteria", &MatchAtomTypeData::hasResidueCriteria )
        .def( "hasAtomCriteria", &MatchAtomTypeData::hasAtomCriteria )
        .def( "set", &MatchAtomTypeData::set )
        .def( "setAtomType", wrap_setAtomType1 )
        .def( "setAtomType", wrap_setAtomType2 )
        .def( "setAtomType", wrap_setAtomType3 )
        .def( "setAtomType", wrap_setAtomType4 )
        .def( "setElement", wrap_setElement1 )
        .def( "setElement", wrap_setElement2 )
        .def( "setElement", wrap_setElement3 )
        .def( "setElement", wrap_setElement4 )
        .def( "setElement", wrap_setElement5 )
        .def( "setElement", wrap_setElement6 )
        .def( "simplify", &MatchAtomTypeData::simplify )
    ;

}

}
