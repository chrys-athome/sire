/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2013  Christopher Woods
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

#include "ti.h"

#include "SireID/index.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Soiree;
using namespace SireMaths;
using namespace SireID;
using namespace SireBase;
using namespace SireStream;
using namespace SireUnits::Dimension;

static const RegisterMetaType<TI> r_ti;

QDataStream SOIREE_EXPORT &operator<<(QDataStream &ds, const TI &ti)
{
    writeHeader(ds, r_ti, 1);
    
    SharedDataStream sds(ds);
    sds << ti.delta_lams << ti.fwds_grads << ti.bwds_grads;
    
    return ds;
}

QDataStream SOIREE_EXPORT &operator>>(QDataStream &ds, TI &ti)
{
    VersionID v = readHeader(ds, r_ti);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> ti.delta_lams >> ti.fwds_grads >> ti.bwds_grads;
    }
    else
        throw version_error(v, "1", r_ti, CODELOC);
    
    return ds;
}

/** Null constructor */
TI::TI() : ConcreteProperty<TI,Property>()
{}

/** Copy constructor */
TI::TI(const TI &other) : ConcreteProperty<TI,Property>(other),
                          delta_lams(other.delta_lams),
                          fwds_grads(other.fwds_grads),
                          bwds_grads(other.bwds_grads)
{}

/** Destructor */
TI::~TI()
{}

/** Copy assignment operator */
TI& TI::operator=(const TI &other)
{
    if (this != &other)
    {
        delta_lams = other.delta_lams;
        fwds_grads = other.fwds_grads;
        bwds_grads = other.bwds_grads;
    }
    
    return *this;
}

/** Comparison operator */
bool TI::operator==(const TI &other) const
{
    return this == &other or
           (delta_lams == other.delta_lams and
            fwds_grads == other.fwds_grads and
            bwds_grads == other.bwds_grads);
}

/** Comparison operator */
bool TI::operator!=(const TI &other) const
{
    return not TI::operator==(other);
}

const char* TI::typeName()
{
    return QMetaType::typeName( qMetaTypeId<TI>() );
}

const char* TI::what() const
{
    return TI::typeName();
}

/** Add the passed free energy gradients to the TI calculation. The gradients
    are in a dictionary, indexed by lambda value, and are "pure" TI gradients, 
    i.e. they have been calculated exactly with an infinitesimal delta lambda */
void TI::add(const QMap<double,FreeEnergyAverage> &gradients)
{
    if (not gradients.isEmpty())
    {
        delta_lams.append( 0 );
        fwds_grads.append( gradients );
        bwds_grads.append( gradients );
    }
}

/** Add the passed free energy gradients to the TI calcualtion. The gradients
    are in a dictionary, indexed by lambda value, and are the raw free energies
    calculated via the zwanzig equation as part of a finite-difference TI calculation.
    The value of delta lambda used must also be passed (so that we can then divide
    each gradient by delta lambda to get an approximation of the gradient) */
void TI::add(const QMap<double,FreeEnergyAverage> &gradients,
             double delta_lambda)
{
    if (not gradients.isEmpty())
    {
        if (delta_lambda <= 0)
            delta_lams.append(0);
        else
            delta_lams.append(delta_lambda);

        fwds_grads.append(gradients);
        bwds_grads.append(gradients);
    }
}

/** Add the passed free energy gradients to the TI calculation. The gradients
    are in dictionaries, indexed by lambda values, and are the raw forwards and
    backwards free energies calculated via the zwanzig equation as part of 
    a finite-difference TI calculation (backwards gradients calculated as the
    free energy from lambda-delta_lambda -> lambda, while forwards gradients calculated as the
    difference between lambda -> lambda+delta_lambda). The value of delta lambda
    must be passed (so that we can then divide each gradient by delta lambda to get 
    an approximation of the true gradient) */
void TI::add(const QMap<double,FreeEnergyAverage> &forwards,
             const QMap<double,FreeEnergyAverage> &backwards,
             double delta_lambda)
{
    if (forwards.isEmpty())
    {
        if (not backwards.isEmpty())
        {
            if (delta_lambda <= 0)
                delta_lams.append(0);
            else
                delta_lams.append(delta_lambda);

            fwds_grads.append(backwards);
            bwds_grads.append(backwards);
        }
    }
    else if (backwards.isEmpty())
    {
        if (not forwards.isEmpty())
        {
            if (delta_lambda <= 0)
                delta_lams.append(0);
            else
                delta_lams.append(delta_lambda);

            fwds_grads.append(forwards);
            bwds_grads.append(backwards);
        }
    }
    else
    {
        if (delta_lambda <= 0)
            delta_lams.append(0);
        else
            delta_lams.append(delta_lambda);

        fwds_grads.append(forwards);
        bwds_grads.append(backwards);
    }
}

/** Return the number of iterations (number of sets of gradients that have been added) */
int TI::nIterations() const
{
    return delta_lams.count();
}

/** Return all values of lambda that have data. The values are returned 
    in numerical order */
QList<double> TI::lambdaValues() const
{
    QMap<double,int> vals;
    
    for (int i=0; i<fwds_grads.count(); ++i)
    {
        foreach (double lam, fwds_grads[i].keys())
        {
            vals.insert(lam, 0);
        }
        
        foreach (double lam, bwds_grads[i].keys())
        {
            vals.insert(lam, 0);
        }
    }
    
    QList<double> lams = vals.keys();
    qSort(lams);
    
    return lams;
}

/** Return the number of lambda values */
int TI::nLambdaValues() const
{
    return lambdaValues().count();
}

