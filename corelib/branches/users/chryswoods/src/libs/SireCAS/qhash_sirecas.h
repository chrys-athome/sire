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

#ifndef SIRECAS_QHASH_SIRECAS_H
#define SIRECAS_QHASH_SIRECAS_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

/** The name lookup rules for argument dependent function lookups is different
    in gcc-4.1. This is a pain, as it means that all "uint qHash(const T &val)"
    functions *must* be declared before QHash is included. This file contains 
    the declarations of all of the SireCAS qHash functions. Ensure that you include
    this file before including any QHash, QSet or SireBase::Set includes... */
    
namespace SireCAS
{
class Symbol;
class Function;
class FunctionSignature;
class ExpressionBase;
class Expression;
}

uint qHash(const SireCAS::Symbol&);
uint qHash(const SireCAS::Function&);
uint qHash(const SireCAS::FunctionSignature&);
uint qHash(const SireCAS::ExpressionBase&);
uint qHash(const SireCAS::Expression&);

SIRE_END_HEADER

#endif
