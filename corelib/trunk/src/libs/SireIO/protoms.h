#ifndef SIREIO_PROTOMS_H
#define SIREIO_PROTOMS_H

#include <QString>
#include <QStringList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
class ParameterDB;
class MatchMol;
class MatchRes;
}

namespace SireIO
{

using SireDB::ParameterDB;

class ProtoMSWS;

/**
This class is used to read in ProtoMS parameter files.
 
@author Christopher Woods
*/
class SIREIO_EXPORT ProtoMS
{
public:
    ProtoMS();
    ~ProtoMS();

    void read(const QString &filename, ParameterDB &ffdb);

protected:

    void readCLJLine(ProtoMSWS &workspace);
    void readBondLine(ProtoMSWS &workspace);
    void readAngleLine(ProtoMSWS &workspace);
    void readUreyBradleyLine(ProtoMSWS &workspace);
    void readDihedralLine(ProtoMSWS &workspace);
    
    void readTemplateLine(ProtoMSWS &workspace);
    void readChainTemplate(ProtoMSWS &workspace);
    void readResidueTemplate(ProtoMSWS &workspace);
    void readSoluteTemplate(ProtoMSWS &workspace);
    void readSolventTemplate(ProtoMSWS &workspace);

    void processBondLine( ProtoMSWS &workspace, const SireDB::MatchMol &matchmol );
    void processAngleLine( ProtoMSWS &workspace, const SireDB::MatchMol &matchmol );
    void processDihedralLine( ProtoMSWS &workspace, const SireDB::MatchMol &matchmol );
    void processUreyBradleyLine( ProtoMSWS &workspace, const SireDB::MatchMol &matchmol );
    
    void processBondLine( ProtoMSWS &workspace, const SireDB::MatchRes &matchres );
    void processAngleLine( ProtoMSWS &workspace, const SireDB::MatchRes &matchres );
    void processDihedralLine( ProtoMSWS &workspace, const SireDB::MatchRes &matchres );
    void processUreyBradleyLine( ProtoMSWS &workspace, const SireDB::MatchRes &matchres );
};

}

SIRE_END_HEADER

#endif
