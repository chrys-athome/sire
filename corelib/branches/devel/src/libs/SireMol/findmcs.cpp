/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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


#include <QDataStream>
#include <QElapsedTimer>

#include <boost/assert.hpp>

#include "atomselection.h"
#include "atomelements.h"
#include "atommasses.h"
#include "connectivity.h"
#include "moleculedata.h"
#include "moleculeinfodata.h"
#include "moleculeview.h"
#include "molecule.h"
#include "mover.hpp"
#include "editor.hpp"
#include "atommatcher.h"

#include "angleid.h"
#include "bondid.h"
#include "dihedralid.h"
#include "improperid.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/units.h"

#include "SireMol/errors.h"
#include "SireBase/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

#include <boost/graph/mcgregor_common_subgraphs.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>

#include <QDebug>

using namespace SireStream;
using namespace SireMol;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;

using namespace boost;

/** This is the struct containing the callback used to process each
    boost::graph subgraph match from the mcgregor_common_subgraphs
    function */
template <typename G0, typename G1>
struct findmcs_callback
{
private:
    const G0& g0;
    const G1& g1;
    int max_nats;
    int *max_size;
    QList< QHash<AtomIdx,AtomIdx> > *best_matches;
    QElapsedTimer *t_total;
    QElapsedTimer *t_delta;
    qint64 last_update;
    qint64 max_time_ns;
    bool *timed_out;

public:
    findmcs_callback(const G0 &mg0, const G1 &mg1,
                     int nats, int *msize,
                     QList< QHash<AtomIdx,AtomIdx> > *matches,
                     QElapsedTimer *tt, QElapsedTimer *td,
                     qint64 maxtime, bool *timedout)
         : g0(mg0), g1(mg1), max_nats(nats), max_size(msize),
           best_matches(matches),
           t_total(tt), t_delta(td), last_update(0), max_time_ns(maxtime), timed_out(timedout)
    {
        *max_size = 0;
    }

    template <typename Map01, typename Map10>
    bool operator()(Map01 map01, Map10 map10,
                    typename graph_traits<G0>::vertices_size_type subgraph_size)
    {
        // count up the number of matching vertices
        int nmatch = 0;
    
        BGL_FORALL_VERTICES_T(v0, g0, G0)
        {
            if (get(map01, v0) != graph_traits<G1>::null_vertex())
            {
                nmatch += 1;
            }
        }
    
        if (nmatch > *max_size)
        {
            qint64 ns = t_delta->nsecsElapsed();
            t_delta->restart();
            *max_size = nmatch;
            best_matches->clear();
            qDebug() << "nmatch ==" << nmatch << " " << *max_size;
            qDebug() << "FOUND LARGER MATCH!";
            qDebug() << (0.000001*ns) << " ms since last maximum match...";
        }
    
        if (nmatch >= *max_size)
        {
            // Save the correspondence between vertices into the "best_matches" list
            QHash<AtomIdx,AtomIdx> map;
            
            BGL_FORALL_VERTICES_T(v0, g0, G0)
            {
                // Skip unmapped vertices
                if (get(map01, v0) != graph_traits<G1>::null_vertex())
                {
                    map.insert( AtomIdx(v0), AtomIdx(get(map01,v0)) );
                }
            }

            best_matches->append(map);
        
            if (nmatch == max_nats)
            {
                qDebug() << "No more atoms to match :-)";
                *timed_out = false;
                return false;
            }
        }

        qint64 ns = t_total->nsecsElapsed();
    
        if (ns > max_time_ns)
        {
            qDebug() << "Ran out of time. Returning the best answer.";
            *timed_out = true;
            return false;
        }
        else if (ns > last_update + 500000000)
        {
            qDebug() << "Still searching..." << (t_total->nsecsElapsed()*0.000001) << "ms";
        }

        return true;
    }
};

