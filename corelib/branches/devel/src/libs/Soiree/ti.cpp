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

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <cmath>

#include "tostring.h"

using namespace Soiree;
using namespace SireMaths;
using namespace SireID;
using namespace SireBase;
using namespace SireStream;
using namespace SireUnits::Dimension;

/////////
///////// Implementation of DataPoint
/////////

static const RegisterMetaType<DataPoint> r_dp;

QDataStream SOIREE_EXPORT &operator<<(QDataStream &ds, const DataPoint &dp)
{
    writeHeader(ds, r_dp, 1);
    
    ds << dp._x << dp._y
       << dp._xminerr << dp._yminerr
       << dp._xmaxerr << dp._ymaxerr;
    
    return ds;
}

QDataStream SOIREE_EXPORT &operator>>(QDataStream &ds, DataPoint &dp)
{
    VersionID v = readHeader(ds, r_dp);
    
    if (v == 1)
    {
        ds >> dp._x >> dp._y
           >> dp._xminerr >> dp._yminerr
           >> dp._xmaxerr >> dp._ymaxerr;
    }
    else
        throw version_error(v, "1", r_dp, CODELOC);
    
    return ds;
}

/** Constructor. This constructs the point (0,0) with no error */
DataPoint::DataPoint()
          : _x(0), _y(0), _xminerr(0), _yminerr(0), _xmaxerr(0), _ymaxerr(0)
{}

/** Construct the point (x,y) with no error */
DataPoint::DataPoint(double x, double y)
          : _x(x), _y(y), _xminerr(0), _yminerr(0), _xmaxerr(0), _ymaxerr(0)
{}

/** Construct the point (x,y) with error (xerror,yerror) */
DataPoint::DataPoint(double x, double y, double xerror, double yerror)
          : _x(x), _y(y), _xminerr(std::abs(xerror)), _yminerr(std::abs(yerror)),
            _xmaxerr(std::abs(xerror)), _ymaxerr(std::abs(yerror))
{}

/** Construct the point (x,y) with a minimum error of (xminerror,yminerror)
    and a maximum error of (xmaxerror,ymaxerror). This is for situations where
    there may be multiple different error measures on a point and you want
    to store the range of errors (based on a range of error estimates) */
DataPoint::DataPoint(double x, double y, double xminerror, double yminerror,
                     double xmaxerror, double ymaxerror)
          : _x(x), _y(y),
            _xminerr(std::abs(xminerror)), _yminerr(std::abs(yminerror)),
            _xmaxerr(std::abs(xmaxerror)), _ymaxerr(std::abs(ymaxerror))
{
    if (_xminerr > _xmaxerr)
        qSwap(_xminerr, _xmaxerr);
    
    if (_yminerr > _ymaxerr)
        qSwap(_yminerr, _ymaxerr);
}

/** Copy constructor */
DataPoint::DataPoint(const DataPoint &other)
          : _x(other._x), _y(other._y),
            _xminerr(other._xminerr), _yminerr(other._yminerr),
            _xmaxerr(other._xmaxerr), _ymaxerr(other._ymaxerr)
{}

/** Destructor */
DataPoint::~DataPoint()
{}

/** Copy assignment operator */
DataPoint& DataPoint::operator=(const DataPoint &other)
{
    if (this != &other)
    {
        _x = other._x;
        _y = other._y;
        _xminerr = other._xminerr;
        _yminerr = other._yminerr;
        _xmaxerr = other._xmaxerr;
        _ymaxerr = other._ymaxerr;
    }
    
    return *this;
}

/** Comparison operator */
bool DataPoint::operator==(const DataPoint &other) const
{
    return _x == other._x and _y == other._y and
           _xminerr == other._xminerr and _yminerr == other._yminerr and
           _xmaxerr == other._xmaxerr and _ymaxerr == other._ymaxerr;
}

/** Comparison operator */
bool DataPoint::operator!=(const DataPoint &other) const
{
    return not operator==(other);
}

const char* DataPoint::what() const
{
    return DataPoint::typeName();
}

const char* DataPoint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<DataPoint>() );
}

