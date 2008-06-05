
#include "SireDB/errors.h"
#include "SireStream/registermetatype.hpp"

using namespace SireDB;
using namespace SireStream;

static const RegisterMetaType<siredb_error> r_error("SireDB::siredb_error");
static const RegisterMetaType<db_error> r_db("SireDB::db_error");
static const RegisterMetaType<invalid_match> r_match("SireDB::invalid_match");
static const RegisterMetaType<missing_parameter> r_param("SireDB::missing_parameter");
