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

#include "cljworkspace.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QVarLengthArray>

using namespace SireMM;
using namespace SireStream;
using namespace SireMol;

namespace SireMM
{
namespace detail
{
    /** This class holds all of the data of a CLJWorkspace. This is held
        as an implicitly shared pointer, with the caveat that copying the 
        pointer from an empty workspace will reset the original to point
        to an empty copy, while taking this copy to the detached (new) 
        object
        
        @author Christopher Woods
    */
    class SIREMM_EXPORT CLJWorkspaceData : public QSharedData
    {
    public:
        CLJWorkspaceData()
        {}
        
        CLJWorkspaceData(const CLJWorkspaceData &other) : deltas(other.deltas)
        {}
        
        ~CLJWorkspaceData()
        {}
        
        void clear()
        {
            deltas.resize(0);
        }
        
        bool isEmpty() const
        {
            return deltas.isEmpty();
        }
        
        bool isSingleID() const
        {
            if (isEmpty())
                return true;
            
            qint32 id = CLJAtoms::idOfDummy()[0];
            const qint32 dummy_id = id;
            
            for (int i=0; i<deltas.count(); ++i)
            {
                const QVector<MultiInt> &ids = deltas.constData()[i].changedAtoms().ID();
                
                for (int j=0; j<ids.count(); ++j)
                {
                    for (int k=0; k<MultiInt::count(); ++k)
                    {
                        if (ids[j][k] != dummy_id)
                        {
                            if (id == dummy_id)
                                id = ids[j][k];

                            else if (id != ids[j][k])
                                //we found an atom with a different ID
                                return false;
                        }
                    }
                }
            }
            
            return false;
        }
        
        void push(const CLJDelta &delta)
        {
            deltas.append(delta);
        }
        
        void push(quint32 idnum, const CLJBoxes &boxes, const QVector<CLJBoxIndex> &old_atoms,
                  const MoleculeView &new_atoms, CLJAtoms::ID_SOURCE source,
                  const PropertyMap &map)
        {
            deltas.resize( deltas.count() + 1 );
            deltas.last().reconstruct(idnum, boxes, old_atoms, new_atoms, source, map);
        }
        
        CLJDelta merge() const
        {
            if (deltas.isEmpty())
                return CLJDelta();
            else if (deltas.count() == 1)
                return deltas.constData()[0];
            else
            {
                return CLJDelta::merge(deltas.constData(), deltas.count(), true);
            }
        }
        
        QVarLengthArray<CLJDelta,8> deltas;
    };
} // end of namespace detail
}

static const RegisterMetaType<CLJWorkspace> r_workspace(NO_ROOT);

QDataStream  &operator<<(QDataStream &ds, const SireMM::detail::CLJWorkspaceData &ws)
{
    ds << quint32( ws.deltas.count() );
    
    for (int i=0; i<ws.deltas.count(); ++i)
    {
        ds << ws.deltas.at(i);
    }
    
    return ds;
}

QDataStream  &operator>>(QDataStream &ds, SireMM::detail::CLJWorkspaceData &ws)
{
    quint32 n;
    ds >> n;
    
    ws.deltas.resize(n);
    
    for (int i=0; i<n; ++i)
    {
        ds >> ws.deltas[i];
    }
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJWorkspace &ws)
{
    writeHeader(ds, r_workspace, 1);
    
    ds << ws.isEmpty();
    
    if (not ws.isEmpty())
    {
        SharedDataStream sds(ds);
        sds << ws.d;
    }
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJWorkspace &ws)
{
    VersionID v = readHeader(ds, r_workspace);
    
    if (v == 1)
    {
        bool is_empty;
        
        ds >> is_empty;
        
        if (is_empty)
        {
            ws = CLJWorkspace();
        }
        else
        {
            SharedDataStream sds(ds);
            sds >> ws.d;
        }
    }
    else
        throw version_error(v, "1", r_workspace, CODELOC);
    
    return ds;
}

/** Constructor */
CLJWorkspace::CLJWorkspace()
{}

/** Copy constructor */
CLJWorkspace::CLJWorkspace(const CLJWorkspace &other)
{
    if (other.isEmpty())
    {
        if (other.d.constData() != 0)
        {
            //we have copied an empty workspace - we will keep the workspace
            //while we reset the pointer of 'other' so that it has a null pointer.
            //This will prevent unnecessary duplication of memory when detaching an
            //empty workspace
            d.swap( const_cast<CLJWorkspace*>(&other)->d );
        }
    }
    else
    {
        d = other.d;
    }
}

/** Destructor */
CLJWorkspace::~CLJWorkspace()
{
    if (d.constData() != 0)
    {
        qDebug() << "Deleting CLJWorkspace" << quintptr(d.constData());
    }
}

/** Copy assignment operator. The new copy will get the memory of an empty workspace */
CLJWorkspace& CLJWorkspace::operator=(const CLJWorkspace &other)
{
    if (this != &other)
    {
        if (other.isEmpty())
        {
            if (d.constData() != 0)
            {
                d->clear();
            }
            else if (other.d.constData() != 0)
            {
                d.swap( const_cast<CLJWorkspace*>(&other)->d );
            }
        }
        else
        {
            d = other.d;
        }
    }
    
    return *this;
}

