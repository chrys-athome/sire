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

using namespace Spier;

////////
//////// Implementation of GLBackground
////////

/** Constructor */
GLBackground::GLBackground() : QSharedData()
{}

/** Copy constructor */
GLBackground::GLBackground(const GLBackground&) : QSharedData()
{}

/** Destructor */
GLBackground::~GLBackground()
{}

////////
//////// Implementation of GradientBackground
////////

/** Constructor */
GradientBackground::GradientBackground() : GLBackground()
{}

/** Copy constructor */
GradientBackground::GradientBackground(const GradientBackground &other)
                   : GLBackground(other),
                     display_list(other.display_list)
{}

/** Destructor */
GradientBackground::~GradientBackground()
{}

/** Copy assignment operator */
GradientBackground& GradientBackground::operator=(const GradientBackground &other)
{
    display_list = other.display_list;
    
    return *this;
}

/** Comparison operator */
bool GradientBackground::operator==(const GradientBackground &other) const
{
    return display_list == other.display_list;
}

/** Comparison operator */
bool GradientBackground::operator!=(const GradientBackground &other) const
{
    return not this->operator==(other);
}

/** Render the background - this leaves the render state in the same
    state as when it started */
void GradientBackground::render(const QGLContext *render_context)
{
    if (not display_list.play(render_context))
    {
        GLDisplayListRecorder recorder( display_list, render_context );
       
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
    
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
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
    
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
}
