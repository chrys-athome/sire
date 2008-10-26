/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "rdfmonitor.h"
#include "system.h"

#include "SireMaths/constants.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMaths;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

using boost::tuple;

////////
//////// Implementation of RDF
////////

static const RegisterMetaType<RDF> r_rdf;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const RDF &rdf)
{
    writeHeader(ds, r_rdf, 1);
    
    SharedDataStream sds(ds);
    
    sds << rdf.distvals << static_cast<const HistogramRangeT<Length>&>(rdf);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, RDF &rdf)
{
    VersionID v = readHeader(ds, r_rdf);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> rdf.distvals >> static_cast<HistogramRangeT<Length>&>(rdf);
        
        rdf.is_dirty = true;
    }
    else
        throw version_error( v, "1", r_rdf, CODELOC );

    return ds;
}

/** Null constructor */
RDF::RDF() : HistogramRangeT<Length>(), is_dirty(false)
{}

static QVector<double> create(const HistogramRange &range)
{
    if (range.nBins() > 0)
    {
        QVector<double> bins(range.nBins(), 0.0);
        bins.squeeze();
        return bins;
    }
    else
        return QVector<double>();
}

/** Construct the RDF that spans from 'min' to 'max' using 'nbins' bins.
    Note that it is not possible to calculate negative distances */
RDF::RDF(const Length &min, const Length &max, int nbins)
    : HistogramRangeT<Length>(min, max, nbins), is_dirty(false)
{
    if (this->minimum() < 0)
        HistogramRange::operator=( HistogramRange( qMax(Length(0), this->minimum()),
                                                   qMax(Length(0), this->maximum()),
                                                   this->nBins()) );

    if (this->nBins() > 0)
    {
        is_dirty = true;
        distvals = ::create(*this);
    }
}

/** Construct the RDF that spans from 'min' to 'max' using a bin width
    of 'binwidth'. Note that it is not possible to calculate negative distances */
RDF::RDF(const Length &min, const Length &max, const Length &binwidth)
    : HistogramRangeT<Length>(min, max, binwidth), is_dirty(false)
{
    if (this->minimum() < 0)
        HistogramRange::operator=( HistogramRange( qMax(Length(0), this->minimum()),
                                                   qMax(Length(0), this->maximum()),
                                                   this->binWidth()) );

    if (this->nBins() > 0)
    {
        is_dirty = true;
        distvals = ::create(*this);
    }
}

/** Construct the RDF with the specified range */
RDF::RDF(const HistogramRangeT<Length> &range)
    : HistogramRangeT<Length>(range), is_dirty(false)
{
    if (this->minimum() < 0)
        HistogramRange::operator=( HistogramRange( qMax(Length(0), this->minimum()),
                                                   qMax(Length(0), this->maximum()),
                                                   this->binWidth()) );

    if (this->nBins() > 0)
    {
        is_dirty = true;
        distvals = ::create(*this);
    }
}

/** Copy constructor */
RDF::RDF(const RDF &other)
    : HistogramRangeT<Length>(other), distvals(other.distvals),
      rdfvals(other.rdfvals), is_dirty(other.is_dirty)
{}

/** Destructor */
RDF::~RDF()
{}

/** Copy assignment operator */
RDF& RDF::operator=(const RDF &other)
{
    if (this != &other)
    {
        HistogramRangeT<Length>::operator=(other);
        distvals = other.distvals;
        rdfvals = other.rdfvals;
        is_dirty = other.is_dirty;
    }
    
    return *this;
}

/** Comparison operator */
bool RDF::operator==(const RDF &other) const
{
    return HistogramRangeT<Length>::operator==(other) and distvals == other.distvals;
}

/** Comparison operator */
bool RDF::operator!=(const RDF &other) const
{
    return HistogramRangeT<Length>::operator!=(other) or distvals != other.distvals;
}

/** Return a string representation of this RDF */
QString RDF::toString() const
{
    return QObject::tr("RDF[ %1 <= x < %2 : nBins() == %3 ]")
                    .arg( Sire::toString(minimum()),
                          Sire::toString(maximum()) )
                    .arg( nBins() );
}

/** Internal function used to reconstruct the RDF */
void RDF::rebuildRDF() const
{
    if (not is_dirty)
        return;
        
    if (this->nBins() == 0)
        return;
        
    QVector<double> rdf( this->nBins() );
    rdf.squeeze();
    
    //calculate the sum of all bins
    double ndist = 0;
    
    for (int i=0; i<this->nBins(); ++i)
    {
        ndist += distvals.constData()[i];
    }
    
    //calculate the volume in which these distances were obtained
    double max = this->maximum();
    double min = this->minimum();
    
    double total_vol = ( pow_3(max) - pow_3(min) );
    
    //get the average density
    double avg_density = ndist / total_vol;
    
    //now calculate the density of each bin and divide that by the 
    //average density - this is the RDF
    double *rdf_array = rdf.data();
    
    for (int i=0; i<this->nBins(); ++i)
    {
        HistogramBinT<Length> bin = this->operator[](i);
        
        max = bin.maximum();
        min = bin.minimum();
        
        double bin_vol = ( pow_3(max) - pow_3(min) );
        
        double bin_density = distvals.constData()[i] / bin_vol;
        
        rdf_array[i] = bin_density / avg_density;
    }
    
    //cache this RDF - (so we don't have to keep recalculating it!)
    const_cast<RDF*>(this)->rdfvals = rdf;
    const_cast<RDF*>(this)->is_dirty = false;
}

