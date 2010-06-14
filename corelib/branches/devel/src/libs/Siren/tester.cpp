/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include <cmath>

#include "tester.h"
#include "stream.h"
#include "objref.h"

#include "Siren/errors.h"

using namespace Siren;

static const RegisterObject<Tester> r_tester;

/** Null constructor - this will write to the default logger */
Tester::Tester() 
       : Implements<Tester,Object>(), Interfaces<Tester,Mutable>(),
         current_test(0), num_current_errors(0), num_errors(0)
{}

/** Construct the tester for the passed object - this 
    will write any output to the default logger */
Tester::Tester(const Object &object)
       : Implements<Tester,Object>(), Interfaces<Tester,Mutable>(),
         tested_class(object.what()),
         current_test(0), num_current_errors(0), num_errors(0)
{}

/** Construct the tester for the passed object, writing
    any output to the passed logger */
Tester::Tester(const Object &object, Logger &log)
       : Implements<Tester,Object>(), Interfaces<Tester,Mutable>(),
         logger(log), tested_class(object.what()),
         current_test(0), num_current_errors(0), num_errors(0)
{}

/** Construct the tester for the passed handle - this 
    will write any output to the default logger */
Tester::Tester(const Handle &handle)
       : Implements<Tester,Object>(), Interfaces<Tester,Mutable>(),
         tested_class(handle.what()),
         current_test(0), num_current_errors(0), num_errors(0)
{}

/** Construct the tester for the passed handle, writing
    any output to the passed logger */
Tester::Tester(const Handle &handle, Logger &log)
       : Implements<Tester,Object>(), Interfaces<Tester,Mutable>(),
         logger(log), tested_class(handle.what()),
         current_test(0), num_current_errors(0), num_errors(0)
{}

/** Construct the tester for the class with the passed name - this 
    will write any output to the default logger */
Tester::Tester(const QString &type_name)
       : Implements<Tester,Object>(), Interfaces<Tester,Mutable>(),
         tested_class(type_name),
         current_test(0), num_current_errors(0), num_errors(0)
{}

/** Construct the tester for the class with the passed name, writing
    any output to the passed logger */
Tester::Tester(const QString &type_name, Logger &log)
       : Implements<Tester,Object>(), Interfaces<Tester,Mutable>(),
         logger(log), tested_class(type_name),
         current_test(0), num_current_errors(0), num_errors(0)
{}

/** Copy constructor */
Tester::Tester(const Tester &other)
       : Implements<Tester,Object>(other), Interfaces<Tester,Mutable>(),
         logger(other.logger), tested_class(other.tested_class),
         current_test(other.current_test),
         num_current_errors(other.num_current_errors), num_errors(other.num_errors)
{}

/** Destructor */
Tester::~Tester()
{
    if (not tested_class.isEmpty())
        this->printSummary();
}

/** Copy assignment operator */
Tester& Tester::operator=(const Tester &other)
{
    if (this != &other)
    {
        logger = other.logger;
        tested_class = other.tested_class;
        current_test = other.current_test;
        num_current_errors = other.num_current_errors;
        num_errors = other.num_errors;
        
        Object::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Tester::operator==(const Tester &other) const
{
    return this == &other  or
           ( logger == other.logger and 
             tested_class == other.tested_class and
             current_test == other.current_test and
             num_current_errors == other.num_current_errors and 
             num_errors == other.num_errors );
}

/** Comparison operator */
bool Tester::operator!=(const Tester &other) const
{
    return not Tester::operator==(other);
}

QString Tester::toString() const
{
    return QObject::tr("Tester( %1, number of errors = %2 )")
                .arg(tested_class).arg(num_errors);
}

uint Tester::hashCode() const
{
    return qHash( Tester::typeName() ) + qHash(tested_class) + num_errors;
}

QString Tester::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Tester>() );
}

