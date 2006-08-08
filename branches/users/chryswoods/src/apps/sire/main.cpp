
#include "mainapp.h"

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <boost/shared_array.hpp>

#include <stdio.h>
#include <getopt.h>

#include "SireError/errors.h"

/** Parse the command line arguments, and return the processed arguments in 
    a hash of PolyTypePtrs. The hash is indexed by the name of the recognised
    arguments. Any unrecognised arguments are placed in a PolyType<QStringList>,
    indexed under 'unrecognised'. */
QStringList parseCmdArgs(int argc, char **argv)
{
    //now parse the command line options to see what we have to do...
    int option_char;
    //we don't want any standard error messages...
    opterr = 0;

    //we will also accept long options - we need to create an array of long
    //options that can be processed...
    boost::shared_array<option> longopts(new option[10]);
    
    //use the 'idx' variable to keep track of the option index...
    int idx = 0;
            
    //add an option to parse a file (--file or -f)
    ++idx;
    longopts[idx].name = "file";
    longopts[idx].has_arg = required_argument;
    longopts[idx].flag = 0;
    longopts[idx].val = 'f';
    
    //need to terminate array with a null long option
    ++idx;
    longopts[idx].name = 0;
    longopts[idx].has_arg = 0;
    longopts[idx].flag = 0;
    longopts[idx].val = 0;

    //a list of all files
    QStringList files;
    
    //reset the option index to 0, and error to 1 - this is necessary
    //to allow the options to be parsed multiple times.
    optind = 0;
    opterr = 1;
    
    while ( (option_char = getopt_long(argc,argv,"f:",longopts.get(),&idx)) != -1 )
    {
        QString argmnt(optarg);
        QString optn(argv[optind-1]);
    
        switch (option_char)
        {
            case 'f':
                files.append(argmnt);
                break;
            case '?':
                break;
            default:
                break;
        }
    }
        
    if (optind < argc)
    {
        //there are still non-getopt options left to process. We will assume that these
        //are files that should be processed...
        for (int i=optind; i < argc; i++)
        {
            QString optn = QString(argv[i]);
            files.append(optn);
        }
    }

    //return the files
    return files;
}

#include <boost/test/included/prg_exec_monitor.hpp>

/** Use the boost-test program execution monitor - this will catch all
    errors that result in program termination and will translate them
    into a uniform output return code. To use the monitor, the 'main' 
    function below has been renamed to 'cpp_main'. The monitor
    is enabled by linking this program to the boost_test_exec_monitor
    library - this is really just the files execution_monitor.cpp and
    cpp_main.cpp compiled together. If you would prefer not to use the
    exectution monitor, then rename the function below from 'cpp_main' 
    back to 'main' and don't link to the boost_test_exec_monitor library
    (by editing sire.pro to remove this from the list of libraries)
*/
int cpp_main(int argc, char **argv)
{
   
    //create the Qt application that has the event loop        
    QCoreApplication *app = new QCoreApplication(argc,argv);
   
    QStringList files = parseCmdArgs(app->argc(), app->argv());
    
    try
    {
        //create a main Sire application - this will start running once
        //the Qt event loop is started
        MainApp *mainapp = new MainApp();
        
        if (not files.isEmpty())
            mainapp->runPython(files);
    
        //wait until the program is correctly exited
        int exitval = app->exec();
        
        qDebug() << QObject::tr("Sire exited with return value = %1").arg(exitval);
        
        return exitval;
    }
    catch( SireError::exception &error )
    {
        qDebug() << error.toString();
        
        throw;
    }
    catch( std::exception &error )
    {
        qDebug() << SireError::std_exception(error).toString();
        
        throw;
    }
    catch( ... )
    {
        SireError::unknown_exception error(QObject::tr("Caught an unknown exception!"));
        qDebug() << error.toString();
        
        throw error;
    }
    
}
