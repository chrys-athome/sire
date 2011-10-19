#ifndef SIREN_STRINGLIST_H
#define SIREN_STRINGLIST_H
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

#include "Siren/siren.h"
#include "Siren/string.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This class provides a list of Siren Strings 
    
        @author Christopher Woods
    */
    class SIREN_EXPORT StringList : public Object
    {
        SIREN_CLASS( StringList, Object )
        
    public:
        typedef List<String>::const_iterator const_iterator;
        typedef const_iterator iterator;
    
        StringList();
        StringList(const String &str);
        StringList(const String *strs, int count);
        StringList(const List<String>::Type &strs);
        
        #ifdef SIREN_QT_SUPPORT
        StringList(const QStringList &strlist);
        operator QStringList() const;
        #endif
        
        StringList(const StringList &other);
        
        ~StringList();
        
        const_iterator constBegin() const;
        const_iterator begin() const;
        
        const_iterator constEnd() const;
        const_iterator end() const;
        
        void assertValidIndex(int i) const;
        
        const String& at(int i) const;
        const String& operator[](int i) const;
        
        int count() const;
        int size() const;
        
        bool isEmpty() const;
        
        String join(const String &seperator) const;
        
        bool contains(const String &str) const;
        
        List<String>::Type toList() const;
        static StringList fromList(const List<String>::Type &list);
        
        operator List<String>::Type() const;

    protected:
        void copy_object(const StringList &other);
        bool compare_object(const StringList &other) const;
        
    private:
        /** The actual list of strings */
        List<String>::Type d;
        
    }; // end of class StringList

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::StringList )

SIREN_END_HEADER

#endif // ifndef SIREN_STRINGLIST_H