QString DataPoint::toString() const
{
    if (hasError())
    {
        QString xstr;
        QString ystr;
    
        if (hasXError())
        {
            if (xMinError() != xMaxError())
            {
                xstr = QString("%1 +/- %2 [%3]").arg(x()).arg(xMinError()).arg(xMaxError());
            }
            else
            {
                xstr = QString("%1 +/- %2").arg(x()).arg(xError());
            }
        }
        else
            xstr = QString("%1").arg(x());

        if (hasYError())
        {
            if (yMinError() != yMaxError())
            {
                ystr = QString("%1 +/- %2 [%3]").arg(y()).arg(yMinError()).arg(yMaxError());
            }
            else
            {
                ystr = QString("%1 +/- %2").arg(y()).arg(yError());
            }
        }
        else
            ystr = QString("%1").arg(y());
        
        return QString("DataPoint( %1, %2 )").arg(xstr, ystr);
    }
    else
        return QString("DataPoint( %1, %2 )").arg(x()).arg(y());
}

/** Return the x value of the point */
double DataPoint::x() const
{
    return _x;
}

/** Return the y value of the point */
double DataPoint::y() const
{
    return _y;
}

/** Return the error on the x value. This is the average
    of the minimum and maximum error */
double DataPoint::xError() const
{
    return 0.5 * (_xminerr + _xmaxerr);
}

/** Return the error on the y value. This is the average
    of the minimum and maximum error */
double DataPoint::yError() const
{
    return 0.5 * (_yminerr + _ymaxerr);
}

/** Return the minimum size of the error on the x value */
double DataPoint::xMinError() const
{
    return _xminerr;
}

/** Return the minimum size of the error on the y value */
double DataPoint::yMinError() const
{
    return _yminerr;
}

/** Return the maximum size of the error on the x value */
double DataPoint::xMaxError() const
{
    return _xmaxerr;
}

/** Return the maximum size of the error on the y value */
double DataPoint::yMaxError() const
{
    return _ymaxerr;
}

/** Return whether or not this data point has any error */
bool DataPoint::hasError() const
{
    return _xminerr > 0 or _yminerr > 0;
}

/** Return whether or not there is any error in the x value */
bool DataPoint::hasXError() const
{
    return _xminerr > 0;
}

/** Return whether or not there is any error in the y value */
bool DataPoint::hasYError() const
{
    return _yminerr > 0;
}

/** Return whether or not this data point is equal to the other, within
    the error range of the two points */
bool DataPoint::equalWithinError(const DataPoint &other) const
{
    return x() + xError() >= other.x() - other.xError() and
           x() - xError() <= other.x() + other.xError() and
    
           y() + yError() >= other.y() - other.yError() and
           y() - yError() <= other.y() + other.yError();
}

/** Return whether or not this data point in equal to the other, within
    the minimum error range of the two points */
bool DataPoint::equalWithinMinError(const DataPoint &other) const
{
    return x() + xMinError() >= other.x() - other.xMinError() and
           x() - xMinError() <= other.x() + other.xMinError() and
    
           y() + yMinError() >= other.y() - other.yMinError() and
           y() - yMinError() <= other.y() + other.yMinError();
}

/** Return whether or not this data point in equal to the other, within
    the maximum error range of the two points */
bool DataPoint::equalWithinMaxError(const DataPoint &other) const
{
    return x() + xMaxError() >= other.x() - other.xMaxError() and
           x() - xMaxError() <= other.x() + other.xMaxError() and
    
           y() + yMaxError() >= other.y() - other.yMaxError() and
           y() - yMaxError() <= other.y() + other.yMaxError();
}

/////////
///////// Implementation of Gradients
/////////

static const RegisterMetaType<Gradients> r_grads;

QDataStream SOIREE_EXPORT &operator<<(QDataStream &ds, const Gradients &grads)
{
    writeHeader(ds, r_grads, 1);
    
    SharedDataStream sds(ds);
    
    sds << grads.fwds << grads.bwds << grads.delta_lam;
    
    return ds;
}

QDataStream SOIREE_EXPORT &operator>>(QDataStream &ds, Gradients &grads)
{
    VersionID v = readHeader(ds, r_grads);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> grads.fwds >> grads.bwds >> grads.delta_lam;
    }
    else
        throw version_error(v, "1", r_grads, CODELOC);
    
    return ds;
}

/** Constructor */
Gradients::Gradients() : ConcreteProperty<Gradients,Property>(), delta_lam(0)
{}

/** Construct from the passed full TI gradients */
Gradients::Gradients(const QMap<double,FreeEnergyAverage> &gradients)
          : ConcreteProperty<Gradients,Property>(),
            fwds(gradients), bwds(gradients), delta_lam(0)
{}

