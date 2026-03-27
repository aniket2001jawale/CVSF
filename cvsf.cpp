#include "cvsf.h"

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;
INode* head = NULL;


///////////////////////////////////////////////////////////////////////////
//
//      Function name: InitialiseUAREA
//      Description:   it is used to initialise to new member
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          13/01/2026
//
//
///////////////////////////////////////////////////////////////////////////


void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName,"Myexe");
    int i=0;
    for(i=0;i<MAXOPENFILES;i++)
    {
        uareaobj.UFDT[i]=NULL;
    }
    printf("Marvellous CVSF: UAREA gets initialised sucessfully\n");
    

}

///////////////////////////////////////////////////////////////////////////
//
//      Function name: InitialiseSuperBlock
//      Description:   it is used to initialise to super block member
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          13/01/2026
//
//
///////////////////////////////////////////////////////////////////////////


void InitialiseSuperBlock()
{
    superobj.TotalInodes=MAXINODE;
    superobj.FreeInodes=MAXINODE;
    
    printf("Marvellous CVSF: Super Block gets initialised sucessfully\n");

}

///////////////////////////////////////////////////////////////////////////
//
//      Function name: CreateDILB
//      Description:   it is used to create linkedlist of inodes
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          13/01/2026
//
//
///////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i=1;
    PINODE newn=NULL;
    PINODE temp=head;

    for(i=1;i<=MAXINODE;i++)
    {
        newn=(PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber=i;
        newn->FileSize=0;
        newn->ActualFileSize=0;
        newn->FileType=0;
        newn->ReferenceCount=0;
        newn->Permission=0;
        newn->Buffer=NULL;
        newn->next=NULL;

        if(temp==NULL)    //LL is empty
        {
            head=newn;
            temp=head;

        }
        else            //LL contains atleast 1 node
        {
            temp-> next =newn;
            temp=temp->next;

        }
    }
    printf("Marvellous CVFS:DILB created sucussfully\n");

} 
///////////////////////////////////////////////////////////////////////////
//
//      Function name: StartAuxillaryDataInitilisation
//      Description:   it is used to call all the such function which are used 
//                     to initialise auxillary data
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          13/01/2026
//
//
///////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation()
{
    strcpy(bootobj.Information,"Booting Process of Marvellous CVSF is done");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVSF: Auxillary data initialised sucessfully\n");
}

///////////////////////////////////////////////////////////////////////////
//
//      Function name: IsFileExist()
//      Description:   it is used to check whether file is already exist or not
//      Input:         it accept file name
//      Output:        it returns  the true or false     
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          16/01/2026
//
//
///////////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char * name       //file name
)
{
    PINODE temp=head;
    bool bflag=false;

    while (temp!=NULL)
    {
        if((strcmp(name,temp->FileName)==0 ) && (temp->FileType==REGULARFILE))
        {
            bflag=true;
            break;
        }
        temp=temp->next;
    }
    return bflag;
    
}

///////////////////////////////////////////////////////////////////////////
//
//      Function name: CreateFile()
//      Description:   it is used to create new regular file
//      Input:         it accept file name and permission
//      Output:        it returns the file descriptor     
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          16/01/2026
//
//
///////////////////////////////////////////////////////////////////////////

