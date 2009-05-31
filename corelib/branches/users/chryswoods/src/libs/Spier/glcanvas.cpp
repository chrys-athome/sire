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

#include "glcanvas.h"

#include <QDebug>

using namespace Spier;

/** Construct the GLCanvas */
GLCanvas::GLCanvas() : QSharedData(), current_w(0), current_h(0)
{
    bg = GradientBackground();
}

/** Copy constructor */
GLCanvas::GLCanvas(const GLCanvas &other) 
         : QSharedData(),
           bg(other.bg), init_state(other.init_state),
           current_w(other.current_w), current_h(other.current_h)
{}

/** Destructor */
GLCanvas::~GLCanvas()
{}

/** Encode the 32bit index into a 24bit colour. Obviously this will only
    encode the index correctly if it is less than 2^24 */
/*void GLCanvas::encodeIndex(quint32 index)
{
    //add 1 to the index, as color 0 is no selection
    index += 1000;
    
    //this works for 24bit colour - can do 16bit by multiplying index by 256 here,
    //then dividing by 256 in the decode
    if (index >= 16777216)
        throw SireError::invalid_arg(QObject::tr(
            "Index is too large for 24bit colour mode! (%1)").arg(index));
            
    uchar r = index % 256;
    uchar g = (index/256) % 256;
    uchar b = (index/65536) % 256;
            
    glColor3ub( r, g, b );
}*/

/** Return the index encoded by the colour with the passed components,
    or -1 if no index is encoded. */
/*qint32 GLCanvas::decodeColorToIndex(uchar red, uchar green, uchar blue) const
{
    quint32 r = red;
    quint32 g = green;
    quint32 b = blue;

    qint32 idx = r + (g * 256) + (b * 65536) - 1000;

    return idx;
}*/

/** Save the current render state */
/*void GLCanvas::saveRenderState()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    pushAttrib();
}*/

/** Restore the render state (which must have been previously saved!) */
/*void GLCanvas::restoreRenderState()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    popAttrib();
}*/

/** Fill the back buffer with each ViewObj rendered as a single, different colour */
/*void GLCanvas::fillBackBuffer()
{
    //we render the scene to the back buffer, painting each object with 
    //a different colour
    
    cam->look(true);
            
    glDrawBuffer(GL_BACK);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    glClearColor(0.0,0.0,0.0,0.0);
    this->clear();
    
    //render all objects to find the selected object...
    int nobjs = dispobjs.count();
    for (int i=0; i<nobjs; ++i)
    {
        this->encodeIndex(i);
    }
    
    back_buffer_ready = true;
}*/

/** Find the object underneath the mouse. This assumes that the back buffer is already 
    filled with each ViewObj rendered using its encoded index value. */
/*int GLCanvas::getObjectUnderMouse()
{
    if (not back_buffer_ready)
    {
        this->fillBackBuffer();
    }

    //we are in selection state whenever the back buffer is ready 
    //(we thus don't need to change the state again)

    QPoint mousepos = current_interpreter.pos();
    qint32 selected = decodeIndex(mousepos.x(), mousepos.y());
    
    if (selected >= 0 and selected < dispobjs.count())
    {
        //the mouse is over an object - we now need to select within the
        //object. The screen is cleared, and now only the object is drawn, with 
        //each separate part of the object drawn with a different colour.
        //(perhaps I could restrict rendering to only a small volume around
        //the current mouse position)
        
        this->clear();
    
        int retval = dispobjs.at(selected)->select(mousepos.x(), mousepos.y());

        //the back buffer now no longer contains the whole-scene selection info
        back_buffer_ready = false;
        
        return retval;
    }
    else
        return -1;   
}*/

/** Convert the colour the pixel at window coordinates x,y into an index */
/*qint32 GLCanvas::decodeIndex(int x, int y) const
{
    unsigned char rgb[3];
    
    glReadBuffer(GL_BACK);
    glReadPixels(x, parent()->height()-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);

    return decodeColorToIndex(rgb[0], rgb[1], rgb[2]);
}*/

/** Clear the current error */
void GLCanvas::clearError()
{
    glGetError();
}

/** Check the status of the openGL error flag */
void GLCanvas::checkError(const QString &codeloc)
{
    GLenum err = glGetError();
    
    switch(err)
    {
        case GL_NO_ERROR:
            return;
        case GL_INVALID_ENUM:
            qDebug() << QObject::tr("%1: GL_INVALID_ENUM").arg(codeloc);
            return;
        case GL_INVALID_OPERATION:
            qDebug() << QObject::tr("%1: GL_INVALID_OPERATION").arg(codeloc);
            return;
        case GL_STACK_OVERFLOW:
            qDebug() << QObject::tr("%1: GL_STACK_OVERFLOW").arg(codeloc);
            return;
        case GL_STACK_UNDERFLOW:
            qDebug() << QObject::tr("%1: GL_STACK_UNDERFLOW").arg(codeloc);
            return;
        case GL_OUT_OF_MEMORY:
            qDebug() << QObject::tr("%1: GL_OUT_OF_MEMORY").arg(codeloc);
            return;
    }
}

/** Resize the display */
void GLCanvas::resize(int w, int h)
{
    //cam->setSize(QSize(w,h));
    
    current_w = w;
    current_h = h;
}

/** Function called to initialise the scene */
void GLCanvas::initialise(const QGLContext *render_context)
{
    if (not init_state.play(render_context))
    {
        GLDisplayListRecorder recorder(init_state, render_context);
                
        glClearColor(0.9,0.9,0.9,1.0);
           
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
    
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP2);
        glFogf(GL_FOG_START,0.0);
        glFogf(GL_FOG_END,300.0);
        glFogf(GL_FOG_DENSITY,0.01);
    
        GLfloat fogcolor[] = { 0.5f, 0.5f, 0.5f, 0.0f };
        glFogfv(GL_FOG_COLOR, fogcolor);
    
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
    
        glEnable(GL_TEXTURE_2D);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
}

/** Paint the canvas (render the scene) */
void GLCanvas::render(const QGLContext *render_context, int w, int h)
{
    if (render_context == 0)
        return;
        
    if (not render_context->isValid())
        return;

    if (w != current_w or h != current_h)
        this->resize(w, h);

    //now switch to the scene rendering state
    this->initialise(render_context);
        
    //paint the background
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    bg->render(render_context);
    
    glPopAttrib();
        
    //check for any openGL errors
    checkError(CODELOC);

    //send the rendered scene to the front buffer
    render_context->swapBuffers();

    //make sure that all commands have gone to the renderer
    glFlush();
}
