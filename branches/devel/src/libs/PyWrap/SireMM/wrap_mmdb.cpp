
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
// #include "SireMM/mmdb.h"
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
// ChargeParameter wrap_getCharge1(ChargeDB &db, const QString &userid, bool &foundcharge)
// {
//     return db.getCharge(userid, &foundcharge);
// }
//
// ChargeParameter wrap_getCharge2(ChargeDB &db, RelateID relateid, bool &foundcharge)
// {
//     return db.getCharge(relateid, &foundcharge);
// }
//
// ChargeParameter wrap_getCharge3(ChargeDB &db, const RelateIDMap &relateids, bool &foundcharge)
// {
//     return db.getCharge(relateids, &foundcharge);
// }
//
// ChargeParameter wrap_getCharge4(ChargeDB &db, const QString &userid)
// {
//     return db.getCharge(userid);
// }
//
// ChargeParameter wrap_getCharge5(ChargeDB &db, RelateID relateid)
// {
//     return db.getCharge(relateid);
// }
//
// ChargeParameter wrap_getCharge6(ChargeDB &db, const RelateIDMap &relateids)
// {
//     return db.getCharge(relateids);
// }
//
// LJParameter wrap_getLJ1(LJDB &db, const QString &userid, bool &foundcharge)
// {
//     return db.getLJ(userid, &foundcharge);
// }
//
// LJParameter wrap_getLJ2(LJDB &db, RelateID relateid, bool &foundcharge)
// {
//     return db.getLJ(relateid, &foundcharge);
// }
//
// LJParameter wrap_getLJ3(LJDB &db, const RelateIDMap &relateids, bool &foundcharge)
// {
//     return db.getLJ(relateids, &foundcharge);
// }
//
// LJParameter wrap_getLJ4(LJDB &db, const QString &userid)
// {
//     return db.getLJ(userid);
// }
//
// LJParameter wrap_getLJ5(LJDB &db, RelateID relateid)
// {
//     return db.getLJ(relateid);
// }
//
// LJParameter wrap_getLJ6(LJDB &db, const RelateIDMap &relateids)
// {
//     return db.getLJ(relateids);
// }
//
// Expression wrap_getBond1(BondDB &db, const QString &userid, bool &found)
// {
//     return db.getBond(userid, &found);
// }
//
// Expression wrap_getBond2(BondDB &db, RelateID relateid, bool &found)
// {
//     return db.getBond(relateid, &found);
// }
//
// Expression wrap_getBond3(BondDB &db, const RelateIDMap &relateids, bool &found)
// {
//     return db.getBond(relateids, &found);
// }
//
// Expression wrap_getBond4(BondDB &db, const QString &userid)
// {
//     return db.getBond(userid);
// }
//
// Expression wrap_getBond5(BondDB &db, RelateID relateid)
// {
//     return db.getBond(relateid);
// }
//
// Expression wrap_getBond6(BondDB &db, const RelateIDMap &relateids)
// {
//     return db.getBond(relateids);
// }
//
// void export_MMDB()
// {
//     void (ChargeDB::*wrap_addCharge1)(const QString&, const ChargeParameter&)
//                                                         = &ChargeDB::addCharge;
//
//     void (ChargeDB::*wrap_relateCharge1)(const AssertMatch<1>&, const QString&)
//                                                         = &ChargeDB::relateCharge;
//     void (ChargeDB::*wrap_relateCharge2)(const AssertMatch<1>&, const ChargeParameter&)
//                                                         = &ChargeDB::relateCharge;
//     void (ChargeDB::*wrap_relateCharge3)(RelateID, const QString&)
//                                                         = &ChargeDB::relateCharge;
//     void (ChargeDB::*wrap_relateCharge4)(RelateID, const ChargeParameter&)
//                                                         = &ChargeDB::relateCharge;
//
//     class_<ChargeDB, bases<AtomDB>, boost::noncopyable>("ChargeDB", no_init)
//         .def("addCharge", wrap_addCharge1)
//         .def("getCharge", &wrap_getCharge1)
//         .def("getCharge", &wrap_getCharge2)
//         .def("getCharge", &wrap_getCharge3)
//         .def("getCharge", &wrap_getCharge4)
//         .def("getCharge", &wrap_getCharge5)
//         .def("getCharge", &wrap_getCharge6)
//         .def("relateCharge", wrap_relateCharge1)
//         .def("relateCharge", wrap_relateCharge2)
//         .def("relateCharge", wrap_relateCharge3)
//         .def("relateCharge", wrap_relateCharge4)
//         .def("typeName", &ChargeDB::typeName ).staticmethod("typeName")
//     ;
//
//
//     void (LJDB::*wrap_addLJ1)(const QString&, const LJParameter&) = &LJDB::addLJ;
//
//     void (LJDB::*wrap_relateLJ1)(const AssertMatch<1>&, const QString&) = &LJDB::relateLJ;
//     void (LJDB::*wrap_relateLJ2)(const AssertMatch<1>&, const LJParameter&) = &LJDB::relateLJ;
//     void (LJDB::*wrap_relateLJ3)(RelateID, const QString&) = &LJDB::relateLJ;
//     void (LJDB::*wrap_relateLJ4)(RelateID, const LJParameter&) = &LJDB::relateLJ;
//
//     class_<LJDB, bases<AtomDB>, boost::noncopyable>("LJDB", no_init)
//         .def("addLJ", wrap_addLJ1)
//         .def("getLJ", &wrap_getLJ1)
//         .def("getLJ", &wrap_getLJ2)
//         .def("getLJ", &wrap_getLJ3)
//         .def("getLJ", &wrap_getLJ4)
//         .def("getLJ", &wrap_getLJ5)
//         .def("getLJ", &wrap_getLJ6)
//         .def("relateLJ", wrap_relateLJ1)
//         .def("relateLJ", wrap_relateLJ2)
//         .def("relateLJ", wrap_relateLJ3)
//         .def("relateLJ", wrap_relateLJ4)
//         .def("typeName", &LJDB::typeName ).staticmethod("typeName")
//     ;
//
//     void (BondDB::*wrap_addBond1)(const QString&, const Expression&) = &BondDB::addBond;
//
//     void (BondDB::*wrap_relateBond1)(const AssertMatch<2>&, const QString&)
//                                                         = &BondDB::relateBond;
//     void (BondDB::*wrap_relateBond2)(const AssertMatch<2>&, const Expression&)
//                                                         = &BondDB::relateBond;
//     void (BondDB::*wrap_relateBond3)(RelateID, const QString&)
//                                                         = &BondDB::relateBond;
//     void (BondDB::*wrap_relateBond4)(RelateID, const Expression&)
//                                                         = &BondDB::relateBond;
//
//     wrap_InternalDB<MolBondInfo>("InternalDB_MolBondInfo_");
//
//     class_<BondDB, bases<Term12DB, InternalDB<MolBondInfo> >,
//                    boost::noncopyable>("BondDB", no_init)
//         .def("addBond", wrap_addBond1)
//         .def("getBond", &wrap_getBond1)
//         .def("getBond", &wrap_getBond2)
//         .def("getBond", &wrap_getBond3)
//         .def("getBond", &wrap_getBond4)
//         .def("getBond", &wrap_getBond5)
//         .def("getBond", &wrap_getBond6)
//         .def("relateBond", wrap_relateBond1)
//         .def("relateBond", wrap_relateBond2)
//         .def("relateBond", wrap_relateBond3)
//         .def("relateBond", wrap_relateBond4)
//         .def("r", &BondDB::r, return_value_policy<copy_const_reference>())
//         .def("typeName", &BondDB::typeName ).staticmethod("typeName")
//     ;
// }
//
// }
