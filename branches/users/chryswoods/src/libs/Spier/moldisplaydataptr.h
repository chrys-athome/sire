#ifndef SPIER_MOLDISPLAYDATAPTR_H
#define SPIER_MOLDISPLAYDATAPTR_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Spier
{

class MolDisplayData;

typedef boost::shared_ptr<MolDisplayData> MolDisplayDataPtr;
typedef boost::weak_ptr<MolDisplayData> MolDisplayDataWeakPtr;

}

#endif
