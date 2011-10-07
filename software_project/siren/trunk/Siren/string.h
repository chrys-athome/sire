#ifndef SIREN_STRING_H
#define SIREN_STRING_H

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

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This class is used to represent a text string in Siren.
        The string is held as a unicode string, and provides
        support for automatic translation. All strings are
        held in a central cache, thus ensuring that only a single
        copy of each string is held in memory.
        
        @author Christopher Woods
    */
    class SIREN_EXPORT String : public Object
    {
        SIREN_CLASS(String, Object)

    public:
        String();
        String(const Char *unicode, int size);
        String(Char c);
        String(int size, Char c);
        String(const Latin1String &latin1);
        
        #ifdef SIREN_QT_SUPPORT
            String(const QString &qstring);
            operator QString() const;
        #endif
            
        String(const String &other);
        
        ~String();

        String& operator=(const String &other);
        String& operator=(Char c);
        String& operator=(const Latin1String &latin1);
        
        bool operator==(const String &other) const;
        bool operator!=(const String &other) const;
        
        String toString() const;
        
        ///////
        /////// String functions
        ///////
        
        int size() const;
        int count() const;
        int length() const;
        bool isEmpty() const;

        String resize(int size) const;

        String truncate(int pos) const;
        String chop(int n) const;
        
        String left(int n) const;
        String right(int n) const;
        String mid(int pos, int n=-1) const;
        
        String toLower() const;
        String toUpper() const;
        String toCaseFolded() const;
        
        String trimmed() const;
        String simplified() const;
        
        String insert(int i, Char c) const;
        String insert(int i, const Char *unicode, int len) const;
        String insert(int i, const String &s) const;
        
        String append(Char c) const;
        String append(const String &s) const;
        String append(const Char *unicode, int len) const;
        
        String prepend(Char c) const;
        String prepend(const String &s) const;
        String prepend(const Char *unicode, int len) const;
        
        String remove(int pos, int len) const;
        
        StringList split() const;
        StringList split(const String &sep, bool case_sensitive=true) const;
        
        String repeated(int n) const;
        
        const Char* unicode() const;
        const Char* data() const;
        
        const Char at(int i) const;
        const Char operator[](int i) const;
        
        static String fromUtf8(const char *string, int size=-1);
        static String fromAscii(const char *string, int size=-1);
        static String fromLocal8Bit(const char *string, int size=-1);
        static String fromLatin1(const char *string, int size=-1);
        static String fromRawData(const Char *unicode, int size);
        static String fromWCharArray(const wchar_t *wchar, int size=-1);
        
        ByteArray toUtf8() const;
        ByteArray toAscii() const;
        ByteArray toLocal8Bit() const;
        ByteArray toLatin1() const;
        int toWCharArray(wchar_t *wchar) const;
    
        String arg(int8 number, int fieldWidth=0, int base=10,
                   const Char &fillChar = Latin1Char(' ')) const;
    
        String arg(int32 number, int fieldWidth=0, int base=10,
                   const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(int64 number, int fieldWidth=0, int base=10,
                   const Char &fillChar = Latin1Char(' ')) const;
        
        String arg(uint8 number, int fieldWidth=0, int base=10,
                   const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(uint32 number, int fieldWidth=0, int base=10,
                   const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(uint64 number, int fieldWidth=0, int base=10,
                   const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(float32 number, int fieldWidth=0, char fmt='g', 
                   int prec=-1, const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(float64 number, int fieldWidth=0, char fmt='g',
                   int prec=-1, const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(char a, int fieldWidth=0, 
                   const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(Char a, int fieldWidth=0,
                   const Char &fillChar = Latin1Char(' ')) const;
                   
        String arg(const String &a, int fieldWidth=0,
                   const Char &fillChar = QLatin1Char(' ')) const;

        String arg(const String &a1, const String &a2) const;
        String arg(const String &a1, const String &a2, const String &a3) const;
        String arg(const String &a1, const String &a2, const String &a3,
                   const String &a4) const;
        String arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5) const;
        String arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6) const;
        String arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6,
                   const String &a7) const;
        String arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6,
                   const String &a7, const String &a8) const;
        String arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6,
                   const String &a7, const String &a8, const String &a9) const;        
    
        static String tr(const char* source_text,
                         const char* comment=0,
                         int n=-1);
                         
        static String trUtf8(const char* source_text,
                             const char* comment=0,
                             int n=-1);
    
        static String number(int8 number, int base=10);
        static String number(int32 number, int base=10);
        static String number(int64 number, int base=10);
        
        static String number(uint8 number, int base=10);
        static String number(uint32 number, int base=10);
        static String number(uint64 number, int base=10);
        
        static String number(float32 number, char f='g', int prec=6);
        static String number(float64 number, char f='g', int prec=6);
    
        int8 toInt8(int base=10) const;
        int32 toInt32(int base=10) const;
        int64 toInt64(int base=10) const;
        int toInt(int base=10) const;

        uint8 toUInt8(int base=10) const;
        uint32 toUInt32(int base=10) const;
        uint64 toUInt64(int base=10) const;
        uint toUInt(int base=10) const;
        
        float toFloat() const;
        double toDouble() const;
        
        float32 toFloat32() const;
        float64 toFloat64() const;
    
    private:
        void incref();
        void decref();
    
        /** The actual string - currently use QString to hold the data */
        QString d;
        
        /** A reference count for the number of times this string is used
            in the program */
        AtomicInt *refcount;
    
    }; // end of class String

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::String )

SIREN_END_HEADER

#endif // SIREN_STRING_H
