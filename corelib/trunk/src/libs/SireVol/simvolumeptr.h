#ifndef SIREVOL_SIMVOLUMEPTR_H
#define SIREVOL_SIMVOLUMEPTR_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace SireVol
{

class SimVolume;

typedef boost::shared_ptr<SimVolume> SimVolumePtr;
typedef boost::weak_ptr<SimVolume> SimVolumeWeakPtr;

}

#endif
