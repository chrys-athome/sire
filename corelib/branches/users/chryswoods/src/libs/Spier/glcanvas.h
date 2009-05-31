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

#include "renderview.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/** This is an abstraction of an openGL canvas - this is the area within 
    which openGL drawing operations are performed. This allows for multiple 
    GLCanvases to be swapped within a single Viewer, thus allowing fast view 
    switching (e.g. to view different molecules, or to change render modes).
 
    @author Christopher Woods
*/
class SPIER_EXPORT GLCanvas : public QObject
{

    Q_OBJECT

public:
    GLCanvas(RenderView *parent);

    virtual ~GLCanvas();

    void pushAttrib();
    void popAttrib();

    void encodeIndex(quint32 index);
    qint32 decodeIndex(int x, int y) const;

    void reset();

    void clear();
    void updateGL();

    bool isRepainting() const;

    const QGLContext& context() const;
    QGLFormat format() const;
    
    const Camera& camera() const;
    Camera& camera();

    QPainter& painter();

    void setMoveMode(bool on);
    bool moveMode() const;
    
    RenderView* parent();
    const RenderView* parent() const;

public slots:
    
    void setTranslationMode();
    void setRotationMode();

protected:

    void initialize();
    void paintGL();
    void resizeGL(int w, int h);
    void setCursor(Qt::CursorShape cursor);

    void paintBackground();
    void fillBackBuffer();

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseClickEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    
    void setSceneState();
    
    void saveRenderState();
    void restoreRenderState();

    void checkError(QString codeloc);
    void clearError();

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    void paintEvent(QPaintEvent *e);

    qint32 decodeColorToIndex(uchar red, uchar green, uchar blue) const;

    /** The parent viewer of this canvas - this cannot change
        during the lifetime of this canvas */
    RenderView *prnt;

    /** The camera used to view the scene */
    Camera *cam;

    /** The current input interpreter */
    SireBase::SharedPolyPointer<InputInterpreter> current_interpreter;

    /** Whether or not the camera view has changed since the last render */
    bool needrepaint;

    /** Whether or not the back buffer contains the selection info */
    bool back_buffer_ready;

    /** Whether or not we are in camera moving mode */
    bool movemode;

    /** Timer used to slow the rendering down to a maximum of 20 fps */
    QTime rendertimer;
    
    /** openGL display list containing the state changes used to draw
        the scene */
    GLuint sceneidx;
    
    /** openGL display list containing the background */
    GLuint bgidx;
    
    /** Whether or not the canvas is in the process of being repainted */
    bool isrepainting;
    
    /** Mutex to ensure that rendering is serialised */
    QMutex rendermutex;
    
    /** Wait condition to slow rendering down to 20 fps (50ms render) */
    QWaitCondition renderwait;
};

}

SIRE_END_HEADER

#endif