/** Return the value of the RDF at the ith bin */
HistogramValueT<Length> RDF::operator[](int i) const
{
    i = Index(i).map( this->nBins() );

    this->rebuildRDF();

    return HistogramValueT<Length>( HistogramRange::operator[](i),
                                    rdfvals.constData()[i] );
}

/** Return a raw pointer to the array of values representing this RDF */
const double* RDF::data() const
{
    this->rebuildRDF();
    return rdfvals.constData();
}

/** Return a raw pointer to the array of values representing this RDF */
const double* RDF::constData() const
{
    return this->data();
}

/** Return the underlying distance histogram that is used to 
    construct this RDF. This is raw histogram of distances */
HistogramT<Length> RDF::distanceHistogram()
{
    return HistogramT<Length>(*this, distvals);
}

/** Add the distance 'distance' to this RDF */
void RDF::add(const Length &distance)
{
    int idx = this->bin(distance);
    
    if (idx != -1)
    {
        distvals[idx] += 1;

        if (not is_dirty)
        {
            is_dirty = true;
            rdfvals = QVector<double>();
        }
    }
}

/** Add the other RDF 'other' onto this RDF */
void RDF::add(const RDF &other)
{
    if ( static_cast<const HistogramRange&>(*this).operator==(other) )
    {
        //we have the same range - just add the values
        for (int i=0; i<this->nBins(); ++i)
        {
            distvals[i] += other.distvals.at(i);
        }
        
        if (not is_dirty)
        {
            is_dirty = true;
            rdfvals = QVector<double>();
        }
        
        return;
    }

    bool changed = false;

    for (int i=0; i<other.nBins(); ++i)
    {
        int min_idx = this->bin( other.minimum() );
        int max_idx = this->bin( other.maximum() );
        
        if (min_idx == -1)
        {
            if (max_idx == -1)
                continue;
                
            //put all of the values into the max bin
            min_idx = 0;
        }
        else if (max_idx == -1)
        {
            max_idx = this->nBins() - 1;
        }
        
        double val = other.distvals.at(i) / double(max_idx - min_idx + 1);
        
        changed = true;
        
        for (int j=min_idx; j<=max_idx; ++j)
        {
            this->distvals[j] += val;
        }
    }
    
    if (changed)
    {
        if (not is_dirty)
        {
            is_dirty = true;
            rdfvals = QVector<double>();
        }
    }
}

////////
//////// Implementation of RDFMonitor
////////

static const RegisterMetaType<RDFMonitor> r_rdfmon;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const RDFMonitor &rdfmon)
{
    writeHeader(ds, r_rdfmon, 1);
    
    SharedDataStream sds(ds);
    
    sds << rdfmon.rdfdata;
    
    sds << quint32( rdfmon.atomids.count() );
    
    for (QList< tuple<AtomIdentifier,AtomIdentifier> >::const_iterator
                                    it = rdfmon.atomids.constBegin();
         it != rdfmon.atomids.constEnd();
         ++it)
    {
        sds << it->get<0>() << it->get<1>();
    }

    sds << static_cast<const SystemMonitor&>(rdfmon);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, RDFMonitor &rdfmon)
{
    VersionID v = readHeader(ds, r_rdfmon);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        quint32 natomids = 0;
        sds >> rdfmon.rdfdata >> natomids;
        
        rdfmon.atomids.clear();
        
        for (quint32 i=0; i<natomids; ++i)
        {
            AtomIdentifier atom0, atom1;
            
            sds >> atom0 >> atom1;
            
            rdfmon.atomids.append( tuple<AtomIdentifier,AtomIdentifier>(atom0, atom1) );
        }
        
        sds >> static_cast<SystemMonitor&>(rdfmon);
    }
    else
        throw version_error( v, "1", r_rdfmon, CODELOC );
        
    return ds;
}

/** Default constructor - by default this monitors only
    the first 10 A, using bins of width 0.2 A */
RDFMonitor::RDFMonitor()
           : ConcreteProperty<RDFMonitor,SystemMonitor>(),
             rdfdata(Length(0), Length(10), Length(0.2))
{}

/** Construct a monitor to monitor the RDF between 
     min <= val < max, using 'nbins' bins */ 
