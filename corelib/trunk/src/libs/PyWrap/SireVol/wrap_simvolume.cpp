
/**
  * This file contains the boost::python wrapping of SimVolume and PairMatrix
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireVol/simvolume.h"
#include "SireVol/cartesian.h"
#include "SireVol/periodicbox.h"

#include "SireMol/cutgroup.h"

using namespace boost::python;
using namespace SireMol;

namespace SireVol
{

//first create a shadow class to wrap the virtual base class
struct SimVolume_PyWrap : SimVolume
{
    //create a shadow class to wrap up the virtual base 'SimVolume'
    SimVolume_PyWrap(PyObject* self_) : SimVolume(), self(self_)
    {}

    //Wrap up the virtual functions...
    SimVolume* clone(QObject *parent=0) const
    {
        return call_method<SimVolume*>(self,"clone");
    }
    
    double calcDist(const CutGroup &group1, const CutGroup &group2,
                    DistMatrix &distmat) const
    {
        return call_method<double>(self,"calcDist");
    }
    
    double calcDist2(const CutGroup &group1, const CutGroup &group2,
                     DistMatrix &distmat) const
    {
        return call_method<double>(self,"calcDist2");
    }
    
    double calcInvDist(const CutGroup &group1, const CutGroup &group2,
                       DistMatrix &distmat) const
    {
        return call_method<double>(self,"calcInvDist");
    }
    
    double calcInvDist2(const CutGroup &group1, const CutGroup &group2,
                        DistMatrix &distmat) const
    {
        return call_method<double>(self,"calcInvDist2");
    }

    bool beyond(const double &dist, const CutGroup &group0, 
                const CutGroup &group2) const
    {
        return call_method<bool>(self,"beyond");
    }

    //pointer to the self object for this class instance
    //(Python's 'this' pointer)
    PyObject* self;
};

void export_SimVolume()
{
    /**
      * Wrapping of SimVolume. This will be wrapped so that it can
      * be overloaded in Python (though boy would that be slow!)
      */
      
    class_<SimVolume, SimVolume_PyWrap, boost::noncopyable>( "SimVolume", no_init )
                .def("calcDist", &SimVolume::calcDist)
                .def("calcDist2", &SimVolume::calcDist2)
                .def("calcInvDist", &SimVolume::calcInvDist)
                .def("calcInvDist2", &SimVolume::calcInvDist2)
                .def("beyond", &SimVolume::beyond)
    ;
    
    class_<Cartesian, bases<SimVolume> >("Cartesian", init<>())
    ;

    class_<PeriodicBox, bases<SimVolume> >("PeriodicBox", init<>())
               .def(init<const Vector&, const Vector&>())
               .def("setDimension", &PeriodicBox::setDimension)
               .def("minCoords", &PeriodicBox::minCoords, 
                        return_value_policy<copy_const_reference>())
               .def("maxCoords", &PeriodicBox::maxCoords, 
                        return_value_policy<copy_const_reference>())
               .def("center", &PeriodicBox::center)
    ;
}

}
