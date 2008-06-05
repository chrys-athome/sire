
#include <QObject>
#include <QDataStream>

#include "SireError/errors.h"
#include "SireStream/registermetatype.hpp"

using namespace SireStream;
using namespace SireError;

namespace boost
{

/** The boost::assert macro calls this function if the assertation fails.
    This function converts the information passed by the macro into 
    a Sire::assertation_failed exception, which is thrown. */
void SIREERROR_EXPORT assertion_failed(char const * expr, char const * function, 
                                       char const * file, long line)
{
    QString s = QObject::tr("**PROGRAM BUG** %1 in %2, at line %3 in file %4")
                     .arg(expr).arg(function).arg(file).arg(line);
    
    throw SireError::assertation_failed(s,CODELOC);
}

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

namespace SireError
{

/** Now implement the std_exception constructors */

std_exception::std_exception()
              : exception(QObject::tr("Null std::exception"))
{}

std_exception::std_exception(const std::logic_error &error)
              : exception(std_exception::getWhatString("logic_error", error))
{}

std_exception::std_exception(const std::domain_error &error)
              : exception(std_exception::getWhatString("logic_error", error))
{}

std_exception::std_exception(const std::invalid_argument &error)
              : exception(std_exception::getWhatString("invalid_argument", error))
{}

std_exception::std_exception(const std::bad_alloc &error)
              : exception(std_exception::getWhatString("bad_alloc", error))
{}

std_exception::std_exception(const std::length_error &error)
              : exception(std_exception::getWhatString("length_error", error))
{}

std_exception::std_exception(const std::out_of_range &error)
              : exception(std_exception::getWhatString("out_of_range", error))
{}

std_exception::std_exception(const std::runtime_error &error)
              : exception(std_exception::getWhatString("runtime_error", error))
{}

std_exception::std_exception(const std::range_error &error)
              : exception(std_exception::getWhatString("range_error", error))
{}

std_exception::std_exception(const std::overflow_error &error)
              : exception(std_exception::getWhatString("overflow_error", error))
{}

std_exception::std_exception(const std::underflow_error &error)
              : exception(std_exception::getWhatString("underflow_error", error))
{}

std_exception::std_exception(const std::exception &error)
              : exception(std_exception::getWhatString("exception", error))
{}

std_exception::~ std_exception() throw()
{}

QString std_exception::getWhatString(QString typstring, const std::exception &error)
{
    return QObject::tr("Caught standard exception 'std::%1' (%2)")
         .arg(typstring).arg(error.what());
}

/** Return a string representation of the QFile::FileError */
QString SIREERROR_EXPORT fileError(const QFile &f)
{
    switch( f.error() )
    {
        case QFile::NoError:
            return QObject::tr("No error occurred while reading \"%1\".")
                                      .arg(f.fileName());
        case QFile::ReadError:
            return QObject::tr("An error occurred when reading from the file \"%1\".")
                                      .arg(f.fileName());
        case QFile::WriteError:
            return QObject::tr("An error occurred when writing to the file \"%1\".")
                                      .arg(f.fileName());
        case QFile::FatalError:
            return QObject::tr("A fatal error occurred while reading the file \"%1\".")
                                      .arg(f.fileName());
        case QFile::ResourceError:
            return QObject::tr("QFile::ResourceError while reading \"%1\"")
                                      .arg(f.fileName());
        case QFile::OpenError:
            return QObject::tr("The file \"%1\" could not be opened.")
                                      .arg(f.fileName());
        case QFile::AbortError:
            return QObject::tr("The operation was aborted while reading \"%1\".")
                                      .arg(f.fileName());
        case QFile::TimeOutError:
            return QObject::tr("A timeout occurred while reading \"%1\".")
                                      .arg(f.fileName());
        case QFile::UnspecifiedError:
            return QObject::tr("An unspecified error occurred while reading \"%1\".")
                                      .arg(f.fileName());
        case QFile::RemoveError:
            return QObject::tr("The file \"%1\" could not be removed.")
                                      .arg(f.fileName());
        case QFile::RenameError:
            return QObject::tr("The file \"%1\" could not be renamed.")
                                      .arg(f.fileName());
        case QFile::PositionError:
            return QObject::tr("The position in the file \"%1\" could not be changed.")
                                      .arg(f.fileName());
        case QFile::ResizeError:
            return QObject::tr("The file \"%1\" could not be resized.")
                                      .arg(f.fileName());
        case QFile::PermissionsError:
            return QObject::tr("The file \"%1\" could not be accessed.")
                                      .arg(f.fileName());
        case QFile::CopyError:
            return QObject::tr("The file \"%1\" could not be copied.")
                                      .arg(f.fileName());
        default:
            return QObject::tr("An unknown error occured while reading \"%1\".")
                                      .arg(f.fileName());
    }
}

}
