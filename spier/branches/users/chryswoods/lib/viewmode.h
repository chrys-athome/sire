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

#ifndef SPIER_VIEWMODE_H
#define SPIER_VIEWMODE_H

#include "inputinterpreter.h"

#include "button.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class GLCanvas;

/**
This InputInterpreter is used in "view" mode. This is the default mode of the canvas, and is used to rotate the view around the molecule, highlight atoms and other items under the mouse, and to select atoms.
 
@author Christopher Woods
*/
class SPIER_EXPORT ViewMode : public InputInterpreterBase
{

Q_OBJECT

public:
    ViewMode(GLCanvas *canvas);
    ~ViewMode();

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseClickEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    
    void wheelEvent(QWheelEvent *e);
    
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

public slots:

    void setTranslationMode();
    void setRotationMode();

protected:

    /** The GLCanvas that this ViewMode is controlling */
    GLCanvas *cnvs;
    
    /** The position of the mouse at the last 'mousePressEvent' */
    QPoint presspos;
    /** The global position of the mouse at the last 'mousePressEvent' */
    QPoint presspos_g;
    
    /** The set of mouse buttons that are currently being pressed
        (updated at each mousePress and mouseRelease event */
    Qt::MouseButtons bttns;
        
    QPoint movingcursor;
        
    enum {ROTATE=1, TRANSLATE=2};
    
    /** Which mode is the camera in? */
    int cameramode;
};

}

SIRE_END_HEADER

#endif
