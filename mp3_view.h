#ifndef MP3_VIEW_H
#define MP3_VIEW_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */


typedef struct 
{
    /* Source  info */
    char *mp3_fname;
    FILE *fptr_mp3;

} MP3;


/*  function prototype */

/* Check operation type */
OperationType check_operation_type(char opt);

/* Read and validate args from argv */
Status read_and_validate_args(char *argv[], MP3 *info);

/* Get File pointers for i/p and o/p files */
Status open_files(MP3 *info);

/* view operation */
void view_operation(MP3 *info);

/* get size */
uint get_size(unsigned char *size_buffer);
#endif