/** Construct from the passed finite difference TI gradients, using the passed
    value of delta lambda */
Gradients::Gradients(const QMap<double,FreeEnergyAverage> &gradients,
                     double delta_lambda)
          : ConcreteProperty<Gradients,Property>(),
            fwds(gradients), bwds(gradients), delta_lam(delta_lambda)
{
    if (delta_lam <= 0)
        throw SireError::invalid_arg( QObject::tr(
                "How can you have finite difference gradients with a value of "
                "delta lambda that is less than or equal to zero? %1")
                    .arg(delta_lam), CODELOC );
}

/** Construct from the passed finite difference TI forwards and backwards
    gradients, using the passed value of delta lambda. Note that the
    forwards gradients should be the zwanzig free energies from 
    lambda->lambda+delta_lambda, while the backwards gradients should
    be the zwanzig free energies from lambda-delta_lambda->lambda */
Gradients::Gradients(const QMap<double,FreeEnergyAverage> &forwards,
                     const QMap<double,FreeEnergyAverage> &backwards,
                     double delta_lambda)
          : ConcreteProperty<Gradients,Property>(),
            fwds(forwards), bwds(backwards), delta_lam(delta_lambda)
{
    if (delta_lam <= 0)
        throw SireError::invalid_arg( QObject::tr(
                "How can you have finite difference gradients with a value of "
                "delta lambda that is less than or equal to zero? %1")
                    .arg(delta_lam), CODELOC );

    //if the lambda values don't match, then copy the gradients from the
    //partners set
    if (backwards.isEmpty())
        bwds = fwds;
    
    else if (forwards.isEmpty())
        fwds = bwds;
    
    else
    {
        for (QMap<double,FreeEnergyAverage>::const_iterator it = bwds.constBegin();
             it != bwds.constEnd();
             ++it)
        {
            if (not fwds.contains(it.key()))
                fwds.insert( it.key(), it.value() );
        }
        
        for (QMap<double,FreeEnergyAverage>::const_iterator it = fwds.constBegin();
             it != fwds.constEnd();
             ++it)
        {
            if (not bwds.contains(it.key()))
                bwds.insert( it.key(), it.value() );
        }
    }
}

/** Copy constructor */
Gradients::Gradients(const Gradients &other)
          : ConcreteProperty<Gradients,Property>(other),
            fwds(other.fwds), bwds(other.bwds), delta_lam(other.delta_lam)
{}

/** Destructor */
Gradients::~Gradients()
{}

/** Return whether or not this is empty */
bool Gradients::isEmpty() const
{
    return (fwds.isEmpty() and bwds.isEmpty());
}

/** Copy assignment operator */
Gradients& Gradients::operator=(const Gradients &other)
{
    if (this != &other)
    {
        fwds = other.fwds;
        bwds = other.bwds;
        delta_lam = other.delta_lam;
    }
    
    return *this;
}

/** Comparison operator */
bool Gradients::operator==(const Gradients &other) const
{
    return fwds == other.fwds and bwds == other.bwds and delta_lam == other.delta_lam;
}

/** Comparison operator */
bool Gradients::operator!=(const Gradients &other) const
{
    return not operator==(other);
}

/** Return the value of the gradient at the passed lambda value */
MolarEnergy Gradients::operator[](double lam) const
{
    return gradient(lam);
}

const char* Gradients::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Gradients>() );
}

const char* Gradients::what() const
{
    return Gradients::typeName();
}

/** Merge together the passed list of Gradients into a single object.
    Note that all of the passed gradients must be compatible, e.g.
    have the same temperature and delta lambda values */