QVector<Element> getElements(const Molecule &mol, const PropertyMap &map)
{
    QVector<Element> elements(mol.nAtoms());

    try
    {
        if (mol.hasProperty(map["element"]))
        {
            const AtomElements &e = mol.property(map["element"]).asA<AtomElements>();
            
            for (int i=0; i<mol.nAtoms(); ++i)
            {
                elements[i] = e.at( mol.data().info().cgAtomIdx(AtomIdx(i)) );
            }
            
            return elements;
        }
    }
    catch(...)
    {}
    
    try
    {
        if (mol.hasProperty(map["mass"]))
        {
            const AtomMasses &m = mol.property(map["mass"]).asA<AtomMasses>();
            
            for (int i=0; i<mol.nAtoms(); ++i)
            {
                elements[i] = Element::elementWithMass( m.at(
                                            mol.data().info().cgAtomIdx(AtomIdx(i)) ) );
            }
            
            return elements;
        }
    }
    catch(...)
    {}

    throw SireBase::missing_property( QObject::tr(
            "Need to work out the element of each atom, so need an element or mass property."),
                CODELOC );

    return elements;
}

struct in_ring_t
{
    typedef edge_property_tag kind;
};

struct user_match_t
{
    typedef vertex_property_tag kind;
};

///////////
/////////// MCS part of Evaluator
///////////

/** Find the maximum common substructure of this molecule view with 'other'. This
    returns the mapping from this structure to 'other' for the matching parts,
    using the optionally supplied propertymap to find the elements, masses,
    connectivity and coordinates of the two molecules, with the passed 'atommatcher'
    used to pre-match atoms before the common substructure search (useful to speed
    up the search and to enforce matching sub-parts) */
