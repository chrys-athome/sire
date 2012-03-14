/********************************************\
  *
  *  Conspire/GUI
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "Conspire/GUI/optionswidget.h"

#include "Conspire/conspire.h"
#include "Conspire/option.h"
#include "Conspire/exceptions.h"

#include <QApplication>
#include <QMainWindow>

using namespace Conspire;

int main(int argc, char **argv)
{
    try
    {
        QApplication a(argc, argv);
        
        QMainWindow *m = new QMainWindow(0);

        Options opts;

        if (argc > 1)
        {
            QString xmlfile = argv[1];
            
            QStringList path;
            
            if (argc > 2)
            {
                for (int i=2; i<argc; ++i)
                {
                    path.append( QString(argv[i]) );
                }
            }
            
            conspireDebug() << xmlfile;
            conspireDebug() << path;
            
            opts = Options::fromXMLFile(xmlfile, path);
        }

        conspireDebug() << opts.toString();
        
        OptionsWidget *view = new OptionsWidget(opts, m);

        m->setCentralWidget(view);

        m->show();
        
        a.exec();
        
        delete m;
    }
    catch (const Conspire::Exception &e)
    {
        conspireDebug() << e.toString();
    }
    catch(const std::exception &e)
    {
        conspireDebug() << Conspire::tr("Caught standard exception: %1")
                                            .arg(e.what());
    }
    catch (...)
    {
        conspireDebug() << Conspire::tr("Caught unknown exception!");
    }
    
    return 0;
}
