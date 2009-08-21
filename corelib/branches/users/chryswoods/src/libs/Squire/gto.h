/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SQUIRE_GTO_H
#define SQUIRE_GTO_H

#include "orbital.h"

#include "SireMaths/nvector.h"

SIRE_BEGIN_HEADER

namespace Squire
{
class GTO;
class CGTO;
}

QDataStream& operator<<(QDataStream&, const Squire::GTO&);
QDataStream& operator>>(QDataStream&, Squire::GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CGTO&);
QDataStream& operator>>(QDataStream&, Squire::CGTO&);

namespace Squire
{

using SireMaths::NVector;

/** This is the base class of all single Gaussian Type Orbital shells (GTOs)
    (S_GTO (l==0), P_GTO (l==1), DPlus_GTO (l>=2))
    
    These orbitals are not used directly - rather they are combined into
    shell-pair orbitals, e.g. SS_GTO, SP_GTO etc. Integral functions then
    use these shell-pair orbital objects. The shell pair classes are;
    
    SS_GTO, CSS_GTO
    PP_GTO, CPP_GTO, SP_GTO, CSP_GTO
    DPlusDPlus_GTO, CDPlusDPlus_GTO, PDPlus_GTO, CPDPlus_GTO, 
                                     SDPlus_GTO, CSDPlus_GTO
    
    An orbital shell contains all of the orbitals for a particular shell
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT GTO : public OrbitalShell
{

friend QDataStream& ::operator<<(QDataStream&, const GTO&);
friend QDataStream& ::operator>>(QDataStream&, GTO&);

public:
    GTO();
    GTO(const GTO &other);
    
    virtual ~GTO();
    
    static const char* typeName();
    
    virtual GTO* clone() const=0;

    double alpha() const;
    double beta() const;
    
    double scale() const;

protected:
    GTO(double alpha, double scale);

    GTO& operator=(const GTO &other);
    
    bool operator==(const GTO &other) const;
    bool operator!=(const GTO &other) const;

private:
    /** The orbital exponent (alpha) */
    double alfa;
    
    /** The multiplication factor (including normalisation
        constant) */
    double scl;
};

/** This is the base class of all Contracted Gaussian Type Orbitals (CGTO).
    (e.g. CS_GTO (l==0), CP_GTO (l==1), CDPlus_GTO (l==2)

    This contains just a single orbital shell, containing the 
    correct number of orbitals for its angular momentum
    (1 for S, 3 for P, 5+ for DPlus etc.)
    
    An orbital shell contains all of the orbitals for a particular shell
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT CGTO : public OrbitalShell
{

friend QDataStream& ::operator<<(QDataStream&, const CGTO&);
friend QDataStream& ::operator>>(QDataStream&, CGTO&);

public:
    CGTO();
    virtual ~CGTO();

    static const char* typeName();
    
    virtual CGTO* clone() const=0;

    int nContractions() const;
    
    const NVector& alpha() const;
    const NVector& beta() const;
    
    const NVector& scale() const;
    
protected:
    CGTO(const NVector &alphas, const NVector &scales);
    
    CGTO& operator=(const CGTO &other);
    
    bool operator==(const CGTO &other) const;
    bool operator!=(const CGTO &other) const;

private:
    /** The orbital exponents (alpha) */
    NVector alfas;
    
    /** The multiplication factors (including normalisation
        constant) */
    NVector scls;
};

}

SIRE_EXPOSE_CLASS( Squire::GTO )
SIRE_EXPOSE_CLASS( Squire::CGTO )

SIRE_END_HEADER

#endif
