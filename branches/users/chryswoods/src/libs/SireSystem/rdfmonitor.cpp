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

#include "qhash_siresystem.h"

#include <boost/tuple/tuple.hpp>

inline uint qHash( const boost::tuple<uint,uint> &ids )
{
    return (ids.get<0>() << 16) | (ids.get<1>() & 0x0000FFFF);
}

inline bool operator==(const boost::tuple<uint,uint> &ids0,
                       const boost::tuple<uint,uint> &ids1)
{
    return ids0.get<0>() == ids1.get<0>() and
           ids0.get<1>() == ids1.get<1>();
}

#include "rdfmonitor.h"

#include "SireSystem/querysystem.h"
#include "SireSystem/systemdata.h"

#include "SireVol/space.h"

#include "SireMol/moleculegroups.h"
#include "SireMol/molecules.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/cgatomid.h"

#include "SireMaths/maths.h"
#include "SireUnits/units.h"

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

using namespace SireSystem;
using namespace SireSystem::detail;

using namespace SireMol;
using namespace SireMaths;
using namespace SireUnits;
using namespace SireVol;
using namespace SireStream;

using Sire::toString;

////////////
//////////// Implementation of RDF
////////////

static const RegisterMetaType<RDF> r_rdf;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const RDF &rdf)
{
    writeHeader(ds, r_rdf, 1);

    SharedDataStream sds(ds);

    sds << rdf.histdata << rdf.minval << rdf.maxval << rdf.binwidth;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, RDF &rdf)
{
    VersionID v = readHeader(ds, r_rdf);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> rdf.histdata >> rdf.minval >> rdf.maxval >> rdf.binwidth;
    }
    else
        throw version_error(v, "1", r_rdf, CODELOC);

    return ds;
}

/** Construct an RDF that monitors distances from 'min' to 'max', using
    a histogram with 'nbins' bins. */
RDF::RDF(double min, double max, uint nbins)
    : minval(min), maxval(max)
{
    if (min < 0)
        min = 0;

    if (max < 0)
        max = 0;

    if (max < min)
        qSwap(min, max);

    if (nbins == 0)
        nbins = 1;

    binwidth = (max - min) / nbins;

    histdata = QVector<quint32>(nbins, 0);
}

/** Copy constructor */
RDF::RDF(const RDF &other)
    : histdata(other.histdata),
      minval(other.minval), maxval(other.maxval),
      binwidth(other.binwidth)
{}

/** Destructor */
RDF::~RDF()
{}

/** Copy assignment operator */
RDF& RDF::operator=(const RDF &other)
{
    if (this != &other)
    {
        histdata = other.histdata;
        minval = other.minval;
        maxval = other.maxval;
        binwidth = other.binwidth;
    }

    return *this;
}

/** Comparison operator */
bool RDF::operator==(const RDF &other) const
{
    return this == &other or
           (minval == other.minval and maxval == other.maxval and
            binwidth == other.binwidth and histdata == other.histdata);
}

/** Comparison operator */
bool RDF::operator!=(const RDF &other) const
{
    return this != &other and
           (minval != other.minval or maxval != other.maxval or
            binwidth != other.binwidth or histdata != other.histdata);
}

/** Rescale this RDF so that it now goes from 'new_min' to 'new_max',
    with the number of bins now equal to 'new_nbins' - this returns
    the resulting new RDF. */
RDF RDF::rescale(double new_min, double new_max, uint new_nbins) const
{
    if (new_min < 0)
        new_min = 0;

    if (new_max < 0)
        new_max = 0;

    if (new_max < new_min)
        qSwap(new_min, new_max);

    if (new_nbins == 0)
        new_nbins = 1;

    if (new_nbins == uint(histdata.count()) and
        new_min == minval and new_max == maxval)
    {
        //there is no change :-)
        return *this;
    }

    //ok... what to do now...
    throw SireError::incomplete_code( QObject::tr(
                "Need to write histogram rescaling code!"),
                    CODELOC );

    RDF copy = *this;

    return copy;
}

/** Change the resolution of this histogram so that it has the same
    scale and bin size as 'other' */
RDF RDF::rescale(const RDF &other) const
{
    return this->rescale(other.minval, other.maxval, other.histdata.count());
}

/** Change the resolution of this histogram - this will redistribute
    the statistics according to the new number of bins, and will return
    the result. */
RDF RDF::changeResolution(uint new_nbins) const
{
    return this->rescale(minval, maxval, new_nbins);
}

/** Add another histogram onto this one */
RDF& RDF::operator+=(const RDF &other)
{
    RDF scaled_other = other.rescale(*this);

    BOOST_ASSERT( histdata.count() == scaled_other.histdata.count() );

    int nbins = histdata.count();
    quint32 *histdata_array = histdata.data();
    const quint32 *other_array = scaled_other.histdata.constData();

    for (int i=0; i<nbins; ++i)
    {
        histdata_array[i] += other_array[i];
    }

    return *this;
}

