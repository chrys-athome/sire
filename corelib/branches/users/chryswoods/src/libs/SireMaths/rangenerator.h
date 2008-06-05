/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

    bool operator==(const RanGenerator &other) const;
    bool operator!=(const RanGenerator &other) const;

    void seed();
    void seed(quint32 seed);
    void seed(const QVector<quint32> &seed);
    void seed(const RanGenerator &other);

    double rand();
    double rand(double maxval);
    double rand(double minval, double maxval);

    double rand53();
    double rand53(double maxval);
    double rand53(double minval, double maxval);

    double randNorm(double mean, double variance);

    Vector vectorOnSphere();
    Vector vectorOnSphere(double radius);

    quint32 randInt();
    quint32 randInt(quint32 maxval);
    qint32 randInt(qint32 minval, qint32 maxval);

    quint64 randInt64();
    quint64 randInt64(quint64 maxval);
    qint64 randInt64(qint64 minval, qint64 maxval);

    QVector<quint32> getState();
    void setState(const QVector<quint32> &state);

private:
    /** Shared pointer to the actual generator */
    boost::shared_ptr<detail::RanGeneratorPvt> d;
};

}

Q_DECLARE_METATYPE(SireMaths::RanGenerator);

SIRE_EXPOSE_CLASS( SireMaths::RanGenerator )

SIRE_END_HEADER

#endif
