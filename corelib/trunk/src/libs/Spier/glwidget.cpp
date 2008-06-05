
#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>

#include "glwidgetbase.h"
#include "canvasui.h"

#include "SireBase/mutablesetiterator.hpp"

using namespace Spier;
using namespace SireStream;
using namespace SireBase;

GLWidgetBase::GLWidgetBase(GLWidget parent) 
             : QObject(), prnt(parent.weakPointer()),
               widgetorigin(0.0,0.0), widgetsize(100.0,75.0),
               sizepolicy(QSizePolicy::Preferred, QSizePolicy::Preferred),
               widgetrotation(0.0), widgetscalex(1.0), widgetscaley(1.0),
               widgetshearx(0.0), widgetsheary(0.0), widgetmatrix(),
               ishidden(false), isfocussed(false)
{
    
    //by default, the widget is a rectangle, with the top left corner at (0,0) 
    //and the bottom right corner at (width, height)
    clippath.addRect( QRectF( QPointF(0,0), widgetsize ) );
    
    this->setParent(parent);
}

GLWidgetBase::~GLWidgetBase()
{}

/** Return the self pointer */
GLWidget GLWidgetBase::self() const
{
    if (selfptr.expired())
    {
        GLWidgetBase *nonconst_this = const_cast<GLWidgetBase*>(this);
    
        boost::shared_ptr<GLWidgetBase> ptr(nonconst_this);
        nonconst_this->selfptr = ptr;
        
        GLWidget glwidget = unlockWrapper<GLWidgetBase>(selfptr);
        return glwidget;
    }
    else
        return unlockWrapper<GLWidgetBase>(selfptr);
}

/** Set the parent of this widget */
void GLWidgetBase::setParent(GLWidget newparent)
{
    if (newparent.get() == this)
        throw SireError::invalid_arg(QObject::tr("A widget cannot be its own parent!"), CODELOC);
    else
    {
        if (newparent)
            newparent->makeMeYourChild(self());
        
        this->makeMeYourParent(newparent);
    }
}

/** Make this widget into an orphan */
void GLWidgetBase::makeOrphan()
{
    if (not isOrphan())
    {
        parent()->iAmNotYourChild(self());
        
        prnt.reset();
    }
}

/** Return whether or not this is an orphan widget */
bool GLWidgetBase::isOrphan() const
{
    return prnt.expired();
}

/** Return the GLWidget that is this widget's parent (this may be null if this 
    is an orphan widget) */
GLWidget GLWidgetBase::parent() const
{
    return unlockWrapper<GLWidgetBase>(prnt);
}

/** Add 'child' to this widgets list of children. The child already 
    thinks that this is its parent. */
void GLWidgetBase::makeMeYourChild(GLWidget child)
{
    kids.insert(child);
}

/** This forces this widget to set 'parent' as its parent. The 
    new parent already thinks that this is one of its children. */
void GLWidgetBase::makeMeYourParent(GLWidget newparent)
{
    //remove this widget from the old parent...
    GLWidget oldparent = this->parent();
        
    if (oldparent == newparent)
        //there is no change of parent
        return;
        
    //we only hold a weak pointer to the parent to prevent
    //cyclic dependencies (this way, deleting the parent 
    //should delete all of the children, unless the child 
    //is referenced elsewhere)
    prnt = newparent.weakPointer();
        
    //remove this from the old parent
    if (oldparent)
        oldparent->iAmNotYourChild(self());
}

/** Remove 'child' from this widgets list of children. The child 
    already has a different parent. */
void GLWidgetBase::iAmNotYourChild(GLWidget child)
{
    kids.remove(child);
}

/** Tell this widget that 'parent' is not its parent. The parent 
    already knows that this is not its child. */
void GLWidgetBase::iAmNotYourParent(GLWidget notparent)
{
    GLWidget current_parent = parent();
    
    if (current_parent == notparent)
        //only the current parent can disown this child!
        prnt.reset();
}

/** Orphan all of this widgets children - this will delete the children
    unless they are being reference elsewhere. */
void GLWidgetBase::orphanAllChildren()
{
    kids.clear();
}

/** Reconstruct the transformation matrix for this widget, which is used 
    to map from parent widget coordinates to local widget coordinates
    (or from global window coordinates if this is an orphan widget) */
