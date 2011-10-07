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
#include "Siren/bytearray.h"
#include "Siren/number.h"
#include "Siren/siren.hpp"

#include "Siren/exceptions.h"

using namespace Siren;

REGISTER_SIREN_CLASS( Siren::String )

/** Null constructor */
String::String() : Object(), refcount(0)
{}

static Hash<QString,AtomicInt*>::Type string_registry;
static ReadWriteLock string_registry_lock;

/** This function registers the passed string, thus ensuring that 
    only one copy of each string is used by the code. */
static QString registerString(const QString &str, AtomicInt **refcount)
{
    if (str.isEmpty())
    {
        *refcount = 0;
        return QString();
    }

    // CODE TO SEE IF THE STRING IS ALREADY REGISTERED
    {
        ReadLocker lkr(&string_registry_lock);
    
        Hash<QString,AtomicInt*>::const_iterator it = string_registry.constFind(str);
    
        if (it != string_registry.constEnd())
        {
            *refcount = it.value();
            return it.key();
        }
    }
    
    // the string is not registered, so register it now
    WriteLocker lkr(&string_registry_lock);
    
    *refcount = new AtomicInt(1);
    
    string_registry.insert(str, *refcount);
    
    // clean the registry every time a multiple of 1000 strings are added
    if (string_registry.count() % 1000 == 0)
    {
        Hash<QString,AtomicInt*>::MutableIterator it(string_registry);
        
        while (it.hasNext())
        {
            it.next();
        
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
    WriteLocker lkr(&string_registry_lock);
    string_registry.remove(str);
}

/** Construct from the passed unicode array */
String::String(const Char *unicode, int size) : Object(), refcount(0)
{
    d = registerString( QString(unicode,size), &refcount );
}

/** Construct from a single unicode character */
String::String(Char c) : Object(), refcount(0)
{
    d = registerString( QString(c), &refcount );
}

String::String(int size, Char c) : Object(), refcount(0)
{
    d = registerString( QString(size,c), &refcount );
}

/** Construct from the passed Latin1 encoded string */
String::String(const Latin1String &latin1) : Object(), refcount(0)
{
    d = registerString( QString(latin1), &refcount );
}

#ifdef SIREN_QT_SUPPORT
    /** Construct from a QString */
    String::String(const QString &qstring) : Object(), refcount(0)
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
        refcount->ref();
}

/** Called to decrease the reference count to a string */
void String::decref()
{
    if (refcount)
    {
        if (not refcount->deref())
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
String::String(const String &other) 
       : Object(other), d(other.d), refcount(other.refcount)
{
    incref();
}

/** Destructor */
String::~String()
{
    decref();
}

/** Copy assignment operator */
void String::copy_object(const String &other)
{
    if (this == &other)
        return;

    decref();

    refcount = other.refcount;
    incref();
    
    d = other.d;
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

/** Return a string representation of this string */
String String::toString() const
{
    return *this;
}

/** Return the length of the string */
int String::size() const
{
    return d.size();
}

/** Return the length of the string */
int String::count() const
{
    return d.count();
}

/** Return the length of the string */
int String::length() const
{
    return d.length();
}

/** Return if this string is empty */
bool String::isEmpty() const
{
    return d.isEmpty();
}

/** Return the string resized to size 'size' */
String String::resize(int size) const
{
    if (size == d.size())
        return *this;
        
    else
    {
        QString d2(d);
        d2.resize(size);
        return String(d2);
    }
}

/** Return the string truncated to position 'pos' */
String String::truncate(int pos) const
{
    if (pos >= d.size())
        return *this;
    
    else
    {
        QString d2(d);
        d2.truncate(pos);
        return String(d2);
    }
}

/** Return the string chopped at position 'n' */
String String::chop(int n) const
{
    QString d2(d);
    d2.chop(n);
    return String(d2);
}

/** Return the left 'n' characters of the string */
String String::left(int n) const
{
    return String( d.left(n) );
}

/** Return the right 'n' characters of the string */
String String::right(int n) const
{
    return String( d.right(n) );
}

/** Return the middle 'n' characters of the string from position 'pos' */
String String::mid(int pos, int n) const
{
    return String( d.mid(pos,n) );
}

/** Return a lower-case version of this string */
String String::toLower() const
{
    return String(d.toLower());
}

/** Return an upper-case version of this string */
String String::toUpper() const
{
    return String(d.toUpper());
}

/** Return a case-folded version of the string. For most unicode
    characters this is the same as "toLower()" */
String String::toCaseFolded() const
{
    return String(d.toCaseFolded());
}

/** Return a string that has whitespace removed from the start and end */
String String::trimmed() const
{
    return String(d.trimmed());
}

/** Return a string that has whitespace removed from the start and the end, 
    and that has each sequence of internal whitespace replaced with a 
    single space */
String String::simplified() const
{
    return String(d.simplified());
}

/** Return a string in which the character 'c' is inserted at position 'i' */
String String::insert(int i, Char c) const
{
    QString d2(d);
    return String( d2.insert(i,c) );
}

/** Return a string in which the characters in the unicode array
    'unicode' of length 'len' are inserted at position 'i' */
String String::insert(int i, const Char *unicode, int len) const
{
    QString d2(d);
    return String( d2.insert(i, unicode, len) );
}

/** Return a string in which the characters in the string 's' are
    inserted at position 'i' */
String String::insert(int i, const String &s) const
{
    QString d2(d);
    return String( d2.insert(i,s) );
}

/** Return a string to which the character 'c' has been appended */
String String::append(Char c) const
{
    QString d2(d);
    return String( d2.append(c) );
}

/** Return a string to which the string 's' has been appended */
String String::append(const String &s) const
{
    QString d2(d);
    return String( d2.append(s) );
}

/** Return a string to which the characters in the unicode array
    'unicode' of length 'len' have been appended */
String String::append(const Char *unicode, int len) const
{
    QString d2(d);
    return String( d2.append( QString(unicode,len) ) );
}

/** Return a string to which the character 'c' has been prepended */
String String::prepend(Char c) const
{
    QString d2(d);
    return String( d2.prepend(c) );
}

/** Return a string to which the string 's' has been prepended */
String String::prepend(const String &s) const
{
    QString d2(d);
    return String( d2.prepend(s) );
}

/** Return a string to which the characters in the unicode array 'unicode'
    of length 'len' have been prepended */
String String::prepend(const Char *unicode, int len) const
{
    QString d2(d);
    return String( d2.prepend( QString(unicode,len) ) );
}

/** Return a string from which 'len' characters from position 'pos'
    have been removed */
String String::remove(int pos, int len) const
{
    QString d2(d);
    d2.remove(pos,len);
    return String(d2);
}

/** Split this string by spaces */
StringList String::split() const
{
    return StringList( d.split(' ', QString::SkipEmptyParts) );
}

/** Split this string using the separator 'sep'. If 'case_sensitive'
    is true, then a case-sensitive split is performed */
StringList String::split(const String &sep, bool case_sensitive) const
{
    if (case_sensitive)
        return StringList( d.split(sep, QString::SkipEmptyParts,
                                        Qt::CaseSensitive) );

    else
        return StringList( d.split(sep, QString::SkipEmptyParts,
                                        Qt::CaseInsensitive) );
}

/** Return a copy of this string repeated 'n' times */
String String::repeated(int n) const
{
    if (n < 2)
        return *this;
    else
        return String( d.repeated(n) );
}

/** Return the raw unicode data for this string */
const Char* String::unicode() const
{
    return d.unicode();
}

/** Return the raw unicode data for this string */
const Char* String::data() const
{
    return d.data();
}

/** Return the unicode character at position 'i' */
const Char String::at(int i) const
{
    return d.at(i);
}

/** Return the unicode character at position 'i' */
const Char String::operator[](int i) const
{
    return d[i];
}

/** Create a string from the UTF8 encoded character sequence
    in 'string' */
String String::fromUtf8(const char *string, int size)
{
    return String( QString::fromUtf8(string,size) );
}

/** Create a string from the ASCII encoded character sequence
    in 'string' */
String String::fromAscii(const char *string, int size)
{
    return String( QString::fromAscii(string,size) );
}

/** Create a string from the local 8 bit encoded character 
    sequence in 'string' */
String String::fromLocal8Bit(const char *string, int size)
{
    return String( QString::fromLocal8Bit(string,size) );
}

/** Create a string from the latin1 encoded character sequence
    in 'string' */
String String::fromLatin1(const char *string, int size)
{
    return String( QString::fromLatin1(string,size) );
}

/** Create a string from the raw unicode data in 'unicode' */
String String::fromRawData(const Char *unicode, int size)
{
    return String( QString::fromRawData(unicode,size) );
}

/** Create a string from the wchar array in 'wchar' */
String String::fromWCharArray(const wchar_t *wchar, int size)
{
    return String( QString::fromWCharArray(wchar,size) );
}

/** Return the string encoded as a UTF8 character array */
ByteArray String::toUtf8() const
{
    return ByteArray( d.toUtf8() );
}

/** Return the string encoded as an ASCII character array */
ByteArray String::toAscii() const
{
    return ByteArray( d.toAscii() );
}

/** Return the string encoded as a local 8 bit character array */
ByteArray String::toLocal8Bit() const
{
    return ByteArray( d.toLocal8Bit() );
}

/** Return the string encoded as a latin1 character array */
ByteArray String::toLatin1() const
{
    return ByteArray( d.toLatin1() );
}

/** place into 'wchar' the string as a wchar array. Returns the
    size of the array */
int String::toWCharArray(wchar_t *wchar) const
{
    return d.toWCharArray(wchar);
}

/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
*/
String String::arg(int8 number, int fieldWidth, int base,
                   const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, base, fillChar) );
}                   

/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
*/
String String::arg(int32 number, int fieldWidth, int base,
                   const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, base, fillChar) );
}                   

