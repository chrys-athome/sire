
#include <QApplication>
#include <QDebug>

#include "Spier/mainwindow.h"
#include "SireError/errors.h"

using namespace Spier;

/** Main function for the spier molecular editor program */
int main(int argc, char **argv)
{
    try
    {
        QApplication *app = new QApplication(argc,argv);
    
        MainWindow *spierwin;
    
        try
        {
            spierwin = new MainWindow();
        }
        catch(int exitcode)
        {
            //we need to exit the program
            return exitcode;
        }
    
        spierwin->show();
    
        return app->exec();
    }
    catch(SireError::exception &e)
    {
        qDebug() << QObject::tr("Caught fatal exception. Program aborting!\n%1")
                                  .arg(e.toString());
    }
    catch(std::exception &e)
    {
        SireError::std_exception stdexcept(e);
        qDebug() << QObject::tr("Caught fatal exception. Program aborting!\n%1")
                          .arg(stdexcept.toString());
    }
    catch(...)
    {
        qDebug() << QObject::tr("Caught an unknown exception! Program aborting!");
    }          
}
