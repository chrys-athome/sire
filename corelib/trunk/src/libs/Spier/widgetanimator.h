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
