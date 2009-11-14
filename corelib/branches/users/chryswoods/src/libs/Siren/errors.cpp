/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <QObject>
#include <QDataStream>
#include <QFile>
#include <QProcess>

#include "Siren/errors.h"

using namespace Siren;

/** Return a string representation of the QFile::FileError */
static QString fileError(const QFile &f)
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

static QString exitStatusString(QProcess::ExitStatus stat)
{
    switch (stat)
    {
        case QProcess::NormalExit:
            return QObject::tr("Normal exit");

        case QProcess::CrashExit:
        default:
            return QObject::tr("Crashed!");
    }
}

static QString processError(const QString &executable, const QProcess &process)
{
    QString err = QObject::tr("There was an error while running the program \"%1\". ")
                          .arg(executable);

    switch (process.error())
    {
        case QProcess::FailedToStart:
            return err + QObject::tr("The process failed to start.");

        case QProcess::Crashed:
            return err + QObject::tr("The process crashed! (exit code %1)")
                                    .arg(process.exitCode());
        case QProcess::Timedout:
            return err + QObject::tr("The last operation timed out (something went wrong?). "
                                     "PID == %1, Exit status == %2, Exit code == %3")
                                        .arg(process.pid())
                                        .arg(exitStatusString(process.exitStatus()))
                                        .arg(process.exitCode());

        case QProcess::WriteError:
            return err + QObject::tr("There was a write error! PID == %1").arg(process.pid());

        case QProcess::ReadError:
            return err + QObject::tr("There was a read error! PID == %1").arg(process.pid());

        case QProcess::UnknownError:
        default:
            return err + QObject::tr("There was an unknown error! ."
                                     "PID == %1, Exit status == %2, Exit code == %3")
                                        .arg(process.pid())
                                        .arg(exitStatusString(process.exitStatus()))
                                        .arg(process.exitCode());
    }
}

namespace boost
{

/** The boost::assert macro calls this function if the assertation fails.
    This function converts the information passed by the macro into
    a Siren::assertation_error exception, which is thrown. */
void SIREN_EXPORT assertion_failed(char const * expr, char const * function,
                                   char const * file, long line)
{
    QString s = QObject::tr("**PROGRAM BUG** %1 in %2, at line %3 in file %4")
                     .arg(expr).arg(function).arg(file).arg(line);

    throw Siren::assertation_error(s,CODELOC);
}

}

//////////
////////// Implementation of program_bug
//////////

static const RegisterMetaType<program_bug> r_prog_bug( 14521739046331749964UL, 1 );

program_bug::program_bug() : ImplementsException<program_bug, exception>()
{}

program_bug::program_bug(QString err, QString place)
            : ImplementsException<program_bug,exception>(err, place)
{}

program_bug::program_bug(const program_bug &other)
            : ImplementsException<program_bug,exception>(other)
{}

program_bug::~program_bug() throw()
{}

//////////
////////// Implementation of unsupported
//////////

static const RegisterMetaType<unsupported> r_unsupported( 82140520453817141UL, 1 );

unsupported::unsupported() : ImplementsException<unsupported, exception>()
{}

unsupported::unsupported(QString err, QString place)
            : ImplementsException<unsupported,exception>(err, place)
{}

unsupported::unsupported(const unsupported &other)
            : ImplementsException<unsupported,exception>(other)
{}

unsupported::~unsupported() throw()
{}

//////////
////////// Implementation of invalid_key
//////////

static const RegisterMetaType<invalid_key> r_invalid_key( 10689590460278841477UL, 1 );

invalid_key::invalid_key() : ImplementsException<invalid_key, exception>()
{}

invalid_key::invalid_key(QString err, QString place)
            : ImplementsException<invalid_key,exception>(err, place)
{}

invalid_key::invalid_key(const invalid_key &other)
            : ImplementsException<invalid_key,exception>(other)
{}

invalid_key::~invalid_key() throw()
{}

//////////
////////// Implementation of invalid_index
//////////

static const RegisterMetaType<invalid_index> r_invalid_index( 4766111025124148855UL, 1 );

invalid_index::invalid_index() : ImplementsException<invalid_index, exception>()
{}

