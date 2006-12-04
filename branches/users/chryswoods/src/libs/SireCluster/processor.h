#ifndef SIRECLUSTER_PROCESSOR_H
#define SIRECLUSTER_PROCESSOR_H

#include "sireglobal.h"

#include <QObject>

SIRE_BEGIN_HEADER

namespace SireCluster
{

/** This is the base class of all processors. A processor is an object
    that allows computation to be conducted in parallel to the main
    thread of the program (e.g. a new thread, a remote processor or
    an external program).
    
    The processor class is subclassed into two main class hierarchies;
    
    (1) FFProcessor - processors that are used to evaluate forcefields.
                      This class provides a psuedo interface to a 
                      forcefield.
                      
    (2) SimProcessor - processors on which an entire simulation may
                       be placed. This class provides a psuedo interface
                       to a Simulation.
                       
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Processor
{
public:
    Processor(const QString &name = QObject::tr("Unnamed"));
    
    virtual ~Processor();
    
    /** Return the name of the type of processor */
    virtual const char* what() const=0;
    
    const QString& name() const;
    
    void setName(const QString &newname);
    
private:
    /** Prevent copying */
    Processor(const Processor&);
    Processor& operator=(const Processor&);

    /** The processor's name */
    QString nme;

};

}

SIRE_END_HEADER

#endif
