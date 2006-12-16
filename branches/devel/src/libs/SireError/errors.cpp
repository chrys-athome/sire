
#include <QObject>
#include <QDataStream>
#include <QFile>
#include <QProcess>

#include "SireError/errors.h"

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

QString exitStatusString(QProcess::ExitStatus stat)
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

QString SIREERROR_EXPORT processError(const QString &executable,
                                      const QProcess &process)
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

}
