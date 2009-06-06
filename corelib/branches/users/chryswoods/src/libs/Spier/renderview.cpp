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

#include "renderview.h"

#include "SireError/exception.h"
#include "SireError/printerror.h"

using namespace Spier;

/** Constructor */
RenderView::RenderView(QWidget *parent, QGLWidget *share_widget)
           : QGLWidget(parent, share_widget), yval(0)
{
    try
    {
        //this GL widget controls its own buffer swapping!
        this->setAutoBufferSwap(false);

        //don't fill the background!
        this->setAutoFillBackground(false);

        render_scenes.append( new RenderScene(this) );
        
        scene_to_render = 0;

        QTimer *timer = new QTimer(this);
        
        connect( timer, SIGNAL(timeout()), this, SLOT(animate()) );
        
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

/** Initialise the openGL view */
void RenderView::initializeGL()
{}

/** Resize the window */
void RenderView::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void RenderView::animate()
{
    yval += 2;
    
    if (yval > height())
        yval = -20;

    this->updateGL();
}

/** Internal function used to draw the corners */
void RenderView::drawCorners(QPainter &painter)
{
    const int corner_size = 25;
    const int corner_depth = 3;

    QPainterPath path;
    
    QPointF p0(0,0);
    QPointF p1 = p0 + QPointF(corner_size,0);
    QPointF p2 = p0 + QPointF(0,corner_size);
    QPointF p3 = p0 + QPointF(corner_depth,corner_depth);
    
    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();
    
    p0 = QPointF(width(), 0);
    p1 = p0 - QPointF(corner_size,0);
    p2 = p0 + QPointF(0,corner_size);
    p3 = p0 + QPointF(-corner_depth, corner_depth);
    
    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();
    
    p0 = QPointF(width(), height());
    p1 = p0 - QPointF(corner_size,0);
    p2 = p0 - QPointF(0,corner_size);
    p3 = p0 - QPointF(corner_depth, corner_depth);

    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();

    p0 = QPointF(0, height());
    p1 = p0 + QPointF(corner_size,0);
    p2 = p0 - QPointF(0,corner_size);
    p3 = p0 + QPointF(corner_depth, -corner_depth);

    path.moveTo(p0);
    path.lineTo(p1);
    path.quadTo(p3,  p2);
    path.closeSubpath();
    
    painter.setPen( QColor(255,255,255,255) );
    painter.setBrush( QColor(255,255,255,255) );
    
    painter.drawPath(path);
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
    painter.fillRect(QRect(0, yval, width(), rect.height() + 2*border),
                       QColor(0, 0, 0, 60));
    painter.drawText((width() - rect.width())/2, yval + border,
                       rect.width(), rect.height(),
                       Qt::AlignCenter | Qt::TextWordWrap, text);
}

/** Paint the scene */
void RenderView::paintGL()
{
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
}
