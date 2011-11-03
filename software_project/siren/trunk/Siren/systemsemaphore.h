#ifndef SIREN_SYSTEMSEMAPHORE_H
#define SIREN_SYSTEMSEMAPHORE_H
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
#include "Siren/block.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This is a system-wide Semaphore, which enables multiple
        processes to share the same semaphore on a system, thereby
        allowing multiple processes to co-ordinate the sharing
        of a resource. It is heavily based on QSystemSemaphore */
    class SIREN_EXPORT SystemSemaphore : public Block
    {
    public:
        enum AccessMode
        {
            Open = QSystemSemaphore::Open,
            Create = QSystemSemaphore::Create
        };

        SystemSemaphore(const String &key, int initialValue=0, AccessMode mode=Open);
        ~SystemSemaphore();

        void setKey(const String &key, int initialValue=0, AccessMode mode=Open);
        String key() const;

        void acquire();
        void release(int n=1);
        
        String toString() const;
        
    protected:
        void checkEndForAges() const;
        
    private:
        QSystemSemaphore s;
    
    }; // end of class SystemSemaphore

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::SystemSempaphore )

SIREN_END_HEADER

#endif // ifndef SIREN_SYSTEMSEMAPHORE_H
