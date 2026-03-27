#ifndef CVSF_H
#define CVSF_H
///////////////////////////////////////////////////////////////////////////
//
//      Header File Inclusion
//
///////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
///////////////////////////////////////////////////////////////////////////
//
//      User Defined Macros
//
///////////////////////////////////////////////////////////////////////////
//Maximum file size that we allow in the project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 1
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define REGULARFILE 1 
#define SPECIALFILE 2 

#define EXECUTE_SUCESS 0


///////////////////////////////////////////////////////////////////////////
//
//      User Defined Macros for error handling
//
///////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8


///////////////////////////////////////////////////////////////////////////
// 
//      User Defined Structures
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//        Structure Name: Bootblock
//        Description:    holds the information to boot the os
//
///////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

///////////////////////////////////////////////////////////////////////////
//
//      Structure Name: superblock
//      Description:    holds the information about the file system
//
///////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

///////////////////////////////////////////////////////////////////////////
//
//      Structure name: Inode
//      Description: holds the information about the file
//
///////////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct INode           //self referencial structure
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct INode *next;
};
typedef struct INode INODE;
typedef struct INode *PINODE;
typedef struct INode **PPINODE;

///////////////////////////////////////////////////////////////////////////
//
//      Structure name: FileTable
//      Description:    holds the information about the opened file
//
///////////////////////////////////////////////////////////////////////////
struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;

};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

///////////////////////////////////////////////////////////////////////////
//
//      Structure name: UAREA
//      Description:    holds the information about the about the process
//
///////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
    
};

///////////////////////////////////////////////////////////////////////////
//
//      Global variables or objects used in the project
//     
///////////////////////////////////////////////////////////////////////////

extern BootBlock bootobj;
extern SuperBlock superobj;
extern UAREA uareaobj;
extern INode* head;

///////////////////////////////////////////////////////////////////////////
//
//      Function declarations used in the project
//     
///////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation();
void LsFiles();
void DisplayHelp();
void ManPageDisplay(char* command);
int  UnlinkFile(char* filename);
int  WriteFile(int fd, char* buffer, int size);
int  CreateFile(char* filename, int permission);
int  ReadFile(int fd, char* buffer, int size);

#endif