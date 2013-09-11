#ifndef SIREN_SIREN_H
#define SIREN_SIREN_H
/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/macros.h"
#include "Siren/tostring.h"

#include "Siren/detail/metatype.h"
#include "Siren/detail/qt4support.h"
#include "Siren/detail/boostsupport.h"

//Forward declaration of key classes
namespace Siren
{
    class Object;
    class Obj;
    class None;

    class Class;
    
    class Exception;

    class TestReport;
    class TestReportEditor;

    class Stream;

    class String;
    class StringList;
    class Number;
    class ByteArray;

    int wrap_index(int i, int count);

    void init(int argc=0, const char **argv=0);
    void loadLibrary(const char *library);
    void fini();

} // end of namespace Siren

#include "Siren/object.h"

#endif // SIREN_SIREN_H