RDFMonitor::RDFMonitor(const Length &min, const Length &max, int nbins)
           : ConcreteProperty<RDFMonitor,SystemMonitor>(),
             rdfdata(min, max, nbins)
{}

/** Construct a monitor to monitor the RDF between
     min <= val < max, using a bin width of 'binwidth' */
RDFMonitor::RDFMonitor(const Length &min, const Length &max, const Length &binwidth)
           : ConcreteProperty<RDFMonitor,SystemMonitor>(),
             rdfdata(min, max, binwidth)
{}

/** Construct a monitor to monitor the RDF using the passed range */
RDFMonitor::RDFMonitor(const HistogramRangeT<Length> &range)
           : ConcreteProperty<RDFMonitor,SystemMonitor>(),
             rdfdata(range)
{}

/** Copy constructor */
RDFMonitor::RDFMonitor(const RDFMonitor &other)
           : ConcreteProperty<RDFMonitor,SystemMonitor>(other),
             rdfdata(other.rdfdata), atomids(other.atomids)
{}

/** Destructor */
RDFMonitor::~RDFMonitor()
{}

/** Copy assignment operator */
RDFMonitor& RDFMonitor::operator=(const RDFMonitor &other)
{
    if (this != &other)
    {
        SystemMonitor::operator=(other);
        rdfdata = other.rdfdata;
        atomids = other.atomids;
    }
    
    return *this;
}

static bool operator==(const tuple<AtomIdentifier,AtomIdentifier> &pair0,
                       const tuple<AtomIdentifier,AtomIdentifier> &pair1)
{
    return (pair0.get<0>() == pair1.get<0>() and
            pair0.get<1>() == pair1.get<1>()) or
           
           (pair0.get<0>() == pair1.get<1>() and
            pair0.get<1>() == pair1.get<0>());
    
}

/** Comparison operator */
bool RDFMonitor::operator==(const RDFMonitor &other) const
{
    return SystemMonitor::operator==(other) and
           rdfdata == other.rdfdata and
           atomids == other.atomids;
}

/** Comparison operator */
bool RDFMonitor::operator!=(const RDFMonitor &other) const
{
    return not this->operator==(other);
}

/** Return the value of the RDF at the ith bin

    \throw SireError::invalid_index
*/
HistogramValueT<Length> RDFMonitor::operator[](int i) const
{
    return rdfdata[i];
}

/** Return the RDF */
const RDF& RDFMonitor::rdf() const
{
    return rdfdata;
}

/** Include the distances between all pairs of atoms that match
    the ID 'atom' to this RDF */
void RDFMonitor::add(const AtomID &atom)
{
    tuple<AtomIdentifier,AtomIdentifier> atompair(atom,atom);
    
    if (not atomids.contains(atompair))
        atomids.append( atompair );
}

/** Include the distances between all atoms that match the ID 'atom0'
    with all atoms that match the ID 'atom1' to this RDF */
void RDFMonitor::add(const AtomID &atom0, const AtomID &atom1)
{
    tuple<AtomIdentifier,AtomIdentifier> atompair(atom0,atom1);
    
    if (not atomids.contains(atompair))
        atomids.append( atompair );
}

/** Set the range for the RDF to min <= x < max, using 'nbins' bins */
void RDFMonitor::setRange(const Length &min, const Length &max, int nbins)
{
    RDF new_rdf(min, max, nbins);
    new_rdf.add( rdfdata );
    
    rdfdata = new_rdf;
}

/** Set the range for the RDF to min <= x < max, using a binwidth of 'binwidth' */
void RDFMonitor::setRange(const Length &min, const Length &max, 
                          const Length &binwidth)
{
    RDF new_rdf(min, max, binwidth);
    new_rdf.add( rdfdata );
    
    rdfdata = new_rdf;
}

/** Set the range for the RDF to 'range' */
void RDFMonitor::setRange(const HistogramRangeT<Length> &range)
{
    RDF new_rdf(range);
    new_rdf.add( rdfdata );
    
    rdfdata = new_rdf;
}

/** Return the minimum distance for the RDF */
Length RDFMonitor::minimum() const
{
    return rdfdata.minimum();
}

/** Return the middle distance for the RDF */
Length RDFMonitor::middle() const
{
    return rdfdata.middle();
}

/** Return the maximum distance for the RDF */
Length RDFMonitor::maximum() const
{
    return rdfdata.maximum();
}

/** Return the width of the bins */
Length RDFMonitor::binWidth() const
{
    return rdfdata.binWidth();
}

/** Return the number of bins */
int RDFMonitor::count() const
{
    return rdfdata.count();
}

/** Return the number of bins */
int RDFMonitor::nBins() const
{
    return rdfdata.nBins();
}

/** Add the matched distances from the system 'system' to this RDF */
void RDFMonitor::monitor(System &system)
{
}
