
#include <QApplication>

#include <QWebView>
#include <QWebPage>
#include <QWebSettings>

#include <QUrl>

#include "pluginfactory.h"


#include "SireError/exception.h"
#include "SireError/printerror.h"

#include <QDebug>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    if (argc > 1)
    {
        try
        {
            QWebSettings::globalSettings()->setAttribute( 
                                                QWebSettings::PluginsEnabled, true );
    
            QWebSettings::globalSettings()->setAttribute( 
                                                QWebSettings::JavascriptEnabled, true );
    
            QWebView *w = new QWebView();
            w->page()->setPluginFactory( new PluginFactory() );

            qDebug() << "LOAD" << QUrl(argv[1]);
            w->load( QUrl(argv[1]) );
            qDebug() << "DONE!";

            w->show();
        }
        catch(const SireError::exception &e)
        {
            SireError::printError(e);
            return -1;
        }

        try
        {
            return a.exec();
        }
        catch(const SireError::exception &e)
        {
            SireError::printError(e);
            return -1;
        }
    }
    else
        return 0;
}
