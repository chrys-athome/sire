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

#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#ifdef BUILD_SHARED_WRAPPERS
#include "wrap_SireCAS.h"
#endif

namespace SireCAS
{
void export_ExBase();
void export_Expression();
void export_ExpressionBase();
void export_Function();
void export_HyperbolicFuncs();
void export_Identities();
void export_PowerFuncs();
void export_Symbol();
void export_TrigFuncs();
void export_Values();

void SIRECAS_EXPORT export_SireCAS()
{
    export_ExBase();
    export_Expression();
    export_ExpressionBase();
    export_Symbol();
    export_Function();
    export_TrigFuncs();
    export_HyperbolicFuncs();
    export_Identities();
    export_Values();
    export_PowerFuncs();
}

}
