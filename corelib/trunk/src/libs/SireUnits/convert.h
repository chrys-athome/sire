#ifndef SIREUNITS_CONVERT_H
#define SIREUNITS_CONVERT_H

namespace SireUnits
{

double convert(double val, double fromunits, double tounits);
double convertFrom(double val, double fromunits);
double convertTo(double val, double tounits);

}

#endif
