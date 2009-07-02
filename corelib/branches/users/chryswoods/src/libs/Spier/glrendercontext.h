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

#ifndef SPIER_GLRENDERCONTEXT_H
#define SPIER_GLRENDERCONTEXT_H

#include <QGLContext>
#include <QMutexLocker>

#include "glrenderfunction.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace Spier
{

class Camera;
class GLViewFrustrum;
class GLDisplayList;
class GLMesh;

namespace detail
{
class GLRenderContextData;
}

/** This is a simple class that holds the mutex lock */
class SPIER_EXPORT GLRenderLocker
{
public:
    GLRenderLocker();
    GLRenderLocker(QMutex *mutex);
    
    GLRenderLocker(const GLRenderLocker &other);
    
    ~GLRenderLocker();
    
    GLRenderLocker& operator=(const GLRenderLocker &other);
    
    void unlock();
    void relock();

private:
    boost::shared_ptr<QMutexLocker> lkr;
};

/** This class holds all of the global information for an 
    OpenGL render context. This includes all of the display
    lists and textures that have been created for this context.
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLRenderContext
{

public:
    static GLRenderContext getContext(QGLWidget &render_widget);

    GLRenderContext();
    GLRenderContext(const GLRenderContext &other);

    ~GLRenderContext();

    GLRenderContext& operator=(const GLRenderContext &other);
    
    bool operator==(const GLRenderContext &other) const;
    bool operator!=(const GLRenderContext &other) const;

    static const char* typeName()
    {
        return "Spier::GLRenderContext";
    }

    const char* what() const
    {
        return GLRenderContext::typeName();
    }

    QGLWidget& widget();
    const QGLContext* context() const;
    
    QSize size() const;
    
    const GLViewFrustrum& frustrum() const;
    
    void makeCurrent();
    void doneCurrent();
    
    GLRenderLocker lock();
    
    void changeView(const Camera &camera);
    
    void render(const GLDisplayList &display_list);
    void render(const GLMesh &mesh);

    void pushState();
    void popState();

    bool isNull() const;

    void deleteAll();

    static void deleteList(const GLDisplayList &display_list);
    
private:
    GLRenderContext(const boost::shared_ptr<detail::GLRenderContextData> &ptr);
    
    /** Private implementation */
    boost::shared_ptr<detail::GLRenderContextData> d;
};

}

SIRE_END_HEADER

#endif