/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
*/
String String::arg(int64 number, int fieldWidth, int base,
                   const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, base, fillChar) );
}                   

/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
*/
String String::arg(uint8 number, int fieldWidth, int base,
                   const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, base, fillChar) );
}                   

/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
*/
String String::arg(uint32 number, int fieldWidth, int base,
                   const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, base, fillChar) );
}                   

/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
*/
String String::arg(uint64 number, int fieldWidth, int base,
                   const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, base, fillChar) );
}                   
           
/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
    
    The number is formatted according to the specified format and precision.
*/
String String::arg(float32 number, int fieldWidth, char fmt, 
                   int prec, const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, fmt, prec, fillChar) );
}
           
/** Returns a copy of this string with the lowest numbered place marker
    replaced by the number 'number', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
    
    The number is formatted according to the specified format and precision.
*/
String String::arg(float64 number, int fieldWidth, char fmt, 
                   int prec, const Char &fillChar) const
{
    return String( d.arg(number, fieldWidth, fmt, prec, fillChar) );
}

/** Returns a copy of this string with the lowest numbered place marker
    replaced by the string 'a', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
    
    The number is formatted according to the specified format and precision.
*/
String String::arg(char a, int fieldWidth, const Char &fillChar) const
{
    return String( d.arg(a,fieldWidth,fillChar) );
}
           
