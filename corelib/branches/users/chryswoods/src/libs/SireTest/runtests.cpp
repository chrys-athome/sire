/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "SireCAS/qhash_sirecas.h"

#include "runtests.h"

#include <QObject>
#include <QTime>

#include <QDebug>

#include "SireCAS/trigfuncs.h"
#include "SireCAS/values.h"

using namespace SireTest;
using namespace SireMaths;
using namespace SireCAS;

RunTests::RunTests()
{}

RunTests::~RunTests()
{}

void RunTests::run()
{
    qDebug() << QObject::tr("Running personal tests...");

    qDebug() << QObject::tr("Time tests for SireMaths expressions");
    
    QTime t;
    
    //now test with SireCAS
    Symbol x("x");
    Expression ex = (1.0 + Cos(x)) + 0.5*(1 + Cos(2*x))
                       - 0.25*(1 + Cos(3*x));
    
    qDebug() << ex.toString();
    qDebug() << ex.differentiate(x).toString();

    double a = 0.0;
    
    t.start();
    Values vals;
    
    for (int i=0; i<1000000; ++i)
    {
        vals.set(x, 0.01*i);
        a += ex( vals );
    }

    int ms = t.elapsed();
    
    qDebug() << QObject::tr("a = %1, ms = %2").arg(a).arg(ms);

    //temporary testing of structural alignment...
    
}

