/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "testff.h"

#include "SireUnits/units.h"

#include <QDebug>
#include <QElapsedTimer>

using namespace SireMM;
using namespace SireMol;
using namespace SireMaths;
using namespace SireUnits;

TestFF::TestFF() : cljfunc(new CLJVacShiftAriFunction(50*angstrom, 50*angstrom))
{}

TestFF::TestFF(const TestFF &other)
       : atoms0(other.atoms0), atoms1(other.atoms1), cljfunc(other.cljfunc)
{}

TestFF::~TestFF()
{}

TestFF& TestFF::operator=(const TestFF &other)
{
    atoms0 = other.atoms0;
    atoms1 = other.atoms1;
    cljfunc = other.cljfunc;
    return *this;
}

void TestFF::add(const Molecules &molecules)
{
    atoms0 = CLJAtoms(molecules);
}

void TestFF::addFixedAtoms(const Molecules &molecules)
{
    atoms1 = CLJAtoms(molecules);
}

void TestFF::calculateEnergy()
{
    QElapsedTimer t;
    t.start();

    double cnrg;
    double ljnrg;
    (*cljfunc)(atoms0, atoms1, cnrg, ljnrg);

    quint64 ns = t.nsecsElapsed();

    qDebug() << "TestFF" << (cnrg+ljnrg) << cnrg << ljnrg << "took" << (0.000001*ns) << "ms";
}
