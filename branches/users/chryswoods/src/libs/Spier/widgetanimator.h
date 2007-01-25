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

#ifndef SPIER_WIDGETANIMATOR_H
#define SPIER_WIDGETANIMATOR_H

#include "glwidget.h"

#include <QTime>

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This is the base class of all animators. An animator is used to control an animation of a GLWidget. It provides the start and end points of the animation, when the animation started, and how long it should run for. The animator is used by the CanvasUI to make the changes to the widget before redrawing it.
 
@author Christopher Woodss
*/
class SPIER_EXPORT WidgetAnimator
{

public:
    WidgetAnimator(GLWidget widget);
    ~WidgetAnimator();
    
    void animate();

private:

    /** The widget being animated */
    GLWidget wdgt;
    
    /** The timer used to time the animation */
    QTime timer;

};

}

SIRE_END_HEADER

#endif
