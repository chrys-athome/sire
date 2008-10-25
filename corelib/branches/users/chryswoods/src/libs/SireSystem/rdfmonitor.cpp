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

////////
//////// Implementation of RDF
////////

static const RegisterMetaType<RDF> r_rdf;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const RDF &rdf)
{
    writeHeader(ds, r_rdf, 1);
    
    SharedDataStream sds(ds);
    
    sds << rdf.distvals << static_cast<const HistogramRange&>(rdf);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, RDF &rdf)
{
    VersionID v = readHeader(ds, r_rdf);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> rdf.distvals >> static_cast<HistogramRange&>(rdf);
        
        rdf.is_dirty = true;
    }
    else
        throw version_error( v, "1", r_rdf, CODELOC );

    return ds;
}

/** Null constructor */
RDF::RDF() : HistogramRange(), is_dirty(false)
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
RDF::RDF(double min, double max, int nbins)
    : HistogramRange(min, max, nbins), is_dirty(false)
{
    if (this->minimum() < 0)
        HistogramRange::operator=( HistogramRange( qMax(0.0, this->minimum()),
                                                   qMax(0.0, this->maximum()),
                                                   this->nBins()) );

    if (this->nBins() > 0)
    {
        is_dirty = true;
        distvals = ::create(*this);
    }
}

/** Construct the RDF that spans from 'min' to 'max' using a bin width
    of 'binwidth'. Note that it is not possible to calculate negative distances */
RDF::RDF(double min, double max, double binwidth)
    : HistogramRange(min, max, binwidth), is_dirty(false)
{
    if (this->minimum() < 0)
        HistogramRange::operator=( HistogramRange( qMax(0.0, this->minimum()),
                                                   qMax(0.0, this->maximum()),
                                                   this->binWidth()) );

    if (this->nBins() > 0)
    {
        is_dirty = true;
        distvals = ::create(*this);
    }
}

/** Copy constructor */
RDF::RDF(const RDF &other)
    : HistogramRange(other), distvals(other.distvals),
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
        HistogramRange::operator=(other);
        distvals = other.distvals;
        rdfvals = other.rdfvals;
        is_dirty = other.is_dirty;
    }
    
    return *this;
}

/** Comparison operator */
bool RDF::operator==(const RDF &other) const
{
    return HistogramRange::operator==(other) and distvals == other.distvals;
}

/** Comparison operator */
bool RDF::operator!=(const RDF &other) const
{
    return HistogramRange::operator!=(other) or distvals != other.distvals;
}

/** Return a string representation of this RDF */
QString RDF::toString() const
{
    return QObject::tr("RDF[ %1 <= x < %2 : nBins() == %3 ]")
                    .arg( minimum() ).arg( maximum() ).arg( nBins() );
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
    double total_vol = ( pow_3(this->maximum()) - pow_3(this->minimum()) );
    
    //get the average density
    double avg_density = ndist / total_vol;
    
    //now calculate the density of each bin and divide that by the 
    //average density - this is the RDF
    double *rdf_array = rdf.data();
    
    for (int i=0; i<this->nBins(); ++i)
    {
        HistogramBin bin = this->operator[](i);
        
        double bin_vol = ( pow_3(bin.maximum()) - pow_3(bin.minimum()) );
        
        double bin_density = distvals.constData()[i] / bin_vol;
        
        rdf_array[i] = bin_density / avg_density;
    }
    
    //cache this RDF - (so we don't have to keep recalculating it!)
    const_cast<RDF*>(this)->rdfvals = rdf;
    const_cast<RDF*>(this)->is_dirty = false;
}

/** Return the value of the RDF at the ith bin */
HistogramValue RDF::operator[](int i) const
{
    i = Index(i).map( this->nBins() );

    this->rebuildRDF();

    return HistogramValue( HistogramRange::operator[](i),
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
Histogram RDF::distanceHistogram()
{
    return Histogram(*this, distvals);
}

/** Add the distance 'distance' to this RDF */
void RDF::add(double distance)
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

////////
//////// Implementation of RDFMonitor
////////

