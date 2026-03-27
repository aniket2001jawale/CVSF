#include "cvsf.h"

///////////////////////////////////////////////////////////////////////////
//
//      Function name: ManPageDisplay()
//      Description:   it is used to display man page
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          14/01/2026
//
//
///////////////////////////////////////////////////////////////////////////

void ManPageDisplay( char Name[])
{
    if(strcmp("ls",Name)==0)
    {
        printf("About: it is used to list the name of the files\n");
        printf("Usage: ls\n");
        
    }
    else if(strcmp("man",Name)==0)
    {
        printf("About: it is used to display manual page\n");
        printf("Usage: man command_name\n");
        printf("command_name: it is the name of the commmand\n"); 
    }
    else if(strcmp("exit",Name)==0)
    {
        printf("About: it is used to terminate the shell\n");
        printf("Usage: exit\n");
    }
    else if(strcmp("clear",Name)==0)
    {
        printf("About: it is used to clear the shell\n");
        printf("Usage: clear\n");
    }
    else if(strcmp("read",Name)==0)
    {
        printf("About: Read a line from the standard input and split it into fields\n");
    }
    else if(strcmp("write",Name)==0)
    {
        printf("About: write to a file descriptor\n");
    }
    else if(strcmp("unlink",Name)==0)
    {
        printf("About:deletes a name from the filesystem\n");
        printf("If that name was the last link to a file and no processes have\n");    
        printf("the file open, the file is deleted and the space it was using\n");      
        printf("is made available for reuse\n");  
    }
    else if(strcmp("creat",Name)==0)
    {
        printf("About:create a new file or rewrite an existing one\n");
    }
    else
    {
        printf("no manual entry page for %s",Name );

    }

}