/** Returns a copy of this string with the lowest numbered place marker
    replaced by the string 'a', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
    
    The number is formatted according to the specified format and precision.
*/
String String::arg(Char a, int fieldWidth, const Char &fillChar) const
{
    return String( d.arg(a,fieldWidth,fillChar) );
}
 
/** Returns a copy of this string with the lowest numbered place marker
    replaced by the string 'a', i.e., %1, %2, ..., %99.
    
    fieldWidth specifies the minimum amount of space that the argument shall 
    occupy. If it requires less space than fieldWidth, it is padded to 
    fieldWidth with the character fillChar. A positive fieldWidth 
    produces right-aligned text. A negative fieldWidth produces left-aligned text.
    
    The number is formatted according to the specified format and precision.
*/
String String::arg(const String &a, int fieldWidth, const Char &fillChar) const
{
    return String( d.arg(a,fieldWidth,fillChar) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2), 
    except that the strings a1 and a2 are replaced in one pass. */
String String::arg(const String &a1, const String &a2) const
{
    return String( d.arg(a1,a2) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2).arg(a3), 
    except that the strings a1, a2 and a3 are replaced in one pass. */
String String::arg(const String &a1, const String &a2, const String &a3) const
{
    return String( d.arg(a1,a2,a3) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2).arg..., 
    except that the strings a1, a2... etc. are replaced in one pass. */
String String::arg(const String &a1, const String &a2, const String &a3,
                   const String &a4) const
{
    return String( d.arg(a1,a2,a3,a4) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2).arg..., 
    except that the strings a1, a2... etc. are replaced in one pass. */
String String::arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5) const
{
    return String( d.arg(a1,a2,a3,a4,a5) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2).arg..., 
    except that the strings a1, a2... etc. are replaced in one pass. */
String String::arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6) const
{
    return String( d.arg(a1,a2,a3,a4,a5,a6) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2).arg..., 
    except that the strings a1, a2... etc. are replaced in one pass. */
String String::arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6,
                   const String &a7) const
{
    return String( d.arg(a1,a2,a3,a4,a5,a6,a7) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2).arg..., 
    except that the strings a1, a2... etc. are replaced in one pass. */
String String::arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6,
                   const String &a7, const String &a8) const
{
    return String( d.arg(a1,a2,a3,a4,a5,a6,a7,a8) );
}

/** This function overloads arg(). This is the same as str.arg(a1).arg(a2).arg..., 
    except that the strings a1, a2... etc. are replaced in one pass. */
String String::arg(const String &a1, const String &a2, const String &a3,
                   const String &a4, const String &a5, const String &a6,
                   const String &a7, const String &a8, const String &a9) const
{
    return String( d.arg(a1,a2,a3,a4,a5,a6,a7,a8,a9) );
}

/** Returns a translated version of sourceText, optionally based on a
    disambiguation string and value of n for strings containing plurals; 
    otherwise returns sourceText itself if no appropriate translated string 
    is available. */
String String::tr(const char* source_text, const char* comment, int n)
{
    return String( QObject::tr(source_text, comment, n) );
}
                 
/** Returns a translated version of sourceText, or QString::fromUtf8(sourceText) 
    if there is no appropriate version. It is otherwise identical to 
    tr(sourceText, disambiguation, n). */
String String::trUtf8(const char* source_text, const char* comment, int n)
{
    return String( QObject::trUtf8(source_text, comment, n) );
}

/** Returns a string equivalent of the number 'num' according to the specified base.
    The base is 10 by default and must be between 2 and 36. For bases other 
    than 10, n is treated as an unsigned integer. */
String String::number(int8 num, int base)
{
    return String( QString::number(num,base) );
}

/** Returns a string equivalent of the number 'num' according to the specified base.
    The base is 10 by default and must be between 2 and 36. For bases other 
    than 10, n is treated as an unsigned integer. */
String String::number(int32 num, int base)
{
    return String( QString::number(num,base) );
}

/** Returns a string equivalent of the number 'num' according to the specified base.
    The base is 10 by default and must be between 2 and 36. For bases other 
    than 10, n is treated as an unsigned integer. */
String String::number(int64 num, int base)
{
    return String( QString::number(num,base) );
}

/** Returns a string equivalent of the number 'num' according to the specified base.
    The base is 10 by default and must be between 2 and 36. For bases other 
    than 10, n is treated as an unsigned integer. */
String String::number(uint8 num, int base)
{
    return String( QString::number(num,base) );
}

/** Returns a string equivalent of the number 'num' according to the specified base.
    The base is 10 by default and must be between 2 and 36. For bases other 
    than 10, n is treated as an unsigned integer. */
String String::number(uint32 num, int base)
{
    return String( QString::number(num,base) );
}

/** Returns a string equivalent of the number 'num' according to the specified base.
    The base is 10 by default and must be between 2 and 36. For bases other 
    than 10, n is treated as an unsigned integer. */
String String::number(uint64 num, int base)
{
    return String( QString::number(num,base) );
}

/** Returns a string equivalent of the number n, formatted according to the 
    specified format and precision. */
String String::number(float32 num, char f, int prec)
{
    return String( QString::number(num,f,prec) );
}

/** Returns a string equivalent of the number n, formatted according to the 
    specified format and precision. */
String String::number(float64 num, char f, int prec)
{
    return String( QString::number(num,f,prec) );
}

static Number getNumber(const QString &d, int base)
{
    bool ok = true;
    
    Number num( d.toInt(&ok) );

    if (not ok)
        throw Siren::invalid_cast( String::tr(
                "Cannot convert the string \"%1\" to an integer, using "
                "base %2.")
                    .arg(d).arg(base), CODELOC );

    return num;
}

/** Returns the string converted to an int using base base, which is 10 by default 
    and must be between 2 and 36. Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into a 8-bit integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
int8 String::toInt8(int base) const
{
    return ::getNumber(d,base).toInt8();
}

/** Returns the string converted to an int using base base, which is 10 by default 
    and must be between 2 and 36. Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into a 32-bit integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
int32 String::toInt32(int base) const
{
    return ::getNumber(d,base).toInt32();
}

/** Returns the string converted to an int using base base, which is 10 by default 
    and must be between 2 and 36. Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into a 64-bit integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
int64 String::toInt64(int base) const
{
    return ::getNumber(d,base).toInt64();
}

/** Returns the string converted to an int using base base, which is 10 by default 
    and must be between 2 and 36. Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into a standard integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
int String::toInt(int base) const
{
    return ::getNumber(d,base).toInt();
}

/** Returns the string converted to an unsigned int using base 'base', 
    which is 10 by default and must be between 2 and 36. 
    Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into an 8 bit unsigned integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
uint8 String::toUInt8(int base) const
{
    return ::getNumber(d,base).toUInt8();
}

/** Returns the string converted to an unsigned int using base 'base', 
    which is 10 by default and must be between 2 and 36. 
    Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into a 32 bit unsigned integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
uint32 String::toUInt32(int base) const
{
    return ::getNumber(d,base).toUInt32();
}

/** Returns the string converted to an unsigned int using base 'base', 
    which is 10 by default and must be between 2 and 36. 
    Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into a 64 bit unsigned integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
uint64 String::toUInt64(int base) const
{
    return ::getNumber(d,base).toUInt64();
}

/** Returns the string converted to an unsigned int using base 'base', 
    which is 10 by default and must be between 2 and 36. 
    Raises a Siren::invalid_cast error if the
    string cannot be converted to a number, and a Siren::numeric_overflow
    error if the number cannot fit into a standard unsigned integer 
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
uint String::toUInt(int base) const
{
    return ::getNumber(d,base).toUInt();
}

static Number getNumber(const QString &d)
{
    bool ok = true;
    
    Number num(d.toFloat(&ok));

    if (not ok)
        throw Siren::invalid_cast( String::tr(
                "Cannot convert the string \"%1\" to a float.")
                    .arg(d), CODELOC );

    return num;
}

/** Returns the string converted to a floating point number. Raises
    a Siren::invalid_cast error if the string cannot be converted 
    to a number, and a Siren::numeric_overflow error if the number
    cannot fit into a standard float
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
float String::toFloat() const
{
    return ::getNumber(d).toFloat();
}

/** Returns the string converted to a floating point number. Raises
    a Siren::invalid_cast error if the string cannot be converted 
    to a number, and a Siren::numeric_overflow error if the number
    cannot fit into a standard double
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
double String::toDouble() const
{
    return ::getNumber(d).toDouble();
}

/** Returns the string converted to a floating point number. Raises
    a Siren::invalid_cast error if the string cannot be converted 
    to a number, and a Siren::numeric_overflow error if the number
    cannot fit into a 32 bit float
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
float32 String::toFloat32() const
{
    return ::getNumber(d).toFloat32();
}

/** Returns the string converted to a floating point number. Raises
    a Siren::invalid_cast error if the string cannot be converted 
    to a number, and a Siren::numeric_overflow error if the number
    cannot fit into a 64 bit float
    
    \throw Siren::invalid_cast
    \throw Siren::numeric_overflow
*/
float64 String::toFloat64() const
{
    return ::getNumber(d).toFloat64();
}
