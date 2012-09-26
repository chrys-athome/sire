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

#ifndef SPIER_RENDERSCENE_H
#define SPIER_RENDERSCENE_H

#include <QObject>
#include <QImage>
#include <QUuid>
#include <QTime>

#include "glcanvas.h"
#include "camera.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

class QPainter;

namespace Spier
{

namespace detail
{
/** This small class is used to hold information about a cached image */
class CachedScene
{
public:
    CachedScene();
    ~CachedScene();
    
    /** This is a rendering from the screen */
    QImage screen_grab;
    
    /** The UID of the canvas from whence this came */
    QUuid canvas_uid;
    
    /** The version of the canvas from whence this came */
    quint64 canvas_version;
    
    /** The camera which was used to view the scene */
    CameraPtr camera;
    
    /** How long did it take to render this scene? */
    int render_time;
    
    /** How long does it take to restore this render to the screen? */
    int restore_time;
};

} // end of namespace detail

/** This class holds a single scene that is being rendered within
    a RenderView. A RenderView can hold multiple scenes, thereby
    allowing for rapid switching between scenes (e.g. via tabs)
    
    All of the RenderScenes within a RenderView shared the same
    GLRenderContext, and so share display lists, textures etc.
    
    @author Christopher Woods
*/
class SPIER_EXPORT RenderScene : public QObject
{
    Q_OBJECT
    
public:
    RenderScene(QObject *parent=0);
    
    ~RenderScene();
    
    const GLCanvas& canvas() const;
    const Camera& camera() const;
    
    void clearRenderCache();
    void clearSelectorCache();
    void clearCache();

    QImage renderImage() const;
    QImage selectorImage() const;
    
    int renderTime() const;
    int renderSelectorTime() const;
    int restoreFromCacheTime() const;
    
    void setCanvas(const GLCanvas &glcanvas);
    void setCamera(const Camera &camera);
    
    void render(GLRenderContext &render_context);
    void renderSelector(GLRenderContext &render_context);
    
public slots:
    void animate();
    
private:
    int forceRender(GLRenderContext &render_context);
    int forceRenderSelector(GLRenderContext &render_context);

    int render(GLRenderContext &render_context, QImage &image);

    /** The canvas containing everything being rendered */
    GLCanvasPtr glcanvas;
    
    /** The camera through which we will view the scene in the canvas */
    CameraPtr cam;
    
    /** A timer used to time how long we go between animation updates */
    QTime animation_timer;
    
    /** Here is a cache of the last render (only populated if rendering
        the scene takes a long time) */
    boost::shared_ptr<detail::CachedScene> last_render;
    
    /** This is the last selector scene that was rendered */
    boost::shared_ptr<detail::CachedScene> last_selector;
};

}

SIRE_END_HEADER

#endif
