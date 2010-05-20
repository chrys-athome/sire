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

#include <QGLWidget>

#include "glinitstate.h"

#include "SireStream/datastream.h"

using namespace Spier;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of GLInitState
/////////

static const RegisterMetaType<GLInitState> r_glinit( MAGIC_ONLY,
                                                     GLInitState::typeName() );

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GLInitState &glinit)
{
    writeHeader(ds, r_glinit, 1);
    
    ds << static_cast<const GLRenderFunction&>(glinit);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, GLInitState &glinit)
{
    VersionID v = readHeader(ds, r_glinit);
    
    if (v == 1)
    {
        ds >> static_cast<GLRenderFunction&>(glinit);
    }
    else
        throw version_error( v, "1", r_glinit, CODELOC );
        
    return ds;
}

/** Constructor */
GLInitState::GLInitState() : GLRenderFunction()
{}

/** Copy constructor */
GLInitState::GLInitState(const GLInitState &other) : GLRenderFunction(other)
{}

/** Destructor */
GLInitState::~GLInitState()
{}

/////////
///////// Implementation of GLInitRenderState
/////////

static const RegisterMetaType<GLInitRenderState> r_glinitrender;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GLInitRenderState &glinit)
{
    writeHeader(ds, r_glinitrender, 1);
    
    ds << static_cast<const GLInitState&>(glinit);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, GLInitRenderState &glinit)
{
    VersionID v = readHeader(ds, r_glinitrender);
    
    if (v == 1)
    {
        ds >> static_cast<GLInitState&>(glinit);
    }
    else
        throw version_error( v, "1", r_glinitrender, CODELOC );
        
    return ds;
}

/** Constructor */
GLInitRenderState::GLInitRenderState()
                  : ConcreteProperty<GLInitRenderState,GLInitState>()
{}

/** Copy constructor */
GLInitRenderState::GLInitRenderState(const GLInitRenderState &other)
                  : ConcreteProperty<GLInitRenderState,GLInitState>(other)
{}

/** Destructor */
GLInitRenderState::~GLInitRenderState()
{}

/** Copy assignment operator */
GLInitRenderState& GLInitRenderState::operator=(const GLInitRenderState &other)
{
    GLInitState::operator=(other);
    return *this;
}

/** Comparison operator */
bool GLInitRenderState::operator==(const GLInitRenderState &other) const
{
    return GLInitState::operator==(other);
}

/** Comparison operator */
bool GLInitRenderState::operator!=(const GLInitRenderState &other) const
{
    return GLInitState::operator!=(other);
}

const char* GLInitRenderState::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GLInitRenderState>() );
}

/** Initialise the scene so it can render things normally */
void GLInitRenderState::operator()() const
{
    glClearColor(1.0,1.0,1.0,1.0);
       
    glEnable(GL_DEPTH_TEST);
       
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  
    glShadeModel(GL_SMOOTH);
  
    GLfloat globalamb[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalamb);

    GLfloat lightpos[] = { 150.0f, 150.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    /*glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_START,0.0);
    glFogf(GL_FOG_END,300.0);
    glFogf(GL_FOG_DENSITY,0.01);

    GLfloat fogcolor[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    glFogfv(GL_FOG_COLOR, fogcolor);*/

    //glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/////////
///////// Implementation of GLInitSelectorState
/////////

static const RegisterMetaType<GLInitSelectorState> r_glinitselector;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GLInitSelectorState &glinit)
{
    writeHeader(ds, r_glinitselector, 1);
    
    ds << static_cast<const GLInitState&>(glinit);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, GLInitSelectorState &glinit)
{
    VersionID v = readHeader(ds, r_glinitselector);
    
    if (v == 1)
    {
        ds >> static_cast<GLInitState&>(glinit);
    }
    else
        throw version_error( v, "1", r_glinitselector, CODELOC );
        
    return ds;
}

/** Constructor */
GLInitSelectorState::GLInitSelectorState()
                    : ConcreteProperty<GLInitSelectorState,GLInitState>()
{}

/** Copy constructor */
GLInitSelectorState::GLInitSelectorState(const GLInitSelectorState &other)
                    : ConcreteProperty<GLInitSelectorState,GLInitState>(other)
{}

/** Destructor */
GLInitSelectorState::~GLInitSelectorState()
{}

/** Copy assignment operator */
GLInitSelectorState& GLInitSelectorState::operator=(const GLInitSelectorState &other)
{
    GLInitState::operator=(other);
    return *this;
}

/** Comparison operator */
bool GLInitSelectorState::operator==(const GLInitSelectorState &other) const
{
    return GLInitState::operator==(other);
}

/** Comparison operator */
bool GLInitSelectorState::operator!=(const GLInitSelectorState &other) const
{
    return GLInitState::operator!=(other);
}

const char* GLInitSelectorState::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GLInitSelectorState>() );
}

/** Initialise the scene so it can render things in selector mode */
void GLInitSelectorState::operator()() const
{}