/** Add 'other' onto a copy of this RDF and return the result */
RDF RDF::operator+(const RDF &other) const
{
    RDF copy = *this;
    copy += other;

    return copy;
}

/** Add the passed distance into this RDF */
RDF& RDF::operator+=(double distance)
{
    if (distance >= minval and distance < maxval)
    {
        int bin = int( (distance - minval) / binwidth );

        histdata[bin] += 1;
    }

    return *this;
}

/** Return a copy of this RDF that has had the passed
    distance added to it. */
RDF RDF::operator+(double distance) const
{
    RDF copy = *this;
    copy += distance;
    return copy;
}

/** Return the minimum distance that this RDF will record */
double RDF::minimumDistance() const
{
    return minval;
}

/** Return the maximum distance that this RDF will record */
double RDF::maximumDistance() const
{
    return maxval;
}

/** Return the total number of data points in this RDF */
int RDF::nValues() const
{
    int nbins = histdata.count();
    const quint32 *histdata_array = histdata.constData();

    int total = 0;

    for (int i=0; i<nbins; ++i)
        total += histdata_array[i];

    return total;
}

/** Return the number of bins in the RDF */
int RDF::nBins() const
{
    return histdata.count();
}

/** Clear all of the statistics */
void RDF::clear()
{
    histdata = QVector<quint32>( histdata.count() );
}

/** Normalise this RDF, returning an array containing the distances
    and the associated, normalised, probabilities. The array
    is ordered according to increasing distance */
QVector< tuple<double,double> > RDF::normalise() const
{
    //calculate the total volume of the space within which
    //RDF statistics have been collected
    double vol = (4/3) * pi * ( pow_3(maxval) - pow_3(minval) );

    //what is the density of the points?
    double density = nValues() / vol;

    //now loop over each bin and calculate the distance at the center
    //of the bin, and the normalised probability (calculated as the
    //actual density of observations in the shell divided by the
    //average density for that shell)
    int nbins = histdata.count();

    const quint32 *histdata_array = histdata.constData();

    QVector< tuple<double,double> > normdata(nbins);
    tuple<double,double> *normdata_array = normdata.data();

    for (int i=0; i<nbins; ++i)
    {
        double dist = (0.5 + i) * binwidth;

        double prob = histdata_array[i] / (4 * pi * pow_2(dist) * binwidth * density);

        normdata_array[i] = tuple<double,double>(dist, prob);
    }

    return normdata;
}

////////////
//////////// Implementation of detail::AtomPairs
////////////

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const AtomPairs &atompairs)
{
    SharedDataStream sds(ds);

    sds << atompairs.get<0>() << atompairs.get<1>();

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          AtomPairs &atompairs)
{
    SharedDataStream sds(ds);

    sds >> atompairs.get<0>() >> atompairs.get<1>();

    return ds;
}

/** Constructor */
AtomPairs::AtomPairs(const QSet<AtomIndex> &atoms0,
                     const QSet<AtomIndex> &atoms1)
          : tuple< QSet<AtomIndex>,QSet<AtomIndex> >(atoms0,atoms1)
{}

/** Copy constructor */
AtomPairs::AtomPairs(const AtomPairs &other)
          : tuple< QSet<AtomIndex>,QSet<AtomIndex> >(other)
{}

/** Destructor */
AtomPairs::~AtomPairs()
{}

/** Copy assignment operator */
AtomPairs& AtomPairs::operator=(const AtomPairs &other)
{
    tuple< QSet<AtomIndex>,QSet<AtomIndex> >::operator=(other);
    return *this;
}

/** Comparison operator */
bool AtomPairs::operator==(const AtomPairs &other) const
{
    return this->get<0>() == other.get<0>() and
           this->get<1>() == other.get<1>();
}

/** Comparison operator */
bool AtomPairs::operator!=(const AtomPairs &other) const
{
    return this->get<0>() != other.get<0>() or
           this->get<1>() != other.get<1>();
}

/** Hash this set of pairs */
uint AtomPairs::hash() const
{
    return ( (this->get<0>().count()) << 16 )
                            | ( (this->get<1>().count()) & 0x0000FFFF );
}

uint qHash(const SireSystem::detail::AtomPairs &pairs)
{
    return pairs.hash();
}

////////////
//////////// Implementation of RDFMonitor
////////////

