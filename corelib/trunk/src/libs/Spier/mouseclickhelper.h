#ifndef SPIER_MOUSECLICKHELPER_H
#define SPIER_MOUSECLICKHELPER_H

#include <QTime>
#include <QMouseEvent>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This class is used to monitor for mouse click events. A mouse click event occurs when the user presses and releases the left mouse button. The click is only generated if the press and release occurs within the time 'mouseClickInterval' and the mouse has moved by fewer than 'mouseClickDelta' pixels.
 
@author Christopher Woods
*/
class SPIER_EXPORT MouseClickHelper
{
public:
    MouseClickHelper();
    ~MouseClickHelper();

    uint mouseClickDelta() const;
    uint mouseClickInterval() const;

    bool clickInProgress() const;

    void press(QMouseEvent *e);
    bool move(QMouseEvent *e);
    QMouseEvent* release(QMouseEvent *e);

private:
  
    /** The interval (in ms) within which the mouse press and release
        must have occured to qualify as a mouse click */
    uint maxinterval;
    
    /** The maximum number of pixels that the mouse can move within
        which the press and release are viewed as being a click, rather
        than a drag */
    uint maxdelta;

    /** Whether or not a mouse click is in progress */
    bool click_in_progress;

    /** The mouse event caused by the initial mouse press */
    QMouseEvent startevent;

    /** Timer used to get the time between the mouse click and release */
    QTime timer;

};

/** Return the mouseClickInterval. This is the time in milliseconds within which 
    the mouse press and release must occur to qualify as a mouse click */
inline uint MouseClickHelper::mouseClickInterval() const
{
    return maxinterval;
}

/** Return the mouseClickDelta. This is the maximum number of pixels that the mouse
    can move after the mouse press before the input is interpreted as a mouse drag */
inline uint MouseClickHelper::mouseClickDelta() const
{
    return maxdelta;
}

/** Return whether or not a mouse click is in progress */
inline bool MouseClickHelper::clickInProgress() const
{
    return click_in_progress;
}

}

SIRE_END_HEADER

#endif
