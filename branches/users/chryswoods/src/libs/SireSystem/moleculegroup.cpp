
#include "moleculegroup.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireStream;
using namespace SireBase;

///////////
/////////// Implementation of MoleculeGroupBase
///////////

static const RegisterMetaType<MoleculeGroupBase> r_mgbase(MAGIC_ONLY,
                                                    "SireSystem::MoleculeGroupBase");
                                                    
/** Serialise to a binary data stream */
QDataStream SIRESTREAM_EXPORT &operator<<(QDataStream &ds, 
                                          const MoleculeGroupBase&)
{
    writeHeader(ds, r_mgbase, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRESTREAM_EXPORT &operator>>(QDataStream &ds, MoleculeGroupBase&)
{
    VersionID v = readHeader(ds, r_mgbase);
    
    if (v != 0)
        throw version_error(v, "0", r_mgbase, CODELOC);
    
    return ds;
}

/** Constructor */
MoleculeGroupBase::MoleculeGroupBase() : QSharedData()
{}

/** Copy constructor */
MoleculeGroupBase::MoleculeGroupBase(const MoleculeGroupBase&)
                  : QSharedData()
{}

/** Destructor */
MoleculeGroupBase::~MoleculeGroupBase()
{}

/** Assignment operator */
MoleculeGroupBase& MoleculeGroupBase::operator=(const MoleculeGroupBase&)
{
    return *this;
}

///////////
/////////// Implementation of MoleculeGroup
///////////

static const RegisterMetaType<MoleculeGroup> r_group;

/** Serialise to a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const MoleculeGroup &group)
{
    writeHeader(ds, r_group, 1);
    
    SharedDataStream(ds)
          << static_cast<const SharedPolyPointer<MoleculeGroupBase>&>(group);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, MoleculeGroup &group)
{
    VersionID v = readHeader(ds, r_group);
    
    if (v == 1)
    {
        SharedDataStream(ds) >> 
              static_cast<SharedPolyPointer<MoleculeGroupBase>&>(group);
    }
    else
        throw version_error(v, "1", r_group, CODELOC);
    
    return ds;
}

static SharedPolyPointer<MoleculeGroupBase> shared_null( new MoleculeGroupBase() );

/** Null constructor */
MoleculeGroup::MoleculeGroup()
              : SharedPolyPointer<MoleculeGroupBase>(shared_null)
{}

/** Construct a copy of the passed group */
MoleculeGroup::MoleculeGroup(const MoleculeGroupBase &group)
              : SharedPolyPointer<MoleculeGroupBase>(group)
{}
    
/** Construct as a copy of 'ptr' (this will be set to shared_null if 'ptr' is null) */
MoleculeGroup::MoleculeGroup(
                  const SireBase::SharedPolyPointer<MoleculeGroupBase> &ptr)
              : SharedPolyPointer<MoleculeGroupBase>(ptr)
{
    if (constData() == 0)
        SharedPolyPointer<MoleculeGroupBase>::operator=(shared_null);
}
    
/** Copy constructor */
MoleculeGroup::MoleculeGroup(const MoleculeGroup &other)
              : SharedPolyPointer<MoleculeGroupBase>(other)
{}
    
/** Destructor */
MoleculeGroup::~MoleculeGroup()
{}
   
/** Copy assignment */
MoleculeGroup& MoleculeGroup::operator=(const MoleculeGroup &other)
{
    SharedPolyPointer<MoleculeGroupBase>::operator=(other);
    return *this;
}

/** Copy assignment from a SharedPolyPointer - this will be set to  
    shared_null if 'ptr' is null */
MoleculeGroup& MoleculeGroup::operator=(
                    const SireBase::SharedPolyPointer<MoleculeGroupBase> &ptr)
{
    if (ptr.constData())
        SharedPolyPointer<MoleculeGroupBase>::operator=(ptr);
    else
        SharedPolyPointer<MoleculeGroupBase>::operator=(shared_null);
        
    return *this;
}
