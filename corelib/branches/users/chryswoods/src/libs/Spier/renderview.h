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

#ifndef SPIER_RENDERVIEW_H
#define SPIER_RENDERVIEW_H

#include <QGLWidget>
#include <QList>
#include <QTime>

#include "renderscene.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class RenderViewCommand;
class CameraCommand;

/** This class provides a view in which a 3D scene may be rendered

    @author Christopher Woods
*/
class SPIER_EXPORT RenderView : public QGLWidget
{
    Q_OBJECT

    //friend classes used to get access to the render view to
    //allow commands (as editing functions can't be public
    //in case they are used accidentally, thereby corrupting
    //the undo stack)
    
    friend class CameraCommand;   // used to get and set the camera
    
public:
    RenderView(QWidget *parent=0, QGLWidget *share_widget=0);
    ~RenderView();

public slots:
    void execute(const RenderViewCommand &command);

    void undo();
    void redo();

    void triggerUpdate();

signals:
    void undoAvailable();
    void undoUnavailable();
    
    void redoAvailable();
    void redoUnavailable();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void drawCorners(QPainter &painter);
    void drawUI(QPainter &painter);

    const Camera& getCamera() const;        // called by CameraCommand
    void setCamera(const Camera &camera);   // called by CameraCommand

protected slots:
    void animate();

private:
    /** The current openGL rendering context */
    GLRenderContext render_context;

    /** All of the scenes that can be rendered on this widget */
    QList<RenderScene*> render_scenes;

    /** An animation timer */
    QTime animation_timer;

    /** The current scene to render on this widget */
    int scene_to_render;

    /** The location of the banner */
    float yval;
    
    /** Does the scene need to be updated? */
    bool needs_rerendering;
};

}

SIRE_END_HEADER

#endif
