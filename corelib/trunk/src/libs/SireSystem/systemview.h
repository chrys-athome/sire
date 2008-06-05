#ifndef SIRESYSTEM_SYSTEMVIEW_H
#define SIRESYSTEM_SYSTEMVIEW_H

#include "system.h"
#include "systemptr.h"

namespace SireSystem
{

/**
This class provides a writable view to a System. This is used to ensure that only a single thread across the entire cluster is able to write to a particular System at a time. This class provides the base class of SystemEditor and SystemRunner, which provide the functionality necessary to edit a System, and run simulations on a System.
 
@author Christopher Woods

*/
class SystemView
{
public:
    SystemView();
    SystemView(const SystemPtr &system);
    
    ~SystemView();

    bool isNull() const;
    bool isValid() const;

    bool hasForcefield(const QString &ffield) const;
    bool hasMoleculeGroup(const QString &groupname) const;

protected:

    System& system();
    const System& system() const;

    FFGroupPtr& forcefield();
    FFGroupPtr& forcefield(const QString &groupname);
    
    MoleculeSet& molecules();
    MoleculeSet& molecules(const QString &groupname);

private:

    /** PtrWriter to the System being changed */
    SystemPtrWriter sysptr;

};

/** Protected member function providing read-write access to the system.
    This will have undefined results if this SystemView is null. */
inline System& SystemView::system()
{
    return *sysptr;
}

/** Protected member function providing read-only access to the system.
    This will have undefined results if this SystemView is null. */
inline const System& SystemView::system() const
{
    return *(sysptr.get());
}

/** Return whether this view is null */
inline bool SystemView::isNull() const
{
    return sysptr.isNull();
}

/** Return whether this view is valid */
inline bool SystemView::isValid() const
{
    return sysptr.isValid();
}

}

#endif
