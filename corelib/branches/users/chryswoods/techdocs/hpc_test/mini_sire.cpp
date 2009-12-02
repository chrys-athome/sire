
#include <mpi.h>

#include <iostream>
#include <fstream>
#include <string>

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

/** Return whether or not the process with exit status
    located at 'exit_status' is still running */
bool processRunning(int exit_status)
{
    if (WIFEXITED(exit_status) == 0)
    {
        if (WIFSIGNALED(exit_status) != 0 or
            WIFSTOPPED(exit_status) != 0)
        {
            //the job was killed or stopped by a signal
            return false;
        }
        else
            return true;
    }
    
    //the job has exited normally
    return false;
}

/** Clean up the child process whose exit status is located
    at 'child_exit_status', and that has process ID 'pid'.
    This returns whether or not the child process exited 
    with an error */
bool cleanJob(int child_exit_status, int pid)
{
    bool is_error = false;

    if (WEXITSTATUS(child_exit_status) != 0)
    {
        //something went wrong with the job
        is_error = true;
    }

    if (WIFSIGNALED(child_exit_status))
    {
        if (WTERMSIG(child_exit_status) == SIGKILL or
            WTERMSIG(child_exit_status) == SIGHUP)
        {
            cerr << "Child process was killed!\n";
            is_error = true;
        }
        else
        {
            is_error = true;
        }
    }
    else if (WIFSTOPPED(child_exit_status))
    {
        if (WSTOPSIG(child_exit_status) == SIGKILL or
            WSTOPSIG(child_exit_status) == SIGHUP)
        {
            cerr << "Child process was killed!\n";
            is_error = true;
        }
        else
        {
            is_error = true;
        }
    }
    
    //make sure that all of the child processes have finished
    //by killing the child's process group
    killpg(pid, SIGKILL);

    return is_error;
}

int main(int argc, char **argv)
{
    //start multi-threaded MPI
    MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);

    //now make sure that we are using a version of MPI that
    //has multi-thread support
    int thread_level = MPI::Query_thread();
    
    if (thread_level != MPI_THREAD_MULTIPLE)
    {
        //we can't run without thread support, as we use multiple
        //communicators running in multiple threads
        if ( MPI::COMM_WORLD.Get_rank() == 0 )
        {
            cerr << "Sire needs to use an MPI that supports threads.\n"
                    "This means that MPI must have been started using "
                    "MPI::Init_thread(MPI_THREAD_MULTIPLE), and that the MPI\n"
                    "library supports multithreaded MPI.\n\n"
                    "Check if your MPI library has been compiled with thread support.\n";
        }
        
        //stop MPI and exit
        MPI::COMM_WORLD.Barrier();
        MPI::Finalize();
        
        //kill the program
        return -1;
    }

    cout << "MPI node " << MPI::COMM_WORLD.Get_rank() << " running child process...\n";

    //have each process write a "$HOME/mini_molpro > /tmp/mini_molpro.log" 
    //shell script in /tmp
    const char *sh_file = "/tmp/run_minimolpro.sh";
    {
        ofstream f( sh_file, std::ios::out );

        f << "#!/bin/sh\n"
             "$HOME/mini_molpro > /tmp/mini_molpro.log\n";
    }

    //now run the shell script
    pid_t pid = fork();
    
    if (pid == -1)
    {
        cerr << "It is not possible to use fork on this platform.\n";
        ::MPI::Finalize();
        return -1;
    }
    else if (pid == 0)
    {
        //this is the child process!

        //move this child (and all of its children) into a new 
        //process ID group - the new group will have the same
        //process ID as this child (this allows all child and
        //grandchild processes to be killed using killpg
        setpgrp();
        
        char *cmd = "/bin/sh";
        char *arg = "/tmp/run_minimolpro.sh";

        char* char_args[ 3 ];
        
        char_args[0] = cmd;
        char_args[1] = arg;
        char_args[2] = '\0';     

        int status = execvp( char_args[0], char_args );
        
        if (status != 0)
        {
            cerr << "Problem starting the command" << cmd
                     << "with arguments" << arg
                     << ". Status ==" << status << ", error =" 
                     << strerror(errno);
                     
            exit(-1);
        }
        
        exit(0);
    }
    else
    {
        //this is the parent process
        sleep(1);        

        //wait for the child to finish
        bool child_finished = false;
        bool is_error = false;

        int time_waiting = 0;

        while ( not child_finished )
        {
            int child_exit_status;

            cout << "Node " << MPI::COMM_WORLD.Get_rank() << " waiting for " << pid << "...\n";
            int out_pid = waitpid(pid, &child_exit_status, WNOHANG);

            if (out_pid == -1)
            {
                cerr << "waitpid exited with status -1\n";
                is_error = true;
                child_finished = true;
            }
            else if ( out_pid == 0 or processRunning(child_exit_status) )
            {
                //the process is still running - wait some more
                sleep(1);
                time_waiting += 1;
            }
            else if (out_pid != pid)
            {
                cerr << "waitpid exited with the wrong pid: " << out_pid
                     << " vs. " << pid << "  " << strerror(errno) << endl;
                is_error = true;
                child_finished = true; 
            }
            else
            {
                //the job has finished - clean it up
                is_error = cleanJob(child_exit_status, pid);
                child_finished = true;
            }

            if (time_waiting > 10 and not child_finished)
            {
                //kill any jobs that have taken more than 10 seconds to run
                killpg(pid, SIGKILL);
            }
        }

        if (is_error)
            cerr << MPI::COMM_WORLD.Get_rank() << ": Child job finished in error!\n";
    }

    //the child job has now finished
    cout << "MPI node " << MPI::COMM_WORLD.Get_rank() << " child process has finished.\n";

    //read the output of the file
    {
        string line;
        ifstream f("/tmp/mini_molpro.log", std::ios::in);

        while (not f.eof())
        {
            getline(f, line);
            cout << "Node " << MPI::COMM_WORLD.Get_rank() << " output == " 
                 << line << endl;
        }
    }

    MPI::COMM_WORLD.Barrier();

    MPI::Finalize();

    return 0;
}
