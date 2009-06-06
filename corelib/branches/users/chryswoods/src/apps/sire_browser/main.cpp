
#include <QApplication>

#include <QWebView>
#include <QWebPage>
#include <QWebSettings>

#include <QUrl>

#include "pluginfactory.h"

#include <QDebug>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    if (argc > 1)
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

        return a.exec();
    }
    else
        return 0;
}
