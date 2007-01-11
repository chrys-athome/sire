#ifndef SIREMATHS_RANGENERATOR_H
#define SIREMATHS_RANGENERATOR_H

#include <boost/shared_ptr.hpp>

#include <QVector>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class RanGenerator;
}

QDataStream& operator<<(QDataStream&, const SireMaths::RanGenerator&);
QDataStream& operator>>(QDataStream&, SireMaths::RanGenerator&);

namespace SireMaths
{

class Vector;

namespace detail
{
class RanGeneratorPvt;
}

/** This class provides a thread-safe, copyable and streamable
    random number generator. Copies are guaranteed to produce
    different random number sequences (thus the possibility
    of accidental repeat random numbers is removed).
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT RanGenerator
{

friend QDataStream& ::operator<<(QDataStream&, const RanGenerator&);
friend QDataStream& ::operator>>(QDataStream&, RanGenerator&);

public:
    RanGenerator();
    RanGenerator(quint32 seed);
    RanGenerator(const QVector<quint32> &seed);

    RanGenerator(const RanGenerator &other);

    ~RanGenerator();

    RanGenerator& operator=(const RanGenerator &other);
    
    void seed();
    void seed(quint32 seed);
    void seed(const QVector<quint32> &seed);
    void seed(const RanGenerator &other);

    double rand();
    void rand(QVector<double> &array, uint n=0);
    
    double rand53();
    void rand53(QVector<double> &array, uint n=0);
    
    double randNorm(double mean, double variance);
    void randNorm(QVector<double> &array, double mean, 
                  double variance, uint n=0);

    Vector vectorOnSphere();
    void vectorOnSphere(QVector<Vector> &array, uint n=0);
    
    quint32 randInt();
    void randInt(QVector<quint32> &array, uint n=0);
    
    quint32 randInt(quint32 maxval);
    void randInt(QVector<quint32> &array, quint32 maxval, uint n=0);
    
    quint64 randInt64();
    void randInt64(QVector<quint64> &array, uint n=0);
    
    quint64 randInt64(quint64 maxval);
    void randInt64(QVector<quint64> &array, quint64 maxval, uint n=0);
    
    QVector<quint32> getState();
    void setState(const QVector<quint32> &state);

private:
    /** Shared pointer to the actual generator */
    boost::shared_ptr<detail::RanGeneratorPvt> d;
};

}

Q_DECLARE_METATYPE(SireMaths::RanGenerator);

SIRE_END_HEADER

#endif
