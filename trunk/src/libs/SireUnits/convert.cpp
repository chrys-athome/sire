
#include "sireglobal.h"

namespace SireUnits
{

/** Convert 'val' from units 'fromunits' into units 'tounits' */
double SIREUNITS_EXPORT convert(double val, double fromunits, double tounits)
{
    return val * (fromunits / tounits);
}

/** Convert from internal units into the specified units */
double SIREUNITS_EXPORT convertFrom(double val, double fromunits)
{
    return val * fromunits;
}

/** Convert to the specified units from internal units */
double SIREUNITS_EXPORT convertTo(double val, double tounits)
{
    return val / tounits;
}

}
