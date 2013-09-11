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
#include "Siren/exceptions.h"
#include "Siren/static.h"
#include "Siren/forages.h"

#ifdef Q_OS_UNIX
    #include <signal.h>    // CONDITIONAL_INCLUDE
#endif

#include <cstdlib>

namespace Siren
{
    /** Wrap the index 'i' so that it indexes into a container
        of 'count' elements. If the index >= 0, then it counts
        forward from the start, while if it is < 0 then it counts
        back from the last item in the container
        
        \throw Siren::invalid_index
    */
    int SIREN_EXPORT wrap_index(int i, int count)
    {
        if (i >= count or i <= -count)
            throw Siren::invalid_index( String::tr(
                    "Invalid index \"%1\" for a container with size \"%2\".")
                        .arg(i).arg(count), CODELOC );
                        
        if (i >= 0)
            return i;
        else
            return count - i;
    }

    /** Wrap the index 'i' so that it indexes into a container
        of 'count' elements. If the index >= 0, then it counts
        forward from the start, while if it is < 0 then it counts
        back from the last item in the container
        
        \throw Siren::invalid_index
    */
    int SIREN_EXPORT wrap_index(int i, int count, CODELOC_ARGS)
    {
        if (i >= count or i <= -count)
            throw Siren::invalid_index( String::tr(
                    "Invalid index \"%1\" for a container with size \"%2\".")
                        .arg(i).arg(count), CODELOC_PASS_ARGS );
                        
        if (i >= 0)
            return i;
        else
            return count - i;
    }

    namespace detail
    {
        /** This code is called in 'abort' if an exception
            manages to escape! */
        static void error_abort()
        {
            Siren::fini();
        }

        #ifdef Q_OS_UNIX
            volatile sig_atomic_t error_in_progress = 0;
             
            /** Signal handler called on fatal errors to ensure
                that Siren cleans up correctly before program termination */
            void fatal_error_signal(int sig)
            {
                if (error_in_progress)
                    raise(sig);
         
                error_in_progress = 1;

                signal(sig, SIG_DFL);
                raise(sig);
            }
            
            void install_handler( int sig, struct sigaction *action )
            {
                sigemptyset( &(action->sa_mask) );
                action->sa_handler = fatal_error_signal;
                action->sa_flags = 0;
                    
                if (sigaction(sig, action, 0) == -1)
                {
                    sirenDebug() << "CANNOT SET THE HANDLER FOR SIGNAL" << sig;
                    std::exit(-1);
                }
            }
        #endif // Q_OS_UNIX

        /** This function installs signal handlers that will ensure that
            all shared memory is detached if this process exits uncleanly */
        void installSignalHandlers()
        {
            #ifdef Q_OS_UNIX
                //set a separate signal stack to be used
                // (so we can work around possible stack corruption)
                static stack_t signal_stack;

                if (signal_stack.ss_sp != 0)
                
                    //the signal handlers have been set already
                    return;

                if ((signal_stack.ss_sp = malloc(SIGSTKSZ)) == 0)
                {
                    sirenDebug() << "CANNOT ALLOCATE SIGNAL STACK SPACE!";
                    std::exit(-1);
                }
                    
                signal_stack.ss_size = SIGSTKSZ;
                signal_stack.ss_flags = 0;
                
                if (sigaltstack(&signal_stack,(stack_t *)0) < 0)
                {
                    sirenDebug() << "CANNOT SET THE SIGNAL STACK!";
                    std::exit(-1);
                }

                static struct sigaction segv_action;
                static struct sigaction fpe_action;
                static struct sigaction term_action;
                static struct sigaction bus_action;
                static struct sigaction int_action;
                static struct sigaction abrt_action;

                Siren::detail::install_handler(SIGSEGV, &segv_action);
                Siren::detail::install_handler(SIGFPE, &fpe_action);
                Siren::detail::install_handler(SIGTERM, &term_action);
                Siren::detail::install_handler(SIGBUS, &bus_action);
                Siren::detail::install_handler(SIGINT, &int_action);
                Siren::detail::install_handler(SIGABRT, &abrt_action);
            #endif

            std::set_terminate( error_abort );
        }
            
        void fini()
        {
            //send the end of for_ages
            for_ages::end();

            //now delete all of the static objects
            Siren::detail::Static::deleteAll();
        }
    }

    /** Call this once before using any Siren classes to ensure that 
        all Siren libraries are initialised */
    void SIREN_EXPORT init(int argc, const char **argv)
    {
        Siren::detail::installSignalHandlers();

        Siren::detail::Static::createAll();
        
        if (std::atexit(&(Siren::detail::fini)) != 0)
        {
            sirenDebug() << "CANNOT REGISTER EXIT HANDLER!!!";
            std::exit(-1);
        }
    }
    
    /** Call this once at the end of your program to ensure that 
        all of the Siren libraries are closed down properly */
    void SIREN_EXPORT fini()
    {
        Siren::detail::fini();
    }
    
    void SIREN_EXPORT loadLibrary(const char *library)
    {
        sirenDebug() << "Loading library" << library << "...";
        
        //find the library
        
        //load the library
        
        //resolve symbols - this will find all of the Siren object metadata
        
        //create all static objects - this initialises the library
        Siren::detail::Static::createAll();
    }


} // end of namespace Siren
