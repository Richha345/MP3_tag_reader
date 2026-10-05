#include <stdio.h>
#include <string.h>
#include "mp3_view.h"
#include "types.h"

/* Array containing the ID3 tag names */
static const char* tag[] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};


//=======================================================================================//

/*
 * Function: read_and_validate_args
 * Description:
 * 1. Checks whether the input file has .mp3 extension.
 * 2. Opens the MP3 file.
 * 3. Checks whether the file contains "ID3" signature.
 * 4. Moves the file pointer to position 10.
 */
Status read_and_validate_args(char *argv[], MP3 *info)
{
    /* Find the last '.' in the file name */
    char *dot = strrchr(argv[2],'.');

     /* Check whether the file extension is .mp3 */
    if(strcmp(dot,".mp3")!=0)
    {
        printf("ERORR : Source file extension should be .mp3\n");
        return e_failure;
    }

     /* Store the MP3 file name in the structure */
    info->mp3_fname=argv[2];

    /* Open the MP3 file */
    if(open_files(info) == e_failure)
    {
        printf("ERROR : unable to open source file\n");
        return e_failure;
    }

    /* Read the first 3 bytes of the file */
    char signature[3];

    fread(signature,3,1,info->fptr_mp3);

     /* Add null character to make it a string */
    signature[3]='\0';

    /* Check whether the file has ID3 signature */
    if(strcmp(signature,"ID3")!=0)
    {
        printf("ERORR : signature doesnot match\n\n");
        return e_failure;
    }

     /* Move file pointer to byte 10 where first frame starts */
    fseek(info->fptr_mp3,10,SEEK_SET);

    return e_success;
}

// ============================================================================================//

/*
 * Function: open_files
 * Description: Opens the MP3 file in read-binary mode.
 */
Status open_files(MP3 *info)
{
    /* Open MP3 file in binary read mode */
    info->fptr_mp3 = fopen(info->mp3_fname,"rb");

    /* Check whether file opening was successful */
    if(info->fptr_mp3 == NULL)
    {
        return e_failure;
    }

    return e_success;
}

// ============================================================================================//

/*
 * Function: view_operation
 * Description:
 * Reads and displays the ID3 tags and their information
 * from the MP3 file.
 */
void view_operation(MP3 *info)
{
    char tag_buffer[5];
    unsigned char size_buffer[4];
    uint size;
    
    /* Print table heading */
    printf("Sl.No\tTags  \t information\n");

    /* Read 6 ID3 frames */
    for(int i=0;i<6;i++)
    {
        /* Read 4-byte tag name */
        fread(tag_buffer,4,1,info->fptr_mp3);

        //printf("%s\n",tag_buffer);
        /* Read 4-byte frame size */
        fread(size_buffer,4,1,info->fptr_mp3);

         /* Convert the size into integer */
        size = get_size(size_buffer);
        //printf("%u\n",size);  

        /* Skip 3 bytes of frame flags */
        fseek(info->fptr_mp3,3,SEEK_CUR);
        
        /* Create buffer to store tag information */
        char info_buffer[size];

        /* Read tag information */
        fread(info_buffer,size-1,1,info->fptr_mp3);

        /* Add null character at the end */
        info_buffer[size-1] = '\0';

        /* Compare the tag with known ID3 tags */
        for(int j=0;j<6;j++)
        {
            if(strcmp(tag_buffer,tag[j]) == 0)
            {
                /* Display tag number, tag name and information */
                printf(" %d \t%s\t%s\n",i+1,tag_buffer,info_buffer);
            }
        }
    }
    
    return;
}

//=============================================================================================//

/*
 * Function: get_size
 * Description:
 * Converts the 4-byte size stored in the MP3 frame
 * into an integer value.
 */
uint get_size(unsigned char *size_buffer)
{
    /* Reverse the byte order */
    for(int i=0;i<2;i++)
    {
        unsigned char temp=size_buffer[i];
        size_buffer[i]=size_buffer[3-i];
        size_buffer[3-i]=temp;
    }
    uint size;

    /* Pointer to access integer byte by byte */
    unsigned char *ptr=(unsigned char *)&size;

    /* Copy bytes into integer */
    for(int i=0;i<4;i++)
    {
        ptr[i] = size_buffer[i];
    }        
    return size;
}
