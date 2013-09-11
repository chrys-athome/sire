#ifndef CONSPIRE_ANIMATION_H
#define CONSPIRE_ANIMATION_H
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

#include "Conspire/conspire.h"

#include <QObject>
#include <QPointer>

CONSPIRE_BEGIN_HEADER

class QAbstractAnimation;
class QTimer;

namespace Conspire
{
    class Animation;

    /** This class holds a safe pointer to an Animation. This
        pointer will automatically manage the reference count
        to the Animation and will ensure that the Animation is
        deleted when it is no longer referenced. In addition,
        it will also automatically lose the reference if the
        Animation is deleted outside of this pointer. */
    class CONSPIRE_EXPORT AnimationPointer
    {
    public:
        AnimationPointer();
        AnimationPointer(Animation *animation);
        
        AnimationPointer(const AnimationPointer &other);
        
        ~AnimationPointer();
        
        Animation* data();
        const Animation* data() const;
        
        bool isNull() const;
        QString toString() const;
        
        operator Animation*();
    
        Animation& operator*() const;
        Animation* operator->() const;
        
        AnimationPointer& operator=(const AnimationPointer &other);
        AnimationPointer& operator=(Animation *animation);
        
        bool operator==(const AnimationPointer &other) const;
        bool operator!=(const AnimationPointer &other) const;

    private:
        QPointer<Animation> a;
    };

    /** This class represents an Animation that will be performed
        on the GUI. Animations are managed so that they can be
        stopped and started globally, or disabled if the user
        doesn't like them, or if they take up too many resources.
        
        To run an Animation, a Page will typically create an
        Animation and then emit a signal that will push the animation
        onto the PageView that holds the Page. If the Animation
        is blocking, then the PageView will disable all input to the
        Pages it controls while the Animation is underway, and will provide a 
        quick and painless way for the user to kill the Animation
        and proceeed immediately to the final frame. At the end
        of the Animation, the PageView will reenable input to the Pages
        it controls.
        
        If the Animation is not blocking, then the Animation will 
        run in the background without disrupting user input, but will
        still be killable by the user. If the Animation is modal, then
        it will go into a queue for the PageView such that only one
        Animation at a time is in process. Otherwise, the Animation
        will be performed in parallel.
        
        The Animation can be set to have an "end" time, so that,
        once this "end" time has passed, it will be killed and moved
        automatically to the end state. This ensures that Animations
        are not built up, killing the responsiveness of the application.
        
        The Animation can retain a PagePointer to the page or pages that created
        the Animation (and that likely contains the objects being animated).
        This should ensure that the objects being animated are not
        deleted before the Animation has completed.
    
        Note that you build the Animation using the classes and 
        capabilities of the Qt Animation framework. You should then
        pass that animation to this class, and let this class then
        control starting, stopping and deleting that animation.
    
        If you want to run or combine multiple animations together,
        use the QAnimationGroup class to merge them into a single
        animation that you can then pass to this class.
    
        Note that "stop" means something different for Animation
        than QAbstractAnimation. For Animation, when you stop the
        animation, it will automatically advance immediately to
        the final frame, so that the animation can be forced
        to jump to the end.
    
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT Animation : public QObject
    {
        Q_OBJECT
    
    public:
        Animation(QObject *parent=0);
        Animation(QAbstractAnimation *animation, QObject *parent=0);
                  
        ~Animation();

        QString toString() const;

        bool isRunning() const;

    public slots:
        void start();
        void stop();

        void pause();
        void resume();

    signals:
        void started(Animation *animation);
        void paused(Animation *animation);
        void resumed(Animation *animation);
        void finished(Animation *animation);

    protected:
        friend class AnimationPointer;
        void incref();
        bool decref();
    
    private:
        void build();
    
        /** Pointer to the animation to be performed */
        QPointer<QAbstractAnimation> anim;

        /** The number of AnimationPointer pointers currently 
            pointing to this animation */
        int ref_count;

        /** Whether or not the animation is running */
        bool is_running;
    };
}

CONSPIRE_END_HEADER

#endif
