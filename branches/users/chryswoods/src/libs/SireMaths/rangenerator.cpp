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

#include <QMutex>
#include <QVector>

#include <QDebug>

#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#include <limits>

#include "rangenerator.h"
#include "vector.h"

#include "ThirdParty/MersenneTwister.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMaths;
using namespace SireMaths::detail;

using namespace SireStream;

namespace SireMaths
{
namespace detail
{
class RanGeneratorPvt;
}
}

QDataStream& operator<<(QDataStream&, const SireMaths::detail::RanGeneratorPvt&);
QDataStream& operator>>(QDataStream&, SireMaths::detail::RanGeneratorPvt&);

namespace SireMaths
{

namespace detail
{

/** This class provides the private implementation of RanGenerator.
    This class is explicitly shared by instances of RanGenerator,
    and is therefore thread-safe.

    @author Christopher Woods
*/
class RanGeneratorPvt : public boost::noncopyable
{

friend QDataStream& ::operator<<(QDataStream&, const RanGeneratorPvt&);
friend QDataStream& ::operator>>(QDataStream&, RanGeneratorPvt&);

typedef MTRand::uint32 MTUInt32;

public:
    /** Construct a generator with a random seed */
    RanGeneratorPvt() : boost::noncopyable(),
                        mutex(QMutex::NonRecursive),
                        mersenne_generator()
    {}

    /** Construct a generator with a specified seed */
    RanGeneratorPvt(quint32 seed) : boost::noncopyable(),
                                    mutex(QMutex::NonRecursive),
                                    mersenne_generator(seed)
    {}

    /** Construct a generator with a specified seed */
    RanGeneratorPvt(const QVector<quint32> &s) : boost::noncopyable(),
                                                 mutex(QMutex::NonRecursive),
                                                 mersenne_generator()
    {
        this->seed(s);
    }

    /** Destructor */
    ~RanGeneratorPvt()
    {}

    /** Mutex to serialise access to the generator */
    QMutex mutex;

    /** The actual generator (Mersenne Twister) */
    MTRand mersenne_generator;

    /** Randomly seed the generator */
    void seed()
    {
        QMutexLocker lkr(&mutex);

        mersenne_generator.seed();
    }

    /** Reseed the generator from an array of uints */
    void seed(const QVector<quint32> &s)
    {
        if (s.isEmpty())
        {
            this->seed();
            return;
        }

        QMutexLocker lkr(&mutex);

        //we need to convert this into an array of MTRand::uint32
        int sz = s.count();

        boost::scoped_array<MTUInt32> array( new MTUInt32[sz] );

        for (int i=0; i<sz; ++i)
            array[i] = s.constData()[i];

        mersenne_generator.seed( array.get(),  sz );
    }

    /** Reseed the generator */
    void seed(quint32 s)
    {
        QMutexLocker lkr(&mutex);

        mersenne_generator.seed(s);
    }

    /** Return an array containing the state of the random generator */
    QVector<quint32> getState()
    {
        mutex.lock();

        //create an array to hold the state
        boost::scoped_array<MTUInt32> array( new MTUInt32[MTRand::N] );

        mersenne_generator.save(array.get());

        mutex.unlock();

        //copy the array into a QVector<quint32>
        QVector<quint32> ret(MTRand::N);

        quint32 *ret_array = ret.data();

        for (int i=0; i<MTRand::N; ++i)
            ret_array[i] = array[i];

        return ret;
    }

    /** Load the state from an array - the array must have size
        MTRand::N (624)

        \throw SireError::incompatible_error
    */
    void loadState(const QVector<quint32> &state)
    {
        //check that the array is of the right size...
        if (state.count() != MTRand::N)
            throw SireError::incompatible_error( QObject::tr(
                "Can only restore the state from an array of size %1, "
                "while you have provided an array of size %2.")
                    .arg(MTRand::N).arg(state.count()), CODELOC );

        //convert the array to the right type...
        boost::scoped_array<MTUInt32> array( new MTUInt32[MTRand::N] );

        const quint32 *state_array = state.constData();

        for (int i=0; i<MTRand::N; ++i)
            array[i] = state_array[i];

        QMutexLocker lkr(&mutex);

        mersenne_generator.load(array.get());
    }
};

} // end of namespace detail

} // end of namespace SireMaths

////////////
//////////// Implementation of RanGeneratorPvt
////////////

