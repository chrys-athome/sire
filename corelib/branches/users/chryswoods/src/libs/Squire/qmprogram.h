/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SQUIRE_QMPROGRAM_H
#define SQUIRE_QMPROGRAM_H

SIRE_BEGIN_HEADER

namespace Squire
{

/** This is the base class of all QM programs. These are wrappers that
    provide the functionality to calculate QM energies and forces
    by calling separate QM programs
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT QMProg : public SireBase::PropertyBase
{
};

/** This is the null QM program that returns zero energy and force */
class SQUIRE_EXPORT NullQM : public QMProg
{
};

/** Property holder for the QMProgram hierarchy of classes */
class SQUIRE_EXPORT QMProgram : public SireBase::Property
{
};

}

Q_DECLARE_METATYPE( Squire::QMProgram )
Q_DECLARE_METATYPE( Squire::NullQM )

SIRE_EXPOSE_CLASS( Squire::QMProg )
SIRE_EXPOSE_CLASS( Squire::NullQM )

SIRE_EXPOSE_PROPERTY( Squire::QMProgram, Squire::QMProg )

SIRE_END_HEADER

#endif
