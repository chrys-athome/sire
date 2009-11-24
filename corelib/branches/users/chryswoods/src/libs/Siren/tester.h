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

#ifndef SIREN_TESTER_H
#define SIREN_TESTER_H

#include "object.h"
#include "logger.h"
#include "mutable.h"
#include "tostring.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class exception;

/** This class is used by the unit tests for each Siren::Object
    to report the progress and status of individual tests
    
    @author Christopher Woods
*/
class SIREN_EXPORT Tester 
        : public Implements<Tester,Object>,
          public Interfaces<Tester,Mutable>
{
public:
    Tester();
    
    Tester(const Object &object);
    Tester(const Object &object, Logger &logger);
    
    Tester(const Tester &other);
    
    ~Tester();

    Tester& operator=(const Tester &other);
    
    bool operator==(const Tester &other) const;
    bool operator!=(const Tester &other) const;
    
    static QString typeName(){ return Implements<Tester,Object>::typeName(); }
    
    QString toString() const;
    uint hashCode() const;
    bool test(Logger &logger) const;
    
    void stream(Stream &s);
    
    bool allPassed() const;

    ///////////////////////
    // Mutable Interface //
    ///////////////////////
    
    ObjRef saveState() const;
    void restoreState(const Object &old_state);

    ///////////////////////
    // Mutable functions //
    ///////////////////////

    void nextTest();
    void nextTest(const QString &description);
    
    template<class S, class T>
    void expect_equal(const QString &description, const QString &location,
                      const S &obj0, const T &obj1);
    
    template<class S, class T>
    void expect_different(const QString &description, const QString &location,
                          const S &obj0, const T &obj1);
    
    void expect_true(const QString &description, const QString &location,
                     bool flag);
    
    void expect_false(const QString &description, const QString &location,
                      bool flag);
    
    void expect_roughly_equal(const QString &description, 
                              const QString &location,
                              double x, double y);
    
    void unexpected_error(const Siren::exception &e);
    
protected:
    friend class Implements<Tester,Object>;
    static QStringList listInterfaces()
    {
        return Interfaces<Tester,Mutable>::listInterfaces();
    }

private:
    void notEqualError(const QString &obj0, const QString &obj1,
                       const QString &description, const QString &location);
    void notDifferentError(const QString &obj1, const QString &obj1,
                           const QString &description, const QString &location);

    void testPassed(const QString &description);
    void testFailed(const QString &description, const QString &location);

    void printLastTestSummary();
    void printSummary();

    /** The logger to which the output of the tests is written */
    Logger logger;
    
    /** The type name of the class being tested */
    QString tested_class;
    
    /** The list of failed locations */
    QStringList failed_locations;
    
    /** The current test number */
    quint32 current_test;
    
    /** The number of errors in this test */
    quint32 num_current_errors;
    
    /** The number of errors in total for all tests */
    quint32 num_errors;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

/** Call this to test if 'obj0' and 'obj1' are equal */
template<class S, class T>
void Tester::expect_equal(const QString &description, const QString &location,
                          const S &obj0, const T &obj1)
{
    if (obj0 != obj1)
        this->notEqualError( Siren::toString(obj0), Siren::toString(obj1),
                             description, location );
    else
        this->testPassed(description);
}

/** Call this to test is 'obj0' and 'obj1' are not equal */
template<class S, class T>
void Tester::expect_different(const QString &description, const QString &location,
                              const S &obj0, const T &obj1)
{
    if (obj0 == obj1)
        this->notDifferentError( Siren::toString(obj0), Siren::toString(obj1),
                                 description, location );
    else
        this->testPassed(description);
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

} // end of namespace Siren

Q_DECLARE_METATYPE( Siren::Tester )

SIREN_EXPOSE_CLASS( Siren::Tester )

SIREN_END_HEADER

#endif
