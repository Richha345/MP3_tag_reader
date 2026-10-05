#include <stdio.h>
#include "types.h"
#include "mp3_view.h"
#include "mp3_edit.h"


int main(int argc, char *argv[])
{
   MP3 info;
   if(argc <2)
   {
        printf("Error : Invalid input\n");
        printf("USAGE : \n");
        printf("To view please pass like: ./a.out -v filename.mp3\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
   }

   if( argc == 3 && check_operation_type(argv[1][1]) == e_view)
   {
        if(read_and_validate_args(argv, &info) == e_failure)
        {
            printf("ERORR : invalid input\n");
            printf("USAGE : \n");
            printf("To view please pass like: ./a.out -v filename.mp3\n");
            return 0;
        }
        view_operation(&info);
   }
   else if( argc == 5 && check_operation_type(argv[1][1]) == e_edit)
   {
          EMP3 einfo;
        if(read_and_validate_edit_args(argv, &einfo) == e_failure)
        {
            printf("ERORR : invalid input\n");
            printf("USAGE : \n");
            printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
            return 0;
        }
        do_edit_operation(&einfo);
   }
   else if((check_operation_type(argv[1][1]) == e_help))
   {
     /*                      add new help menu                        */
        printf("\n1.  -v  -->   to view mp3 file contents\n");
        printf("2.  -e  -->   to edit mp3 file contents\n");
        printf("\t2.1  -t  -->  to edit song title\n");
        printf("\t2.2  -a  -->  to edit artist name\n");
        printf("\t2.3  -A  -->  to edit album name\n");
        printf("\t2.4  -y  -->  to edit year\n");
        printf("\t2.5  -m  -->  to edit Content type / Genre\n");
        printf("\t2.6  -c  -->  to edit Comment\n\n");
   }
   else
   {
        printf("Error : Invalid input\n");
        printf("USAGE : \n");
        printf("To view please pass like: ./a.out -v filename.mp3\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
   }
   
}

//=======================================================================================//

/* 
 * Function: check_operation_type
 * Description: Checks the command-line option entered by the user.
 * 'v' -> View operation
 * 'h' -> Help operation
 * Any other option -> Unsupported operation
 */
OperationType check_operation_type(char opt)
{
    if(opt == 'v')
    {
        return e_view;
    }
    else if(opt == 'e')
    {
          return e_edit;
    }
    else if(opt == 'h')
    {
        return e_help;
    }
    return e_unsupported;
}