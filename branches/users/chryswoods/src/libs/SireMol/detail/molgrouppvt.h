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

#ifndef SIREMOL_DETAIL_MOLGROUPPVT_H
#define SIREMOL_DETAIL_MOLGROUPPVT_H

#include <QSharedData>

#include <boost/tuple/tuple.hpp>

using boost::tuple;

///////////
/////////// Implementation of MolGroupPvt
///////////

namespace SireMol
{

namespace detail
{

/** This is the private implementation of MolGroup - this
    allows the MolGroup class to be implicitly shared.

    @author Christopher Woods
*/
class MolGroupPvt : public QSharedData
{

public:
    MolGroupPvt();
    MolGroupPvt(const QString &name);

    MolGroupPvt(const QString &name, const Molecules &molecules);

    MolGroupPvt(const QString &name, const MolGroupPvt &other);

    MolGroupPvt(const MolGroupPvt &other);

    ~MolGroupPvt();

    MolGroupPvt& operator=(const MolGroupPvt &other);

    bool operator==(const MolGroupPvt &other) const;
    bool operator!=(const MolGroupPvt &other) const;
    
    void add(const MoleculeView &view);
    void add(const ViewsOfMol &views);
    void add(const Molecules &molecules);
    void add(const MolGroupPvt &molgroup);
    
    bool update(const MoleculeData &moldata);
    
    bool remove(const MoleculeView &view);
    bool remove(const ViewsOfMol &views);
    bool remove(const Molecules &molecules);
    
    bool remove(MolNum molnum);
    
    bool removeAll();

    bool setContents(const Molecules &molecules);
    bool setContents(const MolGroupPvt &molgroup);

    void incrementMajor();
    void incrementMinor();

    /** The actual molecules in the group */
    Molecules molecules;
    
    /** The numbers of the molecules, in the order they appear
         in this group */
    QVector<MolNum> molidx_to_num;
    
    /** The molviewids of the views, in the order they appear
        in this group */
    QVector< tuple<MolNum,Index> > molviewidx_to_num;
    
    /** The name of this group */
    MGName name;
    
    /** The number of this group */
    MGNum number;
    
    /** The major version number - this changes whenever
        views are added or removed from this group */
    quint64 major_version;
    
    /** The minor version number - this changes whenever
        molecules in the group are changed */
    quint64 minor_version;
};

} //end of namespace detail

} //end of namespace SireMol

#endif
