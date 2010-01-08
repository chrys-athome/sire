#include "Siren/convertobjptr.hpp"

#include "SireCAS_properties.h"

#include "SireCAS/errors.h"
#include "SireMaths/complex.h"
#include "SireMaths/rational.h"
#include "Siren/errors.h"
#include "Siren/stream.h"
#include "casnode.h"
#include "complexvalues.h"
#include "expression.h"
#include "factor.h"
#include "identities.h"
#include "symbol.h"
#include "values.h"
#include "casnode.h"
void register_SireCAS_properties()
{
    register_objptr< SireCAS::CASNodePtr >();
}
