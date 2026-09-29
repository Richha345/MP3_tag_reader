#include <stdio.h>
#include <string.h>
#include "mp3.h"
#include "types.h"

static const char* tag[] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};


//=======================================================================================//

OperationType check_operation_type(char opt)
{
    if(opt == 'v')
    {
        return e_view;
    }
    else if(opt == 'h')
    {
        return e_help;
    }
    return e_unsupported;
}

//=======================================================================================//

Status read_and_validate_args(char *argv[], MP3 *info)
{
    char *dot = strrchr(argv[2],'.');
    if(strcmp(dot,".mp3")!=0)
    {
        printf("ERORR : Source file extension should be .mp3\n");
        return e_failure;
    }
    info->mp3_fname=argv[2];
    if(open_files(info) == e_failure)
    {
        printf("ERROR : unable to open source file\n");
        return e_failure;
    }
    char signature[3];
    fread(signature,3,1,info->fptr_mp3);
    signature[3]='\0';
    if(strcmp(signature,"ID3")!=0)
    {
        printf("ERORR : signature doesnot match\n\n");
        return e_failure;
    }
    fseek(info->fptr_mp3,10,SEEK_SET);
    return e_success;
}

// ============================================================================================//

Status open_files(MP3 *info)
{
    info->fptr_mp3 = fopen(info->mp3_fname,"rb");
    if(info->fptr_mp3 == NULL)
    {
        return e_failure;
    }
    return e_success;
}

// ============================================================================================//

void view_operation(MP3 *info)
{
    char tag_buffer[5];
    unsigned char size_buffer[4];
    uint size;

    printf("Sl.No\tTags  \t information\n");
    for(int i=0;i<6;i++)
    {
        fread(tag_buffer,4,1,info->fptr_mp3);

        //printf("%s\n",tag_buffer);

        fread(size_buffer,4,1,info->fptr_mp3);
        size = get_size(size_buffer);
        //printf("%u\n",size);  

        fseek(info->fptr_mp3,3,SEEK_CUR);

        char info_buffer[size];
        fread(info_buffer,size-1,1,info->fptr_mp3);
        info_buffer[size-1] = '\0';

        for(int j=0;j<6;j++)
        {
            if(strcmp(tag_buffer,tag[j]) == 0)
            {
                printf(" %d \t%s\t%s\n",i+1,tag_buffer,info_buffer);
            }
        }
    }
    
    return;
}

//=============================================================================================//

uint get_size(unsigned char *size_buffer)
{
    for(int i=0;i<2;i++)
    {
        unsigned char temp=size_buffer[i];
        size_buffer[i]=size_buffer[3-i];
        size_buffer[3-i]=temp;
    }
    uint size;
    unsigned char *ptr=(unsigned char *)&size;
    for(int i=0;i<4;i++)
    {
        ptr[i] = size_buffer[i];
    }        
    return size;
}
