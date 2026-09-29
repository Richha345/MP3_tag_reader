#include <stdio.h>
#include "types.h"
#include "mp3.h"

int main(int argc, char *argv[])
{
   MP3 info;

   if( argc > 2 && check_operation_type(argv[1][1]) == e_view)
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
   else if((check_operation_type(argv[1][1]) == e_help))
   {
    printf("------------------------------- help menu -------------------------------\n");
        printf("USAGE : \n");
        printf("To view please pass like: ./a.out -v filename.mp3\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
        printf("-----------------------------------------------------------------------------\n");
   }
   else
   {
        printf("Error : Invalid input\n");
        printf("USAGE : \n");
        printf("To view please pass like: ./a.out -v filename.mp3\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
   }
}