#ifndef SIREN_INTERFACES_H
#define SIREN_INTERFACES_H
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
    namespace detail
    {
        /** This is the base class of a "None" type, which is used
            as the default argument in Interfaces<...> to represent
            a null interface */
        class SIREN_EXPORT Iface
        {
        protected:
            static bool IS_INTERFACE()
            {
                return false;
            }
            
            static const char* interfaceName()
            {
                return 0;
            }
        
        }; // end of class NullInterface

        class Iface_1 : public Iface {};
        class Iface_2 : public Iface {};
        class Iface_3 : public Iface {};
        class Iface_4 : public Iface {};
        class Iface_5 : public Iface {};
        class Iface_6 : public Iface {};
        class Iface_7 : public Iface {};
        class Iface_8 : public Iface {};
        class Iface_9 : public Iface {};

    } // end of namespace detail


    /** Use this class to provide the list of interfaces that an
        object provides, e.g.
    
        class MyObject : public Object, public Interfaces< Mutable<MyObjectEditor> >
    */
    template<class I0,
             class I1=detail::Iface_1, class I2=detail::Iface_2, class I3=detail::Iface_3,
             class I4=detail::Iface_4, class I5=detail::Iface_5, class I6=detail::Iface_6,
             class I7=detail::Iface_7, class I8=detail::Iface_8, class I9=detail::Iface_9>
    class SIREN_EXPORT Interfaces
            : public virtual I0,
              public virtual I1, public virtual I2, public virtual I3,
              public virtual I4, public virtual I5, public virtual I6,
              public virtual I7, public virtual I8, public virtual I9
    {
    public:
        typedef Interfaces<I0,I1,I2,I3,I4,I5,I6,I7,I8,I9> interfaces;
    
        Interfaces() : I0(), I1(), I2(), I3(), I4(), I5(), I6(), I7(), I8(), I9()
        {}
        
        ~Interfaces()
        {}
        
    protected:
        static bool IS_INTERFACE()
        {
            return false;
        }
        
        static const char** listInterfaces()
        {
        
            if (ifaces_array == 0)
            {
                int n = 0;
            
                if (I0::IS_INTERFACE()) n += 1;
                if (I1::IS_INTERFACE()) n += 1;
                if (I2::IS_INTERFACE()) n += 1;
                if (I3::IS_INTERFACE()) n += 1;
                if (I4::IS_INTERFACE()) n += 1;
                if (I5::IS_INTERFACE()) n += 1;
                if (I6::IS_INTERFACE()) n += 1;
                if (I7::IS_INTERFACE()) n += 1;
                if (I8::IS_INTERFACE()) n += 1;
                if (I9::IS_INTERFACE()) n += 1;
            
                if (n == 0)
                    return 0;
            
                const char **ifs = new const char*[n+1];
                
                int i = 0;
                
                if (I0::IS_INTERFACE()){ ifs[i] = I0::interfaceName(); ++i }
                if (I1::IS_INTERFACE()){ ifs[i] = I1::interfaceName(); ++i }
                if (I2::IS_INTERFACE()){ ifs[i] = I2::interfaceName(); ++i }
                if (I3::IS_INTERFACE()){ ifs[i] = I3::interfaceName(); ++i }
                if (I4::IS_INTERFACE()){ ifs[i] = I4::interfaceName(); ++i }
                if (I5::IS_INTERFACE()){ ifs[i] = I5::interfaceName(); ++i }
                if (I6::IS_INTERFACE()){ ifs[i] = I6::interfaceName(); ++i }
                if (I7::IS_INTERFACE()){ ifs[i] = I7::interfaceName(); ++i }
                if (I8::IS_INTERFACE()){ ifs[i] = I8::interfaceName(); ++i }
                if (I9::IS_INTERFACE()){ ifs[i] = I9::interfaceName(); ++i }
                
                //null terminate the list of interface names
                ifs[i] = 0;
                
                while (ifaces_array == 0)
                {
                    ifaces_array = ifs;
                }
                
                if (ifaces_array != ifs)
                    delete[] ifs;
            }
            
            return ifaces_array;
        }
        
    private:
        static const char **ifaces_array;
    
    }; // end of class Interfaces<...>

} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_INTERFACES_H
