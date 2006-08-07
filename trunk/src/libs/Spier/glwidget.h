#ifndef SPIER_GLWIDGET_H
#define SPIER_GLWIDGET_H

#include "wrapper.hpp"

#include <boost/weak_ptr.hpp>

namespace Spier
{

class GLWidgetBase;
typedef Wrapper<GLWidgetBase> GLWidget;
typedef boost::weak_ptr<GLWidgetBase> GLWidgetSelfPtr;

}

#endif
