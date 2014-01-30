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

#include "SireVol/cartesian.h"

#include "SireUnits/units.h"

#include <QDebug>
#include <QElapsedTimer>

#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

#include "tostring.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireMaths;
using namespace SireUnits;
using namespace SireVol;

TestFF::TestFF() : cljfunc(new CLJVacShiftAriFunction(15*angstrom, 15*angstrom))
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
    cljboxes0 = CLJBoxes(atoms0);
}

void TestFF::addFixedAtoms(const Molecules &molecules)
{
    atoms1 = CLJAtoms(molecules);
    cljboxes1 = CLJBoxes(atoms1);
}

void TestFF::setCutoff(Length coul_cutoff, Length lj_cutoff)
{
    cljfunc.reset( new CLJVacShiftAriFunction(coul_cutoff, lj_cutoff) );
}

class CLJCalculator
{
public:
    CLJCalculator();
    CLJCalculator(const CLJFunction *function,
                  const QList<CLJBoxDistance> &distances,
                  const QMap<CLJBoxIndex,CLJBoxPtr> &cljboxes,
                  const float coulomb_cutoff, const float lenj_cutoff,
                  double *coulomb_energy, double *lj_energy)
        : func(function), dists(&distances), boxes(&cljboxes),
          coul_nrg(coulomb_energy), lj_nrg(lj_energy),
          coul_cutoff(coulomb_cutoff), lj_cutoff(lenj_cutoff)
    {}
    
    ~CLJCalculator()
    {}
    
    void operator()(const tbb::blocked_range<int> &range) const
    {
        for (int i = range.begin(); i != range.end(); ++i)
        {
            const CLJBoxDistance &d = dists->at(i);
            
            if (d.box0() == d.box1())
            {
                (*func)(boxes->constFind(d.box0()).value().read().atoms(), coul_nrg[i], lj_nrg[i]);
            }
            else
            {
                (*func)(boxes->constFind(d.box0()).value().read().atoms(),
                        boxes->constFind(d.box1()).value().read().atoms(),
                        coul_nrg[i], lj_nrg[i]);
            }
        }
    }
    
private:
    const CLJFunction* const func;
    const QList<CLJBoxDistance>* const dists;
    const QMap<CLJBoxIndex,CLJBoxPtr>* boxes;
    
    double *coul_nrg;
    double *lj_nrg;
    
    const float coul_cutoff;
    const float lj_cutoff;
};

void TestFF::calculateEnergy()
{
    QElapsedTimer t;

    double cnrg;
    double ljnrg;

    qDebug() << "inter group energy";

    t.start();

    (*cljfunc)(atoms0, atoms1, cnrg, ljnrg);

    quint64 ns = t.nsecsElapsed();

    qDebug() << "TestFF" << (cnrg+ljnrg) << cnrg << ljnrg << "took" << (0.000001*ns) << "ms";

    qDebug() << "\nintra group energy";
    
    t.start();
    
    (*cljfunc)(atoms1, cnrg, ljnrg);
    
    ns = t.nsecsElapsed();
    
    qDebug() << "TestFF" << (cnrg+ljnrg) << cnrg << ljnrg << "took" << (0.000001*ns) << "ms";

    qDebug() << "\nUsing CLJBoxes to accelerate the calculation";
    
    Cartesian space;
    
    Length coul_cutoff = cljfunc->coulombCutoff();
    Length lj_cutoff = cljfunc->ljCutoff();
    
    QList<CLJBoxDistance> dists;

    const QMap<CLJBoxIndex,CLJBoxPtr> &boxes0 = cljboxes0.occupiedBoxes();
    const QMap<CLJBoxIndex,CLJBoxPtr> &boxes1 = cljboxes1.occupiedBoxes();
    
    qDebug() << "inter energy";

    double icnrg, iljnrg;

    dists = CLJBoxes::getDistances(space, cljboxes0, cljboxes1, coul_cutoff);
    
    cnrg = 0;
    ljnrg = 0;
    
    t.start();
    
    for (QList<CLJBoxDistance>::const_iterator it = dists.constBegin();
         it != dists.constEnd();
         ++it)
    {
        const CLJBoxDistance &d = *it;
        
        (*cljfunc)(boxes0[d.box0()].read().atoms(), boxes1[d.box1()].read().atoms(),
                   icnrg, iljnrg);
        
        cnrg += icnrg;
        ljnrg += iljnrg;
    }
    
    ns = t.nsecsElapsed();
    
    qDebug() << "Boxed" << (cnrg+ljnrg) << cnrg << ljnrg;
    qDebug() << "Took" << (0.000001*ns) << "ms";

    qDebug() << "\nintra energy";
    
    dists = CLJBoxes::getDistances(space, cljboxes1, coul_cutoff);
    
    cnrg = 0;
    ljnrg = 0;
    
    t.start();
    
    for (QList<CLJBoxDistance>::const_iterator it = dists.constBegin();
         it != dists.constEnd();
         ++it)
    {
        const CLJBoxDistance &d = *it;
        
        if (d.box0() == d.box1())
        {
            (*cljfunc)(boxes1[d.box0()].read().atoms(), icnrg, iljnrg);
        }
        else
        {
            (*cljfunc)(boxes1[d.box0()].read().atoms(), boxes1[d.box1()].read().atoms(),
                       icnrg, iljnrg);
        }

        cnrg += icnrg;
        ljnrg += iljnrg;
    }
    
    ns = t.nsecsElapsed();
    
    qDebug() << "Boxed" << (cnrg+ljnrg) << cnrg << ljnrg;
    qDebug() << "Took" << (0.000001*ns) << "ms";

    // parallel implementation
    tbb::task_scheduler_init init;
    
    QVector<double> coul_nrgs(dists.count());
    QVector<double> lj_nrgs(dists.count());
    
    CLJCalculator calc(cljfunc.get(), dists, boxes1, coul_cutoff.value(),
                       lj_cutoff.value(), coul_nrgs.data(), lj_nrgs.data());
    
    t.start();
    tbb::parallel_for(tbb::blocked_range<int>(0,dists.count()), calc);
    
    cnrg = 0;
    ljnrg = 0;
    
    for (int i=0; i<dists.count(); ++i)
    {
        cnrg += coul_nrgs.constData()[i];
        ljnrg += lj_nrgs.constData()[i];
    }
    
    ns = t.nsecsElapsed();
    
    qDebug() << "\nParallel" << (cnrg+ljnrg) << cnrg << ljnrg;
    qDebug() << "Parallel version took" << (0.000001*ns) << "ms";
}
