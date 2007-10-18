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

#include "viewmode.h"
#include "viewer.h"
#include "glcanvas.h"
#include "camera.h"
#include "molselectid.h"

#include <QDebug>

#include "SireError/errors.h"

using namespace Spier;

/** Construct a ViewMode that controls 'canvas'. This cannot be null! */
ViewMode::ViewMode(GLCanvas *canvas)
         : InputInterpreterBase(), cnvs(canvas), cameramode(ROTATE)
{
    if (!cnvs)
        throw SireError::nullptr_error(QObject::tr(
            "Cannot create a ViewMode with a null canvas!"), CODELOC);
}

/** Destructor */
ViewMode::~ViewMode()
{}

/** Switch to translation mode */
void ViewMode::setTranslationMode()
{
    cameramode = TRANSLATE;
}

/** Switch to rotation mode */
void ViewMode::setRotationMode()
{
    cameramode = ROTATE;
}

/** Executed whenever a key is pressed when the mouse is in this viewer */
void ViewMode::keyPressEvent(QKeyEvent *e)
{
    if (!e)
        return;
        
    double delta = 5.0;
    double trandelta = 0.1;
    
    double slowscl = 1.0;
    if (e->modifiers() & Qt::ControlModifier)
        slowscl = 0.1;
    
    //get the pointer to the camera
    Camera &cam = cnvs->camera();
        
    if (e->modifiers() & Qt::ShiftModifier)
    {
        switch( e->key() )
        {
            case Qt::Key_Up:
                cam.zoom(-slowscl*delta);
                break;
            case Qt::Key_Down:
                cam.zoom(slowscl*delta);
                break;
            case Qt::Key_Left:
                cam.spin(Angle::degrees(slowscl*delta));
                break;
            case Qt::Key_Right:
                cam.spin(Angle::degrees(-slowscl*delta));
                break;
            default:
                break;
        }
    }
    else
    {
        switch( e->key() )
        {
            case Qt::Key_Up:
                if (cameramode == ROTATE)
                    cam.rotate( Angle::degrees(slowscl*delta), cam.sideVec() );
                else if (cameramode == TRANSLATE)
                    cam.translate(0, trandelta * slowscl);
                    
                break;
            case Qt::Key_Down:
                if (cameramode == ROTATE)
                    cam.rotate( Angle::degrees(-slowscl*delta), cam.sideVec() );
                else if (cameramode == TRANSLATE)
                    cam.translate(0, -trandelta * slowscl);
                    
                break;
            case Qt::Key_Left:
                if (cameramode == ROTATE)
                    cam.rotate( Angle::degrees(slowscl*delta), cam.upVec() );
                else if (cameramode == TRANSLATE)
                    cam.translate(-trandelta * slowscl, 0);
                
                break;
            case Qt::Key_Right:
                if (cameramode == ROTATE)
                    cam.rotate( Angle::degrees(-slowscl*delta), cam.upVec() );
                else if (cameramode == TRANSLATE)
                    cam.translate(trandelta * slowscl, 0);
                   
                break;
            default:
                break;
        }
    }
    
    if (cam.needRepaint())
    {
        //we have moved the view
        cnvs->setMoveMode(true);
        cnvs->updateGL();
        cnvs->setMoveMode(false);
    }
    
    InputInterpreterBase::keyPressEvent(e);
}

/** Executed whenever a key is pressed when the mouse is not in this viewer */
void ViewMode::keyReleaseEvent(QKeyEvent *e)
{
    InputInterpreterBase::keyReleaseEvent(e);
}

/** Executed when the mouse button is pressed */
void ViewMode::mousePressEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    //save the set of currently pressed buttons
    bttns = e->buttons();
    
    //save the point at which the mouse was pressed
    presspos = e->pos();
    presspos_g = e->globalPos();
    
    //now update the last position of the mouse
    InputInterpreterBase::mousePressEvent(e);
}

/** Executed when the mouse button is released */
void ViewMode::mouseReleaseEvent(QMouseEvent *e)
{
    if (!e)
        return;
        
    //save the set of currently pressed buttons
    bttns = e->buttons();
    
    //if no mouse buttons are being pressed, then we
    //can no longer be in camera moving mode
    if (not (bttns & Qt::LeftButton or bttns & Qt::RightButton))
    {
        if (not movingcursor.isNull())
        {
            QCursor::setPos(movingcursor);
            movingcursor = QPoint();
        }
        
        cnvs->setMoveMode( false );
    }
    
    InputInterpreterBase::mouseReleaseEvent(e);
}

