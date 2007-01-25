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
  * This file contains the boost::python wrapping of the parameter database classes
  *
  * @author Christopher Woods
  *
  */

// #include <Python.h>
// #include <boost/python.hpp>
//
// #include "SireMol/qhash_siremol.h"
//
// #include "SireMM/chargedb.h"
// #include "SireMM/ljdb.h"
// #include "SireMM/bonddb.h"
// #include "SireMM/angledb.h"
// #include "SireMM/dihedraldb.h"
// #include "SireMM/ureybradleydb.h"
//
// #include "SireCAS/expression.h"
// #include "SireCAS/symbol.h"
// #include "SireCAS/function.h"
//
// #include "wrapInternalDB.hpp"
//
// using namespace boost::python;
// using namespace SireDB;
//
// namespace SireMM
// {
//
// Expression wrap_getAngle1(AngleDB &db, const QString &userid, bool &found)
// {
//     return db.getAngle(userid, &found);
// }
//
// Expression wrap_getAngle2(AngleDB &db, RelateID relateid, bool &found)
// {
//     return db.getAngle(relateid, &found);
// }
//
// Expression wrap_getAngle3(AngleDB &db, const RelateIDMap &relateids, bool &found)
// {
//     return db.getAngle(relateids, &found);
// }
//
// Expression wrap_getAngle4(AngleDB &db, const QString &userid)
// {
//     return db.getAngle(userid);
// }
//
// Expression wrap_getAngle5(AngleDB &db, RelateID relateid)
// {
//     return db.getAngle(relateid);
// }
//
// Expression wrap_getAngle6(AngleDB &db, const RelateIDMap &relateids)
// {
//     return db.getAngle(relateids);
// }
//
// Expression wrap_getDihedral1(DihedralDB &db, const QString &userid, bool &found)
// {
//     return db.getDihedral(userid, &found);
// }
//
// Expression wrap_getDihedral2(DihedralDB &db, RelateID relateid, bool &found)
// {
//     return db.getDihedral(relateid, &found);
// }
//
// Expression wrap_getDihedral3(DihedralDB &db, const RelateIDMap &relateids, bool &found)
// {
//     return db.getDihedral(relateids, &found);
// }
//
// Expression wrap_getDihedral4(DihedralDB &db, const QString &userid)
// {
//     return db.getDihedral(userid);
// }
//
// Expression wrap_getDihedral5(DihedralDB &db, RelateID relateid)
// {
//     return db.getDihedral(relateid);
// }
//
// Expression wrap_getDihedral6(DihedralDB &db, const RelateIDMap &relateids)
// {
//     return db.getDihedral(relateids);
// }
//
// Expression wrap_getDihedral7(DihedralDB &db, const Function &function, bool &found)
// {
//     return db.getDihedral(function,&found);
// }
//
// Expression wrap_getDihedral8(DihedralDB &db, const Function &function)
// {
//     return db.getDihedral(function);
// }
//
// Expression wrap_getUreyBradley1(UreyBradleyDB &db, const QString &userid, bool &found)
// {
//     return db.getUreyBradley(userid, &found);
// }
//
// Expression wrap_getUreyBradley2(UreyBradleyDB &db, RelateID relateid, bool &found)
// {
//     return db.getUreyBradley(relateid, &found);
// }
//
// Expression wrap_getUreyBradley3(UreyBradleyDB &db, const RelateIDMap &relateids, bool &found)
// {
//     return db.getUreyBradley(relateids, &found);
// }
//
// Expression wrap_getUreyBradley4(UreyBradleyDB &db, const QString &userid)
// {
//     return db.getUreyBradley(userid);
// }
//
// Expression wrap_getUreyBradley5(UreyBradleyDB &db, RelateID relateid)
// {
//     return db.getUreyBradley(relateid);
// }
//
// Expression wrap_getUreyBradley6(UreyBradleyDB &db, const RelateIDMap &relateids)
// {
//     return db.getUreyBradley(relateids);
// }
//
// void export_MMDB2()
// {
//
//     void (AngleDB::*wrap_addAngle1)(const QString&, const Expression&) = &AngleDB::addAngle;
//
//     void (AngleDB::*wrap_relateAngle1)(const AssertMatch<3>&, const QString&)
//                                                         = &AngleDB::relateAngle;
//     void (AngleDB::*wrap_relateAngle2)(const AssertMatch<3>&, const Expression&)
//                                                         = &AngleDB::relateAngle;
//     void (AngleDB::*wrap_relateAngle3)(RelateID, const QString&)
//                                                         = &AngleDB::relateAngle;
//     void (AngleDB::*wrap_relateAngle4)(RelateID, const Expression&)
//                                                         = &AngleDB::relateAngle;
//
//     wrap_InternalDB<MolAngleInfo>("InternalDB_MolAngleInfo_");
//
//     class_<AngleDB, bases<Term13DB, InternalDB<MolAngleInfo> >,
//                     boost::noncopyable>("AngleDB", no_init)
//         .def("addAngle", wrap_addAngle1)
//         .def("getAngle", &wrap_getAngle1)
//         .def("getAngle", &wrap_getAngle2)
//         .def("getAngle", &wrap_getAngle3)
//         .def("getAngle", &wrap_getAngle4)
//         .def("getAngle", &wrap_getAngle5)
//         .def("getAngle", &wrap_getAngle6)
//         .def("relateAngle", wrap_relateAngle1)
//         .def("relateAngle", wrap_relateAngle2)
//         .def("relateAngle", wrap_relateAngle3)
//         .def("relateAngle", wrap_relateAngle4)
//         .def("r", &AngleDB::r, return_value_policy<copy_const_reference>())
//         .def("theta", &AngleDB::theta, return_value_policy<copy_const_reference>())
//         .def("typeName", &AngleDB::typeName ).staticmethod("typeName")
//     ;
//
//
//     void (DihedralDB::*wrap_addDihedral1)(const QString&, const Expression&)
//                                                         = &DihedralDB::addDihedral;
//     void (DihedralDB::*wrap_addDihedral2)(const Function&, const Expression&)
//                                                         = &DihedralDB::addDihedral;
//
//     void (DihedralDB::*wrap_relateDihedral1)(const AssertMatch<4>&, const QString&)
//                                                         = &DihedralDB::relateDihedral;
//     void (DihedralDB::*wrap_relateDihedral2)(const AssertMatch<4>&, const Expression&)
//                                                         = &DihedralDB::relateDihedral;
//     void (DihedralDB::*wrap_relateDihedral3)(RelateID, const QString&)
//                                                         = &DihedralDB::relateDihedral;
//     void (DihedralDB::*wrap_relateDihedral4)(RelateID, const Expression&)
//                                                         = &DihedralDB::relateDihedral;
//
//     wrap_InternalDB<MolDihedralInfo>("InternalDB_MolDihedralInfo_");
//
//     class_<DihedralDB, bases<Term14DB, InternalDB<MolDihedralInfo> >,
//                        boost::noncopyable>("DihedralDB", no_init)
//         .def("addDihedral", wrap_addDihedral1)
//         .def("addDihedral", wrap_addDihedral2)
//         .def("getDihedral", &wrap_getDihedral1)
//         .def("getDihedral", &wrap_getDihedral2)
//         .def("getDihedral", &wrap_getDihedral3)
//         .def("getDihedral", &wrap_getDihedral4)
//         .def("getDihedral", &wrap_getDihedral5)
//         .def("getDihedral", &wrap_getDihedral6)
//         .def("getDihedral", &wrap_getDihedral7)
//         .def("getDihedral", &wrap_getDihedral8)
//         .def("relateDihedral", wrap_relateDihedral1)
//         .def("relateDihedral", wrap_relateDihedral2)
//         .def("relateDihedral", wrap_relateDihedral3)
//         .def("relateDihedral", wrap_relateDihedral4)
//         .def("r", &DihedralDB::r, return_value_policy<copy_const_reference>())
//         .def("phi", &DihedralDB::phi, return_value_policy<copy_const_reference>())
//         .def("typeName", &DihedralDB::typeName ).staticmethod("typeName")
//     ;
//
//
//     void (UreyBradleyDB::*wrap_addUreyBradley1)(const QString&, const Expression&)
//                                                         = &UreyBradleyDB::addUreyBradley;
//
//     void (UreyBradleyDB::*wrap_relateUreyBradley1)(const AssertMatch<3>&, const QString&)
//                                                         = &UreyBradleyDB::relateUreyBradley;
//     void (UreyBradleyDB::*wrap_relateUreyBradley2)(const AssertMatch<3>&, const Expression&)
//                                                         = &UreyBradleyDB::relateUreyBradley;
//     void (UreyBradleyDB::*wrap_relateUreyBradley3)(RelateID, const QString&)
//                                                         = &UreyBradleyDB::relateUreyBradley;
//     void (UreyBradleyDB::*wrap_relateUreyBradley4)(RelateID, const Expression&)
//                                                         = &UreyBradleyDB::relateUreyBradley;
//
//     class_<UreyBradleyDB, bases<Term13DB>, boost::noncopyable>("UreyBradleyDB", no_init)
//         .def("addUreyBradley", wrap_addUreyBradley1)
//         .def("getUreyBradley", &wrap_getUreyBradley1)
//         .def("getUreyBradley", &wrap_getUreyBradley2)
//         .def("getUreyBradley", &wrap_getUreyBradley3)
//         .def("getUreyBradley", &wrap_getUreyBradley4)
//         .def("getUreyBradley", &wrap_getUreyBradley5)
//         .def("getUreyBradley", &wrap_getUreyBradley6)
//         .def("relateUreyBradley", wrap_relateUreyBradley1)
//         .def("relateUreyBradley", wrap_relateUreyBradley2)
//         .def("relateUreyBradley", wrap_relateUreyBradley3)
//         .def("relateUreyBradley", wrap_relateUreyBradley4)
//         .def("r", &UreyBradleyDB::r, return_value_policy<copy_const_reference>())
//         .def("typeName", &UreyBradleyDB::typeName ).staticmethod("typeName")
//     ;
// }
//
// }
