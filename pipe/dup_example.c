#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
    int pfds[2];
    int status;

    if ( pipe(pfds) == 0 ) {

        if ( fork() == 0 ) {

            close(1);
            dup2( pfds[1], 1 );
            close( pfds[0] );
            execlp( "ls", "ls", "-l", NULL );
            wait(&status);
        } else {

            close(0);
            dup2( pfds[0], 0 );
            close( pfds[1] );
            execlp( "wc", "wc", "-l", NULL );

        }

        return 0;
    }
}
