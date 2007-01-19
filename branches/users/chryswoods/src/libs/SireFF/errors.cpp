
#include "SireFF/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireFF;

static const RegisterMetaType<sireff_error> r_ff;
static const RegisterMetaType<missing_component> r_mcomp;
static const RegisterMetaType<missing_function> r_mfunc;
static const RegisterMetaType<missing_forcefield> r_mff;
static const RegisterMetaType<duplicate_component> r_dupcomp;
static const RegisterMetaType<duplicate_function> r_dupfunc;
static const RegisterMetaType<duplicate_forcefield> r_dupff;
