
#include "magic_error.h"

#include "datastream.h"

#include "version_error.h"

#include "SireError/errors.h"

using namespace SireStream;
using namespace SireError;

// Define the streaming operators for SireError here
// This is to remove the circular dependency of SireError on SireStream

/** Implementation of SireError::exception */
static const RegisterMetaType<exception> r_exception;

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
        throw SireStream::version_error(v, "1", r_exception, CODELOC);

    return ds;
}


static RegisterMetaType<program_bug> r_bug;
static RegisterMetaType<unsupported> r_unsupported;
static RegisterMetaType<id_error> r_id;
static RegisterMetaType<invalid_key> r_key;
static RegisterMetaType<invalid_index> r_index;
static RegisterMetaType<invalid_cast> r_cast;
static RegisterMetaType<noncopyable_error> r_noncopy;
static RegisterMetaType<nullptr_error> r_nullptr;
static RegisterMetaType<lock_error> r_lock;
static RegisterMetaType<assertation_failed> r_assert;
static RegisterMetaType<file_error> r_file;
static RegisterMetaType<io_error> r_io;
static RegisterMetaType<invalid_arg> r_arg;
static RegisterMetaType<invalid_state> r_state;
static RegisterMetaType<incomplete_code> r_incomplete;
static RegisterMetaType<std_exception> r_std;
static RegisterMetaType<unknown_exception> r_unknown;
static RegisterMetaType<SireError::version_error> r_version;

static RegisterMetaType<magic_error> r_magic_error;
