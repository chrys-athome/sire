#ifndef SIREUNITS_CONVERT_H
#define SIREUNITS_CONVERT_H

namespace SireUnits
{

/** Convert 'val' from units of 'fromunits' to units of 'tounits' */
inline double convert(double val, double fromunits, double tounits)
{
    return val * (fromunits / tounits);
}

/** Convert 'val' from units of 'fromunits' to internal units. */
inline double convertFrom(double val, double fromunits)
{
    return val * fromunits;
}

/** Convert 'val' from internal units to value of 'tounits' */
inline double convertTo(double val, double tounits)
{
    return val / tounits;
}

}

#endif
