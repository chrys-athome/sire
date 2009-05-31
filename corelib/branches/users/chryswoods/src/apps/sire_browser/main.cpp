
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

        w->load( QUrl(argv[1]) );

        w->show();

        return a.exec();
    }
    else
        return 0;
}
