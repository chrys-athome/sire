#ifndef SIREN_NUMBER_H
#define SIREN_NUMBER_H

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
    /** This class is used to represent a number (integer or float).
        It is used by the streaming classes as a way of holding
        and representing numbers as a single Siren::Object type 
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Number : public Object
    {
        SIREN_CLASS(Number, Object)
    
    public:
        Number();

        explicit Number(bool number);

        explicit Number(int8 number);
        explicit Number(int32 number);
        explicit Number(int64 number);
        
        explicit Number(uint8 number);
        explicit Number(uint32 number);
        explicit Number(uint64 number);
        
        explicit Number(float32 number);
        explicit Number(float64 number);
        
        Number(const Number &other);
        
        ~Number();
        
        String toString() const;

        /////
        ///// Number functions 
        /////
        
        bool isZero() const;
        bool isFloat() const;
        bool isBool() const;
        bool isInteger() const;
        bool isSigned() const;
        
        int numBits() const;

        bool toBool() const;

        int8 toInt8() const;
        int32 toInt32() const;
        int64 toInt64() const;
        
        uint8 toUInt8() const;
        uint32 toUInt32() const;
        uint64 toUInt64() const;
        
        float32 toFloat32() const;
        float64 toFloat64() const;
        
        int toInt() const;
        uint toUInt() const;
        
        float toFloat() const;
        double toDouble() const;

        static Number fromBool(bool number);

        static Number fromInt8(int8 number);
        static Number fromInt32(int32 number);
        static Number fromInt64(int64 number);
        
        static Number fromUInt8(uint8 number);
        static Number fromUInt32(uint32 number);
        static Number fromUInt64(uint64 number);
        
        static Number fromFloat32(float32 number);
        static Number fromFloat64(float64 number);
        
        static Number fromFloat(float number);
        static Number fromDouble(double number);

        operator int() const;
        operator double() const;
        operator bool() const;

    protected:
        void copy_object(const Number &other);
        bool compare_object(const Number &other) const;

    private:
        union
        {
            bool b1;
        
            int8 i8;
            int32 i32;
            int64 i64;
            
            uint8 u8;
            uint32 u32;
            uint64 u64;
            
            float32 f32;
            float64 f64;
        };
        
        enum NumType
        {
            ZERO = 0,
            
            LOGICAL = 1,
            
            INT8 = 10,
            INT32 = 11,
            INT64 = 12,
            
            UINT8 = 20,
            UINT32 = 21,
            UINT64 = 22,
            
            FLOAT32 = 30,
            FLOAT64 = 31
        };
        
        /** The type of number stored by this class */
        NumType num_type;

    }; // end of class Number
    
} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Number )

SIREN_END_HEADER

#endif // ifndef SIREN_NUMBER_H
