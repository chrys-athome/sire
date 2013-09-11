#ifndef SIREN_THIRDPARTY_FCMP_H
#define SIREN_THIRDPARTY_FCMP_H

#include "Siren/siren.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    namespace third_party
    {
        int fcmp(const double x1, const double x2, const double epsilon);
    }
}

SIREN_END_HEADER

#endif // ifndef SIREN_THIRDPARTY_FCMP_H
