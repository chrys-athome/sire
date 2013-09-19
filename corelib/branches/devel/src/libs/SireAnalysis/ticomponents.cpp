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

#include "ticomponents.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireAnalysis;
using namespace SireSystem;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of ComponentGradients
//////////

static const RegisterMetaType<ComponentGradients> r_grads;

//////////
////////// Implementation of TIComponents
//////////

static const RegisterMetaType<TIComponents> r_ti;

QDataStream SIREANALYSIS_EXPORT &operator<<(QDataStream &ds, const TIComponents &ti)
{
    writeHeader(ds, r_ti, 1);
    
    SharedDataStream sds(ds);
    sds << ti.grads;
    
    return ds;
}

QDataStream SIREANALYSIS_EXPORT &operator>>(QDataStream &ds, TIComponents &ti)
{
    VersionID v = readHeader(ds, r_ti);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> ti.grads;
    }
    else
        throw version_error(v, "1", r_ti, CODELOC);
    
    return ds;
}

/** Constructor */
TIComponents::TIComponents() : ConcreteProperty<TIComponents,Property>()
{}

/** Construct from a single iteration's worth of gradients */
TIComponents::TIComponents(const QMap<double,FreeEnergyMonitor> &gradients)
             : ConcreteProperty<TIComponents,Property>()
{
    grads.append( ComponentGradients(gradients) );
}

/** Construct from a single iteration's worth of gradients */
TIComponents::TIComponents(const ComponentGradients &gradients)
             : ConcreteProperty<TIComponents,Property>()
{
    grads.append(gradients);
}

/** Copy constructor */
TIComponents::TIComponents(const TIComponents &other)
             : ConcreteProperty<TIComponents,Property>(other),
               grads(other.grads)
{}

/** Destructor */
TIComponents::~TIComponents()
{}

/** Copy assignment operator */
TIComponents& TIComponents::operator=(const TIComponents &other)
{
    grads = other.grads;
    return *this;
}

/** Comparison operator */
bool TIComponents::operator==(const TIComponents &other) const
{
    return grads == other.grads;
}

/** Comparison operator */
bool TIComponents::operator!=(const TIComponents &other) const
{
    return not TIComponents::operator==(other);
}

const char* TIComponents::typeName()
{
    return QMetaType::typeName( qMetaTypeId<TIComponents>() );
}

const char* TIComponents::what() const
{
    return TIComponents::typeName();
}

QString TIComponents::toString() const
{
    return QObject::tr("TIComponents( nComponents() == %1, nIterations() == %2 "
                                     "nSamples() == %3, nLambdaValues() == %4 )")
                .arg(nComponents())
                .arg(nIterations())
                .arg(nSamples())
                .arg(nLambdaValues());
}

/** Set the gradients for the ith iteration. Note that these must be compatible
    with the gradients of the other iterations */
void TIComponents::set(int i, const ComponentGradients &gradients)
{
    if (not gradients.isEmpty())
    {
        //make sure it is compatible
        for (int i=0; i<grads.count(); ++i)
        {
            if (not grads.at(i).isEmpty())
            {
                if (not grads.at(i).isCompatible(gradients))
                    throw SireError::incompatible_error( QObject::tr(
                            "Cannot add the passed set of gradients as they are incompatible "
                            "with those that have already been added."), CODELOC );
                
                break;
            }
        }
    }
    
    while (i >= grads.count())
    {
        grads.append( ComponentGradients() );
    }
    
    grads[i] = gradients;
}

/** Set the gradients for the ith iteration. Note that these must be compatible
    with the gradients of the other iterations */
void TIComponents::set(int i, const QMap<double,FreeEnergyMonitor> &gradients)
{
    this->set(i, ComponentGradients(gradients));
}

/** Add the passed set of component gradients. Note that these must be compatible
    with any that are already in this set */
void TIComponents::add(const QMap<double,FreeEnergyMonitor> &gradients)
{
    return this->add( ComponentGradients(gradients) );
}

/** Add the passed set of component gradients. Note that these must be compatible
    with any that are already in this set */
void TIComponents::add(const ComponentGradients &gradients)
{
    return this->set( grads.count(), gradients );
}

/** Return the number of components in this collection (number of views) */
int TIComponents::nComponents() const
{
    if (grads.isEmpty())
        return 0;
    else
        return grads.at(0).nComponents();
}

/** Return the number of iterations */
int TIComponents::nIterations() const
{
    return grads.count();
}

/** Return the number of lambda values */
int TIComponents::nLambdaValues() const
{
    return lambdaValues().count();
}

/** Return the total number of samples */
qint64 TIComponents::nSamples() const
{
    qint64 n = 0;
    
    for (int i=0; i<grads.count(); ++i)
    {
        n += grads.at(i).nSamples();
    }
    
    return n;
}

/** Return the number of iterations */
int TIComponents::count() const
{
    return grads.count();
}

/** Return the number of iterations */
int TIComponents::size() const
{
    return grads.count();
}

/** Return a list of all lambda values that contain data */
QList<double> TIComponents::lambdaValues() const
{
    QMap<double,int> vals;
    
    for (int i=0; i<grads.count(); ++i)
    {
        foreach (double lam, grads.at(i).lambdaValues())
        {
            vals.insert(lam, 0);
        }
    }
    
    QList<double> lams = vals.keys();
    qSort(lams);
    
    return lams;
}

/** Return the ith set of ComponentGradients */
ComponentGradients TIComponents::operator[](int i) const
{
    return grads.at( Index(i).map(grads.count()) );
}

/** Return the ith set of ComponentGradients */
ComponentGradients TIComponents::at(int i) const
{
    return this->operator[](i);
}

/** Return the raw list of component gradients */
QList<ComponentGradients> TIComponents::gradients() const
{
    return grads;
}
