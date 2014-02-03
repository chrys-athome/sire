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

#include "cljcalculator.h"
#include "cljfunction.h"
#include "cljatoms.h"

using namespace SireMM;
using namespace SireBase;
using namespace SireStream;
using namespace SireError;

using boost::tuple;

/** Constructor */
CLJCalculator::CLJCalculator()
{}

/** Copy constructor */
CLJCalculator::CLJCalculator(const CLJCalculator&)
{}

/** Destructor */
CLJCalculator::~CLJCalculator()
{}

/** Copy assignment operator */
CLJCalculator& CLJCalculator::operator=(const CLJCalculator&)
{
    return *this;
}

/** Comarison operator */
bool CLJCalculator::operator==(const CLJCalculator&) const
{
    return true;
}

/** Comparison operator */
bool CLJCalculator::operator!=(const CLJCalculator&) const
{
    return false;
}

const char* CLJCalculator::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJCalculator>() );
}

const char* CLJCalculator::what() const
{
    return CLJCalculator::typeName();
}

QString CLJCalculator::toString() const
{
    return QObject::tr("CLJCalculator()");
}

/** Calculate the energy between all of the atoms in the passed CLJBoxes
    using the passed CLJFunction, returning
    the coulomb and LJ energy as a tuple (coulomb,lj) */
tuple<double,double> CLJCalculator::calculate(const CLJFunction &func, const CLJBoxes &boxes)
{
    return tuple<double,double>(0,0);
}

/** Calculate the energy between all of the atoms in the passed CLJBoxes
    using the passed array of CLJFunctions, returning the energies as
    a tuple of arrays of the coulomb and LJ energy (coulomb,lj) */
tuple< QVector<double>, QVector<double> > CLJCalculator::calculate(
                                const QVector< boost::shared_ptr<CLJFunction> > &funcs,
                                const CLJBoxes &boxes)
{
    QVector<double> cnrgs;
    QVector<double> ljnrgs;
    
    if (funcs.count() == 1)
    {
        tuple<double,double> nrgs = calculate( *(funcs.at(0)), boxes );
        cnrgs = QVector<double>(1, nrgs.get<0>());
        ljnrgs = QVector<double>(1, nrgs.get<1>());
    }
    else if (funcs.count() > 1)
    {
        for (int i=0; i<funcs.count(); ++i)
        {
            tuple<double,double> nrgs = calculate( *(funcs.at(i)), boxes );
            cnrgs.append( nrgs.get<0>() );
            ljnrgs.append( nrgs.get<1>() );
        }
    }
    
    return tuple< QVector<double>,QVector<double> >(cnrgs, ljnrgs);
}
