#ifndef SIREMOL_RIGIDMOLECULES_H
#define SIREMOL_RIGIDMOLECULES_H

#include "SireSystem/moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

/** This class provides a thin-wrapper around a molecule 
    that exposes only the ability to translate and rotate
    the molecule (about its center of geometry)
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RigidMolecule
{
public:
    RigidMolecule();
    
    RigidMolecule(const Molecule &mol);

    RigidMolecule(const RigidMolecule &mol);
    
    ~RigidMolecule();
    
    void translate(const Vector &delta);
    
    void rotate(const Quaternion &quat);
    void rotate(const Matrix &rotmat);

private:
    /** The actual molecule to be rotated */
    Molecule mol;

    /** The central point about which this molecule 
        is rotated */
    Vector cent;
};

/** This class represents a group of molecules that can only undergo
    rigid-body translation and rotation moves. This group forms the
    basis of most move groups, as rigid-body translation and rotation
    is normally always possible if the molecule can be moved by other
    methods.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RigidMolecules : public SireSystem::MoleculeGroupBase
{
public:
    RigidMolecules();
    
    RigidMolecules(const RigidMolecules &other);
    
    ~RigidMolecules();
    
    const char* typeName()
    {
        return "SireMove::RigidMolecules";
    }
    
    const char* what() const
    {
        return RigidMolecules::typeName();
    }
    
    RigidMolecules* clone() const
    {
        return new RigidMolecules(*this);
    }
    
    RigidMolecule molecule(MoleculeID molid) const;
    
    RigidMolecule randomMolecule() const;
    
    QVector<RigidMolecule> molecules() const;
};

}

SIRE_END_HEADER

#endif
