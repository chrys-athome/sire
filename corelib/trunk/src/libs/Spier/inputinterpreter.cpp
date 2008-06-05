
#include "inputinterpreter.h"

using namespace Spier;

/** Create a null InputInterpreter. This ignores everything. */
InputInterpreter::InputInterpreter()
{}

/** Create an InputInterpreter from the passed pointer. This will take over
    ownership of the pointer, so you should not delete it. */
InputInterpreter::InputInterpreter(InputInterpreterBase *base)
                 : boost::shared_ptr<InputInterpreterBase>(base)
{}

/** Copy constructor. This class is explicitly shared, so copying
    the object will mean that both copies will point to the same
    underlying InputInterpreterBase class. */
InputInterpreter::InputInterpreter(const InputInterpreter &other)
                 : boost::shared_ptr<InputInterpreterBase>(other)
{}

/** Destructor */
InputInterpreter::~InputInterpreter()
{}

/** Assignment operator. This class is explicitly shared, so copying
    the object will mean that both copies will point to the same
    underlying InputInterpreterBase class. */
InputInterpreter& InputInterpreter::operator=(const InputInterpreter &other)
{
    boost::shared_ptr<InputInterpreterBase>::operator=(other);
    return *this;
}

/** Assignment operator. This takes over ownership of the passed pointer, so you 
    should not delete it. */
InputInterpreter& InputInterpreter::operator=(InputInterpreterBase *ptr)
{
    boost::shared_ptr<InputInterpreterBase>::reset(ptr);
    return *this;
}

//////////////
////////////// Implementation of InputInterpreterBase
//////////////

/** Empty class has an empty constructor. */
InputInterpreterBase::InputInterpreterBase() : QObject()
{}

/** Destructor */
InputInterpreterBase::~ InputInterpreterBase()
{}

/** Default implementation does nothing */
void InputInterpreterBase::setTranslationMode()
{}

/** Default implementation does nothing */
void InputInterpreterBase::setRotationMode()
{}

/** Default implementation just updates the current position of the mouse */
void InputInterpreterBase::mouseMoveEvent(QMouseEvent *e)
{
    if (e)
    {
        currentpos = e->pos();
        currentpos_g = e->globalPos();
    }
}

/** Default implementation just updates the current position of the mouse */
void InputInterpreterBase::mousePressEvent(QMouseEvent *e)
{
    if (e)
    {
        currentpos = e->pos();
        currentpos_g = e->globalPos();
        
        e->accept();
    }
}

/** Default implementation ignores this event */
void InputInterpreterBase::mouseReleaseEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreterBase::mouseClickEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreterBase::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e)
        e->accept();
}
    
/** Default implementation ignores this event */
void InputInterpreterBase::wheelEvent(QWheelEvent *e)
{
    if (e)
        e->accept();
}
    
/** Default implementation ignores this event */
void InputInterpreterBase::keyPressEvent(QKeyEvent *e)
{
    if (e)
        e->accept();
}

/** Default implementation ignores this event */
void InputInterpreterBase::keyReleaseEvent(QKeyEvent *e)
{
    if (e)
        e->accept();
}
