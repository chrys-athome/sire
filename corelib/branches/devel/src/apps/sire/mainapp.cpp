
#include <QCoreApplication>
#include <QDebug>

#include "mainapp.h"

#include "SirePy/pythonparser.h"
#include "SirePy/pythonscript.h"

#include "SireError/errors.h"

using namespace SirePy;

MainApp::MainApp() : QObject(), parser(0)
{
    this->initialise();
}

MainApp::~MainApp()
{}

/** Initialise the object */
void MainApp::initialise()
{
    //create the Python parser 
    parser = new PythonParser(this);
    connect(parser,SIGNAL(parseError()),this,SLOT(shutdown()));
    connect(parser,SIGNAL(parseComplete()),this,SLOT(shutdown()));
}

/** Run the list of python files */
void MainApp::runPython(const QStringList &files)
{
    if (not parser)
    {
        qDebug() << QObject::tr("There is no python parser with which to "
                                      "run the script!");
        return;
    }

    boost::shared_ptr<PythonScript> script(new PythonScript());
    
    foreach (QString file, files)
        script->addFile(file);
        
    parser->run(script);
}

/** Shutdown the application */
void MainApp::shutdown()
{
    qDebug() << "MainApp::shutdown()";
    QCoreApplication::instance()->quit();    
}
