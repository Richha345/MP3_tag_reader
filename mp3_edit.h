#ifndef EMP3_EDIT_H
#define EMP3_EDIT_H

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

    char *tag_type;
    char *new_data;

    char *mp3_temp_fname;
    FILE *fptr_mp3_temp;



} EMP3;


/*  function prototype */

/* Check operation type */
OperationType check_operation_type(char opt);

/* Read and validate args from argv */
Status read_and_validate_edit_args(char *argv[], EMP3 *einfo);

/* Get File pointers for i/p and o/p files */
Status open_edit_files(EMP3 *einfo);

/* view operation */
void do_edit_operation(EMP3 *einfo);

/* get size */
uint get_esize(unsigned char *size_buffer);

Status check_tag_to_edit(char opt, EMP3 *einfo);

void convert_lit_big(int size, char*new_size);


#endif