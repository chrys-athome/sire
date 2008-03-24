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

#ifndef SIREFF_G1FF_H
#define SIREFF_G1FF_H

#include "ff.h"
#include "ffmolgroup.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class G1FF;
}

QDataStream& operator<<(QDataStream&, const SireFF::G1FF&);
QDataStream& operator>>(QDataStream&, SireFF::G1FF&);

namespace SireFF
{

/** This is the base class of all forcefields that hold just
    a single group of molecules
    
    @author Christopher Woods
*/
class SIREFF_EXPORT G1FF : public FF
{

friend QDataStream& ::operator<<(QDataStream&, const G1FF&);
friend QDataStream& ::operator>>(QDataStream&, G1FF&);

public:
    ~G1FF();

    ///////
    /////// Overloading MolGroups functions
    ///////
    
    void add(const MoleculeView &molview, 
             const ParameterMap &map = PropertyMap());
    void add(const ViewsOfMol &molviews,
             const ParameterMap &map = PropertyMap());
    void add(const Molecules &molecules,
             const ParameterMap &map = PropertyMap());
    void add(const MolGroup &molgroup,
             const ParameterMap &map = PropertyMap());
                            
    void addIfUnique(const MoleculeView &molview,
                     const PropertyMap &map = PropertyMap());
    void addIfUnique(const ViewsOfMol &molviews,
                     const PropertyMap &map = PropertyMap());
    void addIfUnique(const Molecules &molecules,
                     const PropertyMap &map = PropertyMap());
    void addIfUnique(const MolGroup &molgroup,
                     const PropertyMap &map = PropertyMap());
                            
    void add(const MoleculeView &molview, const MGID &mgid,
             const PropertyMap &map);
    void add(const ViewsOfMol &molviews, const MGID &mgid,
             const PropertyMap &map);
    void add(const Molecules &molecules, const MGID &mgid,
             const PropertyMap &map);
    void add(const MolGroup &molgroup, const MGID &mgid,
             const PropertyMap &map);
    
    void addIfUnique(const MoleculeView &molview, const MGID &mgid,
                     const PropertyMap &map);
    void addIfUnique(const ViewsOfMol &molviews, const MGID &mgid
                     const PropertyMap &map);
    void addIfUnique(const Molecules &molecules, const MGID &mgid
                     const PropertyMap &map);
    void addIfUnique(const MolGroup &molgroup, const MGID &mgid
                     const PropertyMap &map);
    
    void remove(const MoleculeView &molview);
    void remove(const ViewsOfMol &molviews);
    void remove(const Molecules &molecules);
    void remove(const MolGroup &molgroup);
    
    void removeAll(const MoleculeView &molview);
    void removeAll(const ViewsOfMol &molviews);
    void removeAll(const Molecules &molecules);
    void removeAll(const MolGroup &molgroup);

    void remove(MolNum molnum);
    void remove(const QSet<MolNum> &molnums);

    void removeAll(const MGID &mgid);
    void removeAll();
    
    void remove(const MoleculeView &molview, const MGID &mgid);
    void remove(const ViewsOfMol &molviews, const MGID &mgid);
    void remove(const Molecules &molecules, const MGID &mgid);
    void remove(const MolGroup &molgroup, const MGID &mgid);
    
    void removeAll(const MoleculeView &molview, const MGID &mgid);
    void removeAll(const ViewsOfMol &molviews, const MGID &mgid);
    void removeAll(const Molecules &molecules, const MGID &mgid);
    void removeAll(const MolGroup &molgroup, const MGID &mgid);

    void remove(MolNum molnum, const MGID &mgid);
    void remove(const QSet<MolNum> &molnums, const MGID &mgid);

    void update(const MoleculeData &moldata);
    void update(const MoleculeView &molview);
    
    void update(const Molecules &molecules);
    void update(const MolGroup &molgroup);
    
    void setContents(const MGID &mgid, const MoleculeView &molview,
                     const PropertyMap &map);
    void setContents(const MGID &mgid, const ViewsOfMol &molviews,
                     const PropertyMap &map);
    void setContents(const MGID &mgid, const Molecules &molecules,
                     const PropertyMap &map);
    void setContents(const MGID &mgid, const MolGroup &molgroup,
                     const PropertyMap &map);

    void setContents(const MoleculeView &molview,
                     const PropertyMap &map = PropertyMap());
    void setContents(const ViewsOfMol &molviews,
                     const PropertyMap &map = PropertyMap());
    void setContents(const Molecules &molecules,
                     const PropertyMap &map = PropertyMap());
    void setContents(const MolGroup &molgroup,
                     const PropertyMap &map = PropertyMap());

protected:
    G1FF();
    G1FF(const G1FF &other);
    
    G1FF& operator=(const G1FF &other);

    ////
    //// Overloading MolGroups virtual functions
    ////
    
    MolGroup& getGroup(MGNum mgnum); 
    const MolGroup& getGroup(MGNum mgnum) const;
    
    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<MolGroup*,10> &groups);

    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<const MolGroup*,10> &groups) const;

    QHash<MGNum,MolGroup*> getGroups();
    QHash<MGNum,const MolGroup*> getGroups() const;

    ////
    //// Virtual functions that must be overridden by
    //// derived virtual forcefields
    ////
    
    virtual void _pvt_updated(const MoleculeData &moldata)=0;
    virtual void _pvt_updated(const Molecules &molecules)=0;
    
    virtual void _pvt_added(const PartialMolecule &molview, 
                            const PropertyMap &map)=0;
                            
    virtual void _pvt_added(const Molecules &molecules, 
                            const PropertyMap &map)=0;
                            
    virtual void _pvt_removed(const PartialMolecule &molview,
                              const PropertyMap &map)=0;

    virtual void _pvt_removed(const Molecules &molecules, 
                              const PropertyMap &map)=0;

    virtual void _pvt_removedAll()=0;

private:
    /** The single group of molecules in this forcefield */
    detail::FFMolGroupPvt molgroup;
};

}

SIRE_END_HEADER

#endif
