/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "Conspire/GUI/animation.h"
#include "Conspire/GUI/page.h"

#include "Conspire/exceptions.h"

#include <QAbstractAnimation>
#include <QTimer>

using namespace Conspire;

////////
//////// Implementation of AnimationPointer
////////

/** Constructor */
AnimationPointer::AnimationPointer()
{}

/** Construct to hold a pointer to 'Animation'. */
AnimationPointer::AnimationPointer(Animation *animation) : a(animation)
{
    if (a)
    {
        a->incref();
    }
}

/** Copy constructor */
AnimationPointer::AnimationPointer(const AnimationPointer &other) : a(other.a)
{
    if (a)
    {
        a->incref();
    }
}

/** Destructor */
AnimationPointer::~AnimationPointer()
{
    if (a)
    {
        if (a->decref())
        {
            a->stop();
            a->deleteLater();
        }
    }
}

/** Return the raw pointer to the Animation */
Animation* AnimationPointer::data()
{
    return a;
}

/** Return the raw pointer to the Animation */
const Animation* AnimationPointer::data() const
{
    return a;
}

/** Return whether or not this pointer is null */
bool AnimationPointer::isNull() const
{
    return (not a);
}

/** Return the raw pointer to the Animation */
AnimationPointer::operator Animation*()
{
    return a;
}

/** Dereference the pointer */
Animation& AnimationPointer::operator*() const
{
    if (not a)
    {
        throw Conspire::program_bug( Conspire::tr( 
                "An attempt has been made to dereference a null AnimationPointer!"),
                    CODELOC );
    }
    
    return *a;
}

/** Return the raw pointer to the Animation */
Animation* AnimationPointer::operator->() const
{
    return a;
}

/** Copy assignment operator */
AnimationPointer& AnimationPointer::operator=(Animation *animation)
{
    if (a != animation)
    {
        if (a)
        {
            //decrease the reference count
            if (a->decref())
            {
                a->stop();
                a->deleteLater();
            }
        }

        if (animation)
            animation->incref();
            
        a = animation;
    }
    
    return *this;
}

/** Copy assignment operator */
AnimationPointer& AnimationPointer::operator=(const AnimationPointer &other)
{
    return this->operator=(other.a);
}

/** Comparison operator */
bool AnimationPointer::operator==(const AnimationPointer &other) const
{
    return a == other.a;
}

/** Comparison operator */
bool AnimationPointer::operator!=(const AnimationPointer &other) const
{
    return a != other.a;
}

////////
//////// Implementation of Animation
////////

void Animation::build()
{
    ref_count = 0;
    autostop_delay = 250;
    is_running = false;
}

/** Constructor */
Animation::Animation(QObject *parent) : QObject(parent)
{
    build();
}

/** Construct, passing in the animation to perform */
Animation::Animation(QAbstractAnimation *animation, QObject *parent)
          : QObject(parent), anim(animation)
{
    build();
}

/** Construct, passing in the animation to perform, and the
    Page on which the animation acts */
Animation::Animation(QAbstractAnimation *animation,
                     PagePointer page, QObject *parent)
          : QObject(parent), anim(animation)
{
    pages.append(page);
}

/** Construct, passing in the animation to perfrom, and the
    Pages on which the animation acts */
Animation::Animation(QAbstractAnimation *animation,
                     PagePointer page0, PagePointer page1,
                     QObject *parent)
          : QObject(parent), anim(animation)
{
    pages.append(page0);
    pages.append(page1);
}
          
/** Destructor */
Animation::~Animation()
{
    if (anim)
    {
        if (is_running)
        {
            anim->disconnect(this);
            anim->stop();

            if (anim->direction() == QAbstractAnimation::Forward)
            {
                anim->setCurrentTime(anim->duration());
            }
            else
            {
                anim->setCurrentTime(0);
            }
        }

        anim->deleteLater();
    }
}

/** Return whether or not the animation is running */
bool Animation::isRunning() const
{
    return is_running;
}

/** Set the delay after the end of the expected end of the 
    animation at which the animation will be automatically stopped.
    If any value less than or equal to zero is passed, then
    the animation will never be automatically stopped. Note that
    this is ignored if the animation has an undefined duration
    (e.g. if it runs continuously for ever) */
void Animation::setAutoStopDelay(int ms)
{
    if (ms < 0)
        autostop_delay = 0;
    else
        autostop_delay = ms;
}

/** Return the delay after which the animation will be automatically
    stopped. */
int Animation::autoStopDelay() const
{
    return autostop_delay;
}

/** Start the animation. If it is already running, then it stops
    the current run, and then restarts it from the beginning. */
void Animation::start()
{
    if (anim)
    {
        if (stop_timer or (anim->state() != QAbstractAnimation::Stopped))
        {
            if (stop_timer)
            {
                stop_timer->disconnect();
                stop_timer->deleteLater();
                stop_timer = 0;
            }
            
            anim->disconnect(this);
            anim->stop();
        }

        int stop_time = 0;
        
        if (anim->duration() > 0 and autostop_delay > 0)
        {
            stop_time = anim->duration() + autostop_delay;
        }
        
        is_running = true;
        connect(anim, SIGNAL(finished()), this, SLOT(stop()));
        anim->start();
        
        if (stop_time > 0)
        {
            stop_timer = new QTimer(this);
            stop_timer->setSingleShot(true);
            connect(stop_timer.data(), SIGNAL(timeout()), this, SLOT(stop()));
            stop_timer->start();
        }
    }
}

/** Stop the animation. This will automatically advance the animation
    to the last frame (or the last frame of the loop, if this is a looping
    animation) */
void Animation::stop()
{
    if (stop_timer)
    {
        stop_timer->disconnect();
        stop_timer->deleteLater();
        stop_timer = 0;
    }
    
    anim->disconnect(this);
    
    if (anim->state() != QAbstractAnimation::Stopped)
    {
        anim->stop();
    }
    
    if (anim->direction() == QAbstractAnimation::Forward)
    {
        anim->setCurrentTime(anim->duration());
    }
    else
    {
        anim->setCurrentTime(0);
    }
    
    if (is_running)
    {
        is_running = false;
        emit( finished() );
    }
}

/** Internal function used to increase the reference count */
void Animation::incref()
{
    ref_count += 1;
}

/** Internal function used to decrease the reference count. This 
    returns "true" if the reference count has dropped to zero */
bool Animation::decref()
{
    ref_count -= 1;
    
    if (ref_count <= 0)
    {
        ref_count = 0;
        return true;
    }
    else
        return false;
}