QHash<AtomIdx,AtomIdx> Evaluator::findMCS(const MoleculeView &other,
                                          const AtomMatcher &matcher,
                                          const Time &timeout,
                                          const PropertyMap &map0,
                                          const PropertyMap &map1) const
{
    //first, see if the user has specified any match
    QHash<AtomIdx,AtomIdx> user_map01;
    QHash<AtomIdx,AtomIdx> user_map10;
    
    if (not matcher.isNull())
    {
        user_map01 = matcher.match(*this, map0, other, map1);
        
        //create the inverted map from molecule 1 to molecule 1
        for (QHash<AtomIdx,AtomIdx>::const_iterator it = user_map01.constBegin();
             it != user_map01.constEnd();
             ++it)
        {
            user_map10.insert( it.value(), it.key() );
        }
    }

    //now try to match up the atoms using their connectivity
    Connectivity c0;
    Connectivity c1;
    
    try
    {
        c0 = data().property( map0["connectivity"] ).asA<Connectivity>();
    }
    catch(...)
    {
        c0 = Connectivity( this->molecule() );
    }
    
    try
    {
        c1 = other.data().property( map1["connectivity"] ).asA<Connectivity>();
    }
    catch(...)
    {
        c1 = Connectivity( other.molecule() );
    }
    
    int nats0 = this->molecule().nAtoms();
    int nats1 = other.molecule().nAtoms();
    
    //remove any non-heavy or unselected atoms
    AtomSelection selected0 = this->selection();
    AtomSelection selected1 = other.selection();
    
    QVector<Element> elements0 = ::getElements(this->molecule(), map0);
    QVector<Element> elements1 = ::getElements(other.molecule(), map1);
    
    int nskip0 = 0;
    int nskip1 = 0;
    
    QVector<bool> skip0(nats0, false);
    QVector<bool> skip1(nats1, false);
    
    for (int i=0; i<nats0; ++i)
    {
        if (elements0[i].nProtons() < 6 or not selected0.selected(AtomIdx(i)))
        {
            nskip0 += 1;
            skip0[i] = true;
        }
    }

    for (int i=0; i<nats1; ++i)
    {
        if (elements1[i].nProtons() < 6 or not selected1.selected(AtomIdx(i)))
        {
            nskip1 += 1;
            skip1[i] = true;
        }
    }
    
    // we are now going to create two graphs. The vertices are the selected heavy
    // atoms, and the edges are the bonds between those atoms. The edges are
    // labelled according to whether the bond is part of a ring (in_ring_t is true of false).
    // The vertices are labelled according to the user-supplied match using the
    // AtomMatcher at the top of this function. If the atom has been already matched,
    // then, in graph 0, the vertex is labelled with the AtomIdx of the atom in
    // molecule 0, while in graph 1, the vertex is labelled with the AtomIdx of the
    // atom in molecule 0 that matches this atom in molecule 1. If the atom has not been
    // already matched then the vertex is labelled with "-1", meaning it is available
    // to match using the below algorithm
    
    typedef property<user_match_t,int> VertexProperty;
    typedef property<in_ring_t,bool> EdgeProperty;
    typedef adjacency_list<vecS,vecS,undirectedS,VertexProperty,EdgeProperty> Graph;
    
    Graph g0(nats0-nskip0);
    QVector<int> atomidx_to_idx0(nats0, -1);
    
    property_map<Graph,in_ring_t>::type in_ring_0 = get(in_ring_t(), g0);
    property_map<Graph,user_match_t>::type user_match_0 = get(user_match_t(), g0);
    int nvert0 = 0;

    for (int i=0; i<nats0; ++i)
    {
        if (not skip0[i])
        {
            atomidx_to_idx0[i] = nvert0;
            
            if (user_map01.contains(AtomIdx(i)))
            {
                put(user_match_0, nvert0, i);
            }
            else
            {
                put(user_match_0, nvert0, -1);
            }

            nvert0 += 1;
        
            foreach (const AtomIdx &atom, c0.connectionsTo(AtomIdx(i)))
            {
                if (atom.value() < i and not skip0[atom.value()])
                {
                    int idx0 = atomidx_to_idx0[i];
                    int idx1 = atomidx_to_idx0[atom.value()];
                
                    get(in_ring_t(), g0)[add_edge(idx0,idx1,g0).first] = c0.inRing(AtomIdx(i),atom);
                }
            }
        }
    }
    
    Graph g1(nats1-nskip1);
    QVector<int> atomidx_to_idx1(nats1, -1);
    
    property_map<Graph,in_ring_t>::type in_ring_1 = get(in_ring_t(), g1);
    property_map<Graph,user_match_t>::type user_match_1 = get(user_match_t(), g1);
    
    int nvert1 = 0;
    
    for (int i=0; i<nats1; ++i)
    {
        if (not skip1[i])
        {
            atomidx_to_idx1[i] = nvert1;
            
            if (user_map10.contains(AtomIdx(i)))
            {
                put(user_match_1, nvert1, user_map10.value(AtomIdx(i)).value());
            }
            else
            {
                put(user_match_1, nvert1, -1);
            }
            
            nvert1 += 1;
        
            foreach (const AtomIdx &atom, c1.connectionsTo(AtomIdx(i)))
            {
                if (atom.value() < i and not skip1[atom.value()])
                {
                    int idx0 = atomidx_to_idx1[i];
                    int idx1 = atomidx_to_idx1[atom.value()];
                
                    get(in_ring_t(), g1)[add_edge(idx0,idx1,g1).first] = c1.inRing(AtomIdx(i),atom);
                }
            }
        }
    }

    int max_size = 0;
    QList< QHash<AtomIdx,AtomIdx> > best_matches;
    const qint64 max_time_ns = timeout.to(nanosecond);
    qDebug() << "Using timeout" << timeout.to(second) << "second(s)";
    QElapsedTimer t_total;
    QElapsedTimer t_delta;
    bool timed_out = false;
    findmcs_callback<Graph,Graph> func(g0, g1, qMin(nats0-nskip0,nats1-nskip1), &max_size,
                                       &best_matches, &t_total, &t_delta, max_time_ns,
                                       &timed_out);
    
    t_total.start();
    mcgregor_common_subgraphs_unique(g0, g1, true, func,
                    edges_equivalent(make_property_map_equivalent(in_ring_0,in_ring_1)).
                    vertices_equivalent(make_property_map_equivalent(user_match_0,user_match_1)) );
                    
    qDebug() << "MATCH TOOK" << (0.000001*t_total.nsecsElapsed()) << "ms";
    
    if (timed_out)
    {
        qDebug() << "We ran out of time when looking for a match. You can speed things"
                 << "up by using an AtomMatcher to pre-match some of the atoms that you"
                 << "know should be equivalent, e.g. one of the rings, or the common"
                 << "framework of the molecules. As it is, only the best-found match"
                 << "in the time available is being returned, which may not correspond"
                 << "to the best possible match.";
    }
    
    if (best_matches.isEmpty())
    {
        qDebug() << "FOUND NO MATCHES?";
        return QHash<AtomIdx,AtomIdx>();
    }
    else
    {
        return best_matches.at(0);
    }
}