/** Run the unit tests of the Tester class ;-) */
bool Tester::test(Logger &logger) const
{
    Tester tester(*this, logger);

    #ifndef SIREN_DISABLE_TESTS    
    
    try
    {
        //Tester t( Logger::null() );
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
}

void Tester::stream(Stream &s)
{
    s.assertVersion<Tester>(1);
    
    Schema schema = s.item<Tester>();
    
    schema.data("tested_class") & tested_class;
    schema.data("current_test") & current_test;
    schema.data("num_current_errors") & num_current_errors;
    schema.data("num_errors") & num_errors;
    
    Object::stream( schema.base() );
}

/** Return whether or not all of the tests passed */
bool Tester::allPassed() const
{
    return num_errors == 0;
}

void Tester::testPassed(const QString &description)
{
    logger.write( QObject::tr("%1 : Passed!").arg(description) );
}

void Tester::testFailed(const QString &description, const QString &location)
{
    logger.write( QObject::tr("%1 : Failed").arg(description) );

    num_current_errors += 1;
    num_errors += 1;
    
    failed_locations.append(location);
}

void Tester::printLastTestSummary()
{
    if (num_current_errors > 0)
        logger.writeHeader( QObject::tr("%1 of the above tests failed!")
                                    .arg(num_current_errors), 3 );
}

/** Advance to the next test */
void Tester::nextTest()
{
    printLastTestSummary();

    current_test += 1;
    num_current_errors = 0;
    
    logger.writeHeader( QObject::tr("Test %1").arg(current_test), 1 );
}

/** Advance to the next test, which is described in 'description' */
void Tester::nextTest(const QString &description)
{
    printLastTestSummary();

    current_test += 1;
    num_current_errors = 0;
    
    logger.writeHeader( QObject::tr("Test %1\n%2")
                           .arg(current_test).arg(description), 1 );
}

/** Test if 'flag' is true. 'flag' is the output of the test run at
    'location', described in 'description' */
void Tester::expect_true(const QString &description, const QString &location,
                         bool flag)
{
    if (flag)
        this->testPassed(description);
    else
        this->testFailed(description, location);
    
}

/** Test is 'flag' is false. 'flag' is the output of the test run at
    'location', described in 'description' */
void Tester::expect_false(const QString &description, const QString &location,
                          bool flag)
{
    if (not flag)
        this->testPassed(description);
    else
        this->testFailed(description, location);

}

/** Test that 'x' and 'y' are roughly equal. This tests if the difference
    between x and y is less than 0.0001% of their sum */ 
void Tester::expect_roughly_equal(const QString &description, 
                                  const QString &location,
                                  double x, double y)
{
    const double diff = std::fabs(x - y);
    
    if (diff < 1e-6)
        this->testPassed(description);
    else
    {
        const double sum = std::fabs(x) + std::fabs(y);
        
        if (diff < 1e-6*sum)
            this->testPassed(description);
        else
            this->testFailed(description, location);
    }
}

/** Call this function if you catch an error during a test that you are
    not expecting */
void Tester::unexpected_error(const Siren::exception &e)
{
    this->testFailed( QObject::tr("Unexpectedly threw an exception: %1 (%2).")
                        .arg(e.what(), e.error()), e.from() );
}

/** Internal function called when two object are not equal when they should be */
void Tester::notEqualError(const QString &obj0, const QString &obj1,
                           const QString &description, const QString &location)
{
    this->testFailed( QObject::tr("%1 (%2 == %3)")
                         .arg(description, obj0, obj1), location );
}

/** Internal function called when two object are equal when they should not be */
void Tester::notDifferentError(const QString &obj0, const QString &obj1,
                               const QString &description, const QString &location)
{
    this->testFailed( QObject::tr("%1 (%2 == %3)")
                        .arg(description, obj0, obj1), location );

}

/** Print a summary of all of the tests */
void Tester::printSummary()
{
    printLastTestSummary();

    if (num_errors == 0)
    {
        logger.writeHeader( QObject::tr("All tests passed!"), 2 );
        return;
    }

    QStringList lines;
    
    lines.append( QObject::tr("In total, %1 of the above tests failed.")
                        .arg(num_errors) );
                        
    lines.append( QObject::tr("\nThe locations of the errors, in order, are;") );
    
    lines += failed_locations;
    
    logger.writeHeader( lines.join("\n"), 2 );
}
