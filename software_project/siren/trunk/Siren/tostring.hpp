#ifndef SIREN_TOSTRING_HPP
#define SIREN_TOSTRING_HPP
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

#include "Siren/tostring.h"
#include "Siren/string.h"
#include "Siren/stringlist.h"

#include "Siren/detail/qt4support.h"
#include "Siren/detail/boostsupport.h"

namespace Siren
{
    namespace detail
    {
        /** Return a string representation of number */
        template<typename T>
        struct to_string_numtype
        {
            static String to_string(const T &val)
            {
                return String::number(val);
            }
        };

        /** Return a string representation of a class type (must have 
            implemented a .toString() member function, or else you
            must supply a specific instantiation) */
        template<class T>
        struct to_string_class_with_toString
        {
            static String to_string(const T &obj)
            {
                return obj.toString();
            }
        };

        /** Used to return the string of a non-container type */
        template<class T>
        SIREN_INLINE_TEMPLATE
        String to_string(const T &obj)
        {
            return boost::mpl::if_<boost::is_pod<T>,
                                      to_string_numtype<T>, 
                                      to_string_class_with_toString<T> >
                                 ::type::to_string(obj);
        }

        /** Used to return a string representation of a String! */
        template<>
        SIREN_INLINE_TEMPLATE
        String to_string(const String &string)
        {
            return string;
        }

        /** Used to return a string representation of a String! */
        template<>
        SIREN_INLINE_TEMPLATE
        String to_string(const Latin1String &string)
        {
            return string;
        }

        /** Used to return a string representation of a StringList */
        template<>
        SIREN_INLINE_TEMPLATE
        String to_string(const StringList &strings)
        {
            if (strings.isEmpty())
                return String("( )");
            else
                return String("( %1 )").arg( strings.join(", ") );
        }

        /** Used to return a string representation of a List */
        template<class T>
        SIREN_INLINE_TEMPLATE
        String to_string(const typename List<T>::Type &objs)
        {
            List<String>::Type strngs;
            
            for (typename List<T>::const_iterator it = objs.begin();
                 it != objs.end();
                 ++it)
            {
                strngs.append( to_string(*it) );
            }
            
            return String("[ %1 ]").arg( StringList(strngs).join(","));
        }

        /** Used to return a string representation of a Vector */
        template<class T>
        SIREN_INLINE_TEMPLATE
        String to_string(const typename Vector<T>::Type &objs)
        {
            List<String>::Type strngs;
            
            for (typename Vector<T>::const_iterator it = objs.begin();
                 it != objs.end();
                 ++it)
            {
                strngs.append( to_string(*it) );
            }
            
            return String("[ %1 ]").arg( StringList(strngs).join(","));
        }

        /** Used to return a string representation of a Set */
        template<class T>
        SIREN_INLINE_TEMPLATE
        String to_string(const typename Set<T>::Type &objs)
        {
            List<String>::Type strngs;
            
            for (typename Set<T>::const_iterator it = objs.begin();
                 it != objs.end();
                 ++it)
            {
                strngs.append( to_string(*it) );
            }
            
            return String("{ %1 }").arg( StringList(strngs).join(",") );
        }

        /** Used to return a string representation of a Hash */
        template<class S, class T>
        SIREN_INLINE_TEMPLATE
        String to_string(const typename Hash<S,T>::Type &objs)
        {
            List<String>::Type strngs;
            
            for (typename Hash<S,T>::const_iterator it = objs.begin();
                 it != objs.end();
                 ++it)
            {
                strngs.append( String("%1 : %2")
                        .arg( to_string(it.key()), to_string(it.value()) ) );
            }
            
            return String("{ %1 }").arg( StringList(strngs).join(",") );
        }

        /** Used to return a string representation of a QMap */
        template<class S, class T>
        SIREN_INLINE_TEMPLATE
        String to_string(const typename Map<S,T>::Type &objs)
        {
            List<String>::Type strngs;
            
            for (typename Map<S,T>::const_iterator it = objs.begin();
                 it != objs.end();
                 ++it)
            {
                strngs.append( String(" %1 : %2")
                                    .arg( to_string(it.key()), to_string(it.value()) ) );
            }
            
            return String("{ %1 }").arg( StringList(strngs).join(",") );
        }
    
    } // end of namespace detail

    /** Return a String representation of the object 'obj' */
    template<class T>
    SIREN_INLINE_TEMPLATE
    String toString(const T &obj)
    {
        return detail::to_string(obj);
    }

} // end of namespace Siren

#endif // ifndef SIREN_TOSTRING_HPP
