#ifndef SIRESYSTEM_MOLECULEGROUP_H
#define SIRESYSTEM_MOLECULEGROUP_H

#include "SireBase/sharedpolypointer.hpp"
#include "SireMol/molecule.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class MoleculeGroupBase;
class MoleculeGroup;
}

QDataStream& operator<<(QDataStream&, const SireSystem::MoleculeGroupBase&);
QDataStream& operator>>(QDataStream&, SireSystem::MoleculeGroupBase&);

QDataStream& operator<<(QDataStream&, const SireSystem::MoleculeGroupBase&);
QDataStream& operator>>(QDataStream&, SireSystem::MoleculeGroup&);

namespace SireSystem
{

/** This is the base class of all MoleculeGroups. A MoleculeGroup is an object
    that holds a group of Molecules. MoleculeGroups are used by SireMove
    to provide the foundations of groups of Molecules that can be moved 
    in different ways.
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MoleculeGroupBase : public QSharedData
{
public:
    MoleculeGroupBase();
    
    MoleculeGroupBase(const MoleculeGroupBase &other);
    
    virtual ~MoleculeGroupBase();
    
    MoleculeGroupBase& operator=(const MoleculeGroupBase &other);
    
    /** Clone this object */
    virtual MoleculeGroupBase* clone() const
    {
        return new MoleculeGroupBase(*this);
    }
    
    static const char* typeName()
    {
        return "SireSystem::MoleculeGroupBase";
    }
    
    /** Return the name of the type of this object */
    virtual const char* what() const
    {
        return MoleculeGroupBase::typeName();
    }
    
    /** Is this of type 'T'? */
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    /** Return this object cast as type 'T'. This
        will have undefined results is isA<T>() returns
        false. */
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }
};

/** This class provides a convienient wrapping of a shared polymorphic
    pointer to a MoleculeGroupBase
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MoleculeGroup 
          : public SireBase::SharedPolyPointer<MoleculeGroupBase>
{
public:
    MoleculeGroup();
    MoleculeGroup(const MoleculeGroupBase &group);
    
    MoleculeGroup(const SireBase::SharedPolyPointer<MoleculeGroupBase> &ptr);
    
    MoleculeGroup(const MoleculeGroup &other);
    
    ~MoleculeGroup();
    
    MoleculeGroup& operator=(const MoleculeGroup &other);
    MoleculeGroup& operator=(
                    const SireBase::SharedPolyPointer<MoleculeGroupBase> &ptr);

    const char* what() const;
};

/** Return the type of this MoleculeGroup */
inline const char* MoleculeGroup::what() const
{
    return constData()->what();
}

}

Q_DECLARE_METATYPE(SireSystem::MoleculeGroup);

SIRE_END_HEADER

#endif
