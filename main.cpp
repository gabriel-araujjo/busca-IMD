// trata os comandos do usuário

#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char ** argv) {

    if(argc > 1){//the user should input at least one argument, otherwise, the program will not peform any action

        //search base arguments
        //<-i <file_directory>, -r <file_name>, -li, -la, -lt>
        if(strcmp(argv[1], "-i") == 0){
            //insert a new file on the search base
            for(int i = 2; i < argc; i++){
                //insert one or more files to the search base
            }//if it has zero files to insert, print a message explaining that the "-i" argument need at least
            //one file directory
        }else if(strcmp(argv[1], "-r") == 0){
            //remove a file from the search base
            for(int i = 2; i < argc; i++){
                //remove one or more files to the search base
            }//if it has zero files to insert, print a message explaining that the "-r" argument need at least
            //one file name
        }else if(strcmp(argv[1], "-li") == 0){
            //list the files in the search base with NATURAl order
        }else if(strcmp(argv[1], "-la") == 0){
            //list the files in the search base with ALPHABETICAL order
        }else if(strcmp(argv[1], "-lt") == 0){
            //list the files in the search base with MATCH_COUNT order
        }

        //search options
        // <-bAND, -bOR> <-pA, -pC, -pI> <-tT, -tF> <key_words[]>
        //if it hasn't any of the print options or time options
        //-pI and -tF are the default arguments
        if(strcmp(argv[1], "-bAND") == 0){
            //search using the bAND option
            //---------------------------- pensar numa melhor maneira de filtrar esses próximos argumentos
            //function to return the impression options (passing a argv[2] and returning the const Result::SortOrder)
            //function to return the time option (passing a argv[3] and returning the const bool showTime)

            //searchAnd(const Result::SortOrder order, const bool showTime, char ** words)
        }else if(strcmp(argv[1], "-bOR") == 0){
            //search using the bOR option
            //---------------------------- pensar numa melhor maneira de filtrar esses próximos argumentos
            //function to return the impression options (passing a argv[2] and returning the const Result::SortOrder)
            //function to return the time option (passing a argv[3] and returning the const bool showTime)

            //searchOr(const Result::SortOrder order, const bool showTime, char ** words)
        }else{
            //print an message telling that the user inputed a wrong argument
            //list all the possible arguments
        }

        //extra: add an -h or -help option to list all the arguments and explain them
        //extra2: add a search function that uses the AND-OR method
    }else{
        //print an message telling that the user need to input at least one argument
        //list all the possible arguments
    }

    return 0;
}