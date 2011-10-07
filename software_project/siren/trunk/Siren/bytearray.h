#ifndef SIREN_BYTEARRAY_H
#define SIREN_BYTEARRAY_H
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

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This class provides a binary array of bytes. This is useful
        for storing binary data
        
        @author Christopher Woods
    */
    class SIREN_EXPORT ByteArray : public Object
    {
        SIREN_CLASS(ByteArray, Object);
        
    public:
        ByteArray();
        ByteArray(const char *str);
        ByteArray(const char *data, int size);
        
        #ifdef SIREN_QT_SUPPORT
        ByteArray(const QByteArray &data);
        operator QByteArray() const;
        #endif
        
        ByteArray(const ByteArray &other);
        
        ~ByteArray();
        
        char at(int i) const;
        
        const char* data() const;
        
        int count() const;
        int size() const;
        int length() const;
        
        bool isEmpty() const;
        
        ByteArray toBase64() const;
        static ByteArray fromBase64(const ByteArray &data);

    protected:
        void copy_object(const ByteArray &other);
        bool compare_object(const ByteArray &other) const;
        
    private:
        /** The actual QByteArray holding the data... */
        QByteArray d;
    };


} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_BYTEARRAY_H