invalid_index::invalid_index(QString err, QString place)
            : ImplementsException<invalid_index,exception>(err, place)
{}

invalid_index::invalid_index(const invalid_index &other)
            : ImplementsException<invalid_index,exception>(other)
{}

invalid_index::~invalid_index() throw()
{}

//////////
////////// Implementation of invalid_cast
//////////

static const RegisterMetaType<invalid_cast> r_invalid_cast( 18327737796183917019UL, 1 );

invalid_cast::invalid_cast() : ImplementsException<invalid_cast, exception>()
{}

invalid_cast::invalid_cast(QString err, QString place)
            : ImplementsException<invalid_cast,exception>(err, place)
{}

invalid_cast::invalid_cast(const invalid_cast &other)
            : ImplementsException<invalid_cast,exception>(other)
{}

invalid_cast::~invalid_cast() throw()
{}

//////////
////////// Implementation of incompatible_error
//////////

static const RegisterMetaType<incompatible_error> r_incomp_err( 17356572411443054038UL, 
                                                                1 );

incompatible_error::incompatible_error() 
                   : ImplementsException<incompatible_error, exception>()
{}

incompatible_error::incompatible_error(QString err, QString place)
            : ImplementsException<incompatible_error,exception>(err, place)
{}

incompatible_error::incompatible_error(const incompatible_error &other)
            : ImplementsException<incompatible_error,exception>(other)
{}

incompatible_error::~incompatible_error() throw()
{}

//////////
////////// Implementation of unknown_type
//////////

static const RegisterMetaType<unknown_type> r_unknown_type( 17879964937639156288UL, 1 );

unknown_type::unknown_type() : ImplementsException<unknown_type, exception>()
{}

unknown_type::unknown_type(QString err, QString place)
            : ImplementsException<unknown_type,exception>(err, place)
{}

unknown_type::unknown_type(const unknown_type &other)
            : ImplementsException<unknown_type,exception>(other)
{}

unknown_type::~unknown_type() throw()
{}

//////////
////////// Implementation of noncopyable_error
//////////

static const RegisterMetaType<noncopyable_error> r_noncopyable( 249454149985806282UL, 1 );

noncopyable_error::noncopyable_error() 
                  : ImplementsException<noncopyable_error, exception>()
{}

noncopyable_error::noncopyable_error(QString err, QString place)
            : ImplementsException<noncopyable_error,exception>(err, place)
{}

noncopyable_error::noncopyable_error(const noncopyable_error &other)
            : ImplementsException<noncopyable_error,exception>(other)
{}

noncopyable_error::~noncopyable_error() throw()
{}

//////////
////////// Implementation of nullptr_error
//////////

static const RegisterMetaType<nullptr_error> r_nullptr( 7197621227417089819UL, 1 );

nullptr_error::nullptr_error() : ImplementsException<nullptr_error, exception>()
{}

nullptr_error::nullptr_error(QString err, QString place)
            : ImplementsException<nullptr_error,exception>(err, place)
{}

nullptr_error::nullptr_error(const nullptr_error &other)
            : ImplementsException<nullptr_error,exception>(other)
{}

nullptr_error::~nullptr_error() throw()
{}

//////////
////////// Implementation of lock_error
//////////

static const RegisterMetaType<lock_error> r_lock_err( 62505207938972832UL, 1 );

lock_error::lock_error() : ImplementsException<lock_error, exception>()
{}

lock_error::lock_error(QString err, QString place)
            : ImplementsException<lock_error,exception>(err, place)
{}

lock_error::lock_error(const lock_error &other)
            : ImplementsException<lock_error,exception>(other)
{}

lock_error::~lock_error() throw()
{}

//////////
////////// Implementation of assertation_error
//////////

static const RegisterMetaType<assertation_error> r_assert( 4827665807031565780UL, 1 );

assertation_error::assertation_error()
                  : ImplementsException<assertation_error, exception>()
{}

assertation_error::assertation_error(QString err, QString place)
            : ImplementsException<assertation_error,exception>(err, place)
{}