static const RegisterMetaType<RanGeneratorPvt> r_ranpvt(MAGIC_ONLY,
                                  "SireMaths::detail::RanGeneratorPvt");

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const RanGeneratorPvt &ranpvt)
{
    writeHeader(ds, r_ranpvt, 1);

    //this is a large object - full state is 624 quint32 (~2.5kB)
    typedef MTRand::uint32 MTUInt32;

    boost::scoped_array<MTUInt32> array( new MTUInt32[MTRand::N] );

    const_cast<QMutex&>(ranpvt.mutex).lock();
    ranpvt.mersenne_generator.save(array.get());
    const_cast<QMutex&>(ranpvt.mutex).unlock();

    ds << quint32(MTRand::N);

    for (int i=0; i<MTRand::N; ++i)
        //convert to platform neutral quint32
        ds << quint32( array[i] );

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, RanGeneratorPvt &ranpvt)
{
    VersionID v = readHeader(ds, r_ranpvt);

    if (v == 1)
    {
        quint32 n;

        ds >> n;

        if (n != MTRand::N)
            throw SireError::program_bug( QObject::tr(
                "The size of the Mersenne Twister state array has changed without "
                "the version number changing - this is a bug! (%1 vs. %2)")
                    .arg(n).arg(MTRand::N), CODELOC );

        //this is a large object - full state is 624 quint32 (~2.5kB)
        typedef MTRand::uint32 MTUInt32;

        boost::scoped_array<MTUInt32> array( new MTUInt32[MTRand::N] );

        for (int i=0; i<MTRand::N; ++i)
        {
            quint32 val;
            ds >> val;

            array[i] = val;
        }

        QMutexLocker lkr( &(ranpvt.mutex) );
        ranpvt.mersenne_generator.load(array.get());
    }
    else
        throw version_error(v, "1", r_ranpvt, CODELOC);

    return ds;
}

////////////
//////////// Implementation of RanGenerator
////////////

static const RegisterMetaType<RanGenerator> r_rangen;

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const RanGenerator &rangen)
{
    writeHeader(ds, r_rangen, 1);

    SharedDataStream sds(ds);
    sds << *(rangen.d);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, RanGenerator &rangen)
{
    VersionID v = readHeader(ds, r_rangen);

    if (v == 1)
    {
        if (not rangen.d.unique())
            rangen.d.reset( new RanGeneratorPvt() );

        SharedDataStream sds(ds);
        sds >> *(rangen.d);
    }
    else
        throw version_error(v, "1", r_rangen, CODELOC);

    return ds;
}

static boost::shared_ptr<RanGeneratorPvt> shared_null( new RanGeneratorPvt() );

/** Create a randomly seeded generator
    (actually a copy of the global, random generator) */
RanGenerator::RanGenerator() : d(shared_null)
{}

/** Create a generator seeded with 'seed' */
RanGenerator::RanGenerator(quint32 seed)
             : d( new RanGeneratorPvt(seed) )
{}

/** Create a generator seeded with 'seed' */
RanGenerator::RanGenerator(const QVector<quint32> &seed)
             : d( new RanGeneratorPvt(seed) )
{}

/** Copy constructor - this takes an explicitly shared
    copy of 'other' (this is to prevent repeat random numbers
    from being generated by implicit copies!) */
RanGenerator::RanGenerator(const RanGenerator &other)
             : d( other.d )
{}

/** Destructor */
RanGenerator::~RanGenerator()
{}

