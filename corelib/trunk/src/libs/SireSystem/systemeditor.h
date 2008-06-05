#ifndef SIRESYSTEM_SYSTEMEDITOR_H
#define SIRESYSTEM_SYSTEMEDITOR_H

#include "systemview.h"

namespace SireSystem
{

class MoleculeSetEditor;
class FFGroupEditor;

/**
This class provides a front-end to the System class, providing functions that are used to edit the System (e.g. during the setup of the System, or to change the System in between runs of the simulation).
 
This is the only view of the System that is available to the user via the python interface.
 
@author Christopher Woods
*/
class SystemEditor : public SystemView
{
public:
    SystemEditor();
    ~SystemEditor();

//     FFGroupEditor forcefield();
//     FFGroupEditor forcefield(const QString &group);
    
//     MoleculeSetEditor molecules();
//     MoleculeSetEditor molecules(const QString &group);

};

}

#endif
