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
#include <QRectF>
#include <QTimer>
#include <QImage>
#include <QTime>
#include <QCoreApplication>
#include <QSemaphore>

#include "renderview.h"
#include "eventmonitor.h"
#include "renderviewcommand.h"
#include "cameracommand.h"

#include "SireError/exception.h"
#include "SireError/printerror.h"

#include "SireError/errors.h"

using namespace Spier;

namespace Spier
{

void printError(const Command &command, const SireError::exception &e)
{
    QTextStream ts(stdout);
    
    ts << "\n**********************************************************\n"
       << "An error occured while trying to run the command:\n"
       << command.toString() << "\n"
       << "**********************************************************\n\n"
       << e.toString()
       << "\n\nSorry! Scroll back up\n"
       << "to the top of the error to work out what has gone wrong.\n\n";
}

}

/** Constructor */
RenderView::RenderView(QWidget *parent, QGLWidget *share_widget)
           : QGLWidget(parent, share_widget), yval(0)
{
    try
    {
        //start the animation timer
        animation_timer.start();
    
        //this widget is click to focus
        this->setFocusPolicy(Qt::WheelFocus);
    
        //install an event filter to filter all events to this widget
        this->installEventFilter( new EventMonitor(*this) );
    
        //this GL widget controls its own buffer swapping!
        this->setAutoBufferSwap(false);

        //don't fill the background!
        this->setAutoFillBackground(false);

        render_scenes.append( new RenderScene(this) );
        render_scenes[0]->setCamera( OrbitCamera() );
        
        scene_to_render = 0;

        QTimer *timer = new QTimer(this);
        
        connect( timer, SIGNAL(timeout()), this, SLOT(animate()) );
        
        needs_rerendering = true;
        
        timer->start(50);
    }
    catch(const SireError::exception &e)
    {
        SireError::printError(e);
        QCoreApplication::exit(-1);
    }
}

/** Destructor */
RenderView::~RenderView()
{
    //update the render context
    render_context = GLRenderContext::getContext(*this);

    //lock this context (ensures nothing else is rendering)
    GLRenderLocker lkr = render_context.lock();
    
    //ensure that this is the context on which OpenGL calls will be made
    render_context.makeCurrent();

    //delete everything attached to this context
    render_context.deleteAll();
}

/** Execute the command 'command' */
void RenderView::execute(const RenderViewCommand &command)
{
    try
    {
        command(*this);
    }
    catch(const SireError::exception &e)
    {
        Spier::printError(command, e);
    }
    catch(const std::exception &e)
    {
        Spier::printError(command, SireError::std_exception(e));
    }
    catch(...)
    {
        Spier::printError(command, SireError::unknown_exception( QObject::tr(
                "An unknown exception occured!" ), CODELOC ));
    }
}

/** Undo the last performed command. This does nothing if there
    are no commands available to undo */
void RenderView::undo()
{}

/** Redo the last command that was undone. This does nothing if
    there are no commands available to redo */
void RenderView::redo()
{}

/** Internal function called by CameraCommand used to get the
    current state of the camera */
const Camera& RenderView::getCamera() const
{
    return render_scenes.at(scene_to_render)->camera();
}

/** Internal function called by CameraCommand to set the current
    state of the camera */
void RenderView::setCamera(const Camera &camera)
{
    render_scenes[scene_to_render]->setCamera(camera);
    this->triggerUpdate();
}

/** Initialise the openGL view */
void RenderView::initializeGL()
{}

/** Resize the window */
void RenderView::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

/** Call this to trigger a re-render of the view. This returns
    immediately, as all it does is signal that a rendering is required */
void RenderView::triggerUpdate()
{
    needs_rerendering = true;
}

void RenderView::animate()
{
    yval += 50 * (0.001 * animation_timer.elapsed());
    
    animation_timer.start();
    
    if (yval > height())
        yval = -20;

    if (needs_rerendering)
    {
        needs_rerendering = false;
        this->updateGL();
    }
}

static void drawCorners(QPainter &painter, const int width, 
                        const int height, const int corner_size,
                        const int corner_depth, const QColor &color)
{

    QPainterPath path;
    
    QPointF p0(0,0);
    QPointF p1 = p0 + QPointF(corner_size,0);
    QPointF p2 = p0 + QPointF(0,corner_size);
    QPointF p3 = p0 + QPointF(corner_depth,corner_depth);
    
    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();
    
    p0 = QPointF(width, 0);
    p1 = p0 - QPointF(corner_size,0);
    p2 = p0 + QPointF(0,corner_size);
    p3 = p0 + QPointF(-corner_depth, corner_depth);
    
    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();
    
    p0 = QPointF(width, height);
    p1 = p0 - QPointF(corner_size,0);
    p2 = p0 - QPointF(0,corner_size);
    p3 = p0 - QPointF(corner_depth, corner_depth);

    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();

    p0 = QPointF(0, height);
    p1 = p0 + QPointF(corner_size,0);
    p2 = p0 - QPointF(0,corner_size);
    p3 = p0 + QPointF(corner_depth, -corner_depth);

    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();

    painter.setPen( color );
    painter.setBrush( color );
    
    painter.drawPath(path);
}

/** Internal function used to draw the corners */
void RenderView::drawCorners(QPainter &painter)
{
    if (this->hasFocus())
    {
        ::drawCorners(painter, width(), height(), 25, 3, QColor(0,0,255,128));
        ::drawCorners(painter, width(), height(), 12, 2, QColor(255,255,255,255));
    }
    else
    {
        ::drawCorners(painter, width(), height(), 25, 3, QColor(255,255,255,255));
    }
}

/** Internal function used to draw the user interface */
void RenderView::drawUI(QPainter &painter)
{
    //now draw the overlay
    QString text = tr("This is the Sire::Spier molecular viewer plugin");
    QFontMetrics metrics = QFontMetrics(font());
    int border = qMax(4, metrics.leading());

    QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
                                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setPen(Qt::white);
    painter.fillRect(QRectF(0, yval, width(), rect.height() + 2*border),
                       QColor(0, 0, 0, 60));
    painter.drawText((width() - rect.width())/2, yval + border,
                       rect.width(), rect.height(),
                       Qt::AlignCenter | Qt::TextWordWrap, text);
}

/** Paint the scene */
void RenderView::paintGL()
{
    QTime t;
    t.start();

    try
    {
        if (not this->isVisible())
            return;

        if (not this->context()->isValid())
            return;

        //update the render context
        render_context = GLRenderContext::getContext(*this);

        //lock this context (ensures nothing else is rendering)
        GLRenderLocker lkr = render_context.lock();
        
        //ensure that this is the context on which OpenGL calls will be made
        render_context.makeCurrent();

        //render the desired scene
        if (scene_to_render >= 0 and scene_to_render < render_scenes.count())
        {
            render_scenes[scene_to_render]->render(render_context);
        }
        
        //now draw the UI and the corners
        render_context.pushState();
        {
            QPainter painter;
            painter.begin(this);
            painter.setRenderHint(QPainter::Antialiasing);

            this->drawUI(painter);
            this->drawCorners(painter);
        }
        render_context.popState();
        
        this->swapBuffers();
        
        render_context.doneCurrent();
    }
    catch(const SireError::exception &e)
    {
        SireError::printError(e);
        QCoreApplication::exit(-1);
    }
    
    //don't bother rendering at more than 50 fps
    int delta = 20 - t.elapsed();
    
    if (delta > 0)
    {
        QSemaphore s(1);
        s.acquire();
        
        s.tryAcquire(1, delta);
        
        s.release();
    }
}
