
#include <Python.h>

#include <cstdio>

#include <QDir>

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireBase/process.h"
#include "SireBase/getinstalldir.h"

#include "sire_python2_config.h"

#include <boost/scoped_array.hpp>

using std::printf;

using namespace SireBase;

#include <QDebug>

#ifdef Q_OS_UNIX

#include <signal.h>

//handle CTRL-C signal - this should kill the calculation
// - with thanks to 
//  <http://www.gnu.org/software/libtool/manual/libc/Termination-in-Handler.html#Termination-in-Handler>

volatile sig_atomic_t fatal_error_in_progress = 0;
     
void fatal_error_signal (int sig)
{
    // Since this handler is established for more than one kind of signal, 
    // it might still get invoked recursively by delivery of some other kind
    // of signal.  Use a static variable to keep track of that.
    if (fatal_error_in_progress)
        raise (sig);
 
    fatal_error_in_progress = 1;

    printf("You're killing me!!!\n");
     
    // Kill any child processes
    SireBase::Process::killAll();

    printf("\nI, and all of my children are now dead. Adieu...\n");

    // Now reraise the signal.  We reactivate the signal's
    // default handling, which is to terminate the process.
    // We could just call exit or abort,
    // but reraising the signal sets the return status
    // from the process correctly.
    signal (sig, SIG_DFL);
    raise (sig);
}

#endif // Q_OS_UNIX

int main(int argc, char **argv)
{
    int status = 0;

    try
    {

    //run through the command line arguments and filter out the ones we want
    boost::scoped_array<wchar_t*> python_argv( new wchar_t*[argc] );
    int python_argc = 0;

    bool ignore_pythonpath = false;
    int ppn = 1;

    QList< std::wstring > warg_strings;

    for (int i=0; i<argc; ++i)
    {
        QString arg = argv[i];
        //qDebug() << "ARG" << i << arg;

        if (arg.startsWith("--ppn"))
        {
        }
        else if (arg == "--ignore-pythonpath")
        {
            ignore_pythonpath = true;
        }
        else
        {
            warg_strings.append(arg.toStdWString());
            python_argv[python_argc] = const_cast<wchar_t*>(warg_strings.last().data());
            python_argc += 1;
        }
    }

    #ifdef Q_OS_UNIX
        signal(SIGINT, fatal_error_signal);
        signal(SIGTERM, fatal_error_signal);
    #endif // Q_OS_UNIX

    QDir site_packages( QString("%1/%2").arg( getInstallDir(), SIRE_PYTHON2_DIR ) );

    if (not site_packages.exists())
        throw SireError::file_error( QObject::tr(
            "Cannot find the directory containing the Sire python modules (%1). "
            "Please check your installation of Sire in directory %2.")
                .arg(site_packages.absolutePath()).arg(getInstallDir()), CODELOC );

    QString pythonpath;

    if (not ignore_pythonpath)
        pythonpath = qgetenv("PYTHONPATH");

    if (pythonpath.isEmpty())
        pythonpath = site_packages.canonicalPath();
    else
        pythonpath = QString("%1:%2").arg(site_packages.canonicalPath()).arg(pythonpath);

    qDebug() << "Setting PYTHONPATH to" << pythonpath;

    qputenv("PYTHONPATH", pythonpath.toUtf8());

    printf("Starting %ls: number of threads equals %d\n", python_argv[0], ppn);

    //name this process and thread
    SireError::setProcessString("master");
    SireError::setThreadString("main");

    // run the standard python interpreter
    qDebug() << "Entering python main...";
    status = Py_Main(python_argc, python_argv.get());
    qDebug() << "Leaving python main...";

    }
    catch(const SireError::exception &e)
    {
        SireError::printError(e);
        status = -1;
    }
    catch(const std::exception &e)
    {
        SireError::printError( SireError::std_exception(e) );
        status = -1;
    }
    catch(...)
    {
        SireError::printError(SireError::unknown_exception(
                                 QObject::tr("An unknown error occurred!"), CODELOC ) );

        status = -1;
    }

    SireBase::Process::killAll();

    return status;
}
