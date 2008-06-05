#ifndef SPIER_MATERIALPTR_H
#define SPIER_MATERIALPTR_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Spier
{
class Material;

typedef boost::shared_ptr<Material> MaterialPtr;
typedef boost::weak_ptr<Material> MaterialWeakPtr;

}

#endif
