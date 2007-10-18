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

#ifndef SIRESYSTEM_RDFMONITOR_H
#define SIRESYSTEM_RDFMONITOR_H

#include <QMap>

#include "SireSystem/systemmonitor.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/atomindex.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireSystem
{
class RDFMonitor;
class RDF;

namespace detail
{
class AtomPairs;
}

}

QDataStream& operator<<(QDataStream&, const SireSystem::RDFMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::RDFMonitor&);

QDataStream& operator<<(QDataStream&, const SireSystem::RDF&);
QDataStream& operator>>(QDataStream&, SireSystem::RDF&);

QDataStream& operator<<(QDataStream&, const SireSystem::detail::AtomPairs&);
QDataStream& operator>>(QDataStream&, SireSystem::detail::AtomPairs&);

namespace SireSystem
{

using SireMol::Molecule;
using SireMol::MoleculeGroup;
using SireMol::AtomIndex;

using boost::tuple;

/** This class holds a radial distribution function. This is
    a very basic class, holding just the statistics that are
    used to construct the RDF, and the functions necessary
    to transform those statistics

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT RDF
{

friend QDataStream& ::operator<<(QDataStream&, const RDF&);
friend QDataStream& ::operator>>(QDataStream&, RDF&);

public:
    RDF(double min = 0, double max = 10, uint nbins = 100);

    RDF(const RDF &other);

    ~RDF();

    RDF& operator=(const RDF &other);

    RDF& operator+=(const RDF &other);
    RDF operator+(const RDF &other) const;

    RDF& operator+=(double distance);
    RDF operator+(double distance) const;

    bool operator==(const RDF &other) const;
    bool operator!=(const RDF &other) const;

    RDF changeResolution(uint new_nbins) const;

    RDF rescale(double new_min, double new_max, uint new_nbins) const;
    RDF rescale(const RDF &other) const;

    double minimumDistance() const;
    double maximumDistance() const;

    int nValues() const;
    int nBins() const;

    void clear();

    QVector< tuple<double,double> > normalise() const;

private:
    /** The actual histogram of data */
    QVector<quint32> histdata;

    /** The maximum and minimum values of the histogram */
    double minval, maxval;

    /** The width of each histogram bin */
    double binwidth;
};

namespace detail
{

/** This class is used to hold atom pairs, and is provided
    so that I can use these pairs as a key in a hash */
class SIRESYSTEM_EXPORT AtomPairs : public tuple< QSet<AtomIndex>,QSet<AtomIndex> >
{

friend QDataStream& ::operator<<(QDataStream&, const AtomPairs&);
friend QDataStream& ::operator>>(QDataStream&, AtomPairs&);

public:
    AtomPairs( const QSet<AtomIndex> &atoms0 = QSet<AtomIndex>(),
               const QSet<AtomIndex> &atoms1 = QSet<AtomIndex>() );

    AtomPairs(const AtomPairs &other);

    ~AtomPairs();

    AtomPairs& operator=(const AtomPairs &other);

    bool operator==(const AtomPairs &other) const;
    bool operator!=(const AtomPairs &other) const;

    uint hash() const;
};

} //end of namespace detail

/** This is a monitor that collects the RDFs between a molecule
    and the molecules in a MoleculeGroup.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT RDFMonitor : public SystemMonitorBase
{

friend QDataStream& ::operator<<(QDataStream&, const RDFMonitor&);
friend QDataStream& ::operator>>(QDataStream&, RDFMonitor&);

public:
    RDFMonitor();

    RDFMonitor(const Molecule &molecule,
               const MoleculeGroup &molecules);

    RDFMonitor(const RDFMonitor &other);

    ~RDFMonitor();

    RDFMonitor& operator=(const RDFMonitor &other);

    RDFMonitor& operator+=(const RDFMonitor &other);

    RDFMonitor operator+(const RDFMonitor &other) const;

    static const char* typeName()
    {
        return "SireSystem::RDFMonitor";
    }

    const char* what() const
    {
        return RDFMonitor::typeName();
    }

    RDFMonitor* clone() const
    {
        return new RDFMonitor(*this);
    }

    void monitor(QuerySystem &system);

    double value() const;

    void clear();

    void assertCompatibleWith(const QuerySystem &system) const;

    void addRDF(const QSet<AtomIndex> &atoms0,
                const QSet<AtomIndex> &atoms1,
                const RDF &rdf = RDF());

    const RDF& getRDF(const QSet<AtomIndex> &atoms0,
                      const QSet<AtomIndex> &atoms1) const;

    QList< tuple<detail::AtomPairs,RDF> > rdfs() const;

    const Molecule& molecule() const;

    const MoleculeGroup& group() const;

protected:
    bool _pvt_isEqual(const PropertyBase &other) const;

private:
    /** All of the RDFs in this monitor, indexed by the pairs
        of atoms being monitored */
    QHash<detail::AtomPairs,RDF> allrdfs;

    /** The molecule that must contain the first atom in the RDF atom pair */
    Molecule mol;

    /** The molecule group that contains the molecules that must contain the
        second atom in the RDF atom pair */
    MoleculeGroup mols;
};

}

Q_DECLARE_METATYPE(SireSystem::RDF);
Q_DECLARE_METATYPE(SireSystem::RDFMonitor);

SIRE_END_HEADER

#endif
