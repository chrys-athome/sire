/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "glrenderfunction.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Spier;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of GLRenderFunction
//////////

static const RegisterMetaType<GLRenderFunction> r_glrenderfunc( MAGIC_ONLY,
                                                       "Spier::GLRenderFunction" );
                                                       
/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, 
                                     const GLRenderFunction &glrenderfunc)
{
    writeHeader(ds, r_glrenderfunc, 1);
    
    ds << static_cast<const Property&>(glrenderfunc);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds,
                                     GLRenderFunction &glrenderfunc)
{
    VersionID v = readHeader(ds, r_glrenderfunc);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(glrenderfunc);
    }
    else
        throw version_error( v, "1", r_glrenderfunc, CODELOC );
        
    return ds;
}

/** Constructor */
GLRenderFunction::GLRenderFunction() : Property()
{}

/** Copy constructor */
GLRenderFunction::GLRenderFunction(const GLRenderFunction &other)
                 : Property(other)
{}

/** Destructor */
GLRenderFunction::~GLRenderFunction()
{}

/** Copy assignment operator */
GLRenderFunction& GLRenderFunction::operator=(const GLRenderFunction &other)
{
    Property::operator=(other);
    return *this;
}

/** Comparison operator */
bool GLRenderFunction::operator==(const GLRenderFunction &other) const
{
    return Property::operator==(other);
}

/** Comparison operator */
bool GLRenderFunction::operator!=(const GLRenderFunction &other) const
{
    return Property::operator!=(other);
}

Q_GLOBAL_STATIC( NullGLRenderFunction, nullGLRenderFunction );

/** Return the global null function */
const NullGLRenderFunction& GLRenderFunction::null()
{
    return *(nullGLRenderFunction());
}

//////////
////////// Implementation of NullGLRenderFunction
//////////

static const RegisterMetaType<NullGLRenderFunction> r_nullglrenderfunc;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, 
                                     const NullGLRenderFunction &nullglrenderfunc)
{
    writeHeader(ds, r_nullglrenderfunc, 1);
    
    ds << static_cast<const GLRenderFunction&>(nullglrenderfunc);
    
    return ds;
}

/** Extract from the binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds,
                                     NullGLRenderFunction &nullglrenderfunc)
{
    VersionID v = readHeader(ds, r_nullglrenderfunc);
    
    if (v == 1)
    {
        ds >> static_cast<GLRenderFunction&>(nullglrenderfunc);
    }
    else
        throw version_error( v, "1", r_nullglrenderfunc, CODELOC );
        
    return ds;
}

/** Constructor */
NullGLRenderFunction::NullGLRenderFunction()
                     : ConcreteProperty<NullGLRenderFunction,GLRenderFunction>()
{}

/** Copy constructor */
NullGLRenderFunction::NullGLRenderFunction(const NullGLRenderFunction &other)
                     : ConcreteProperty<NullGLRenderFunction,GLRenderFunction>(other)
{}

/** Destructor */
NullGLRenderFunction::~NullGLRenderFunction()
{}

/** Copy assignment operator */
NullGLRenderFunction& NullGLRenderFunction::operator=(const NullGLRenderFunction &other)
{
    GLRenderFunction::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullGLRenderFunction::operator==(const NullGLRenderFunction&) const
{
    return true;
}

/** Comparison operator */
bool NullGLRenderFunction::operator!=(const NullGLRenderFunction&) const
{
    return false;
}

const char* NullGLRenderFunction::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullGLRenderFunction>() );
}

/** The null render function does nothing */
void NullGLRenderFunction::operator()() const
{}