Gradients Gradients::merge(const QList<Gradients> &gradients)
{
    if (gradients.isEmpty())
        return Gradients();
    else if (gradients.count() == 1)
        return gradients.at(0);
    
    bool have_first = false;
    double delta_lam;
    Temperature temperature;
    
    QMap<double,FreeEnergyAverage> fwds;
    QMap<double,FreeEnergyAverage> bwds;
    
    foreach (const Gradients &grads, gradients)
    {
        if (have_first)
        {
            if (grads.delta_lam != delta_lam)
                throw SireError::incompatible_error( QObject::tr(
                        "Cannot combine together this set of free energy Gradients "
                        "as the values of delta lambda in the set are different. %1 vs. %2.")
                            .arg(delta_lam).arg(grads.delta_lam), CODELOC );
        }
    
        for (QMap<double,FreeEnergyAverage>::const_iterator it = grads.fwds.constBegin();
             it != grads.fwds.constEnd();
             ++it)
        {
            double lam = it.key();
            const FreeEnergyAverage &grad = it.value();
            
            if (not grad.nSamples() == 0)
            {
                if (have_first)
                {
                    if (grad.temperature() != temperature)
                        throw SireError::incompatible_error( QObject::tr(
                            "Cannot combine together this set of free energy Gradients "
                            "as the temperatures of some of the set are different. %1 vs. %2.")
                                .arg(temperature.toString())
                                .arg(grad.temperature().toString()), CODELOC );
                }
                else
                {
                    have_first = true;
                    delta_lam = grads.delta_lam;
                    temperature = grad.temperature();
                }
                
                if (fwds.contains(lam))
                {
                    fwds[lam] += grad;
                }
                else
                {
                    fwds.insert(lam, grad);
                }
            }
        }
        
        if (grads.fwds == grads.bwds)
        {
            bwds = fwds;
        }
        else
        {
            for (QMap<double,FreeEnergyAverage>::const_iterator it = grads.bwds.constBegin();
                 it != grads.bwds.constEnd();
                 ++it)
            {
                double lam = it.key();
                const FreeEnergyAverage &grad = it.value();
                
                if (not grad.nSamples() == 0)
                {
                    if (have_first)
                    {
                        if (grad.temperature() != temperature)
                            throw SireError::incompatible_error( QObject::tr(
                                "Cannot combine together this set of free energy Gradients "
                                "as the temperatures of some of the set are different. %1 vs. %2.")
                                    .arg(temperature.toString())
                                    .arg(grad.temperature().toString()), CODELOC );
                    }
                    else
                    {
                        have_first = true;
                        delta_lam = grads.delta_lam;
                        temperature = grad.temperature();
                    }
                    
                    if (bwds.contains(lam))
                    {
                        bwds[lam] += grad;
                    }
                    else
                    {
                        bwds.insert(lam, grad);
                    }
                }
            }
        }
    }
    
    Gradients ret;
    ret.fwds = fwds;
    ret.bwds = bwds;
    ret.delta_lam = delta_lam;
    
    return ret;
}

/** Return the (sorted) list of the lambda values */
QList<double> Gradients::lambdaValues() const
{
    if (delta_lam == 0 or fwds == bwds)
    {
        QList<double> lams = fwds.keys();
        qSort(lams);
        return lams;
    }
    else
    {
        QMap<double,int> lams;
        
        foreach (double lam, fwds.keys())
        {
            lams.insert(lam, 1);
        }
        
        foreach (double lam, bwds.keys())
        {
            lams.insert(lam, 1);
        }
        
        QList<double> l = lams.keys();
        qSort(l);
        
        return l;
    }
}

/** Return the (sorted) list of all lambda values */
QList<double> Gradients::keys() const
{
    return lambdaValues();
}

/** Return the total number of lambda values */
int Gradients::nLambdaValues() const
{
    return lambdaValues().count();
}

/** Return the total number of samples used to calculate these gradients */
qint64 Gradients::nSamples() const
{
    qint64 n = 0;

    if (delta_lam == 0 or fwds == bwds)
    {
        for (QMap<double,FreeEnergyAverage>::const_iterator it = fwds.constBegin();
             it != fwds.constEnd();
             ++it)
        {
            n += it.value().nSamples();
        }
    }
    else
    {
        for (QMap<double,FreeEnergyAverage>::const_iterator it = fwds.constBegin();
             it != fwds.constEnd();
             ++it)
        {
            n += it.value().nSamples();
        }

        for (QMap<double,FreeEnergyAverage>::const_iterator it = bwds.constBegin();
             it != bwds.constEnd();
             ++it)
        {
            n += it.value().nSamples();
        }
    }
    
    return n;
}

/** Return the forwards gradient for the passed lambda value */
MolarEnergy Gradients::forwards(double lam) const
{
    if (not fwds.contains(lam))
        throw SireError::invalid_key( QObject::tr(
                "There is no gradient value at lambda == %1. Available lambda values "
                "are %2.").arg(lam).arg( Sire::toString(lambdaValues()) ), CODELOC );
    
    if (delta_lam == 0)
        //pure TI, so just need the normal average energy
        return MolarEnergy( fwds[lam].histogram().mean() );
    else
    {
        //finite difference TI, so take the average of the forwards and backwards
        //values and divide by delta lambda
        return MolarEnergy( fwds[lam].average() / delta_lam );
    }
}

