
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{

    pid_t pid = fork();

    if (pid == -1)
    {
        cerr << "Could not fork!\n";
        return 0;
    }
    else if (pid == 0)
    {
        //child
        cout << "CHILD: pid group = " << getpgrp() << endl;
        
        setpgrp();

        cout << "CHILD: new pid group = " << getpgrp() << endl;

        int status = execlp("sh" ,"sh", "testforkexec.sh", (char*)0);

        cout << "CHILD: Error status = " << status << endl; 

        exit(0);
    }
    else
    {
        //parent
        cout << "PARENT: pid group = " << getpgrp() << endl;
        cout << "PARENT: child pid = " << pid << endl;

        int child_exit_status;
 
        int status = waitpid(pid, &child_exit_status, 0);

        if (status == -1)
        {
            cerr << "waitpid exited with status -1: " << strerror(errno) << endl;
        }
        else if (status != pid)
        {
            cerr << "waitpid did not return the correct child pid (" << status << " vs. "
                 << pid << ")\n";
        }

        if ( WIFEXITED(child_exit_status) == 0 )
        {
            cerr << "PARENT: child did not exit normally!\n"; 
        }
        else
        {
            cout << "PARENT: child exited with status " << WEXITSTATUS(child_exit_status) << endl;

            if (WIFSIGNALED(child_exit_status))
            {
                cout << "PARENT: child exited because of an uncaught signal: "
                     << WTERMSIG(child_exit_status) << endl;
            }

            if (WIFSTOPPED(child_exit_status))
            {
                cout << "PARENT: child has been stopped: "
                     << WSTOPSIG(child_exit_status) << endl;
            }
        }

        //kill the child's process group
        killpg(pid, SIGKILL);
    }

    return 0;
}

