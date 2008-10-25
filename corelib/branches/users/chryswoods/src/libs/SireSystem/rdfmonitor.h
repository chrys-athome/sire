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

#ifndef SIRESYSTEM_RDFMONITOR_H
#define SIRESYSTEM_RDFMONITOR_H

#include <QList>

#include <boost/tuple/tuple.hpp>

#include "systemmonitor.h"

#include "SireMaths/histogram.h"

#include "SireMol/atomidentifier.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class RDF;
class RDFMonitor;
}

QDataStream& operator<<(QDataStream&, const SireSystem::RDF&);
QDataStream& operator>>(QDataStream&, SireSystem::RDF&);

QDataStream& operator<<(QDataStream&, const SireSystem::RDFMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::RDFMonitor&);

namespace SireSystem
{

using SireMaths::HistogramValue;
using SireMaths::HistogramRange;
using SireMaths::Histogram;

using SireMol::AtomID;
using SireMol::AtomIdentifier;

/** This class holds a radial distribution function */
class SIRESYSTEM_EXPORT RDF : public HistogramRange
{

friend QDataStream& ::operator<<(QDataStream&, const RDF&);
friend QDataStream& ::operator>>(QDataStream&, RDF&);

public:
    RDF();
    RDF(double min, double max, int nbins=100);
    RDF(double min, double max, double binwidth);
    
    RDF(const RDF &other);
    
    ~RDF();
    
    RDF& operator=(const RDF &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RDF>() );
    }
    
    const char* what() const
    {
        return RDF::typeName();
    }
    
    RDF* clone() const
    {
        return new RDF(*this);
    }
    
    bool operator==(const RDF &other) const;
    bool operator!=(const RDF &other) const;

    QString toString() const;
    
    HistogramValue operator[](int i) const;
    
    const double* data() const;
    const double* constData() const;
    
    Histogram distanceHistogram();
    
    void add(double distance);

private:
    void rebuildRDF() const;

    /** The raw histogram of distances */
    QVector<double> distvals;
    
    /** The actual RDF */
    QVector<double> rdfvals;
    
    /** Whether or not the RDF values are dirty (need recalculating) */
    bool is_dirty;
};

/** This monitor collects the radial distribution function (RDF)
    between two sets of atoms
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT RDFMonitor 
         : public SireBase::ConcreteProperty<RDFMonitor,SystemMonitor>
{

friend QDataStream& ::operator<<(QDataStream&, const RDFMonitor&);
friend QDataStream& ::operator>>(QDataStream&, RDFMonitor&);

public:
    RDFMonitor();
    
    RDFMonitor(double min, double max, int nbins);
    RDFMonitor(double min, double max, double binwidth);
    
    RDFMonitor(const HistogramRange &range);
    
    RDFMonitor(const RDFMonitor &other);
    
    ~RDFMonitor();
    
    RDFMonitor& operator=(const RDFMonitor &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RDFMonitor>() );
    }
    
    RDFMonitor* clone() const
    {
        return new RDFMonitor(*this);
    }
    
    bool operator==(const RDFMonitor &other) const;
    bool operator!=(const RDFMonitor &other) const;
    
    HistogramValue operator[](int i) const;
    
    const RDF& rdf() const;
    
    void add(const AtomID &atom);
    void add(const AtomID &atom0, const AtomID &atom1);

    void setRange(double min, double max, int nbins);
    void setRange(double min, double max, double binwidth);
    void setRange(const HistogramRange &range);
    
    double minimum() const;
    double middle() const;
    double maximum() const;
    
    double binWidth() const;
    
    int count() const;
    int nBins() const;

    void monitor(System &system);

private:
    /** The actual RDF */
    RDF rdfdata;

    /** The collection of IDs used to identify the atoms whose
        distances will be evaluated */
    QList< boost::tuple<AtomIdentifier,AtomIdentifier> > atomids;
};

}

Q_DECLARE_METATYPE( SireSystem::RDF )
Q_DECLARE_METATYPE( SireSystem::RDFMonitor )

SIRE_EXPOSE_CLASS( SireSystem::RDF )
SIRE_EXPOSE_CLASS( SireSystem::RDFMonitor )

SIRE_END_HEADER

#endif