static const RegisterMetaType<RDFMonitor> r_rdfmonitor;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const RDFMonitor &rdfmonitor)
{
    writeHeader(ds, r_rdfmonitor, 1);

    SharedDataStream sds(ds);

    sds << rdfmonitor.allrdfs << rdfmonitor.mol << rdfmonitor.mols;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          RDFMonitor &rdfmonitor)
{
    VersionID v = readHeader(ds, r_rdfmonitor);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> rdfmonitor.allrdfs >> rdfmonitor.mol >> rdfmonitor.mols;
    }
    else
        throw version_error(v, "1", r_rdfmonitor, CODELOC);

    return ds;
}

/** Null constructor */
RDFMonitor::RDFMonitor() : SystemMonitorBase()
{}

/** Construct an RDFMonitor that monitors the RDF between the molecule
    'molecule' and all of the molecules in the the group 'molecules' */
RDFMonitor::RDFMonitor(const Molecule &molecule,
                       const MoleculeGroup &molecules)
           : SystemMonitorBase(),
             mol(molecule), mols(molecules)
{}

/** Copy constructor */
RDFMonitor::RDFMonitor(const RDFMonitor &other)
           : SystemMonitorBase(other),
             allrdfs(other.allrdfs), mol(other.mol), mols(other.mols)
{}

/** Destructor */
RDFMonitor::~RDFMonitor()
{}

/** Copy assignment operator */
RDFMonitor& RDFMonitor::operator=(const RDFMonitor &other)
{
    if (this != &other)
    {
        allrdfs = other.allrdfs;
        mol = other.mol;
        mols = other.mols;
    }

    return *this;
}

/** Comparison function */
bool RDFMonitor::_pvt_isEqual(const PropertyBase &other) const
{
    BOOST_ASSERT(other.isA<RDFMonitor>());

    const RDFMonitor &other_rdf = other.asA<RDFMonitor>();

    return allrdfs == other_rdf.allrdfs and
           mol == other_rdf.mol and
           mols == other_rdf.mols;
}

/** Add the contents of the other RDF onto this one. This does not
    check that the two RDFMonitors are consistent - it just
    adds all of the RDFs from 'other' onto this monitor's RDFs */
RDFMonitor& RDFMonitor::operator+=(const RDFMonitor &other)
{
    for (QHash<AtomPairs,RDF>::const_iterator it = other.allrdfs.constBegin();
         it != other.allrdfs.constEnd();
         ++it)
    {
        if (allrdfs.contains(it.key()))
            allrdfs[it.key()] += *it;
        else
            allrdfs.insert(it.key(), it.value());
    }

    return *this;
}

/** Return the result of adding another RDFMonitor onto this one.
    This doesn't check whether or not the RDFs are compatible -
    it just adds the RDFs from 'other' onto this monitor's RDFs */
RDFMonitor RDFMonitor::operator+(const RDFMonitor &other) const
{
    RDFMonitor copy = *this;
    copy += other;
    return copy;
}

static void addToRDF(RDF &rdf,
                     const CGAtomID &cgatomid0, const CGAtomID &cgatomid1,
                     const QVector<CoordGroup> &coords0,
                     const QVector<CoordGroup> &coords1,
                     const Space &space,
                     QHash<tuple<uint,uint>, DistMatrix> &pairdist2)
{
    tuple<uint,uint> cgids( cgatomid0.cutGroupID(),
                            cgatomid1.cutGroupID() );

    //have we already calculated the intergroup distances for
    //these two CoordGroups?
    QHash<tuple<uint,uint>,DistMatrix>::const_iterator
                                          it = pairdist2.constFind(cgids);

    if (it == pairdist2.constEnd())
    {
        //no - we need to calculate the distances
        const CoordGroup &group0 = coords0.at(cgatomid0.cutGroupID());
        const CoordGroup &group1 = coords1.at(cgatomid1.cutGroupID());

        QHash<tuple<uint,uint>,DistMatrix>::iterator
                            it2 = pairdist2.insert(cgids, DistMatrix());

        if (not space.beyond(rdf.maximumDistance(), group0, group1))
        {
            space.calcDist2(group0, group1, *it2);
        }

        it = it2;
    }

    //if the distance matrix is empty then the CoordGroups are out of range
    if (it->nInner() == 0 or it->nOuter() == 0)
        return;

    //get the distance and add it to the RDF
    rdf += std::sqrt( it->at(cgatomid0.atomID(), cgatomid1.atomID()) );
}

/** Monitor the passed system - this calculates the current set of RDFs
    for this configuration of the system and adds them onto the
    running average. */