/** Executed when the mouse button is clicked (the left button is pressed and 
    released within a specified time, without moving much between the press and release).
    The passed mouse event is the event that was generated on the mouse press. */
void ViewMode::mouseClickEvent(QMouseEvent *e)
{
    //do nothing if there is no event, or the canvas is in move mode
    if (!e or cnvs->moveMode())
        return;
    
    //clicking is used to select whatever is under the mouse in this viewmode
    //If the control key is pressed, then we add to the selection
    //If the shift key is pressed, then we switch the ViewMode to rubber-band selection
    //If shift and control is pressed, then we switch to rubber-band selection, and 
    //add to the current selection
    //If shift and control are not pressed, then we set the selection
    
    if (e->modifiers() & Qt::ShiftModifier)
    {
        //switch to rubber band mode - tell it whether or not 
        //we are adding to the selection, or setting the selection
        qDebug() << QObject::tr("Entering rubber band selection mode...");
        //cnvs->setViewMode( new RubberBandMode(cnvs, e) );
    }
    else
    {
        //see if we have a valid, highlighted atom...
        if (cnvs->highlighted().isNull())
        {
            QPoint clickpos = e->pos();
      
            //if the control key is not pressed, then clear the current selection
            if (not (e->modifiers() & Qt::ControlModifier))
                cnvs->clearSelection();
        }
        else
        {
            if (e->modifiers() & Qt::ControlModifier)
                //if the control key is pressed, then toggle the selection of this object
                cnvs->toggleSelected(cnvs->highlighted());
            else
            {
                //set this object as selected
                cnvs->setSelected(cnvs->highlighted());
            }
        }
    }

    InputInterpreterBase::mouseClickEvent(e);
}

/** Executed when the mouse button is double-clicked */
void ViewMode::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        //we will treat double-click events the same as single click
        //events
        this->mouseClickEvent(e);
    else
        InputInterpreterBase::mouseDoubleClickEvent(e);
}

/** Executed when the mouse is moved */
void ViewMode::mouseMoveEvent(QMouseEvent *e)
{
    if (!e)
        return;
    
    //get the change in mouse position since the last mouse press event
    QPoint del = currentpos_g - e->globalPos();
    
    //if there has been no move, then return
    if (del.x() == 0 and del.y() == 0)
        return;
    
    //if the left or right mouse buttons are pressed then we are 
    //moving the camera
    if (bttns & Qt::RightButton or bttns & Qt::LeftButton)
    {
        //tell the canvas that we are moving the view (this changes some
        //settings so as to speed up the rendering while the view is being changed)
        cnvs->setMoveMode(true);
        
        if (movingcursor.isNull())
            movingcursor = presspos_g;
    
        double slowscl = 0.5;
        if (e->modifiers() & Qt::ControlModifier)
            slowscl = 0.05;
    
        //get a pointer to the camera of the canvas
        Camera &cam = cnvs->camera();
    
        if (bttns & Qt::RightButton or (e->modifiers() & Qt::ShiftModifier))
        {
            cam.zoom(-0.2 * slowscl * del.y());
            cam.spin( Angle::degrees(slowscl*del.x()) );
        }    
        else
        {
            if (cameramode == ROTATE)
            {
                cam.rotate( Angle::degrees(slowscl*del.x()), cam.upVec() );
                cam.rotate( Angle::degrees(slowscl*del.y()), cam.sideVec() );
            }
            else if (cameramode == TRANSLATE)
            {
                cam.translate(-0.1*slowscl*del.x(), 0.1*slowscl*del.y());
            }
        }
    }
    else
        cnvs->setMoveMode(false);
    
    //record the current mouse position
    InputInterpreterBase::mouseMoveEvent(e);
       
    //always repaint the canvas, as we may have changed the selection
    cnvs->updateGL();
}

/** Executed on mouse wheel events */
void ViewMode::wheelEvent(QWheelEvent *e)
{
    if (!e) 
        return;
        
    //we are definitely moving the camera
    cnvs->setMoveMode(true); 
       
    //get the amount that the mouse wheel moved
    int del = e->delta();
    
    double slowscl = 1.0;
    if (e->modifiers() & Qt::ControlModifier)
        slowscl = 0.1;
        
    Camera &cam = cnvs->camera();
    cam.zoom(-0.01 * slowscl * del);
    
    if (cam.needRepaint())
        cnvs->updateGL();
        
    cnvs->setMoveMode(false);

    InputInterpreterBase::wheelEvent(e);
}
