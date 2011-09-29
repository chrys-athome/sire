#ifndef SIREN_OBJ_H
#define SIREN_OBJ_H
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

#include "siren.h"
#include "detail/boostsupport.h"

SIREN_BEGIN_HEADER

namespace Siren
{

    /** An Obj provides a weakly-typed holder for any Siren object.
        It behaves exactly like the object that it holds, and can
        implicitly cast to object of that type. It is not, however,
        a Siren object. It just provides an interface that is 
        identical to a Siren object
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Obj
    {
    public:
        Obj();
        Obj(const Object &object);
        Obj(Object *object);
        
        Obj(const Obj &other);
        
        ~Obj();
        
        Obj& operator=(const Obj &other);
        
        Obj& operator=(const Object &other);
        Obj& operator=(Object *object);
        
        bool operator==(const Obj &other) const;
        bool operator!=(const Obj &other) const;
        
        bool operator==(const Object &other) const;
        bool operator!=(const Object &other) const;
        
        Class getClass() const;

        const char* what() const;

        void copy(const Object &other);
        bool equals(const Object &other) const;

        Obj clone() const;
        
        void save(Stream &s) const;
        void load(Stream &s);

        bool test() const;
        
        template<class T>
        bool isA() const;
        
        template<class T>
        const T& asA() const;

        String toString() const;

        bool test(Logger &logger) const;

        String docString() const;
        String docString(const String &function) const;
        
        uint hashCode() const;
        
        void stream(Stream &s);
        
        operator const Object&() const;
        operator const Object*() const;
        
    private:
        /** Holder for the generic pointer to the Object */
        exp_shared_ptr<Object>::Type d;
    
    }; // end of class Obj

} // end of namespace Siren


SIREN_END_HEADER

#endif // ifndef SIREN_OBJ_H
