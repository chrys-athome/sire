
/**
  * This file contains the boost::python wrapping of MolAngleInfo
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMM/molangleinfo.h"
#include "SireMM/resangleinfo.h"

#include "wrapInternalInfo.hpp"

using namespace boost::python;
using namespace SireMol;

namespace SireMM
{

MolAngleInfo::const_iterator wrap_angles_QSet(
                        const MolAngleInfo &m, const QSet<ResNum> &resnums)
{
    return m.angles(resnums);
}
    
MolAngleInfo::const_iterator wrap_angles_QList(
                        const MolAngleInfo &m, const QList<ResNum> &resnums)
{
    return m.angles(resnums);
}

MolAngleInfo::const_iterator wrap_interAngles_QSet(
                        const MolAngleInfo &m, const QSet<ResNum> &resnums)
{
    return m.interAngles(resnums);
}
    
MolAngleInfo::const_iterator wrap_interAngles_QList(
                        const MolAngleInfo &m, const QList<ResNum> &resnums)
{
    return m.interAngles(resnums);
}

MolAngleInfo::const_iterator wrap_intraAngles_QSet(
                        const MolAngleInfo &m, const QSet<ResNum> &resnums)
{
    return m.intraAngles(resnums);
}
    
MolAngleInfo::const_iterator wrap_intraAngles_QList(
                        const MolAngleInfo &m, const QList<ResNum> &resnums)
{
    return m.intraAngles(resnums);
}

void wrap_addAngles_QList(MolAngleInfo &m, const QList<Angle> &bonds)
{
    m.addAngles(bonds);
}

void wrap_addAngles_QSet(MolAngleInfo &m, const QSet<Angle> &bonds)
{
    m.addAngles(bonds);
}

void wrap_removeAngles_QList(MolAngleInfo &m, const QList<Angle> &bonds)
{
    m.removeAngles(bonds);
}

void wrap_removeAngles_QSet(MolAngleInfo &m, const QSet<Angle> &bonds)
{
    m.removeAngles(bonds);
}

void  
SIREMM_EXPORT
export_MolAngleInfo()
{
    //wrap the iterator type
    wrap_InternalIterator<Angle>("AngleInfoIterator");
    
    //wrap the MolAngleInfo class
    class_<MolAngleInfo> wrap_MolAngleInfo("MolAngleInfo", init<>());
    
    //wrap the common parts of this object
    wrap_MolInternalInfo<MolAngleInfo>( wrap_MolAngleInfo );
    
    //now wrap the specific parts
    int (MolAngleInfo::*wrap_nAngles1)() const = &MolAngleInfo::nAngles;
    int (MolAngleInfo::*wrap_nAngles2)(GroupID) const = &MolAngleInfo::nAngles;
    int (MolAngleInfo::*wrap_nAngles3)(ResNum) const = &MolAngleInfo::nAngles;
    int (MolAngleInfo::*wrap_nAngles4)(ResNum, ResNum) const = &MolAngleInfo::nAngles;
    int (MolAngleInfo::*wrap_nAngles5)(ResNum, ResNum, ResNum) const = &MolAngleInfo::nAngles;

    int (MolAngleInfo::*wrap_nInterAngles1)() const = &MolAngleInfo::nInterAngles;
    int (MolAngleInfo::*wrap_nInterAngles2)(ResNum resnum) const = &MolAngleInfo::nInterAngles;
    
    int (MolAngleInfo::*wrap_nIntraAngles1)() const = &MolAngleInfo::nIntraAngles;
    int (MolAngleInfo::*wrap_nIntraAngles2)(ResNum resnum) const = &MolAngleInfo::nIntraAngles;

    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_angles1)() const 
                                            = &MolAngleInfo::angles;
    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_angles2)(ResNum) const
                                            = &MolAngleInfo::angles;
    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_angles3)(ResNum, ResNum) const
                                            = &MolAngleInfo::angles;
    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_angles4)(ResNum,ResNum,ResNum) const
                                            = &MolAngleInfo::angles;
    
    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_interAngles1)(ResNum) const
                                            = &MolAngleInfo::interAngles;
    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_interAngles2)() const
                                            = &MolAngleInfo::interAngles;
    
    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_intraAngles1)(ResNum) const
                                            = &MolAngleInfo::intraAngles;
    MolAngleInfo::const_iterator (MolAngleInfo::*wrap_intraAngles2)() const
                                            = &MolAngleInfo::intraAngles;
    
    bool (MolAngleInfo::*wrap_residuesAngled1)(ResNum, ResNum) const
                                            = &MolAngleInfo::residuesAngled;
    bool (MolAngleInfo::*wrap_residuesAngled2)(ResNum, ResNum, ResNum) const
                                            = &MolAngleInfo::residuesAngled;
    
    void (MolAngleInfo::*wrap_removeAngle1)(const Angle&) = &MolAngleInfo::removeAngle;
    void (MolAngleInfo::*wrap_removeAngle2)(const GroupIndexID&) = &MolAngleInfo::removeAngle;
    
    wrap_MolAngleInfo
        .def( "addAngle", &MolAngleInfo::addAngle )
        .def( "removeAngle", wrap_removeAngle1 )
        .def( "removeAngle", wrap_removeAngle2 )
        .def( "addAngles", &wrap_addAngles_QList )
        .def( "removeAngles", &wrap_removeAngles_QList )
        .def( "addAngles", &wrap_addAngles_QSet )
        .def( "removeAngles", &wrap_removeAngles_QSet )
        .def( "nAngles", wrap_nAngles1 )
        .def( "nAngles", wrap_nAngles2 )
        .def( "nAngles", wrap_nAngles3 )
        .def( "nAngles", wrap_nAngles4 )
        .def( "nAngles", wrap_nAngles5 )
        .def( "nIntraAngles", wrap_nIntraAngles1 )
        .def( "nIntraAngles", wrap_nIntraAngles2 )
        .def( "nInterAngles", wrap_nInterAngles1 )
        .def( "nInterAngles", wrap_nInterAngles2 )
        .def( "residuesAngled", wrap_residuesAngled1 )
        .def( "residuesAngled", wrap_residuesAngled2 )
        .def( "angles", wrap_angles1 )
        .def( "angles", wrap_angles2 )
        .def( "angles", wrap_angles3 )
        .def( "angles", wrap_angles4 )
        .def( "interAngles", wrap_interAngles1 )
        .def( "interAngles", wrap_interAngles2 )
        .def( "intraAngles", wrap_intraAngles1 )
        .def( "intraAngles", wrap_intraAngles2 )
        .def( "angles", &wrap_angles_QSet )
        .def( "interAngles", &wrap_interAngles_QSet )
        .def( "intraAngles", &wrap_intraAngles_QSet )
        .def( "angles", &wrap_angles_QList )
        .def( "interAngles", &wrap_interAngles_QList )
        .def( "intraAngles", &wrap_intraAngles_QList )
    ;

}

}
