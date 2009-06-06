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

#include <QMutex>
#include <QHash>

#include <boost/weak_ptr.hpp>

#include "glrendercontext.h"
#include "gldisplaylist.h"
#include "glrenderfunction.h"

namespace Spier
{
namespace detail
{

class GLRenderContextData
{
public:
    GLRenderContextData(QGLContext *r) : render_context(r)
    {}
    
    ~GLRenderContextData();
    
    /** Mutex to lock access to GL rendering */
    QMutex render_mutex;
    
    /** The render context */
    QGLContext *render_context;
    
    /** The list of functions associated with each display list */
    QHash<const GLRenderFunction*,GLuint> display_lists; 
    
    /** The list of display lists to delete */
    QList<GLuint> lists_to_delete;
};

} // end of namespace detail
} // end of namespace Spier

using namespace Spier;
using namespace Spier::detail;

////////
//////// Implementation of GLRenderLocker
////////

GLRenderLocker::GLRenderLocker()
{}

GLRenderLocker::GLRenderLocker(QMutex *mutex)
               : lkr( new QMutexLocker(mutex) )
{}

GLRenderLocker::GLRenderLocker(const GLRenderLocker &other)
               : lkr(other.lkr)
{}

GLRenderLocker::~GLRenderLocker()
{}

GLRenderLocker& GLRenderLocker::operator=(const GLRenderLocker &other)
{
    lkr = other.lkr;
    return *this;
}

void GLRenderLocker::unlock()
{
    if (lkr.get())
        lkr->unlock();
}

void GLRenderLocker::relock()
{
    if (lkr.get())
        lkr->relock();
}

////////
//////// Implementation of GLRenderContext
////////

/** Null constructor */
GLRenderContext::GLRenderContext()
{}

/** Construct for the passed context */
GLRenderContext::GLRenderContext(const boost::shared_ptr<GLRenderContextData> &ptr)
                : d(ptr)
{}

/** Copy constructor */
GLRenderContext::GLRenderContext(const GLRenderContext &other) : d(other.d)
{}

GLRenderContextData::~GLRenderContextData()
{
    QMutexLocker lkr( &render_mutex );

    //make this context current
    render_context->makeCurrent();
    
    //delete all of the display lists
    foreach (GLuint id, lists_to_delete)
    {
        glDeleteLists( id, 1 );
    }

    for (QHash<const GLRenderFunction*,GLuint>::const_iterator
                                it = display_lists.constBegin();
         it != display_lists.constEnd();
         ++it)
    {
        glDeleteLists( it.value(), 1 );
    }
    
    render_context->doneCurrent();
}

/** Destructor */
GLRenderContext::~GLRenderContext()
{}

/** Copy assignment operator */
GLRenderContext& GLRenderContext::operator=(const GLRenderContext &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool GLRenderContext::operator==(const GLRenderContext &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool GLRenderContext::operator!=(const GLRenderContext &other) const
{
    return d.get() != other.d.get();
}

Q_GLOBAL_STATIC( QMutex, renderContextMutex );

static QHash< const QGLContext*, boost::weak_ptr<GLRenderContextData> > *glreg(0);

/** Return the context for the passed render context */
GLRenderContext GLRenderContext::getContext(QGLContext *render_context)
{
    QMutexLocker lkr( renderContextMutex() );
    
    if (not glreg)
        glreg = new QHash< const QGLContext*,boost::weak_ptr<GLRenderContextData> >();
    
    boost::shared_ptr<GLRenderContextData> r = glreg->value(render_context).lock();
    
    if (r.get() == 0)
    {
        r.reset( new GLRenderContextData(render_context) );
        glreg->insert( render_context, r );
    }
    
    return GLRenderContext(r);
}

/** Delete the cached data for the passed list in all contexts */
void GLRenderContext::deleteList(const GLDisplayList &display_list)
{
    if (display_list.isEmpty())
        return;

    QMutexLocker lkr( renderContextMutex() );
    
    if (glreg == 0)
        return;
    
    for (QHash< const QGLContext*,boost::weak_ptr<GLRenderContextData> >::const_iterator
                                            it = glreg->constBegin();
         it != glreg->constEnd();
         ++it)
    {
        boost::shared_ptr<GLRenderContextData> d = it.value().lock();
        
        if (d.get() != 0)
        {
            QMutexLocker lkr2( &(d->render_mutex) );
            
            if (d->display_lists.contains( &(display_list.renderFunction()) ))
            {
                GLuint id = d->display_lists.take( &(display_list.renderFunction()) );
                d->lists_to_delete.append(id);
            }
        }
    }
}

/** Return whether or not this context is null */
bool GLRenderContext::isNull() const
{
    return d.get() == 0;
}

/** Return the underlying OpenGL render context */
const QGLContext* GLRenderContext::context() const
{
    if (d.get() != 0)
        return d->render_context;
        
    else
        return 0;
}

/** Make this context the current OpenGL render context */
void GLRenderContext::makeCurrent()
{
    if (d.get() != 0)
        d->render_context->makeCurrent();
}

/** Make no render context current */
void GLRenderContext::doneCurrent()
{
    if (d.get() != 0)
        d->render_context->doneCurrent();
}

/** Lock the OpenGL render context, and return a locker used to unlock it.
    This also makes this context the current context */
GLRenderLocker GLRenderContext::lock()
{
    if (d.get() == 0)
        return GLRenderLocker();
        
    else
    {
        GLRenderLocker locker( &(d->render_mutex) );
        
        d->render_context->makeCurrent();
        
        foreach (GLuint list_id, d->lists_to_delete)
        {
            glDeleteLists(list_id, 1);
        }
        
        d->lists_to_delete.clear();
    
        return locker;
    }
}

/** Render the passed display list in the current context - this
    context *must* be locked */
void GLRenderContext::render(const GLDisplayList &display_list)
{
    if (d.get() == 0)
        display_list.renderFunction()();

    GLuint list_id = d->display_lists.value( &(display_list.renderFunction()), 0 );
    
    if (list_id == 0)
    {
        list_id = glGenLists(1);
        
        glNewList(list_id, GL_COMPILE);
        display_list.renderFunction()();
        glEndList();
        
        d->display_lists.insert( &(display_list.renderFunction()), list_id );
    }
    
    glCallList(list_id);
}
