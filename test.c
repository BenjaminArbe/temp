/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  29/01/2015 23:24:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Benjamin Arbe (BA), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int global;

int
main () {
    pid_t child_pid;
    int status;
    int local = 0;

    /*  now create a new process */
    child_pid = vfork();

    if (child_pid >= 0) { /* fork succeeded */
        if (child_pid == 0) { /*  fork() returns 0 for the child process */
            setuid (getuid());
            printf ("child process!\n");

            // increment the local and global variables
            local++;
            global++;

            printf ("child PID=%d, parent pid=%d\n", getpid(), getppid());
            printf ("\n child's local = %d, child's global = %d\n", local, global);

            _exit(0);
        } else { /*  parent process */
            wait (&status); /*  wait for child to exit, and store child's exit status' */
            printf ("Child exit code: %d\n", WEXITSTATUS(status));


            printf ("parent process!\n");
            printf ("parent PID=%d, child pid=%d\n", getpid(), child_pid);

            // the change in local and global variable in child process should not reflect here in parent process
            printf ("\n Parent's local=%d, parent's global=%d\n", local, global);

            char *cmd[] = { "cp", "test", "/usr/local/bin", (char*)0};
            execv ("/usr/bin/cp", cmd);
 
            printf ("Parent says bye!\n");
            exit (0); /* parent exits */
        }
    } else { /*  failure */
        perror ("fork");
        exit (-1);
    }
}

