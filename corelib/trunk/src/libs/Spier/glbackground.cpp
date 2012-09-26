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

#include "glbackground.h"

#include "SireStream/datastream.h"

using namespace Spier;
using namespace SireBase;
using namespace SireStream;

////////
//////// Implementation of GLBackground
////////

static const RegisterMetaType<GLBackground> r_glbg( MAGIC_ONLY, "Spier::GLBackground" );

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GLBackground &glbg)
{
    writeHeader(ds, r_glbg, 1);
    
    ds << static_cast<const GLRenderFunction&>(glbg);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, GLBackground &glbg)
{
    VersionID v = readHeader(ds, r_glbg);
    
    if (v == 1)
    {
        ds >> static_cast<GLRenderFunction&>(glbg);
    }
    else
        throw version_error( v, "1", r_glbg, CODELOC );
        
    return ds;
}

/** Constructor */
GLBackground::GLBackground() : GLRenderFunction()
{}

/** Copy constructor */
GLBackground::GLBackground(const GLBackground &other) 
             : GLRenderFunction(other)
{}

/** Destructor */
GLBackground::~GLBackground()
{}

////////
//////// Implementation of GradientBackground
////////

static const RegisterMetaType<GradientBackground> r_gradbg;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GradientBackground &gradbg)
{
    writeHeader(ds, r_gradbg, 1);
    
    ds << static_cast<const GLBackground&>(gradbg);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, GradientBackground &gradbg)
{
    VersionID v = readHeader(ds, r_gradbg);
    
    if (v == 1)
    {
        ds >> static_cast<GLBackground&>(gradbg);
    }
    else
        throw version_error(v, "1", r_gradbg, CODELOC);
        
    return ds;
}

/** Constructor */
GradientBackground::GradientBackground() 
                   : ConcreteProperty<GradientBackground,GLBackground>()
{}

/** Copy constructor */
GradientBackground::GradientBackground(const GradientBackground &other)
                   : ConcreteProperty<GradientBackground,GLBackground>(other)
{}

/** Destructor */
GradientBackground::~GradientBackground()
{}

/** Copy assignment operator */
GradientBackground& GradientBackground::operator=(const GradientBackground &other)
{
    GLBackground::operator=(other);
    return *this;
}

/** Comparison operator */
bool GradientBackground::operator==(const GradientBackground&) const
{
    return true;
}

/** Comparison operator */
bool GradientBackground::operator!=(const GradientBackground&) const
{
    return false;
}

/** Render the background - this leaves the render state in the same
    state as when it started */
void GradientBackground::operator()() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
     
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
        
    glBegin(GL_QUADS);
    glColor3f(0.0,0.0,0.0);
    glVertex2f(0.0,0.0);
    glColor3f(0.0,0.0,0.5);
    glVertex2f(1.0,0.0);
    glColor3f(0.9,0.9,0.9);
    glVertex2f(1.0,1.0);
    glVertex2f(0.0,1.0);
    glEnd();
}

const char* GradientBackground::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GradientBackground>() );
}