/** Comparison operator */
bool CLJWorkspace::operator==(const CLJWorkspace &other) const
{
    return (isEmpty() and other.isEmpty()) or (d.constData() == other.d.constData());
}

/** Comparison operator */
bool CLJWorkspace::operator!=(const CLJWorkspace &other) const
{
    return not operator==(other);
}

/** Return the ith delta */
const CLJDelta& CLJWorkspace::operator[](int i) const
{
    if (isEmpty())
        throw SireError::invalid_index( QObject::tr(
                "Cannot return the delta at index %1 at this is an empty workspace.")
                        .arg(i), CODELOC );
    
    if (i < 0 or i >= d->deltas.count())
        throw SireError::invalid_index( QObject::tr(
                "Cannot return the delta at index %1 as there number of deltas is only %2.")
                    .arg(i).arg(d->deltas.count()), CODELOC );
    
    return d->deltas.at(i);
}

/** Return the ith delta */
const CLJDelta& CLJWorkspace::at(int i) const
{
    return operator[](i);
}

/** Return the ith delta */
const CLJDelta& CLJWorkspace::getitem(int i) const
{
    return operator[](i);
}

/** Return a raw pointer to the array of deltas */
const CLJDelta* CLJWorkspace::data() const
{
    if (d.constData() == 0)
        return 0;
    else
        return d->deltas.constData();
}

/** Return a raw pointer to the array of deltas */
const CLJDelta* CLJWorkspace::constData() const
{
    return data();
}

/** Return the number of deltas */
int CLJWorkspace::count() const
{
    if (d.constData() == 0)
        return 0;
    else
        return d->deltas.count();
}

/** Return the number of deltas */
int CLJWorkspace::size() const
{
    return count();
}

const char* CLJWorkspace::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJWorkspace>() );
}

const char* CLJWorkspace::what() const
{
    return CLJWorkspace::typeName();
}

/** Push a new delta onto the set */
void CLJWorkspace::push(const CLJDelta &delta)
{
    if (d.constData() == 0)
    {
        d = new detail::CLJWorkspaceData();
        qDebug() << "Created workspace" << quintptr(d.constData());
    }
    
    const void *oldptr = d.constData();
    
    d->push(delta);
    
    if (oldptr != d.constData())
    {
        qDebug() << "Detached workspace" << quintptr(oldptr) << "created workspace"
                 << quintptr(d.constData());
    }
}

/** Push a new delta onto the set. This is used to bypass the CLJDelta constructor,
    and instead (possibly) optimise memory access by re-using memory in the workspace */
void CLJWorkspace::push(quint32 idnum, const CLJBoxes &boxes, const QVector<CLJBoxIndex> &old_atoms,
                        const MoleculeView &new_atoms, const PropertyMap &map)
{
    if (d.constData() == 0)
    {
        d = new detail::CLJWorkspaceData();
        qDebug() << "Created workspace" << quintptr(d.constData());
    }
    
    const void *oldptr = d.constData();
    
    d->push(idnum, boxes, old_atoms, new_atoms, CLJAtoms::USE_MOLNUM, map);
    
    if (oldptr != d.constData())
    {
        qDebug() << "Detached workspace" << quintptr(oldptr) << "created workspace"
                 << quintptr(d.constData());
    }
}

/** Push a new delta onto the set. This is used to bypass the CLJDelta constructor,
    and instead (possibly) optimise memory access by re-using memory in the workspace */
void CLJWorkspace::push(quint32 idnum, const CLJBoxes &boxes, const QVector<CLJBoxIndex> &old_atoms,
                        const MoleculeView &new_atoms, CLJAtoms::ID_SOURCE source,
                        const PropertyMap &map)
{
    if (d.constData() == 0)
    {
        d = new detail::CLJWorkspaceData();
        qDebug() << "Created workspace" << quintptr(d.constData());
    }
    
    const void *oldptr = d.constData();
    
    d->push(idnum, boxes, old_atoms, new_atoms, source, map);
    
    if (oldptr != d.constData())
    {
        qDebug() << "Detached workspace" << quintptr(oldptr) << "created workspace"
                 << quintptr(d.constData());
    }
}

/** Return the number of deltas in this workspace */
int CLJWorkspace::nDeltas() const
{
    return count();
}

/** Return whether or not this workspace contains deltas with a single
    ID (a single CLJAtoms ID) */
bool CLJWorkspace::isSingleID() const
{
    if (d.constData() == 0)
        return true;
    else
        return d->isSingleID();
}

/** Return whether or not this workspace is empty */
bool CLJWorkspace::isEmpty() const
{
    if (d.constData() == 0)
        return true;
    else
        return d->isEmpty();
}

/** Merge all of the deltas together into a single delta that
    can be used for the change in energy calculation */
CLJDelta CLJWorkspace::merge() const
{
    if (isEmpty())
        return CLJDelta();
    else
        return d->merge();
}

/** Clear this workspace */
void CLJWorkspace::clear()
{
    if (d.constData() != 0)
    {
        const void *oldptr = d.constData();
        
        d->clear();
        
        if (oldptr != d.constData())
        {
            qDebug() << "Clearing has detached workspace" << quintptr(oldptr)
                     << "creating workspace" << quintptr(d.constData());
        }
    }
}
