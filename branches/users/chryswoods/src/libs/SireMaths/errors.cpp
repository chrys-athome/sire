
#include "SireMaths/errors.h"

#include "SireStream/registermetatype.hpp"

using namespace SireStream;
using namespace SireMaths;

static const RegisterMetaType<siremaths_error> r_siremaths("SireMaths::siremaths_error");
static const RegisterMetaType<math_error> r_math("SireMaths::math_error");
static const RegisterMetaType<domain_error> r_domain("SireMaths::domain_error");