/** Return the backwards gradient for the passed lambda value */
MolarEnergy Gradients::backwards(double lam) const
{
    if (not fwds.contains(lam))
        throw SireError::invalid_key( QObject::tr(
                "There is no gradient value at lambda == %1. Available lambda values "
                "are %2.").arg(lam).arg( Sire::toString(lambdaValues()) ), CODELOC );
    
    if (delta_lam == 0)
        //pure TI, so just need the normal average energy
        return MolarEnergy( fwds[lam].histogram().mean() );
    else
    {
        //finite difference TI, so take the average of the forwards and backwards
        //values and divide by delta lambda
        return MolarEnergy( bwds[lam].average() / delta_lam );
    }
}

/** Return the gradient at the passed lambda value. This is the 
    average of the forwards and backwards gradient if finite difference
    is used */
MolarEnergy Gradients::gradient(double lam) const
{
    if (not fwds.contains(lam))
        throw SireError::invalid_key( QObject::tr(
                "There is no gradient value at lambda == %1. Available lambda values "
                "are %2.").arg(lam).arg( Sire::toString(lambdaValues()) ), CODELOC );
    
    if (delta_lam == 0)
        //pure TI, so just need the normal average energy
        return MolarEnergy( fwds[lam].histogram().mean() );
    else
    {
        //finite difference TI, so take the average of the forwards and backwards
        //values and divide by delta lambda
        return MolarEnergy( 0.5 * (fwds[lam].average() + bwds[lam].average()) / delta_lam );
    }
}

/** Return the value of delta lambda. This will be zero if these are 
    pure TI gradients */
double Gradients::deltaLambda() const
{
    return delta_lam;
}

/** Return the raw data for the forwards free energy gradients */
QMap<double,FreeEnergyAverage> Gradients::forwardsData() const
{
    return fwds;
}

/** Return the raw data for the backwards free energy gradients */
QMap<double,FreeEnergyAverage> Gradients::backwardsData() const
{
    return bwds;
}

/** Return the values of the gradients as data points. This returns the 
    average of the forwards and backwards gradients, with errors calculated
    based on both the difference between the forwards and backwards values,
    and the 90% confidence level of the average of gradients */
QVector<DataPoint> Gradients::values() const
{
    QList<double> lamvals = this->lambdaValues();
 
    if (lamvals.isEmpty())
        return QVector<DataPoint>();
    
    QVector<DataPoint> points( lamvals.count() );
    
    for (int i=0; i<lamvals.count(); ++i)
    {
        double lam = lamvals[i];
    
        if (delta_lam == 0)
        {
            //pure TI data
            const FreeEnergyAverage &avg = *(fwds.constFind(lam));
            points[i] = DataPoint(lam, avg.histogram().mean(), 0,
                                       avg.histogram().standardError(90));
        }
        else
        {
            const FreeEnergyAverage &fwdsavg = *(fwds.constFind(lam));
            const FreeEnergyAverage &bwdsavg = *(bwds.constFind(lam));
            
            if (fwdsavg == bwdsavg)
            {
                points[i] = DataPoint(lam, fwdsavg.average() / delta_lam, 0,
                                      fwdsavg.histogram().standardError(90) / delta_lam);
            }
            else
            {
                double fwdsval = fwdsavg.average() / delta_lam;
                double bwdsval = bwdsavg.average() / delta_lam;
                double fwdserr = fwdsavg.histogram().standardError(90) / delta_lam;
                double bwdserr = bwdsavg.histogram().standardError(90) / delta_lam;
                
                double val = 0.5 * (fwdsval + bwdsval);
                double maxerr = 0.5 * (fwdserr + bwdserr);
                
                double minerr = 0.5 * std::abs(fwdsval - bwdsval);
                
                if (maxerr < minerr)
                    qSwap(maxerr, minerr);
                
                points[i] = DataPoint(lam, val, 0, minerr, 0, maxerr);
            }
        }
    }
    
    return points;
}

/** Return the values of the forwards gradients as data points. This returns the 
    average forwards gradient for each lambda value, together with the
    standard error at the 90% confidence level */