void GLWidgetBase::reconstructMatrix()
{
    //first, construct the transformation matrix for this matrix...
    
    //set to identity - top-left is at (0,0)
    widgetmatrix.reset();
    
    //translate the top-left corner to its correct position
    widgetmatrix.translate(widgetorigin.x(),widgetorigin.y());
    
    //rotate about the center of the widget
    widgetmatrix.rotate(widgetrotation.toDegrees());

    //scale equally about the center of the widget
    widgetmatrix.scale(widgetscalex, widgetscaley);
    
    //shear along x and y
    widgetmatrix.shear(widgetshearx, widgetsheary);
    
    bool invertible;
    inversematrix = widgetmatrix.inverted(&invertible);
    
    if (not invertible)
        qDebug() << QObject::tr("Widget matrix is not invertible!");
}

/** Set the position of the top-left corner of the widget (its origin) */
void GLWidgetBase::setPosition(const QPointF &pos)
{
    if (widgetorigin != pos)
    {
        //this widget is being moved
        //QMoveEvent e(pos, widgetorigin);
    
        widgetorigin = pos;
        this->reconstructMatrix();
        
        //send this widget a move event
        //this->moveEvent(&e);
    }
}

/** Set the position of the center of the GLWidgetBase */
void GLWidgetBase::setCenter(const QPointF &center)
{
    this->setPosition( center - QPointF(0.5*widgetsize.width(),0.5*widgetsize.height()) );
}

/** Set the scale factor of this widget */
void GLWidgetBase::setScale(double scalex, double scaley)
{
    widgetscalex = scalex;
    widgetscaley = scaley;
    this->reconstructMatrix();
}

/** Set the shear factor of this widget */
void GLWidgetBase::setShear(double shearx, double sheary)
{
    widgetshearx = shearx;
    widgetsheary = sheary;
    this->reconstructMatrix();
}
    
/** Set the rotation of this widget */
void GLWidgetBase::setRotation(const SireMaths::Angle &angle)
{
    widgetrotation = angle;
    this->reconstructMatrix();
}

/** Validate 'newsize' to ensure that it does not violate the 
    widget's user-set minimum and maximum sizes */
QSizeF GLWidgetBase::validateNewSize(QSizeF newsize) const
{
    if (minsize.isValid())
    {
        newsize.setWidth( qMax(newsize.width(), minsize.width()) );
        newsize.setHeight( qMax(newsize.height(), minsize.height()) );
    }
    
    if (maxsize.isValid())
    {
        newsize.setWidth( qMin(newsize.width(), maxsize.width()) );
        newsize.setHeight( qMin(newsize.height(), maxsize.height()) );
    }
    
    return newsize;

    //note that sizeHint is used by the QLayout class. Maybe I want to make a layout
    //class?
}

/** Set the size of the GLWidgetBase */
void GLWidgetBase::setSize(const QSizeF &size)
{
    //get the allowable new size of the widget
    QSizeF newsize = this->validateNewSize(size);

    if (widgetsize != newsize)
    {
        //QResizeEvent e(newsize, widgetsize);
    
        //the widget is changing size
        widgetsize = newsize;
    
        //we will need to update the clip path - the default is a plain
        //rectangle

        //by default, the widget is a rectangle, with the top left corner at (0,0)
        //and the bottom right corner at (width,height)
        clippath = QPainterPath();
        clippath.addRect( QRectF(QPointF(0,0),widgetsize) );
        
        //send a resize event to the widget
        //this->resizeEvent(&e);
    }
}
   
/** Overloaded version of 'setCenter' - used for convienience */
void GLWidgetBase::setCenter(qreal x, qreal y)
{
    this->setCenter( QPointF(x,y) );
}

/** Overloaded version of 'setPosition' - used for convienience */
void GLWidgetBase::setPosition(qreal x, qreal y)
{
    this->setPosition( QPointF(x,y) );
}

/** Overloaded version of 'setSize' - used for convienience */
void GLWidgetBase::setSize(qreal w, qreal h)
{
    this->setSize( QSizeF(w,h) );
}

/** Overloaded version of 'setSize' - used for convienience */
void GLWidgetBase::setWidth(qreal w)
{
    this->setSize( QSizeF(w, widgetsize.height()) );
}

/** Overloaded version of 'setSize' - used for convienience */
void GLWidgetBase::setHeight(qreal h)
{
    this->setSize( QSizeF(widgetsize.width(), h) );
}

/** Set the widget to occupy the rectangle 'rect' in global widget coordinates
    (i.e. the top left corner of the widget will be at the same point 
    as the top-left corner of 'rect', and the widget will have the same
    internal size as 'rect' (though may be rotated, scaled and sheared) */
void GLWidgetBase::setRect(const QRectF &rect)
{
    this->setPosition(rect.topLeft());
    this->setSize(rect.size());
}

