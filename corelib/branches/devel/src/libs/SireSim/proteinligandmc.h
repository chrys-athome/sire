/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#ifndef SIRESIM_PROTEINLIGANDMC_H
#define SIRESIM_PROTEINLIGANDMC_H

#include "simparams.h"

#include "SireMove/ensemble.h"
#include "SireMove/moves.h"

#include "SireCAS/symbol.h"

#include "SireSystem/system.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/temperature.h"

namespace SireSim
{
class ProteinLigandMC;
class ProteinLigandMCParams;
}

QDataStream& operator<<(QDataStream&, const SireSim::ProteinLigandMC&);
QDataStream& operator>>(QDataStream&, SireSim::ProteinLigandMC&);

QDataStream& operator<<(QDataStream&, const SireSim::ProteinLigandMCParams&);
QDataStream& operator>>(QDataStream&, SireSim::ProteinLigandMCParams&);

namespace SireSim
{

using SireBase::PropertyPtr;

using SireSystem::System;

using SireMove::Moves;
using SireMove::MovesPtr;
using SireMove::Ensemble;

using SireCAS::Symbol;

using SireUnits::Dimension::Length;
using SireUnits::Dimension::Area;
using SireUnits::Dimension::Angle;
using SireUnits::Dimension::Temperature;
using SireUnits::Dimension::Pressure;

/** This is a simple simulation that runs Monte Carlo on a protein
    ligand system that is loaded from the files produced via
    auto-setup
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT ProteinLigandMC
        : public SireBase::ConcreteProperty<ProteinLigandMC,Sim>
{

friend QDataStream& ::operator<<(QDataStream&, const ProteinLigandMC&);
friend QDataStream& ::operator>>(QDataStream&, ProteinLigandMC&);

public:
    ProteinLigandMC();
    ProteinLigandMC(const ProteinLigandMCParams &params);
    ProteinLigandMC(const QString &s3file);
    
    ProteinLigandMC(const ProteinLigandMC &other);
    
    ~ProteinLigandMC();
    
    static const char* typeName();
    
    ProteinLigandMC& operator=(const ProteinLigandMC &other);
    
    bool operator==(const ProteinLigandMC &other) const;
    bool operator!=(const ProteinLigandMC &other) const;
    
    void run();
    
    System system() const;
    MovesPtr moves() const;
    
    void setSystem(const System &system);
    void setMoves(const Moves &moves);
};

/** This class provides all of the user-supplied parameters that
    are needed to run a Monte Carlo simulation on a protein-ligand
    system using the ProteinLigandMC class 
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT ProteinLigandMCParams
        : public SireBase::ConcreteProperty<ProteinLigandMCParams,SimParams>
{

friend QDataStream& ::operator<<(QDataStream&, const ProteinLigandMCParams&);
friend QDataStream& ::operator>>(QDataStream&, ProteinLigandMCParams&);

public:
    ProteinLigandMCParams();
    ProteinLigandMCParams(const QString &filename);
    
    ProteinLigandMCParams(const ProteinLigandMCParams &other);
    
    ~ProteinLigandMCParams();
    
    static const char* typeName();
    
    ProteinLigandMCParams& operator=(const ProteinLigandMCParams &other);
    
    bool operator==(const ProteinLigandMCParams &other) const;
    bool operator!=(const ProteinLigandMCParams &other) const;
    
    void setSimulationTitle(const QString &title);
    QString getSimulationTitle() const;
    
    void setInputDirectory(const QString &indir);
    void setOutputDirectory(const QString &outdir);
    void setOutputBaseName(const QString &base_name);
    
    QString getInputDirectory() const;
    QString getOutputDirectory() const;
    QString getOutputBaseName() const;
    
    void setCRDFile(const QString &crdfile);
    void setTOPFile(const QString &topfile);
    
    void setCRDFile(int i, const QString &crdfile);
    void setTOPFile(int i, const QString &topfile);
    
    void setCRDFiles(QStringList crdfiles);
    void setTOPFiles(QStringList topfiles);
    
    QString getCRDFile() const;
    QString getTOPFile() const;
    
    QString getCRDFile(int i) const;
    QString getTOPFile(int i) const;
    
    QStringList getCRDFiles() const;
    QStringList getTOPFiles() const;
    
    void setProteinMolecule(int protein, int molecule);
    void setProteinMolecule(int protein, int file, int molecule);

    void setSoluteMolecule(int solute, int molecule);
    void setSoluteMolecule(int solute, int file, int molecule);

    void setIgnoreSolventMolecule(int molecule);
    void setIgnoreSolventMolecule(int file, int molecule);
    
    void setIgnoreSolventMolecules(QList<int> molecules);
    void setIgnoreSolventMolecules(int file, QList<int> molecules);

    QHash< int, QPair<int,int> > getProteinMolecules() const;
    QHash< int, QPair<int,int> > getSoluteMolecules() const;

    QList<int> getIgnoredSolventMolecules() const;
    QList<int> getIgnoredSolventMolecules(int file) const;
        
    void setSoluteName(int solute, const QString &name);
    void setSoluteFlexFile(int solute, const QString &filename) const;
    
    QString getSoluteName(int solute) const;
    QString getSoluteFlexFile(int solute) const;
    
    void setProteinZMatrixFile(int protein, const QString &filename);
    void setProteinFlexFile(int protein, const QString &filename);
    
    QString getProteinZMatrixFile(int protein) const;
    QString getProteinFlexFile(int protein) const;
    
    void setPreferentialSamplingSolute(int solute);
    void setPreferentialSamplingConstant(Area k);
    
    int getPreferentialSamplingSolute() const;
    Area getPreferentialSamplingConstant() const;
    
    void setNonBondedCutoff(Length cutoff);
    void setNonBondedCoulombCutoff(Length cutoff);
    void setNonBondedLJCutoff(Length cutoff);
    
    Length getNonBondedCutoff() const;
    Length getNonBondedCoulombCutoff() const;
    Length getNonBondedLJCutoff() const;
    
    void setNonBondedFeather(Length feather);
    void setNonBondedCoulombFeather(Length feather);
    void setNonBondedLJFeather(Length feather);
    
    Length getNonBondedFeather() const;
    Length getNonBondedCoulombFeather() const;
    Length getNonBondedLJFeather() const;
    
    void setUseArithmeticCombiningRules();
    void setUseGeometricCombiningRules();
    
    bool getUseArithmeticCombiningRules() const;
    bool getUseGeometricCombiningRules() const;
        
    void setTemperature(Temperature temperature);
    void setPressure(Pressure pressure);
    
    void setEnsemble(const Ensemble &ensemble);
    
    Temperature getTemperature() const;
    Pressure getPressure() const;
    
    Ensemble getEnsemble() const;
    
    void setProteinMoveRatio(int n);
    void setProteinSideChainMoveRatio(int n);
    void setProteinBackBoneMoveRatio(int n);
    
    void setSoluteMoveRatio(int n);
    void setSoluteIntraMoveRatio(int n);
    void setSoluteRBMoveRatio(int n);
    
    void setSolventMoveRatio(int n);
    
    void setVolumeMoveRatio(int n);
    
    int getProteinMoveRatio() const;
    int getProteinSideChainMoveRatio() const;
    int getProteinBackBoneMoveRatio() const;
    
    int getSoluteMoveRatio() const;
    int getSoluteIntraMoveRatio() const;
    int getSoluteRBMoveRatio() const;
    
    int getSolventMoveRatio() const;
    
    int getVolumeMoveRatio() const;
    
    void setSynchroniseSoluteTranslation(int solute0, int solute1, bool yes);
    void setSynchroniseSoluteRotation(int solute0, int solute1, bool yes);
    
    bool setSynchroniseSoluteTranslation(int solute0, int solute1) const;
    bool setSynchroniseSoluteRotation(int solute0, int solute1) const;
    
    void setNMoves(int n);
    
    int getNMoves() const;

    void setMonitorSoluteResidueEnergyCutoff(int solute, Length distance);
    void setMonitorSoluteSolventEnergyAtoms(int solute, QStringList atomnames);
                                            
    void setMonitorSoluteResidueFrequency(int solute, int n);
    void setMonitorSoluteSolventFrequency(int solute, int n);
    
    Length getMonitorSoluteResidueEnergyCutoff(int solute) const;
    QStringList getMonitorSoluteSolventEnergyAtoms(int solute) const;

    int getMonitorSoluteResidueFrequency(int solute) const;
    int getMonitorSoluteSolventFrequency(int solute) const;

    void setEnergyComponentMonitorFrequency(int n);
    void setTrajectoryMonitorFrequency(int n);
    
    int getEnergyComponentMonitorFrequency() const;
    int getTrajectoryMonitorFrequency() const;
    
    void setRandomNumberSeed(int seed);
    
    int getRandomNumberSeed() const;
    
    void setUsingSoftCore(int solute, bool yes);

    void setSoftCoreCoulombPower(double power);
    void setSoftCoreShiftDelta(double shift);

    void setSoftCoreCoulombPower(int solute, double power);
    void setSoftCoreShiftDelta(int solute, double shift);
    
    bool getUsingSoftCore(int solute) const;
    
    double getSoftCoreCoulombPower() const;
    double getSoftCoreShiftDelta() const;

    double getSoftCoreCoulombPower(int solute) const;
    double getSoftCoreShiftDelta(int solute) const;
    
    void setSoluteLambda(int solute, double lam);
    void setSoluteAlpha(int solute, double alpha);
    
    double getSoluteLambda(int solute) const;
    double getSoluteAlpha(int solute) const;
    
    void setUseForwardWindow(int solute, bool yes);
    void setUseBackwardWindow(int solute, bool yes);
    
    void setDeltaLambda(int solute, double delta_lam);
    void setDeltaAlpha(int solute, double delta_alpha);

    bool getUseForwardWindow(int solute) const;
    bool getUseBackwardWindow(int solute) const;
    
    double getDeltaLambda(int solute) const;
    double getDeltaAlpha(int solute) const;
};

}

Q_DECLARE_METATYPE( SireSim::ProteinLigandMC )
Q_DECLARE_METATYPE( SireSim::ProteinLigandMCParams )

SIRE_EXPOSE_CLASS( SireSim::ProteinLigandMC )
SIRE_EXPOSE_CLASS( SireSim::ProteinLigandMCParams )

#endif
