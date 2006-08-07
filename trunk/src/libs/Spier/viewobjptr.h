#ifndef SPIER_VIEWOBJPTR_HPP
#define SPIER_VIEWOBJPTR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Spier
{

class ViewObj;
typedef boost::shared_ptr<ViewObj> ViewObjPtr;
typedef boost::weak_ptr<ViewObj> ViewObjWeakPtr;

}

#endif