QVector<DataPoint> Gradients::forwardsValues() const
{
    QList<double> lamvals = this->lambdaValues();
 
    if (lamvals.isEmpty())
        return QVector<DataPoint>();
    
    QVector<DataPoint> points( lamvals.count() );
    
    for (int i=0; i<lamvals.count(); ++i)
    {
        double lam = lamvals[i];
    
        if (delta_lam == 0)
        {
            //pure TI data
            const FreeEnergyAverage &avg = *(fwds.constFind(lam));
            points[i] = DataPoint(lam, avg.histogram().mean(), 0,
                                       avg.histogram().standardError(90));
        }
        else
        {
            const FreeEnergyAverage &fwdsavg = *(fwds.constFind(lam));
            
            points[i] = DataPoint(lam, fwdsavg.average() / delta_lam, 0,
                                  fwdsavg.histogram().standardError(90) / delta_lam);
        }
    }
    
    return points;
}

/** Return the values of the backwards gradients as data points. This returns the 
    average backwards gradient for each lambda value, together with the
    standard error at the 90% confidence level */
QVector<DataPoint> Gradients::backwardsValues() const
{
    QList<double> lamvals = this->lambdaValues();
 
    if (lamvals.isEmpty())
        return QVector<DataPoint>();
    
    QVector<DataPoint> points( lamvals.count() );
    
    for (int i=0; i<lamvals.count(); ++i)
    {
        double lam = lamvals[i];
    
        if (delta_lam == 0)
        {
            //pure TI data
            const FreeEnergyAverage &avg = *(bwds.constFind(lam));
            points[i] = DataPoint(lam, avg.histogram().mean(), 0,
                                       avg.histogram().standardError(90));
        }
        else
        {
            const FreeEnergyAverage &bwdsavg = *(bwds.constFind(lam));
            
            points[i] = DataPoint(lam, bwdsavg.average() / delta_lam, 0,
                                  bwdsavg.histogram().standardError(90) / delta_lam);
        }
    }
    
    return points;
}

/////////
///////// Implementation of TI
/////////

static const RegisterMetaType<TI> r_ti;

QDataStream SOIREE_EXPORT &operator<<(QDataStream &ds, const TI &ti)
{
    writeHeader(ds, r_ti, 1);
    
    SharedDataStream sds(ds);
    sds << ti.grads;
    
    return ds;
}

QDataStream SOIREE_EXPORT &operator>>(QDataStream &ds, TI &ti)
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

/** Null constructor */
TI::TI() : ConcreteProperty<TI,Property>()
{}

/** Construct from the passed set of gradients */
TI::TI(const Gradients &gradients) : ConcreteProperty<TI,Property>()
{
    if (not gradients.isEmpty())
        grads.append(gradients);
}

/** Construct from the passed list of gradients from each iteration */
TI::TI(const QList<Gradients> &gradients) : ConcreteProperty<TI,Property>()
{
    foreach (const Gradients &g, gradients)
    {
        if (not g.isEmpty())
            grads.append(g);
    }
}

/** Copy constructor */
TI::TI(const TI &other) : ConcreteProperty<TI,Property>(other), grads(other.grads)
{}

/** Destructor */
TI::~TI()
{}

/** Copy assignment operator */
TI& TI::operator=(const TI &other)
{
    grads = other.grads;
    return *this;
}

