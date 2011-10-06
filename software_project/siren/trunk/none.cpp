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

#include "Siren/none.h"
#include "Siren/siren.hpp"

using namespace Siren;

REGISTER_SIREN_CLASS( Siren::None )

/** Constructor */
None::None() : super()
{}

/** Copy constructor */
None::None(const None &other) : super(other)
{}

/** Destructor */
None::~None()
{}

/** Copy assignment operator */
None& None::operator=(const None &other)
{
    return *this;
}

/** Comparison operator */
bool None::operator==(const None &other) const
{
    return true;
}

/** Comparison operator */
bool None::operator!=(const None &other) const
{
    return false;
}

/*void None::stream(Stream &s)
{
    s.assertVersion<None>(1);

    Schema schema = s.item<None>();
    Object::stream( schema.base() );
}*/

/*bool None::test(Logger &logger) const
{
    Tester tester(*this, logger);

    #ifndef SIREN_DISABLE_TESTS
    
    try
    {
        // Test 1
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("Test a clone is equal."),
                                 CODELOC,
                                 *this, this->clone() );
        }
        
        // Test 2
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("None.what() is \"Siren::None\""),
                                 CODELOC,
                                 this->what(), QLatin1String("Siren::None") );
        }
        
        // Test 3
        {
            tester.nextTest();
        
            QByteArray b;

            DataStream ds( &b, QIODevice::WriteOnly );
        
            ds << *this;
        
            DataStream ds2( b );
            
            None new_none;
            
            ds2 >> new_none;
            
            tester.expect_true( QObject::tr("Reloaded None object is equal."),
                                CODELOC, new_none.equals( *this ) );
        }
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    catch(const std::exception &e)
    {
        tester.unexpected_error( std_exception(e) );
    }
    catch(...)
    {
        tester.unexpected_error( unknown_error(CODELOC) );
    }
    
    #endif // SIREN_DISABLE_TESTS
    
    return tester.allPassed();
}*/
