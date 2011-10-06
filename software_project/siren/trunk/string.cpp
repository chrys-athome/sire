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

#include "Siren/string.h"
#include "Siren/siren.hpp"

using namespace Siren;

REGISTER_SIREN_CLASS( Siren::String )

/** Null constructor */
String::String() : refcount(0)
{}

static Hash<QString,int*>::Type string_registry;
static ReadWriteLock string_registry_lock;

/** This function registers the passed string, thus ensuring that 
    only one copy of each string is used by the code. */
static QString registerString(const QString &str, int **refcount)
{
    if (str.isEmpty())
    {
        *refcount = 0;
        return QString();
    }

    // CODE TO SEE IF THE STRING IS ALREADY REGISTERED
    {
        ReadLocker lkr(&string_registry_lock);
    
        Hash<QString,int*>::Type::const_iterator it = string_registry.constFind(str);
    
        if (it != string_registry.constEnd())
        {
            *refcount = it.value();
            return it.key();
        }
    }
    
    // the string is not registered, so register it now
    WriteLocker lkr(&string_registry_lock);
    
    *refcount = new int(1);
    
    string_registry.insert(str, *refcount);
    
    // clean the registry every time a multiple of 1000 strings are added
    if (string_registry.count() % 1000 == 0)
    {
        MutableHashIterator<QString,int*>::Type it(string_registry);
        
        while (it.hasNext())
        {
            ++it;
        
            if ( *(it.value()) == 0 )
            {
                //there are no more references to this string
                delete it.value();
                it.remove();
            }
        }
    }
    
    return str;
}

/** Unregister a string */
static void unregisterString(const QString &str)
{
    QWriteLocker lkr(&string_registry);
    string_registry.remove(str);
}

/** Construct from the passed unicode array */
String::String(const Char *unicode, int size) : refcount(0)
{
    d = registerString( QString(unicode,size), &refcount );
}

/** Construct from a single unicode character */
String::String(Char c) : refcount(0)
{
    d = registerString( QString(c), &refcount );
}

String::String(int size, Char c) : refcount(0)
{
    d = registerString( QString(size,c), &refcount );
}

/** Construct from the passed Latin1 encoded string */
String::String(const Latin1String &latin1) : refcount(0)
{
    d = registerString( QString(latin1), &refcount );
}

#ifdef SIREN_QT_SUPPORT
    /** Construct from a QString */
    String::String(const QString &qstring) : refcount(0)
    {
        d = registerString(qstring, &refcount);
    }
    
    /** Allow automatic casting to a QString */
    String::operator QString() const
    {
        return d;
    }
#endif

/** Called to increase the reference count to a string */
void String::incref()
{
    if (refcount)
        atomic_increment(refcount);
}

/** Called to decrease the reference count to a string */
void String::decref()
{
    if (refcount)
    {
        if (not atomic_decrement(refcount))
        {
            //delete the reference count
            delete refcount;
        
            //this was the last reference
            ::unregisterString(d);
        }
        
        refcount = 0;
    }
}
    
/** Copy constructor */
String::String(const String &other) : d(other.d), refcount(other.refcount)
{
    incref();
}

/** Destructor */
String::~String()
{
    decref();
}

/** Copy assignment operator */
String& String::operator=(const String &other)
{
    if (this == &other)
        return;

    decref();

    refcount = other.refcount;
    incref();
    
    d = other.d;
    
    return *this;
}

/** Copy assignment operator */
String& String::operator=(Char c)
{
    return this->operator=( String(c) );
}

/** Copy assignment operator */
String& String::operator=(const Latin1String &latin1)
{
    return this->operator=( String(latin1) );
}

/** String comparison - fast as all identical strings use
    the same storage */
bool String::operator==(const String &other) const
{
    return refcount == other.refcount;
}

/** String comparison - fast as all identical strings use
    the same storage */
bool String::operator!=(const String &other) const
{
    return refcount != other.refcount;
}

/** Return a string representation of this string */
String String::toString() const
{
    return *this;
}

int String::size() const;
int String::count() const;
int String::length() const;
bool String::isEmpty() const;

String String::resize(int size) const;

String String::truncate(int pos) const;
String String::chop(int n) const;

String String::left(int n) const;
String String::right(int n) const;
String String::mid(int pos, int n=-1) const;