/** Copy assignment */
RanGenerator& RanGenerator::operator=(const RanGenerator &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator - two generators are equal if they use the
    same underlying generator */
bool RanGenerator::operator==(const RanGenerator &other) const
{
    return d == other.d;
}

/** Comparison operator - two generators are equal if they use the
    same underlying generator */
bool RanGenerator::operator!=(const RanGenerator &other) const
{
    return d != other.d;
}

/** See the generator with a new, random seed - this will detach
    this explicitly shared copy of the generator */
void RanGenerator::seed()
{
    if (d.unique())
        d->seed();
    else
        d.reset( new RanGeneratorPvt() );
}

/** Seed the generator with 's'  - this will detach
    this explicitly shared copy of the generator */
void RanGenerator::seed(quint32 s)
{
    if (d.unique())
        d->seed(s);
    else
        d.reset( new RanGeneratorPvt(s) );
}

/** Seed the generator with 'seed' - this will detach
    this explicitly shared copy of the generator */
void RanGenerator::seed(const QVector<quint32> &s)
{
    if (d.unique())
        d->seed(s);
    else
        d.reset( new RanGeneratorPvt(s) );
}

/** Seed the generator with another generator - this
    really just copies the generator as they are
    all explicit copies of one another! */
void RanGenerator::seed(const RanGenerator &other)
{
    d = other.d;
}

/** Return a random real number on [0,1) */
double RanGenerator::rand()
{
    QMutexLocker lkr( &(d->mutex) );
    return d->mersenne_generator.rand();
}

/** Return a random real number on [0,maxval) */
double RanGenerator::rand(double maxval)
{
    return maxval * rand();
}

/** Return a random real number on [minval,maxval) */
double RanGenerator::rand(double minval, double maxval)
{
    return minval + rand() * (maxval-minval);
}

/** Return a high-precision random real number on [0,1) */
double RanGenerator::rand53()
{
    QMutexLocker lkr( &(d->mutex) );
    return d->mersenne_generator.rand53();
}

/** Return a high-precision random real number on [0,1) */
double RanGenerator::rand53(double maxval)
{
    return maxval * rand53();
}

/** Return a high-precision random real number on [minval,maxval) */
double RanGenerator::rand53(double minval, double maxval)
{
    return minval + rand53()*(maxval-minval);
}

/** Return a high-precision random number from the normal distribution
    with supplied mean and variance. */
double RanGenerator::randNorm(double mean, double variance)
{
    QMutexLocker lkr( &(d->mutex) );

    double rand0 = d->mersenne_generator.rand53();
    double rand1 = d->mersenne_generator.rand53();

    lkr.unlock();

    // Return a real number from a normal (Gaussian) distribution with given
    // mean and variance by Box-Muller method
    double r = std::sqrt( -2.0 * log(1.0-rand0) ) * variance;
    double phi = 2.0 * 3.14159265358979323846264338328 * rand1;

    return mean + r * std::cos(phi);
}

/** Return a random vector on the unit sphere */
Vector RanGenerator::vectorOnSphere()
{
    QMutexLocker lkr( &(d->mutex) );

    while( true )
    {
        //use von Neumann acceptance/rejection method
        Vector v;

        v.setX( 1.0 - 2.0 * d->mersenne_generator.rand53() );
        v.setY( 1.0 - 2.0 * d->mersenne_generator.rand53() );
        v.setZ( 1.0 - 2.0 * d->mersenne_generator.rand53() );

        double lgth2 = v.length2();

        if (lgth2 < 1)
        {
            v /= std::sqrt(lgth2);
            return v;
        }
    }
}

/** Return a random 32bit unsigned integer in [0,2^32 - 1] */
quint32 RanGenerator::randInt()
{
    QMutexLocker lkr( &(d->mutex) );
    return d->mersenne_generator.randInt();
}

/** Return a random 32bit unsigned integer in [0,maxval] */
quint32 RanGenerator::randInt(quint32 maxval)
{
    QMutexLocker lkr( &(d->mutex) );
    return d->mersenne_generator.randInt(maxval);
}

/** Return a random 32bit integer in [minval,maxval] */
qint32 RanGenerator::randInt(qint32 minval, qint32 maxval)
{
    if (maxval == minval)
        return maxval;
    else if (maxval < minval)
        qSwap(minval,maxval);

    return minval + randInt(maxval-minval);
}

static quint64 randInt64(MTRand &mersenne_generator)
{
    quint64 ran0 = mersenne_generator.randInt();
    quint64 ran1 = mersenne_generator.randInt();

    return (ran0 << 32) | ran1;
}

/** Return a random 64bit unsigned integer on [0,2^64 - 1] */
quint64 RanGenerator::randInt64()
{
    QMutexLocker lkr( &(d->mutex) );
    return ::randInt64(d->mersenne_generator);
}

/** Return a random 64bit unsigned integer on [0,maxval] */
quint64 RanGenerator::randInt64(quint64 maxval)
{
    QMutexLocker lkr( &(d->mutex) );

    if (maxval <= std::numeric_limits<quint32>::max())
        //maxval can fit into a 32bit int - there is no
        //point using a 64bit generator!
        return d->mersenne_generator.randInt( quint32(maxval) );

    //use same algorithm in MersenneTwister.h
    quint64 used = maxval;

    used |= used >> 1;
    used |= used >> 2;
    used |= used >> 4;
    used |= used >> 8;
    used |= used >> 16;
    used |= used >> 32;

    quint64 i;

    do
    {
        i = ::randInt64(d->mersenne_generator) & used;
    } while ( i > maxval );

    return i;
}

/** Return a random 64bit integer on [minval,maxval] */
qint64 RanGenerator::randInt64(qint64 minval, qint64 maxval)
{
    if (maxval == minval)
        return maxval;
    else if (maxval < minval)
        qSwap(minval,maxval);

    return minval + randInt(maxval-minval);
}

/** Return the current state of the random number generator.
    Use this if you truly wish to get reproducible sequences
    of random numbers */
QVector<quint32> RanGenerator::getState()
{
    return d->getState();
}

/** Load the state into this generator - the state must have
    been produced by the getState() function above.

    This will detach this copy from shared storage.

    \throw SireError::incompatible_error
*/
void RanGenerator::setState(const QVector<quint32> &state)
{
    if (d.unique())
    {
        d->loadState(state);
    }
    else
    {
        d.reset( new RanGeneratorPvt() );
        d->loadState(state);
    }
}
