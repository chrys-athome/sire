#ifndef SIRESYSTEM_SYSTEM_H
#define SIRESYSTEM_SYSTEM_H

#include <QString>

namespace SireSystem
{

class Simulation
{
public:
    Simulation(const QString &name = QObject::tr("Unnamed"));
    
    ~Simulation();
    
    void addProcessor(Processor &processor);    
    void removeProcessor(const QString &processor);
    
    void setSystem(const System &sys);
    
    void moveToProcessor(const QString &ffield, const QString &processor);
    
private:

    /** The forcefields calculated in the same thread as the
        simulation, indexed by name */
    QHash<QString, ForceField> localff;
    
    /** The processors indexed by the name of the
        forcefield which they are processing */
    QHash<QString, Processor> remoteff;

    /** The unused processors, indexed by name */
    QHash<QString, Processor> unused_procs;
    
    /** The names of the used processors, mapped to the
        names of the forcefields that they are processing */
    QHash<QString, QString> used_procs;

    /** Metadata about the system */
    SystemData sysdata;
};

class System
{
public:
    System(const QString &name = QObject::tr("Unnamed"));
    ~System();
    
    void addFF( const ForceField &ffield );
    void removeFF( const QString &ffield );
    
    const FField& ff(const QString &ffield) const;
    
    void addExpression( const QString &name, const Expression &expression );
    void removeExpression( const QString &name );
    Function expressionSymbol(const QString &name);
    
    void addProperty( const QString &name, double value=0.0 );
    void removeProperty( const QString &name );
    void removeProperty( const Symbol &symbol );
    
    void setProperty( const Symbol &property, double value );
    double getProperty( const Symbol &property );
    Symbol propertySymbol(const QString &name);
    
    const SymbolValues& properties() const;
    
    void addToGroup(const QString &group, const Molecule &molecule);
    void addToGroup(const QString &group, const Residue &residue);
                    
    void removeFromGroup(const QString &group, const Molecule &molecule);
    void removeFromGroup(const QString &group, const Residue &residue);
                    
    void addToFF(const QString &ffield, const Molecule &molecule);
    void addToFF(const QString &ffield, const Residue &residue);
                 
    void removeFromFF(const QString &ffield, const Molecule &molecule);
    void removeFromFF(const QString &ffield, const Residue &residue);
                 
    void addGroupToFF(const QString &ffield, const QString &group);
    void removeGroupFromFF(const QString &ffield, const QString &group);
                 
    const Molecule& molecule(Molecule ID molid) const;
    
    MolGroup moleculesInGroup(const QString &group) const;
    MolGroup moleculesInFF(const QString &ffield) const;
    
private:

    /** All of the local forcefields, indexed by name */
    QHash<QString, ForceField> localff;
    
    /** All of the remote forcefields, indexed by name */
    QHash<QString, ForceField> remoteff;

    /** Hash mapping all of the different forcefield functions
        to their corresponding equation infos */
    QHash<SymbolID, FFEquation> ff_equations;

    /** Hash mapping the name of a forcefield function to its
        function symbol */
    QHash<QString, Function> ff_functions;

    /** Copies of all of the molecules in the system, 
        indexed by MoleculeID */
    QHash<MoleculeID, Molecule> mols;
    
    /** The various groups that exist in this system, 
        e.g. 'protein', 'solutes', 'solvent' */
    QHash< QString, GroupIDs > molgroups;  //GroupIDs holds MoleculeID and MolResNumID
    
    /** Hash mapping a name of a property to its symbol */
    QHash<QString, Symbol> property_symbols;
    
    /** The values of various properties of this system
        (e.g. temperature, pressure, lambda etc.) */
    SymbolValues property_values;
    
    /** The name of the system */
    QString nme;
};

}

#endif