/** Comparison operator */
bool TI::operator==(const TI &other) const
{
    return grads == other.grads;
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

/** Return the raw list of gradients */
QList<Gradients> TI::gradients() const
{
    return grads;
}

/** Add the passed free energy gradients to the TI calculation. The gradients
    are in a dictionary, indexed by lambda value, and are "pure" TI gradients, 
    i.e. they have been calculated exactly with an infinitesimal delta lambda */
void TI::add(const QMap<double,FreeEnergyAverage> &gradients)
{
    if (not gradients.isEmpty())
        grads.append( Gradients(gradients) );
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
        grads.append( Gradients(gradients, delta_lambda) );
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
    if (not (forwards.isEmpty() and backwards.isEmpty()))
    {
        grads.append( Gradients(forwards, backwards, delta_lambda) );
    }
}

void TI::add(const Gradients &gradients)
{
    if (not gradients.isEmpty())
        grads.append(gradients);
}

/** Return the number of iterations (number of sets of gradients that have been added) */
int TI::nIterations() const
{
    return grads.count();
}

/** Return all values of lambda that have data. The values are returned 
    in numerical order */
QList<double> TI::lambdaValues() const
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

/** Return the number of lambda values */
int TI::nLambdaValues() const
{
    return lambdaValues().count();
}

/** Return the total number of samples in this calculation */
qint64 TI::nSamples() const
{
    qint64 n = 0;
    
    for (int i=0; i<grads.count(); ++i)
    {
        n += grads.at(i).nSamples();
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
Gradients TI::operator[](int i) const
{
    i = Index(i).map(grads.count());
    return grads.at(i);
}

/** Return the free energy gradient data for the ith iteration. This returns
    a tuple of the forwards gradients, backwards gradients and the value
    of delta lambda. Note that for pure TI calculations, the forwards and
    backwards gradients will be equal and the value of delta lambda will be 0 */
Gradients TI::at(int i) const
{
    return operator[](i);
}

/** Set the gradients for the ith iteration equal to 'gradients'. These
    must be pure TI gradients, with no associated delta lambda value */
void TI::set(int i, const QMap<double,FreeEnergyAverage> &gradients)
{
    if (gradients.isEmpty())
        return;

    if (i == grads.count())
        this->add(gradients);
    else
    {
        i = Index(i).map(grads.count());
        grads[i] = Gradients(gradients);
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
    
    if (i == grads.count())
        this->add(gradients, delta_lambda);
    else
    {
        i = Index(i).map(grads.count());
        grads[i] = Gradients(gradients, delta_lambda);
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
    
    else if (i == grads.count())
    {
        this->add(forwards, backwards, delta_lambda);
        return;
    }
    else
    {
        i = Index(i).map(grads.count());
        grads.append(Gradients(forwards, backwards, delta_lambda));
    }
}

void TI::set(int i, const Gradients &gradients)
{
    if (not gradients.isEmpty())
    {
        i = Index(i).map(grads.count());
        grads[i] = gradients;
    }
}

/** Remove the data for iteration 'i' */
void TI::removeAt(int i)
{
    i = Index(i).map(grads.count());
    grads.removeAt(i);
}

/** Remove every iteration from 'start' to 'end' (inclusively) */
void TI::removeRange(int start, int end)
{
    start = Index(start).map(grads.count());
    end = Index(end).map(grads.count());
    
    if (start > end)
        qSwap(start, end);
    
    for (int i = start; i <= end; ++i)
    {
        grads.removeAt(start);
    }
}

/** Remove all values from the histogram */
void TI::clear()
{
    this->operator=( TI() );
}

/** Merge (average) together the gradients from iteration "start" to iteration
    "end" inclusive */
Gradients TI::merge(int start, int end)
{
    start = Index(start).map(grads.count());
    end = Index(end).map(grads.count());
    
    QList<Gradients> set;
    
    for (int i=start; i<=end; ++i)
    {
        set.append( grads.at(i) );
    }
    
    return Gradients::merge(set);
}

/** Merge together the gradients from the iterations with the passed indicies */
Gradients TI::merge(QList<int> indicies)
{
    QList<Gradients> set;
    
    foreach (int idx, indicies)
    {
        set.append( grads.at( Index(idx).map(grads.count()) ) );
    }
 
    return Gradients::merge(set);
}
    
/** Return a list of Gradients that represents the rolling average over 'niterations'
    iterations over this TI data set. If this data set contains 100 iterations, and 
    we calculate the rolling average over 50 iterations, then the returned Gradients
    will be the average from 1-50, then 2-51, 3-52.....51-100 */
QList<Gradients> TI::rollingAverage(int niterations) const
{
    QList<Gradients> merged;

    if (niterations >= grads.count())
        merged.append( Gradients::merge(grads) );

    else if (niterations <= 0)
        merged = grads;
    
    else
    {
        QList<Gradients> set;
        
        for (int i=0; i<niterations; ++i)
            set.append(grads.at(i));
        
        merged.append( Gradients::merge(set) );
        
        for (int i=niterations; i<grads.count(); ++i)
        {
            set.removeFirst();
            set.append(grads.at(i));
            merged.append( Gradients::merge(set) );
        }
    }
    
    return merged;
}

QString TI::toString() const
{
    return QObject::tr( "TI( nLambdaValues() == %1, nIterations() == %2, nSamples() == %3 )" )
                .arg(nLambdaValues()).arg(nIterations()).arg(nSamples());
}
