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

#ifndef SPIER_GLCANVAS_H
#define SPIER_GLCANVAS_H

#include <QSharedData>

#include "SireBase/sharedpolypointer.hpp"

#include "glbackground.h"
#include "gldisplaylist.h"
//#include "camera.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/** This is an abstraction of an openGL canvas - this is the area within 
    which openGL drawing operations are performed. This allows for multiple 
    GLCanvases to be swapped within a single Viewer, thus allowing fast view 
    switching (e.g. to view different molecules, or to change render modes).
 
    @author Christopher Woods
*/
class SPIER_EXPORT GLCanvas : public QSharedData
{
public:
    GLCanvas();
    GLCanvas(const GLCanvas &other);

    ~GLCanvas();

    GLCanvas& operator=(const GLCanvas &other);
    
    bool operator==(const GLCanvas &other) const;
    bool operator!=(const GLCanvas &other) const;

    void render(const QGLContext *render_context, int w, int h);

protected:
    void resize(int w, int h);
    
    void initialise(const QGLContext *render_context);

    void clearError();
    void checkError(const QString &codeloc);

    /** The background of this canvas */
    SireBase::SharedPolyPointer<GLBackground> bg;
    
    /** The camera for this scene */
    //SireBase::SharedPolyPointer<Camera> cam;
    
    /** The display list containing everything needed to initialise the state */
    GLDisplayList init_state;
    
    /** The current height and width of this canvas */
    int current_w, current_h;
};

}

SIRE_END_HEADER

#endif
