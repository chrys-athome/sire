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

#include <QUuid>

#include "gldisplaylist.h"
#include "glrendercontext.h"

#include "SireBase/property.h"
#include "SireBase/majorminorversion.h"

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

    const QUuid& UID() const;
    quint64 version() const;

    void render(GLRenderContext &render_context) const;
    void renderSelector(GLRenderContext &render_context) const;

    static const GLCanvas& null();

protected:
    void clearError() const;
    void checkError(const QString &codeloc) const;
    
    void incrementVersion();
    
    /** The display list containing the commands to set up 
        the scene for normal rendering */
    GLDisplayList render_state;

    /** The display list containing the commands to set up
        the scene for selector rendering */
    GLDisplayList selector_state;

    /** The background of this canvas */
    GLDisplayList bg;

    /** The UID for this canvas */
    QUuid uid;
    
    /** The current version number of this canvas - this is 
        used to identify cached rendered images */
    SireBase::MajorMinorVersion version_number;
};

typedef SireBase::PropPtr<GLCanvas> GLCanvasPtr;

}

Q_DECLARE_METATYPE( Spier::GLCanvas )

SIRE_EXPOSE_CLASS( Spier::GLCanvas )

SIRE_EXPOSE_PROPERTY( Spier::GLCanvasPtr, Spier::GLCanvas )

SIRE_END_HEADER

#endif