String String::toLower() const;
String String::toUpper() const;
String String::toCaseFolded() const;

String String::trimmed() const;
String String::simplified() const;

String String::insert(int i, Char c) const;
String String::insert(int i, const Char *unicode, int len) const;
String String::insert(int i, const String &s) const;

String String::append(Char c) const;
String String::append(const String &s) const;
String String::append(const Char *unicode, int len) const;

String String::prepend(Char c) const;
String String::prepend(const String &s) const;
String String::prepend(const Char *unicode, int len) const;

String String::remove(int pos, int len) const;

StringList String::split() const;
StringList String::split(const String &sep) const;

String String::repeated(int n) const;

const Char* String::unicode() const;
const Char* String::data() const;

const Char& String::at(int i) const;
const Char& String::operator[](int i) const;

static String String::fromUtf8(const char *string, int size=-1);
static String String::fromAscii(const char *string, int size=-1);
static String String::fromLocal8Bit(const char *string, int size=-1);
static String String::fromLatin1(const char *string, int size=-1);
static String String::fromRawData(const Char *unicode, int size);
static String String::fromWCharArray(const wchar_t *wchar, int size=-1);

ByteArray String::toUtf8() const;
ByteArray String::toAscii() const;
ByteArray String::toLocal8Bit() const;
ByteArray String::toLatin1() const;
int String::toWCharArray(wchar_t *wchar) const;

String String::arg(int8 number, int fieldWidth=0, int base=10,
           const Char &fillChar = Latin1Char(' ')) const;

String String::arg(int32 number, int fieldWidth=0, int base=10,
           const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(int64 number, int fieldWidth=0, int base=10,
           const Char &fillChar = Latin1Char(' ')) const;

String String::arg(uint8 number, int fieldWidth=0, int base=10,
           const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(uint32 number, int fieldWidth=0, int base=10,
           const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(uint64 number, int fieldWidth=0, int base=10,
           const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(float32 number, int fieldWidth=0, char fmt='g', 
           int prec=-1, const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(float64 number, int fieldWidth=0, char fmt='g',
           int prec=-1, const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(char a, int fieldWidth=0, 
           const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(Char a, int fieldWidth=0,
           const Char &fillChar = Latin1Char(' ')) const;
           
String String::arg(const String &a, int fieldWidth=0,
           const Char &fillChar = QLatin1Char(' ')) const;

String String::arg(const String &a1, const String &a2) const;
String String::arg(const String &a1, const String &a2, const String &a3) const;
String String::arg(const String &a1, const String &a2, const String &a3,
           const String &a4) const;
String String::arg(const String &a1, const String &a2, const String &a3,
           const String &a4, const String &a5) const;
String String::arg(const String &a1, const String &a2, const String &a3,
           const String &a4, const String &a5, const String &a6) const;
String String::arg(const String &a1, const String &a2, const String &a3,
           const String &a4, const String &a5, const String &a6,
           const String &a7) const;
String String::arg(const String &a1, const String &a2, const String &a3,
           const String &a4, const String &a5, const String &a6,
           const String &a7, const String &a8) const;
String String::arg(const String &a1, const String &a2, const String &a3,
           const String &a4, const String &a5, const String &a6,
           const String &a7, const String &a8, const String &a9) const;        

static String String::tr(const char* source_text,
                 const char* comment=0,
                 int n=-1);
                 
static String String::trUtf8(const char* source_text,
                     const char* comment=0,
                     int n=-1);

static String String::number(int8 number, int base=10);
static String String::number(int32 number, int base=10);
static String String::number(int64 number, int base=10);

static String String::number(uint8 number, int base=10);
static String String::number(uint32 number, int base=10);
static String String::number(uint64 number, int base=10);

static String String::number(float32 number, char f='g', int prec=6);
static String String::number(float64 number, char f='g', int prec=6);

int8 String::toInt8(int base=10) const;
int32 String::toInt32(int base=10) const;
int64 String::toInt64(int base=10) const;
int String::toInt(int base=10) const;

uint8 String::toUInt8(int base=10) const;
uint32 String::toUInt32(int base=10) const;
uint64 String::toUInt64(int base=10) const;
uint String::toUInt(int base=10) const;

float String::toFloat() const;
double String::toDouble() const;

float32 String::toFloat32() const;
float64 String::toFloat64() const;
