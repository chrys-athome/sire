
#include "systemeditor.h"
//#include "moleculeseteditor.h"
#include "ffgroupeditor.h"

using namespace SireSystem;

/** Create a new SystemEditor to edit a newly created System */
SystemEditor::SystemEditor()
{}

/** Destructor - this will delete the System unless it is referenced elsewhere */
SystemEditor::~SystemEditor()
{
    //verify that the System is in a sane state...
    //system().sanitise();
}

/** Return the editor used to edit the default forcefield for this System */
// FFGroupEditor SystemEditor::forcefield()
// {
// }

/** Return the editor used to edit the extra forcefield called 'ffname'. This 
    will create a new forcefield with this name if it doesn't already exist */
// FFGroupEditor SystemEditor::forcefield(const QString &ffname)
// {
// }

/** Return the editor used to edit the complete set of molecules in this 
    system. Note that removing a molecule from this set will also remove it
    from all forcefield components and all molecule groups. */
// MoleculeSetEditor SystemEditor::molecules()
// {
// }

/** Return the editor used to edit the set of molecules in the identified group 
    'groupname'. This is used to group molecules together into identifiable
    groups, e.g. for the purposes of choosing subgroups of molecules for MC moves.
    Note that a molecule can exist in multiple groups. If a molecule is added to 
    a group that is not in the main molecule list (returned by molecules()) then 
    the molecule is added to that list. Removing a molecule from a group will
    not remove it from the main molecule list, nor will it remove it from 
    any forcefield components. If there is no molecule group with this name then 
    a new one is created and returned. */
// MoleculeSetEditor SystemEditor::molecules(const QString &groupname)
// {
// }
