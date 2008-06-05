/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SPIER_GLWIDGETBASE_H
#define SPIER_GLWIDGETBASE_H

#include <QObject>
#include <QPointF>
#include <QSizeF>
#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QMatrix>
#include <QWidget>

#include "glwidget.h"
#include "glwidgetset.h"

#include "SireMaths/angle.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class CanvasUI;

/**
This is the virtual base class of all mini widgets that are drawn on the openGL canvas.
 
@author Christopher Woods
*/
class SPIER_EXPORT GLWidgetBase : public QObject
{

friend class Wrapper<GLWidgetBase>;  //friend, so can sort out self pointer
friend class CanvasUI;               //friend so can call paint and event functions

Q_OBJECT

public:
    ~GLWidgetBase();

    GLWidget parent() const;
    void setParent(GLWidget parent);
    
    const GLWidgetSet& children() const;
    
    void makeOrphan();
    bool isOrphan() const;
    
    bool isVisible() const;
    bool isHidden() const;

    bool isFocussed() const;

    void orphanAllChildren();

    GLWidget self() const;
    
    bool contains(const QPointF &point) const;
    GLWidget widgetAt(const QPointF &point);

    QPointF mapToLocal(const QPointF &global) const;
    QPointF mapToGlobal(const QPointF &local) const;

    void setRect(const QRectF &rect);
    void setCenter(const QPointF &position);
    void setPosition(const QPointF &position);
    
    void setGeometry(const QRectF &geometry);
    QRectF geometry() const;
    
    void setSize(const QSizeF &size);
    void setMinimumSize(const QSizeF &minimum);
    void setMaximumSize(const QSizeF &maximum);

    QSizeF minimumSize() const;
    QSizeF maximumSize() const;
    
    virtual QSizeF minimumSizeHint() const;
    virtual QSizeF maximumSizeHint() const;
    virtual QSizeF sizeHint() const;

    void setSizePolicy(const QSizePolicy &policy);
    QSizePolicy sizePolicy() const;

    void setCenter(qreal x, qreal y);
    void setPosition(qreal x, qreal y);
    
    void setSize(qreal w, qreal h);
    void setWidth(qreal w);
    void setHeight(qreal h);

    void setScale(double scalex, double scaley);
    void setShear(double shearx, double sheary);
    
    void setRotation(const SireMaths::Angle &angle);

    QPointF center() const;
    QPointF topLeft() const;
    QPointF topRight() const;
    QPointF bottomLeft() const;
    QPointF bottomRight() const;
    
    qreal width() const;
    qreal height() const;

public slots:
    virtual void update();
    
    void hide();
    void show();
    void setHidden(bool hidden);

signals:
    void needsUpdate();

protected:

    GLWidgetBase(GLWidget parent = GLWidget());
    
    void paint(QPainter &painter);

    bool containsPvt(const QPointF &point) const;
    GLWidget widgetAtPvt(const QPointF &point);

    virtual void paintWidget(QPainter &painter);

    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseClickEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
        
    virtual void resizeEvent(QResizeEvent *e);
    virtual void moveEvent(QMoveEvent *e);
        
    void reconstructMatrix();
    QSizeF validateNewSize(QSizeF newsize) const;
        
    QPainterPath clipPath();
        
    const QMatrix& widgetMatrix() const;
    const QMatrix& inverseMatrix() const;
        
    void makeMeYourChild(GLWidget child);
    void makeMeYourParent(GLWidget parent);
    void iAmNotYourChild(GLWidget child);
    void iAmNotYourParent(GLWidget parent);

    /** Weak pointer to this widget */
    GLWidgetSelfPtr selfptr;
    
    /** The parent widget */
    GLWidgetSelfPtr prnt;

    /** The set of child widgets */
    GLWidgetSet kids;

    /** the position of the origin of the local coordinates 
        of this widget (in global coordinates). This is the
        position of the top left corner of this widget */
    QPointF widgetorigin;

    /** The size of this widget in local coordinates */
    QSizeF widgetsize;
    
    /** The user-set maximum and minimum sizes of this widget */
    QSizeF maxsize, minsize;
    
    /** The widget's resizing policy */
    QSizePolicy sizepolicy;
    
    /** The rotation angle of this widget, in degrees clockwise */
    SireMaths::Angle widgetrotation;
    
    /** The x and y scale factors of this widget */
    qreal widgetscalex, widgetscaley;
    
    /** The x and y shear factors for this widget */
    qreal widgetshearx, widgetsheary;

    /** The matrix used to transform from global coordinates to 
        local widget coordinates. This is the combination of the 
        above transformations, together with the transformations
        of all parents. We cache it here for speed. */
    QMatrix widgetmatrix;

    /** The inverse of the widget matrix */
    QMatrix inversematrix;

    /** The clipping path of this widget in local widget coordinates */
    QPainterPath clippath;

    /** Whether this widget is hidden */
    bool ishidden;
    
    /** Whether or not this widget is focussed */
    bool isfocussed;
};

/** Return the set of children of this widget */
inline const GLWidgetSet& GLWidgetBase::children() const
{
    return kids;
}

/** Return the width of this widget */
inline qreal GLWidgetBase::width() const
{
    return widgetsize.width();
}

/** Return the height of this widget */
inline qreal GLWidgetBase::height() const
{
    return widgetsize.height();
}

/** Return the transformation matrix for this widget that is used to 
    transform from global window coordinates into local widget coordinates. */
inline const QMatrix& GLWidgetBase::widgetMatrix() const
{
    return widgetmatrix;
}

/** Return the inverse of the widget matrix */
inline const QMatrix& GLWidgetBase::inverseMatrix() const
{
    return inversematrix;
}

/** Return whether or not this widget is focussed */
inline bool GLWidgetBase::isFocussed() const
{
    return isfocussed;
}

/** Return the size policy of this widget */
inline QSizePolicy GLWidgetBase::sizePolicy() const
{
    return sizepolicy;
}

}

SIRE_END_HEADER

#endif