assertation_error::assertation_error(const assertation_error &other)
            : ImplementsException<assertation_error,exception>(other)
{}

assertation_error::~assertation_error() throw()
{}

//////////
////////// Implementation of file_error
//////////

static const RegisterMetaType<file_error> r_file_err( 6032034653749184580UL, 1 );

file_error::file_error() : ImplementsException<file_error, exception>()
{}

file_error::file_error(const QFile &file, QString place)
           : ImplementsException<file_error,exception>(::fileError(file), place)
{}

file_error::file_error(QString err, QString place)
            : ImplementsException<file_error,exception>(err, place)
{}

file_error::file_error(const file_error &other)
            : ImplementsException<file_error,exception>(other)
{}

file_error::~file_error() throw()
{}

//////////
////////// Implementation of process_error
//////////

static const RegisterMetaType<process_error> r_proc_err( 11859495717971765757UL, 1 );

process_error::process_error() : ImplementsException<process_error, exception>()
{}

process_error::process_error(QString err, QString place)
            : ImplementsException<process_error,exception>(err, place)
{}

process_error::process_error(const QString &executable, const QProcess &process,
                             QString place)
              : ImplementsException<process_error,exception>(
                    ::processError(executable, process), place )
{}

process_error::process_error(const process_error &other)
            : ImplementsException<process_error,exception>(other)
{}

process_error::~process_error() throw()
{}

//////////
////////// Implementation of unavailable_resource
//////////

static const RegisterMetaType<unavailable_resource> r_unavail_resource( 
                                                        9654613445668579139UL, 1 );

unavailable_resource::unavailable_resource() 
                     : ImplementsException<unavailable_resource, exception>()
{}

unavailable_resource::unavailable_resource(QString err, QString place)
            : ImplementsException<unavailable_resource,exception>(err, place)
{}

unavailable_resource::unavailable_resource(const unavailable_resource &other)
            : ImplementsException<unavailable_resource,exception>(other)
{}

unavailable_resource::~unavailable_resource() throw()
{}

//////////
////////// Implementation of io_error
//////////

static const RegisterMetaType<io_error> r_io_error( 8066259227570334535UL, 1 );

io_error::io_error() : ImplementsException<io_error, exception>()
{}

io_error::io_error(QString err, QString place)
            : ImplementsException<io_error,exception>(err, place)
{}

io_error::io_error(const io_error &other)
            : ImplementsException<io_error,exception>(other)
{}

io_error::~io_error() throw()
{}

//////////
////////// Implementation of invalid_arg
//////////

static const RegisterMetaType<invalid_arg> r_invalid_arg( 6551594718474985959UL, 1 );

invalid_arg::invalid_arg() : ImplementsException<invalid_arg, exception>()
{}

invalid_arg::invalid_arg(QString err, QString place)
            : ImplementsException<invalid_arg,exception>(err, place)
{}

invalid_arg::invalid_arg(const invalid_arg &other)
            : ImplementsException<invalid_arg,exception>(other)
{}

invalid_arg::~invalid_arg() throw()
{}

//////////
////////// Implementation of invalid_state
//////////

static const RegisterMetaType<invalid_state> r_invalid_state( 15253950952007822907UL, 1 );

invalid_state::invalid_state() : ImplementsException<invalid_state, exception>()
{}

invalid_state::invalid_state(QString err, QString place)
            : ImplementsException<invalid_state,exception>(err, place)
{}

invalid_state::invalid_state(const invalid_state &other)
            : ImplementsException<invalid_state,exception>(other)
{}

invalid_state::~invalid_state() throw()
{}

//////////
////////// Implementation of invalid_operation
//////////

static const RegisterMetaType<invalid_operation> r_invalid_op( 6770270493015655276UL, 1 );

invalid_operation::invalid_operation() 
            : ImplementsException<invalid_operation, exception>()
{}

invalid_operation::invalid_operation(QString err, QString place)
            : ImplementsException<invalid_operation,exception>(err, place)
{}

invalid_operation::invalid_operation(const invalid_operation &other)
            : ImplementsException<invalid_operation,exception>(other)
{}

invalid_operation::~invalid_operation() throw()
{}

