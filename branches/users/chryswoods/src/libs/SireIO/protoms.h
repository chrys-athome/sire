/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
