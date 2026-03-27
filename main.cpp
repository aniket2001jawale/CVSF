#include "cvsf.h"

int main()
{ 
    char str[80]={'\0'};
    char Command[5][20]={{'\0'}};
    char InputBuffer[MAXFILESIZE]={'\0'};
    int iCount=0;  
    int iRet=0;

    char *EmptyBuffer=NULL;

    StartAuxillaryDataInitilisation();

    printf("----------------------------------------------------------------\n");
    printf("--------------Marvellous CVSF started sucessfully---------------\n");
    printf("----------------------------------------------------------------\n");
    
    //infinite listening shell
    while (1)
    {
        fflush(stdin);

        strcpy(str,"");
        printf("\nMarvellous CVSF: > ");
        fgets(str,sizeof(str),stdin);

        iCount=sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3],Command[4]);

        fflush(stdin);

        if(iCount==1)
        {
            //Marvellous CVSF: > exit
            if(strcmp("exit",Command[0])==0)
            {
                printf("thank you for using Marvellous CVSF:\n");
                printf("deallocating all the resources\n");

                break;
            }
            //Marvellous CVSF: > ls
            else if(strcmp("ls",Command[0])==0)
            {
                LsFiles();
            }
            //Marvellous CVSF: > help
            else if (strcmp("help",Command[0])==0)
            {
                DisplayHelp();
            }
            //Marvellous CVSF: >clear
            else if(strcmp("clear",Command[0])==0)
            {
                #ifdef _WIN32
                    system("cls"); 
                #else
                    system("clear");
                #endif
            }
            else
            {
                printf("commmand not found\n");
                printf("please refer help options to get more information\n");
            }

        } //end of else if 1
        else if(iCount==2)
        {
            //Marvellous CVSF: > man ls
            if(strcmp("man",Command[0])==0)
            {
                ManPageDisplay(Command[1]);
            }
            //Marvellous CVSF: > unlink Demo.txt
            else if(strcmp("unlink",Command[0])==0)
            {
                iRet= UnlinkFile(Command[1]);
                
                if(iRet==ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }
                if(iRet==ERR_FILE_NOT_EXIST)
                {
                    printf("Error : unable to delete as there is nosuch file\n");
                }
                if(iRet==EXECUTE_SUCESS)
                {
                    printf("file gets sucessfully deleted\n");
                }
            } 
            //Marvellous CVSF :> write 2
            else if(strcmp("write",Command[0])==0)
            {
                printf("Enter the data you want to write:%d\n");
                fgets(InputBuffer,MAXFILESIZE,stdin);
                iRet =WriteFile(atoi(Command[1]),InputBuffer,strlen(InputBuffer)-1);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error: invalid parameter\n");
                }
                else if (iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("ERROr: there is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("error: unable to write as there is not permission\n");
                }
                else if(iRet ==ERR_INSUFFICIENT_DATA)
                {
                    printf("Error: unable to write as there is not space\n");
                }
                else
                {
                    printf("%d bytes gets sucessfully written\n",iRet);
                }

            } 
            else
            {
                printf("there is no such command\n");
            } 

        }  //end of else if 2
        else if (iCount==3)
        {
            //Marvellous CVSF: > create Ganesh.txt 3
           if(strcmp("creat",Command[0])==0)
            {
                iRet=CreateFile( Command[1],atoi(Command[2]));

                if(iRet==ERR_INVALID_PARAMETER)
                {
                    printf("Error: Unable to create the file as parameter are invalid\n");
                    printf("Please refer man page\n");
                }
                if(iRet==ERR_NO_INODES)
                {
                    printf("Error: Unable to create file as there is no inode\n");
                }
                if(iRet==ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error:Unable to create file bacause file is already present\n");
                }
                if(iRet==ERR_MAX_FILES_OPEN)
                {
                    printf("Error:Unable to create the file\n");
                    printf("Max opened files limit reached\n");
                }

                printf("files gets sucessfully created with fd %d\n",iRet);
                   

            }
            // MArvellous CVSF : > read 3 10
            else if(strcmp("read",Command[0])==0)
            {
                EmptyBuffer = (char*)malloc(sizeof(atoi(Command[2])));

                iRet = ReadFile(atoi(Command[1]),EmptyBuffer,atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("error: invalid Parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("ERRor: file not exit\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("error: permission denied\n");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("error: insufficient data\n");
                }
                else 
                {
                    printf("read operation is sucessfull\n");
                    printf("data from file is : %s\n",EmptyBuffer);

                    free(EmptyBuffer);
                }
            }

            else
            {
                printf("there is not such command\n");

            }

        } //end of else if 3
        else if(iCount==4)
        {

        }   //end of else if 4 
        else
        {
            printf("commmand not found\n");
            printf("please refer help options to get more information\n");

        }//end of else 


    } //end of while
    
    



    return 0;
}//end of main