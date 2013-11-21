
#include "qcustomplot.h"

#include <QApplication>
#include <QMainWindow>

#include "SireStream/streamdata.hpp"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireStream;

int main(int argc, char **argv)
{
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }

    QString file_to_load;

    if (argc > 0)
    {
        //load up the first s3 file in the argument list
        for (int i=1; i<argc; ++i)
        {
            QString arg(argv[i]);
            
            try
            {
                qDebug() << "Testing argument" << arg;
                FileHeader header = getDataHeader(arg);
                file_to_load = arg;
                qDebug() << "This is a valid s3 file :-)";
                qDebug() << header.requiredLibraries();
                qDebug() << header.dataTypes();
                break;
            }
            catch(...)
            {
                qDebug() << "Not a valid s3 file :-(";
            }
        }
    }

    if (not file_to_load.isEmpty())
    {
        qDebug() << "Loading" << file_to_load;
        
        QList< boost::tuple<boost::shared_ptr<void>,QString> > objects;
        
        try
        {
            objects = load(file_to_load);
        }
        catch(const SireError::exception &e)
        {
            qDebug() << e.toString();
            return -1;
        }
        catch(...)
        {
            qDebug() << "Caught an unknown exception!";
            return -1;
        }
        
        qDebug() << "Loaded" << objects.count() << "objects";
        
        for (int i=0; i<objects.count(); ++i)
        {
            qDebug() << (i+1) << objects.at(i).get<1>();
            
            int typ = QMetaType::type(objects.at(i).get<1>().toUtf8());
            
            qDebug() << QMetaType::typeName(typ) << QMetaType::isRegistered(typ);
            
            QMetaType meta(typ);
            
            qDebug() << meta.isValid() << meta.sizeOf(typ);
        }
        
        return 0;
    }
    
    QApplication *a = new QApplication(argc, argv);

    QCustomPlot *customPlot = new QCustomPlot();
    
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();

    customPlot->setWindowTitle("inquire");
    customPlot->resize(600,600);

    customPlot->show();

    return a->exec();
}