//////////
////////// Implementation of incomplete_code
//////////

static const RegisterMetaType<incomplete_code> r_incomplete( 10388818633832027461UL, 1 );

incomplete_code::incomplete_code() : ImplementsException<incomplete_code, exception>()
{}

incomplete_code::incomplete_code(QString err, QString place)
            : ImplementsException<incomplete_code,exception>(err, place)
{}

incomplete_code::incomplete_code(const incomplete_code &other)
            : ImplementsException<incomplete_code,exception>(other)
{}

incomplete_code::~incomplete_code() throw()
{}

//////////
////////// Implementation of unknown_error
//////////

static const RegisterMetaType<unknown_error> r_unknown( 2931794926382754668UL, 1 );

unknown_error::unknown_error() : ImplementsException<unknown_error, exception>()
{}

unknown_error::unknown_error(QString err, QString place)
            : ImplementsException<unknown_error,exception>(err, place)
{}

unknown_error::unknown_error(const unknown_error &other)
            : ImplementsException<unknown_error,exception>(other)
{}

unknown_error::~unknown_error() throw()
{}

//////////
////////// Implementation of dependency_error
//////////

static const RegisterMetaType<dependency_error> r_depend( 14692603025929040001UL, 1 );

dependency_error::dependency_error() : ImplementsException<dependency_error, exception>()
{}

dependency_error::dependency_error(QString err, QString place)
            : ImplementsException<dependency_error,exception>(err, place)
{}

dependency_error::dependency_error(const dependency_error &other)
            : ImplementsException<dependency_error,exception>(other)
{}

dependency_error::~dependency_error() throw()
{}

//////////
////////// Implementation of corrupted_data
//////////

static const RegisterMetaType<corrupted_data> r_corrupted( 982238311178893803UL, 1 );

corrupted_data::corrupted_data() : ImplementsException<corrupted_data, exception>()
{}

corrupted_data::corrupted_data(QString err, QString place)
            : ImplementsException<corrupted_data,exception>(err, place)
{}

corrupted_data::corrupted_data(const corrupted_data &other)
            : ImplementsException<corrupted_data,exception>(other)
{}

corrupted_data::~corrupted_data() throw()
{}

//////////
////////// Implementation of id_error
//////////

static const RegisterMetaType<id_error> r_id_error( 4789079396563058588UL, 1 );

id_error::id_error() : ImplementsException<id_error, exception>()
{}

id_error::id_error(QString err, QString place)
            : ImplementsException<id_error,exception>(err, place)
{}

id_error::id_error(const id_error &other)
            : ImplementsException<id_error,exception>(other)
{}

id_error::~id_error() throw()
{}

//////////
////////// Implementation of std_exception
//////////

static const RegisterMetaType<std_exception> r_std_exp( 3838993618888824392, 1 );

static QString getWhatString(QString typstring, const std::exception &error)
{
    return QObject::tr("Caught standard exception 'std::%1' (%2)")
         .arg(typstring).arg(error.what());
}

std_exception::std_exception()
              : ImplementsException<std_exception,exception>()
{}

std_exception::std_exception(const std::logic_error &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("logic_error", error), place)
{}

std_exception::std_exception(const std::domain_error &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("logic_error", error), place)
{}

std_exception::std_exception(const std::invalid_argument &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("invalid_argument", error), place)
{}

std_exception::std_exception(const std::bad_alloc &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("bad_alloc", error), place)
{}

std_exception::std_exception(const std::length_error &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("length_error", error), place)
{}

std_exception::std_exception(const std::out_of_range &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("out_of_range", error), place)
{}

std_exception::std_exception(const std::runtime_error &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("runtime_error", error), place)
{}

std_exception::std_exception(const std::range_error &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("range_error", error), place)
{}

std_exception::std_exception(const std::overflow_error &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("overflow_error", error), place)
{}

std_exception::std_exception(const std::underflow_error &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("underflow_error", error), place)
{}

std_exception::std_exception(const std::exception &error, QString place)
              : ImplementsException<std_exception,exception>(
                        ::getWhatString("exception", error), place)
{}

std_exception::~ std_exception() throw()
{}
