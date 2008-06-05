#ifndef SIRESYSTEM_SYSTEM_H
#define SIRESYSTEM_SYSTEM_H

#include <QString>

#include "systemptr.h"

#include "SireFF/ffgroupptr.h"
#include "SireMol/moleculeptr.h"
#include "SireMol/moleculeset.h"

namespace SireSystem
{

using SireFF::FFGroupPtr;
using SireMol::MoleculePtr;
using SireMol::MoleculeSet;

class SystemView;

/**
This class contains all of the data necessary to represent a single simulation system 
(i.e. it contains all of the molecules in the system, the forcefield(s) used to calculate
the energy and forces, the volume(s) within which the molecules exist.

The System class has no non-const member functions. It is not intended to be used on its
own, but rather it just provides access to the data contained in the system. Two further
helper-classes are used to modify a system;

(1) SystemEditor: This class is used to edit a System, e.g. to add or remove forcefields,
                  molecules etc.
                 
(2) SystemRunner: This class is used to run a simulation of the System. It provides no
                  editing functionality, instead only providing access to functions necessary
                  to move molecules and calculate energies and forces. It also provides functions
                  necessary to dynamically add or delete molecules during a running simulation,
                  e.g. for grand canonical simulations

These helper classes will themselves return helper classes to access the forcefields or
molecules (e.g. FFGroupEditor, FFEditor, from SystemEditor, and FFGroupRunner from SystemRunner).

The SystemEditor and SystemRunner classes hold the System via a PtrWriter<System>. This ensures
that only a single SystemEditor or SystemRunner may be active at a time. In addition, a 
recursive mutex in the PtrWriter class ensures that only a single thread may access the data of
the System class at a time. This ensures that the System can only be edited or simulated by 
a single thread across the entire cluster at a time. This prevents the user from accidentally
editing the System while it is being simulated.

The System is held via a NetObjects::Ptr<System>, so may be sent/copied to remote processors.
There is no invoker to a System as it is not intended to be remote-controlled.

As the SystemEditor and SystemRunner hold a PtrWriter<System>, the changes that they cause
to the System are only distributed to other copies on the cluster when they have finished.
This speeds up editing and simulation, as all of the network IO is saved until the editing 
or simulation have finished, and thus no intermediates are sent across the network.

The current state of the System can be saved via the SystemRunner class. This provides a
checkpoint mechanism (via SireSystem::CheckPoint) which can save checkpoints of the System
during a running simulation. Note that the checkpoints are only valid during a running
simulation, and are cleared once the simulation is finished. There is no checkpointing system
available to the SystemEditor, as checkpointing the editing of a System would be a very 
costly and complicated task.

One further way of saving the state of the system is to use the "dumpRestart()" function 
to dump a complete binary representation of the entire system. This representation includes
copies of all of the molecules and forcefields and other data in the System, and provides 
a means of writing restart files. The restart information can then be loaded back into 
a new System (via "loadRestart()"), which will create a whole new System that is a perfect
copy of the original. Note that the copy contains *copies* of the molecules and forcefields
of the original, so the copy is completely independent of the original. This is different
to the NetObjects::Ptr.dump() function, which dumps a representation of the current System, 
suitable for sending the System over the network. The NetObjects::dump representation does
not contain copies of the molecules and forcefields, instead it only contains NetObjects 
references to the Ptr<Molecule> and Ptr<FField> that themselves point to the molecules
and forcefields held in the System.

@author Christopher Woods
*/
class System
{

friend class SystemView;

public:
    System(QString name = QObject::tr("Unnamed"));
    ~System();
                   
    QString toString() const;
    const QString& name() const;
    QByteArray dumpRestart() const;
    SystemPtr clone() const;
                   
protected:

//     FFGroup& forcefield();
//     const FFGroup& forcefield() const;
//                   
//     FFGroup& forcefield(const QString &name);
//     const FFGroup& forcefield(const QString &name) const;

    void setName(const QString &name);
    void loadRestart(const QByteArray &data);

    QMutex& mutex();

    /** The name of the System. */
    QString nme;

    /** The molecules are arranged into user-defined groups, indexed by 
        name (e.g. "solvent", "ligand", "ions" etc.) */
    QHash<QString, MoleculeSet> mols;
    
    /** The default forcefield group that provides the energy and forces
        within the system */
    FFGroupPtr ffgroup;
    
    /** Addition forcefield groups that may be used as an alternative to 
        the default, e.g. during multiple-timestep MC, where a fast forcefield
        is used to advance the simulation, with an extra test used to correct
        for the difference to the default forcefield. Again, the additional
        forcefields are indexed by name, e.g. "fast", "implicit", "short cutoff"
        etc. */
    QHash<QString, FFGroupPtr> ffgroups;
};

}

#endif
