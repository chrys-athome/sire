#include <Python.h>
#include <boost/python.hpp>

#include "Base/convertproperty.hpp"
#include "Spier_properties.h"

#include "SireStream/datastream.h"
#include "command.h"
#include "renderview.h"
#include "command.h"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"
#include "glrenderfunction.h"
#include "glrenderfunction.h"
#include "SireStream/datastream.h"
#include "glmesh.h"
#include "glrendercontext.h"
#include "glmesh.h"
#include "SireStream/datastream.h"
#include "SireUnits/units.h"
#include "camera.h"
#include <QGLWidget>
#include "camera.h"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"
#include "glbackground.h"
#include "glcanvas.h"
#include "glinitstate.h"
#include "glsphere.h"
#include <QDebug>
#include <QGLWidget>
#include "glcanvas.h"
void register_Spier_properties()
{
    register_property_container< Spier::CommandPtr, Spier::Command >();
    register_property_container< Spier::GLRenderFuncPtr, Spier::GLRenderFunction >();
    register_property_container< Spier::GLMeshPtr, Spier::GLMesh >();
    register_property_container< Spier::CameraPtr, Spier::Camera >();
    register_property_container< Spier::GLCanvasPtr, Spier::GLCanvas >();
}