int CreateFile(
                    char *name,              //Name of the new file
                    int permission          //permission for that file
                )
{
    PINODE temp=head;
    int i=0;

    printf("total number of inodes remaining: %d\n",superobj.FreeInodes);

    //if name is missing
    if(name==NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    //if permission value is wrong
    //permission->1->read
    //permission->2->write
    //permission->3->read+write
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    //if the inodes are full
    if(superobj.FreeInodes==0)
    {
        return ERR_NO_INODES;
    }
    //if file is already present
    if(IsFileExist(name)==true)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //search empty inode
    while (temp!=NULL)
    {
        if(temp->FileType==0)
        {
            break;
        }
        temp=temp->next;
    }
    
    if(temp==NULL)
    {
        printf("there is no inode\n");
        return ERR_NO_INODES;
    }

    //search for empty UFDT entry
    //note: 0,1,2 are reserved
    for(i=3;i<MAXOPENFILES;i++)
    {  
        if(uareaobj.UFDT[i]==NULL)
        {
            break;
        }
    }
    //UFDT is full
    if(i==MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    //Allocate memory for file table
    uareaobj.UFDT[i]=(PFILETABLE)malloc(sizeof(FileTable));

    //Initialise file table
    uareaobj.UFDT[i]->ReadOffset=0;
    uareaobj.UFDT[i]->WriteOffset=0;
    uareaobj.UFDT[i]->Mode=permission;

    //connect file table with inode
    uareaobj.UFDT[i]->ptrinode=temp;

    //initialise element of inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize=MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize=0;
    uareaobj.UFDT[i]->ptrinode->FileType=REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount=1;
    uareaobj.UFDT[i]->ptrinode->Permission=permission;

    //Allocate memory for file data
    uareaobj.UFDT[i]->ptrinode->Buffer=(char*)malloc(sizeof(MAXFILESIZE));

    superobj.FreeInodes--;

    return i;            //file descriptor

}

///////////////////////////////////////////////////////////////////////////
//
//      Function name: LsFile()
//      Description:   it is used to list all file
//      Input:         nothing
//      Output:        nothing 
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          16/01/2026
//
//
///////////////////////////////////////////////////////////////////////////

//
void LsFiles()
{
    PINODE temp=head;

    printf("----------------------------------------------------------------\n");
    printf("--------------Marvellous CVSF files information-----------------\n");
    printf("----------------------------------------------------------------\n");

    while(temp!=NULL)
    {
        if(temp->FileType!=0)
        {
            printf("%d\t%s\t %d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        
        temp=temp->next;

    }
    printf("----------------------------------------------------------------\n");

}

///////////////////////////////////////////////////////////////////////////
//
//      Function name: UnlinkFile()
//      Description:   it is used to delete the files
//      Input:         filename
//      Output:        nothing 
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          22/01/2026
//
//
///////////////////////////////////////////////////////////////////////////

int UnlinkFile(
                    char *name
                )
{
    int i=0;
    if(name==NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name)==false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //travel the UFDT
    for(i=0;i<MAXOPENFILES;i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName,name)==0)
            {
                //deallocate the memory of buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer=NULL;

                //reset all value of inode
                //dont deallocate memory if inode
                uareaobj.UFDT[i]->ptrinode->FileSize==0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize=0;
                uareaobj.UFDT[i]->ptrinode->FileType=0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount=0;
                uareaobj.UFDT[i]->ptrinode->Permission=0;
                
                memset(uareaobj.UFDT[i]->ptrinode->FileName,'\0',sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                //Deallocate memory of  file table
                free(uareaobj.UFDT[i]);

                //set null to UFDT
                uareaobj.UFDT[i]=NULL;

                //increment free inodes count
                superobj.FreeInodes++;

                break;         //IMP

            }  //end of if

        }  //end of if
    }  //end of for

    return EXECUTE_SUCESS;

}   //end of function

///////////////////////////////////////////////////////////////////////////
//
//      Function name: WriteFile()
//      Description:   it is used write the data into the file
//      Input:         fileDescriptor
//                     Address of buffer which contains data
//                     size of data we want to write
//      Output:        number of byte sucessfully written 
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          22/01/2026
//
///////////////////////////////////////////////////////////////////////////

int WriteFile(
                int fd,
                char *data,
                int size
                )
{
    printf("file descriptor: %d\n",fd);
    printf("data that we want to write:%d\n",data);
    printf("number of bytes that we want to write: %d\n",size);

    //invalid fd
    if(fd < 0   || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    //fd point to NULL
    if(uareaobj.UFDT[fd]==NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //there is no permission to write
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    //insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    //write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset,data,size);

    //update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    //update the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;


    return size;
}

///////////////////////////////////////////////////////////////////////////
//
//      Function name: ReadFile()
//      Description:   it is used read the data into the file
//      Input:         fileDescriptor
//                     Address of empty buffer
//                     size of data we want to write
//      Output:        number of byte sucessfully written 
//      Author:        Aniket Dnyaneshwar Jawale
//      Date:          22/01/2026
//
///////////////////////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
              )
{
    //invalid fd
    if(fd < 0  || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }
    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }
    if(uareaobj.UFDT[fd]==NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //filter for the permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    //insufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    //read the data
    strncmp(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset,size);

    //update the readoffset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}




