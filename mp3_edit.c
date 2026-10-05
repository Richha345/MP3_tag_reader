#include <stdio.h>
#include <string.h>
#include "mp3_edit.h"
#include "types.h"

/* Read and validate the command-line arguments */
Status read_and_validate_edit_args(char *argv[], EMP3 *einfo)
{
    /* Check whether the given tag option is valid */
    if(check_tag_to_edit(argv[2][1], einfo) == e_failure)
    {
        e_failure;
    }

    /* Store the new data that has to be written into the tag */
    einfo->new_data = argv[3];

    /* Find the last dot in the file name */
    char *dot = strrchr(argv[4], '.');

    /* Check whether the input file has .mp3 extension */
    if(strcmp(dot, ".mp3") != 0)
    {
        printf("ERORR : Source file extension should be .mp3\n");
        return e_failure;
    }

    /* Store the MP3 file name in the structure */
    einfo->mp3_fname = argv[4];
    //printf("%s\n",einfo->mp3_fname);
    /* Set the temporary file name */
    einfo->mp3_temp_fname = "temp.mp3";

    /* Open the source MP3 file and temporary file */
    if(open_edit_files(einfo) == e_failure)
    {
        printf("ERROR : unable to open source file\n");
        return e_failure;
    }

    /* Read the first 3 bytes of the MP3 file */
    char signature[3];

    fread(signature, 3, 1, einfo->fptr_mp3);

    /* Add null character to make the data a string */
    signature[3] = '\0';

    /* Check whether the file contains the ID3 signature */
    if(strcmp(signature, "ID3") != 0)
    {
        printf("ERORR : signature doesnot match\n\n");
        return e_failure;
    }

    /* Move the file pointer back to the beginning */
    rewind(einfo->fptr_mp3);

    return e_success;
}

/* Open the source MP3 file and temporary output file */
Status open_edit_files(EMP3 *einfo)
{
    /* Open the source MP3 file in binary read mode */
    einfo->fptr_mp3 = fopen(einfo->mp3_fname, "rb");

    /* Check whether the source file was opened successfully */
    if(einfo->fptr_mp3 == NULL)
    {
        //printf("FAAA\n");
        return e_failure;
    }

    /* Open the temporary file in binary write mode */
    einfo->fptr_mp3_temp = fopen(einfo->mp3_temp_fname, "wb");

    /* Check whether the temporary file was opened successfully */
    if(einfo->fptr_mp3_temp == NULL)
    {
        //printf("AAAA\n");
        return e_failure;
    }

    return e_success;
}

/* Perform the MP3 tag editing operation */
void do_edit_operation(EMP3 *einfo)
{
    /* Declare buffers for ID3 header, tag, size and new size */
    char tag_buffer[5];
    char header_buffer[10];
    unsigned char size_buffer[4];
    uint size;
    unsigned char new_size[4];

    /* Read and copy the 10-byte ID3 header */
    fread(header_buffer, 10, 1, einfo->fptr_mp3);
    fwrite(header_buffer, 10, 1, einfo->fptr_mp3_temp);

    /* Read and process the first 6 ID3 frames */
    for(int i = 0; i < 6; i++)
    {
        /* Read the 4-byte frame ID */
        fread(tag_buffer, 4, 1, einfo->fptr_mp3);

        /* Copy the frame ID to the temporary file */
        fwrite(tag_buffer, 4, 1, einfo->fptr_mp3_temp);

        /* Add null character to make it a string */
        tag_buffer[4] = '\0';

        /* Read the 4-byte frame size */
        fread(size_buffer, 4, 1, einfo->fptr_mp3);

        /* Convert the frame size into integer */
        size = get_esize(size_buffer);

        /* Convert the size back to big-endian format */
        convert_lit_big(size, size_buffer);

        /* Check whether the current frame is the tag to be edited */
        if(strcmp(tag_buffer, einfo->tag_type) == 0)
        {
            /* Calculate and convert the new data size */
            convert_lit_big((strlen(einfo->new_data) + 1), new_size);

            /* Write the new frame size */
            fwrite(new_size, 4, 1, einfo->fptr_mp3_temp);

            /* Read and copy the frame flags */
            char flag_buffer[3];
            fread(flag_buffer, 3, 1, einfo->fptr_mp3);
            fwrite(flag_buffer, 3, 1, einfo->fptr_mp3_temp);

            /* Write the new data into the frame */
            fwrite(einfo->new_data, strlen(einfo->new_data), 1,
                   einfo->fptr_mp3_temp);

            /* Skip the remaining old frame data */
            fseek(einfo->fptr_mp3, size - 1, SEEK_CUR);

            /* Stop searching after editing the required tag */
            break;
        }

        /* Copy the original frame size */
        fwrite(size_buffer, 4, 1, einfo->fptr_mp3_temp);

        /* Read and copy the frame flags */
        char flag_buffer[3];
        fread(flag_buffer, 3, 1, einfo->fptr_mp3);
        fwrite(flag_buffer, 3, 1, einfo->fptr_mp3_temp);

        /* Read and copy the frame data */
        char info_buffer[size - 1];
        fread(info_buffer, size - 1, 1, einfo->fptr_mp3);
        fwrite(info_buffer, size - 1, 1, einfo->fptr_mp3_temp);
    }

    /* Copy the remaining MP3 data into the temporary file */
    char data;
    while(fread(&data, 1, 1, einfo->fptr_mp3) == 1)
    {
        fwrite(&data, 1, 1, einfo->fptr_mp3_temp);
    }

    /* Display successful editing message */
    printf("edited.\n");

    return;
}

/* Convert the 4-byte frame size into an integer */
uint get_esize(unsigned char *size_buffer)
{
    /* Reverse the byte order of the size bytes */
    for(int i = 0; i < 2; i++)
    {
        unsigned char temp = size_buffer[i];
        size_buffer[i] = size_buffer[3 - i];
        size_buffer[3 - i] = temp;
    }

    uint size;

    /* Get the address of the integer byte by byte */
    unsigned char *ptr = (unsigned char *)&size;

    /* Copy the bytes into the integer */
    for(int i = 0; i < 4; i++)
    {
        ptr[i] = size_buffer[i];
    }

    return size;
}

/* Convert an integer into 4-byte big-endian format */
void convert_lit_big(int size, char *new_size)
{
    /* Access the integer byte by byte */
    unsigned char *ptr = (unsigned char *)&size;

    /* Store the bytes in reverse order */
    for(int i = 0; i < 4; i++)
    {
        new_size[i] = ptr[3 - i];
    }

    return;
}

/* Check the tag option and assign the corresponding ID3 frame */
Status check_tag_to_edit(char opt, EMP3 *einfo)
{
    switch(opt)
    {
        /* Title tag */
        case 't':
            einfo->tag_type = "TIT2";
            break;

        /* Artist tag */
        case 'a':
            einfo->tag_type = "TPE1";
            break;

        /* Album tag */
        case 'A':
            einfo->tag_type = "TALB";
            break;

        /* Year tag */
        case 'y':
            einfo->tag_type = "TYER";
            break;

        /* Genre tag */
        case 'm':
            einfo->tag_type = "TCON";
            break;

        /* Comment tag */
        case 'c':
            einfo->tag_type = "COMM";
            break;

        /* Invalid tag option */
        default:
            printf("Invalid Input\n");
            return e_failure;
    }

    return e_success;
}