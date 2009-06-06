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

#include "gldisplaylist.h"
#include "glrendercontext.h"

#include "SireBase/property.h"

//#include "camera.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class GLCanvas;
}

QDataStream& operator<<(QDataStream&, const Spier::GLCanvas&);
QDataStream& operator>>(QDataStream&, Spier::GLCanvas&);

namespace Spier
{

/** This is an abstraction of an openGL canvas - this is the area within 
    which openGL drawing operations are performed. This allows for multiple 
    GLCanvases to be swapped within a single Viewer, thus allowing fast view 
    switching (e.g. to view different molecules, or to change render modes).
 
    @author Christopher Woods
*/
class SPIER_EXPORT GLCanvas
            : public SireBase::ConcreteProperty<GLCanvas,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const GLCanvas&);
friend QDataStream& ::operator>>(QDataStream&, GLCanvas&);

public:
    GLCanvas();
    GLCanvas(const GLCanvas &other);

    virtual ~GLCanvas();

    GLCanvas& operator=(const GLCanvas &other);
    
    bool operator==(const GLCanvas &other) const;
    bool operator!=(const GLCanvas &other) const;

    static const char* typeName();
    
    virtual GLCanvas* clone() const
    {
        return new GLCanvas(*this);
    }

    void render(QGLContext *render_context, int w, int h);
    void renderSelector(QGLContext *render_context, int w, int h);

protected:
    void resize(int w, int h);

    void clearError();
    void checkError(const QString &codeloc);

    /** The current openGL rendering context */
    GLRenderContext render_context;
    
    /** The display list containing the commands to set up 
        the scene for normal rendering */
    GLDisplayList render_state;

    /** The display list containing the commands to set up
        the scene for selector rendering */
    GLDisplayList selector_state;

    /** The background of this canvas */
    GLDisplayList bg;
    
    /** The camera for this scene */
    //SireBase::SharedPolyPointer<Camera> cam;
    
    /** The current height and width of this canvas */
    qint32 current_w, current_h;
};

typedef SireBase::PropPtr<GLCanvas> GLCanvasPtr;

}

Q_DECLARE_METATYPE( Spier::GLCanvas )

SIRE_EXPOSE_CLASS( Spier::GLCanvas )

SIRE_EXPOSE_PROPERTY( Spier::GLCanvasPtr, Spier::GLCanvas )

SIRE_END_HEADER

#endif