/** Return the global window coordinates of the center of this widget */
QPointF GLWidgetBase::center() const
{
    return mapToGlobal( QPointF(width()*0.5, height()*0.5) );
}

/** Return the global window coordinates of the top left corner of this widget */
QPointF GLWidgetBase::topLeft() const
{
    return mapToGlobal( QPointF(0,0) );
}
    
/** Return the global window coordinates of the top right corner of this widget */
QPointF GLWidgetBase::topRight() const
{
    return mapToGlobal( QPointF(width(),0) );
}

/** Return the global window coordinates of the bottom left corner of this widget */
QPointF GLWidgetBase::bottomLeft() const
{
    return mapToGlobal( QPointF(0,height()) );
}

/** Return the global window coordinates of the bottom right corner of this widget */
QPointF GLWidgetBase::bottomRight() const
{
    return mapToGlobal( QPointF(width(),height()) );
}
    
/** Return the clipping path for this widget - the default is to 
    return the bounding rectangle. This clipping path is given in 
    local widget coordinates */
QPainterPath GLWidgetBase::clipPath()
{
    return clippath;
}

/** Paint this widget. This will set up a clipping region that will prevent drawing 
    outside this widget. Note that this clipping region will be the intersection of 
    the clipping region of this widget with that of all of its parents */
void GLWidgetBase::paint(QPainter &painter)
{
    //we do not draw hidden widgets, or their children
    if (isHidden())
        return;

    //save the painter's current state
    painter.save();

    //transform from parent coordinates to local widget coordinates
    // ( 'true' as we are combining the local widget matrix with the
    //   current transformation matrix, which will be the parent's
    //   matrix )
    painter.setMatrix(widgetmatrix,true);

    if (isOrphan())
    {
        //this is a top-level widget - create a new clipping region
        //painter.setClipPath( clippath, Qt::ReplaceClip );
        //painter.setClipping(true);
          
        //paint the widget
        this->paintWidget(painter);
    
        //now paint all of its children (and all of their children, etc.)
        MutableSetIterator<GLWidgetBase*,GLWidget> it(kids);
        while (it.hasNext())
        {
            it.next();
            it.value()->paint(painter);
        }
    
        //clear the clip region
        //painter.setClipping(false);
    }
    else
    {
        //add this widgets clip region to that of its parent
        //painter.setClipPath( this->clipPath(), Qt::IntersectClip );
        
        //paint the widget
        this->paintWidget(painter);
        
        //now paint all of its children (and all of their children, etc.)
        MutableSetIterator<GLWidgetBase*,GLWidget> it(kids);
        while (it.hasNext())
        {
            it.next();
            it.value()->paint(painter);
        }
    }
    
    //restore the painter's state
    painter.restore();
}

/** Paint the contents of this widget. This default implementation just 
    draws a rectangle for the background of this widget */
void GLWidgetBase::paintWidget(QPainter &painter)
{
    //we are already in the local widget coordinates.
    
    //just draw the QPainterPath
    if (isfocussed)
        painter.setBrush( QBrush(QColor(100,100,100,100)) );
    else
        painter.setBrush( QBrush(QColor(150,150,150,150)) );
        
    painter.setPen( QPen(QColor(0,0,0,255)) );
    
    painter.drawPath(clippath);
}

/** Return the widget at point 'point' in window global coordinates. */
GLWidget GLWidgetBase::widgetAt(const QPointF &point)
{
    if (isHidden())
        return GLWidget();
    else if (isOrphan())
        return widgetAtPvt(point);
    else
    {
        //we need to map the point into the coordinate 
        //system of the parent widget
        return widgetAtPvt( parent()->mapToLocal(point) );
    }
}

/** Return the widget at point 'point', or 0 if there is no widget there.
    (this can only return this widget, or any children of this widget).
    'point' is in the coordinate system of the parent widget (or 
    in global window coordinates if this is an orphan widget) */
GLWidget GLWidgetBase::widgetAtPvt(const QPointF &point)
{
    //convert the point in widget local coordinates
    QPointF localpoint = inversematrix.map(point);
    
/*    printLine(DEBUG,QObject::tr("Global = (%1,%2), local = (%3,%4)")
                    .arg(point.x()).arg(point.y())
                    .arg(localpoint.x()).arg(localpoint.y()));*/
    
    //does this widget contain this point?
    if (clippath.contains(localpoint))
    {
        //see if any of the children contain this point...
        GLWidget foundwidget;
        
        MutableSetIterator<GLWidgetBase*,GLWidget> it(kids);
        while (it.hasNext())
        {
            it.next();
            foundwidget = it.value()->widgetAtPvt(localpoint);
            if (foundwidget.isValid())
                return foundwidget;
        }
        
        //none of the kids contain this point, so return this widget
        return self();
    }
    else
        return GLWidget();
}

