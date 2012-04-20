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

QString AnimationPointer::toString() const
{
    if (a)
        return a->toString();
    else
        return "Animation::NULL";
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

QString Animation::toString() const
{
    return QString("Animation( %1, isRunning() == %2 )")
                .arg( QString::number( (size_t)(anim.data()), 16 ) )
                .arg( isRunning() );
}

/** Return whether or not the animation is running */
bool Animation::isRunning() const
{
    return is_running;
}

/** Start the animation. If it is already running, then it stops
    the current run, and then restarts it from the beginning. */
void Animation::start()
{
    if (anim)
    {
        if (anim->state() != QAbstractAnimation::Stopped)
        {
            anim->disconnect(this);
            this->disconnect(anim);
            
            if (anim->direction() == QAbstractAnimation::Forward)
            {
                anim->setCurrentTime(anim->duration());
            }
            else
            {
                anim->setCurrentTime(0);
            }
            
            anim->stop();
        }
        
        is_running = true;
        connect(anim, SIGNAL(finished()), this, SLOT(stop()));
        anim->start();
        emit( started(this) );
    }
}

/** Stop the animation. This will automatically advance the animation
    to the last frame (or the last frame of the loop, if this is a looping
    animation) */
void Animation::stop()
{
    if (not is_running)
        return;

    if (anim)
    {
        anim->disconnect(this);
        this->disconnect(anim);
        
        if (anim->direction() == QAbstractAnimation::Forward)
        {
            anim->setCurrentTime(anim->duration());
        }
        else
        {
            anim->setCurrentTime(0);
        }
        
        if (anim->state() != QAbstractAnimation::Stopped)
        {
            anim->stop();
        }
    }
    
    if (is_running)
    {
        is_running = false;
        emit( finished(this) );
    }
}

/** Pause the animation */
void Animation::pause()
{
    conspireDebug() << "Pausing animation" << this->toString();

    if (anim)
    {
        if (anim->state() == QAbstractAnimation::Running)
        {
            //anim->pause();
            //emit( paused(this) );
        }
    }
}

/** Resume a paused animation */
void Animation::resume()
{
    conspireDebug() << "Resuming animation" << this->toString();

    if (anim)
    {
        if (anim->state() == QAbstractAnimation::Paused)
        {
            //anim->resume();
            //emit( resumed(this) );
        }
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