static qint64 count( const QMap<double,FreeEnergyAverage> &grads )
{
    qint64 n = 0;
    
    for (QMap<double,FreeEnergyAverage>::const_iterator it = grads.constBegin();
         it != grads.constEnd();
         ++it)
    {
        n += it.value().nSamples();
    }
    
    return n;
}

/** Return the total number of samples in this calculation */
qint64 TI::nSamples() const
{
    qint64 n = 0;
    
    for (int i=0; i<delta_lams.count(); ++i)
    {
        if (delta_lams[i] == 0)
        {
            n += ::count(fwds_grads[i]);
        }
        else if (fwds_grads[i] == bwds_grads[i])
        {
            n += ::count(fwds_grads[i]);
        }
        else
        {
            n += ::count(fwds_grads[i]);
            n += ::count(bwds_grads[i]);
        }
    }
    
    return n;
}

/** Return the number of iterations */
int TI::count() const
{
    return nIterations();
}

/** Return the number of iterations */
int TI::size() const
{
    return nIterations();
}

/** Return the free energy gradient data for the ith iteration. This returns
    a tuple of the forwards gradients, backwards gradients and the value
    of delta lambda. Note that for pure TI calculations, the forwards and
    backwards gradients will be equal and the value of delta lambda will be 0 */
boost::tuple< QMap<double,FreeEnergyAverage>,
              QMap<double,FreeEnergyAverage>,
              double> TI::operator[](int i) const
{
    i = Index(i).map(delta_lams.count());

    return boost::tuple< QMap<double,FreeEnergyAverage>,
                         QMap<double,FreeEnergyAverage>,
                         double>( fwds_grads.at(i), bwds_grads.at(i),
                                  delta_lams.at(i) );
}

/** Return the free energy gradient data for the ith iteration. This returns
    a tuple of the forwards gradients, backwards gradients and the value
    of delta lambda. Note that for pure TI calculations, the forwards and
    backwards gradients will be equal and the value of delta lambda will be 0 */
boost::tuple< QMap<double,FreeEnergyAverage>,
              QMap<double,FreeEnergyAverage>,
              double> TI::at(int i) const
{
    return operator[](i);
}

/** Set the gradients for the ith iteration equal to 'gradients'. These
    must be pure TI gradients, with no associated delta lambda value */
void TI::set(int i, const QMap<double,FreeEnergyAverage> &gradients)
{
    if (gradients.isEmpty())
        return;

    if (i == delta_lams.count())
        this->add(gradients);
    else
    {
        i = Index(i).map(delta_lams.count());
        
        delta_lams[i] = 0;
        fwds_grads[i] = gradients;
        bwds_grads[i] = gradients;
    }
}

/** Set the gradients for the ith iteration equal to 'gradients'. These
    are finite difference TI gradients, which are the raw zwanzig
    free energies together with the passed value of delta lambda */
void TI::set(int i, const QMap<double,FreeEnergyAverage> &gradients,
             double delta_lambda)
{
    if (gradients.isEmpty())
        return;
    
    if (i == delta_lams.count())
        this->add(gradients, delta_lambda);
    else
    {
        i = Index(i).map(delta_lams.count());
        
        if (delta_lambda <= 0)
            delta_lams[i] = 0;
        else
            delta_lams[i] = delta_lambda;

        fwds_grads[i] = gradients;
        bwds_grads[i] = gradients;
    }
}

/** Set the gradients for the ith iteration to the passed forwards
    and backwards finite difference TI values (together with associated
    delta lambda). The forwards gradients should be the raw zwanzig
    values from lambda -> lambda+delta_lambda, while the backwards
    gradients should be the raw zwanzig values from lambda-delta_lambda -> lambda */
void TI::set(int i, const QMap<double,FreeEnergyAverage> &forwards,
                    const QMap<double,FreeEnergyAverage> &backwards,
                    double delta_lambda)
{
    if (forwards.isEmpty() and backwards.isEmpty())
        return;
    
    else if (i == delta_lams.count())
    {
        this->add(forwards, backwards, delta_lambda);
        return;
    }
    else
    {
        i = Index(i).map(delta_lams.count());
    
        if (forwards.isEmpty())
        {
            fwds_grads[i] = backwards;
            bwds_grads[i] = backwards;
        }
        else if (backwards.isEmpty())
        {
            fwds_grads[i] = forwards;
            bwds_grads[i] = forwards;
        }
        else
        {
            fwds_grads[i] = forwards;
            bwds_grads[i] = backwards;
        }
        
        if (delta_lambda <= 0)
            delta_lams[i] = 0;
        else
            delta_lams[i] = delta_lambda;
    }
}

/** Remove the data for iteration 'i' */
void TI::removeAt(int i)
{
    i = Index(i).map(delta_lams.count());
    
    delta_lams.removeAt(i);
    fwds_grads.removeAt(i);
    bwds_grads.removeAt(i);
}

/** Remove every iteration from 'start' to 'end' (inclusively) */
void TI::removeRange(int start, int end)
{
    start = Index(start).map(delta_lams.count());
    end = Index(end).map(delta_lams.count());
    
    if (start > end)
        qSwap(start, end);
    
    for (int i = start; i <= end; ++i)
    {
        delta_lams.removeAt(start);
        fwds_grads.removeAt(start);
        bwds_grads.removeAt(start);
    }
}

QString TI::toString() const
{
    return QObject::tr( "TI( nLambdaValues() == %1, nIterations() == %2, nSamples() == %3 )" )
                .arg(nLambdaValues()).arg(nIterations()).arg(nSamples());
}
