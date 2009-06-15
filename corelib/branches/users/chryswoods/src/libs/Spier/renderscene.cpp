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

#include <QPainter>

#include "renderscene.h"

using namespace Spier;
using namespace Spier::detail;

/////////
///////// Implementation of CachedScene
/////////

CachedScene::CachedScene()
{}

CachedScene::~CachedScene()
{}

/////////
///////// Implementation of CachedScene
/////////

/** Constructor */
RenderScene::RenderScene(QObject *parent) : QObject(parent)
{}

/** Destructor */
RenderScene::~RenderScene()
{}

/** Return the canvas for the scene */
const GLCanvas& RenderScene::canvas() const
{
    return glcanvas;
}

/** Return the camera used to view the scene */
const Camera& RenderScene::camera() const
{
    return cam;
}

/** Clear the render cache */
void RenderScene::clearRenderCache()
{
    last_render.reset();
}

/** Clear the selector cache */
void RenderScene::clearSelectorCache()
{
    last_selector.reset();
}

/** Clear all caches */
void RenderScene::clearCache()
{
    clearRenderCache();
    clearSelectorCache();
}

/** Return the last cached render image - this will be null
    if there is no cached render image */
QImage RenderScene::renderImage() const
{
    if (last_render.get())
        return last_render->screen_grab;
    else
        return QImage();
}

/** Return the last cached selector image - this will be null
    if there is no cached selector image */
QImage RenderScene::selectorImage() const
{
    if (last_selector.get())
        return last_selector->screen_grab;
    else
        return QImage();
}

/** Return the time it took to render the last scene - this will
    be 0 if it was very quick or we don't know */
int RenderScene::renderTime() const
{
    if (last_render.get())
        return last_render->render_time;
    else
        return 0;
}

/** Return the time it took to render the last selector scene - this will
    be 0 if it was very quick or we don't know */
int RenderScene::renderSelectorTime() const
{
    if (last_selector.get())
        return last_selector->render_time;
    else
        return 0;
}

/** Return the time it took to restore the last scene from the cache.
    This will be 0 if it was very quick or we don't know */
int RenderScene::restoreFromCacheTime() const
{
    if (last_render.get())
        return last_render->restore_time;
    else
        return 0;
}

/** Set the canvas to be rendered */
void RenderScene::setCanvas(const GLCanvas &canvas)
{
    glcanvas = canvas;
}

/** Set the camera with which to view the scene */
void RenderScene::setCamera(const Camera &camera)
{
    cam = camera;
}

/** Internal function used to render the passed image
    to the current context - this returns the amount of time
    it took to render the image */
int RenderScene::render(GLRenderContext &render_context, QImage &image)
{
    QTime t;
    t.start();
        
    render_context.pushState();
    {
        QPainter painter;
        painter.begin( &(render_context.widget()) );

        if (image.size() != render_context.size())
        {
            //we need to scale the image
            image = image.scaled(render_context.size(), Qt::IgnoreAspectRatio, 
                                 Qt::SmoothTransformation);
        }

        painter.drawImage(0, 0, image);
    }
    render_context.popState();

    return t.elapsed();
}

/** Render the scene to the passed context */
void RenderScene::render(GLRenderContext &render_context)
{
    if (last_render.get())
    {
        //we have a cached scene
        if ( last_render->canvas_uid == glcanvas->UID() and
             last_render->canvas_version == glcanvas->version() and
             last_render->camera == cam )
        {
            //nothing in the scene has changed
            int ms = this->render(render_context, last_render->screen_grab);

            //save the amount of time it took to restore the scene
            last_render->restore_time = ms;

            return;
        }
    }
    
    //ok, we don't have a previously rendered scene (or it is out of date)
    int ms = this->forceRender( render_context );
    
    if (ms > 50)
    {
        qDebug() << "Rendering was slow, so we'll cache the result" << ms;

        last_render.reset( new CachedScene() );
        
        glReadBuffer( GL_BACK );
    
        last_render->screen_grab = render_context.widget().grabFrameBuffer();
        last_render->canvas_uid = glcanvas->UID();
        last_render->canvas_version = glcanvas->version();
        last_render->camera = cam;
        
        last_render->render_time = ms;
        last_render->restore_time = 0;
    }
}

/** Render the selector scene using the passed context */
void RenderScene::renderSelector(GLRenderContext &render_context)
{
    if (last_selector.get())
    {
        //we have a cached scene
        if ( last_selector->canvas_uid == glcanvas->UID() and
             last_selector->canvas_version == glcanvas->version() and
             last_selector->camera == cam )
        {
            //nothing in the scene has changed - nothing to do
            return;
        }
    }

    //something has changed - we need to rerender the scene
    int ms = this->forceRenderSelector( render_context );
    
    last_selector.reset( new CachedScene() );
        
    glReadBuffer( GL_BACK );
    
    last_selector->screen_grab = render_context.widget().grabFrameBuffer();
    last_selector->canvas_uid = glcanvas->UID();
    last_selector->canvas_version = glcanvas->version();
    last_selector->camera = cam;
        
    last_selector->render_time = ms;
    last_selector->restore_time = 0;
}

/** Call this function to animate the scene */
void RenderScene::animate()
{
    qDebug() << animation_timer.elapsed() 
             << "ms since last animation update...";
             
    animation_timer.start();
}

/** Internal function used to actually render the scene, returning
    the amount of time it took to render the scene */
int RenderScene::forceRender(GLRenderContext &render_context)
{
    QTime t;
    t.start();

    //look through the camera
    render_context.changeView(cam);
    
    //now render the scene
    glcanvas.read().render( render_context );
    
    return t.elapsed();
}

/** Internal function used to actually render the selector scene, 
    returning the amount of time it took to render the scene */
int RenderScene::forceRenderSelector(GLRenderContext &render_context)
{
    QTime t;
    t.start();
    
    //look through the camera
    render_context.changeView(cam);
    
    //now render the scene
    glcanvas.read().renderSelector( render_context );
    
    return t.elapsed();
}
