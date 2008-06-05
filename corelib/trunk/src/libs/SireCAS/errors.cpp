
#include "SireCAS/errors.h"
#include "SireStream/registermetatype.hpp"

using namespace SireCAS;
using namespace SireStream;

static const 
RegisterMetaType<sirecas_error> r_error("SireCAS::sirecas_error");

static const 
RegisterMetaType<unregistered_expression> r_unreg("SireCAS::unregistered_expression");

static const 
RegisterMetaType<unavailable_differential> r_diff("SireCAS::unavailable_differential");

static const 
RegisterMetaType<unavailable_integral> r_int("SireCAS::unavailable_integral");
