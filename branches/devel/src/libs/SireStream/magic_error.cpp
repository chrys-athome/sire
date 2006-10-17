
#include "magic_error.h"
#include "registermetatype.hpp"

#include "datastream.h"

#include "SireError/errors.h"

using namespace SireStream;
using namespace SireError;

// Define the streaming operators for SireError here
// This is to remove the circular dependency of SireError on SireStream

/** Implementation of SireError::exception */
static const RegisterMetaType<exception> r_exception("SireError::exception");

/** Serialise to a binary data stream */
QDataStream SIRESTREAM_EXPORT &operator<<(QDataStream &ds, const exception &e)
{
    writeHeader(ds, r_exception, 1)
       << e.err << e.plce << e.bt << e.pidstr;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRESTREAM_EXPORT &operator>>(QDataStream &ds, exception &e)
{
    VersionID v = readHeader(ds, r_exception);

    if (v == 1)
    {
        ds >> e.err >> e.plce >> e.bt >> e.pidstr;
    }
    else
        throw version_error(v, "1", r_exception, CODELOC);

    return ds;
}

static RegisterMetaType<program_bug> r_program_bug("SireError::program_bug");
static RegisterMetaType<unsupported> r_unsupported("SireError::unsupported");
static RegisterMetaType<invalid_key> r_invalid_key("SireError::invalid_key");
static RegisterMetaType<invalid_index> r_invalid_index("SireError::invalid_index");
static RegisterMetaType<invalid_cast> r_invalid_cast("SireError::invalid_cast");
static RegisterMetaType<incompatible_error> r_incompat("SireError::incompatible_error");
static RegisterMetaType<unknown_type> r_unknown_type("SireError::unknown_type");
static RegisterMetaType<noncopyable_error> r_noncopyable_error("SireError::noncopyable_error");
static RegisterMetaType<nullptr_error> r_nullptr_error("SireError::nullptr_error");
static RegisterMetaType<lock_error> r_lock_error("SireError::lock_error");
static RegisterMetaType<assertation_failed> r_assertation_failed("SireError::assertation_failed");
static RegisterMetaType<file_error> r_file_error("SireError::file_error");
static RegisterMetaType<io_error> r_io_error("SireError::io_error");
static RegisterMetaType<invalid_arg> r_invalid_arg("SireError::invalid_arg");
static RegisterMetaType<invalid_state> r_invalid_state("SireError::invalid_state");
static RegisterMetaType<incomplete_code> r_incomplete_code("SireError::incomplete_code");
static RegisterMetaType<std_exception> r_std_exception("SireError::std_exception");
static RegisterMetaType<unknown_exception> r_unknown_exception("SireError::unknown_exception");

static RegisterMetaType<magic_error> r_magic_error("SireStream::magic_error");
