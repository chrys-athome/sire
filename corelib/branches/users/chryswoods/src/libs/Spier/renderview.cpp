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

#include "renderview.h"

using namespace Spier;

/** Constructor */
RenderView::RenderView(QWidget *parent, QGLWidget *share_widget)
           : QGLWidget(parent, share_widget), yval(0)
{
    this->setAutoBufferSwap(false);

    setAutoFillBackground(false);

    glcanvases.append( GLCanvas() );
    canvas_images.append( QImage() );
    canvas_rendertimes.append( 0 );
    
    QTimer *timer = new QTimer(this);
    
    connect( timer, SIGNAL(timeout()), this, SLOT(animate()) );
    
    timer->start(50);
}

/** Destructor */
RenderView::~RenderView()
{
    makeCurrent();
    
    //clean up display lists...
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

/** Paint the scene */
void RenderView::paintGL()
{
    QTime t;

    if (canvas_images[0].isNull())
    {
        t.start();
        glcanvases[0].edit().render( const_cast<QGLContext*>(this->context()), 
                                     this->width(), this->height() );
    
        canvas_rendertimes[0] = t.elapsed();
    
        if (canvas_rendertimes[0] > 50)
        {
            //rendering was quite slow - save the output so that
            //we don't slow down the interface
            glReadBuffer( GL_BACK );
    
            canvas_images[0] = this->grabFrameBuffer();
        }
    }
    else
    {
        t.start();
        QPainter painter;
        painter.begin(this);
        painter.drawImage(0, 0, canvas_images[0]);
        
        if (t.elapsed() > 2*canvas_rendertimes[0])
        {
            //it is taking more than twice as long to recover the
            //image than it did to render it! It is not worth saving it
            qDebug() << "Recovering took" << t.elapsed()
                     << "but drawing took" << canvas_rendertimes[0];
                     
            canvas_images[0] = QImage();
        }
    }
    
    //now draw the overlay
    QPainter painter;
    
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

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
    
    this->swapBuffers();
}
