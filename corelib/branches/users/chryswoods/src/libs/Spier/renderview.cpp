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

#include "renderview.h"

using namespace Spier;

/** Constructor */
RenderView::RenderView(QWidget *parent, QGLWidget *share_widget)
           : QGLWidget(parent, share_widget)
{}

/** Destructor */
RenderView::~RenderView()
{}

/** Initialise the openGL view */
void RenderView::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

/** Resize the window */
void RenderView::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

/** Paint the scene */
void RenderView::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
    
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    
//        pushAttrib();
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_FOG);
        
//        if (GLEW_ARB_multisample)
//            glDisable(GL_MULTISAMPLE_ARB);

        glBegin(GL_QUADS);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(0.0,0.0);
        glColor3f(0.0,0.0,0.5);
        glVertex2f(1.0,0.0);
        glColor3f(0.9,0.9,0.9);
        glVertex2f(1.0,1.0);
        glVertex2f(0.0,1.0);
        glEnd();
    
//        popAttrib();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
}