/** Map the point 'global' in global window coordinates to local widget coordinates. */
QPointF GLWidgetBase::mapToLocal(const QPointF &global) const
{
    if (isOrphan())
    {
        //map the point from parent coordinates to local widget coordinates
        return inversematrix.map(global);
    }
    else
    {
        //need to map the point from global coordinates into the local coordinates 
        return inversematrix.map( parent()->mapToLocal(global) );
    }
}

/** Map the point 'local' in local widget coordinates to global window coordinates. */
QPointF GLWidgetBase::mapToGlobal(const QPointF &local) const
{
    if (isOrphan())
    {
        //map from local widget coordinates into global window coordinates
        QPointF global = widgetmatrix.map(local);
        
        return global;
    }
    else
    {
        //map from local widget coordinates into parent widget coordinates
        QPointF parentpos = widgetmatrix.map(local);
            
        return parent()->mapToGlobal(parentpos);
    }
}

/** Return whether or not this widget contains the point 'point'
    (p is in global window coordinates) */
bool GLWidgetBase::contains(const QPointF &point) const
{
    if (isHidden())
        return false;
    else if (isOrphan())
        return containsPvt(point);
    else
    {
        //we need to map the point from global coordinates to 
        //the local coordinates of the parent widget
        return containsPvt( parent()->mapToLocal(point) );
    }
}

/** Return whether or not this widget contains the point 'point'
    (which is in the coordinate system of the parent widget, or 
    in global window coordinates if this is an orphan) */
bool GLWidgetBase::containsPvt(const QPointF &point) const
{
    //map the point from parent coordinates to local widget
    //coordinates
    QPointF localpoint = inversematrix.map(point);
    
    return clippath.contains(localpoint);
}

/** Update the display so that this widget is repainted. Note that this will
    repaint the entire openGL canvas, so this could be slow (100s of milliseconds) */
void GLWidgetBase::update()
{
    //tell the parent to update (eventually, the top level parent 
    //will tell the canvas that we want an update)
    if (isOrphan())
        emit needsUpdate();
    else
        parent()->update();
}

/** Return whether or not this widget is visible */
bool GLWidgetBase::isVisible() const
{
    return not ishidden;
}

/** Return whether or not this widget is hidden */
bool GLWidgetBase::isHidden() const
{
    return ishidden;
}
    
/** Hide this widget (and all of its children) */
void GLWidgetBase::hide()
{
    ishidden = true;
}

/** Show this widget */
void GLWidgetBase::show()
{
    ishidden = false;
}

/** Set whether or not this widget is hidden */
void GLWidgetBase::setHidden(bool hidden)
{
    ishidden = hidden;
}

void GLWidgetBase::focusInEvent(QFocusEvent*)
{
    isfocussed = true;
    this->update();
}

void GLWidgetBase::focusOutEvent(QFocusEvent*)
{
    isfocussed = false;
    this->update();
}
    
void GLWidgetBase::mousePressEvent(QMouseEvent *e)
{
    //we ignore events by default
    e->ignore();
}

void GLWidgetBase::mouseReleaseEvent(QMouseEvent *e)
{
    //we ignore events by default
    e->ignore();
}

void GLWidgetBase::mouseClickEvent(QMouseEvent *e)
{
    //we ignore events by default
    e->ignore();
}

void GLWidgetBase::mouseDoubleClickEvent(QMouseEvent *e)
{
    //we ignore events by default
    e->ignore();
}

void GLWidgetBase::wheelEvent(QWheelEvent *e)
{
    //we ignore events by default
    e->ignore();
}

void GLWidgetBase::keyPressEvent(QKeyEvent *e)
{
    //we ignore events by default
    e->ignore();
}

void GLWidgetBase::keyReleaseEvent(QKeyEvent *e)
{
    //we ignore events by default
    e->ignore();
}

void GLWidgetBase::resizeEvent(QResizeEvent*)
{}

void GLWidgetBase::moveEvent(QMoveEvent*)
{}

QSizeF GLWidgetBase::minimumSizeHint() const
{
    return QSizeF();
}

QSizeF GLWidgetBase::maximumSizeHint() const
{
    return QSizeF();
}

QSizeF GLWidgetBase::sizeHint() const
{
    return QSizeF();
}