void RDFMonitor::monitor(QuerySystem &system)
{
    //get the space that is used to calculate intermolecular distances
    const Space &space = system.info().space();

    //get the molecules over which the RDF will be calculated
    mol = system.molecule(mol.ID());
    mols = system.groups().group(mols.ID());

    Molecules molecules = mols.molecules();

    //get the CoordGroups containing the coordinates of the central
    //molecule
    QVector<CoordGroup> coords0 = mol.extract().coordGroups();

    BOOST_ASSERT( coords0.count() != 0 );

    //loop over all of the molecules and calculate all of the RDFs for each
    //molecule in turn
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        //this is a cache of inter-group distances for a pair of CutGroups
        QHash< tuple<uint,uint>, DistMatrix > pairdist2;

        const MoleculeInfo &molinfo = it->info();

        //get the CoordGroups containing the coordinates of all the atoms in
        //this molecule
        QVector<CoordGroup> coords1 = it->molecule().extract().coordGroups();

        BOOST_ASSERT(coords1.count() != 0);

        //now loop over all of the atom-pair RDFs...
        for (QHash<AtomPairs,RDF>::iterator it = allrdfs.begin();
             it != allrdfs.end();
             ++it)
        {
            const QSet<AtomIndex> &atoms0 = it.key().get<0>();
            const QSet<AtomIndex> &atoms1 = it.key().get<1>();

            RDF &rdf = *it;

            //loop over all pairs of atoms...
            foreach (AtomIndex atom0, atoms0)
            {
                CGAtomID cgatomid0 = molinfo[atom0];

                foreach (AtomIndex atom1, atoms1)
                {
                    CGAtomID cgatomid1 = molinfo[atom1];

                    addToRDF(rdf, cgatomid0, cgatomid1,
                             coords0, coords1, space, pairdist2);
                }
            }
        }
    }
}

/** An RDFMonitor has no valid value, so always returns '0' */
double RDFMonitor::value() const
{
    return 0;
}

/** Completely clear the statistics of this RDFMonitor */
void RDFMonitor::clear()
{
    for (QHash<AtomPairs,RDF>::iterator it = allrdfs.begin();
         it != allrdfs.end();
         ++it)
    {
        it->clear();
    }
}

/** Assert that this monitor is compatible with the passed system */
void RDFMonitor::assertCompatibleWith(const QuerySystem &system) const
{
    //assert that the system contains the central molecule
    system.assertContains(mol.ID());

    //now assert that the system contains this group of molecules,
    //and that all of the molecules contain all of the atoms to
    //which an RDF will be calculated
    Molecules sysmols = system.groups().group(mols.ID()).molecules();

    for (Molecules::const_iterator it = sysmols.constBegin();
         it != sysmols.constEnd();
         ++it)
    {
        for (QHash<AtomPairs,RDF>::const_iterator it2 = allrdfs.constBegin();
             it2 != allrdfs.constEnd();
             ++it2)
        {
            foreach (AtomIndex atom1, it2.key().get<1>())
            {
                it->info().assertAtomExists(atom1);
            }
        }
    }
}

/** Add an RDF between 'atom0' of the central molecule and 'atom1'
    of all of the surrounding molecules to the list of RDFs to be
    monitored

    \throw SireMol::missing_atom
    \throw SireError::invalid_arg
*/
void RDFMonitor::addRDF(const QSet<AtomIndex> &atoms0,
                        const QSet<AtomIndex> &atoms1,
                        const RDF &rdf)
{
    if (atoms0.isEmpty() or atoms1.isEmpty())
        throw SireError::invalid_arg( QObject::tr(
            "You must specify at least one atom in each pair! { %1 }-{ %2 }")
                .arg( toString(atoms0), toString(atoms1) ), CODELOC );

    foreach (AtomIndex atom0, atoms0)
    {
        mol.info().assertAtomExists(atom0);
    }

    AtomPairs atmpair(atoms0, atoms1);

    if (allrdfs.contains(atmpair))
        allrdfs[atmpair] += rdf;
    else
        allrdfs.insert(atmpair, rdf);
}

/** Return the RDF between the specified pair of atoms

    \throw SireMol::missing_atom
*/
const RDF& RDFMonitor::getRDF(const QSet<AtomIndex> &atoms0,
                              const QSet<AtomIndex> &atoms1) const
{
    QHash<AtomPairs,RDF>::const_iterator it = allrdfs.find( AtomPairs(atoms0,atoms1) );

    if (it == allrdfs.end())
        throw SireMol::missing_atom( QObject::tr(
            "There is no RDF associated with the atom pairs { %1 }-{ %2 }.")
                .arg( toString(atoms0), toString(atoms1) ),
                    CODELOC );

    return *it;
}

/** Return the hash of all of the RDFs in this monitor */
const QHash<AtomPairs,RDF>& RDFMonitor::rdfs() const
{
    return allrdfs;
}

/** Return the central molecule from which all RDFs are calculated */
const Molecule& RDFMonitor::molecule() const
{
    return mol;
}

/** Return the MoleculeGroup that contains all of the molecules
    from which the RDFs are calculated */
const MoleculeGroup& RDFMonitor::group() const
{
    return mols;
}
