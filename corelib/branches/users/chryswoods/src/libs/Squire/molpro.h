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

#ifndef SQUIRE_MOLPRO_H
#define SQUIRE_MOLPRO_H

#include "qmprogram.h"

SIRE_BEGIN_HEADER

namespace Squire
{

/** This is a wrapper that allows Molpro to be used to calculate
    QM and QM/MM energies
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT Molpro : public QMProg
{
};

}

Q_DECLARE_METATYPE( Squire::Molpro )

SIRE_EXPOSE_CLASS( Squire::Molpro )

SIRE_END_HEADER

#endif
