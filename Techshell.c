#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>


#define INPUT_MAX 256
#define ARGS_MAX  64

//4. Forbidden functions include system()and popen()


    /// displays money sign plus what ever 
    // ~$ if they ` it goes to home and if
    // they make itso what ever direcotry your in it display is and what ever is in it 
    //prints wirking direcotry 
char extractor(char args[],char input[])
char ls(char input[], char args[],pid_t pid)


int main(void){
    // its making the 
char directory[PATH_MAX];
char input[67];
char args[4]

/*
execvp basically do the project lol

ls_args[0] = "ls";
ls_args[1] = "-l" ;
ls_args[2] = "ls";
ls_args[3] = NULL




//variable pointer to home 
char *root = getenv("ROOT") 
 pid_t pid = fork();

 */

char *home = getenv ("HOME");


//the while makes it contunie until a condition is meet
while(1){
if (getcwd(directory, sizeof(directory)) != NULL){
            printf("$ %s$ \n", directory);
        }
//if stirng they typed is ~ then  go to home on computer
// ok so i have to to stdin to read the leyobard store into array the fgets to read the string then 
// this reads my keyboard
fgets(input,67,stdin)
//werid cases like cd having extra stuff 
    exctractor();


if(strcmp(args[0],"cd"){
cd(); 
}
if (input[0] == "exit" ){
    exit();
}
if(input[0] == "ls"){
    exctractor();
}
if(input[0] == "pwd"){
    exctractor();

}
if(input[0] == "cat"){
extractor();
}

}
//if command equals  ~
//if  our enviorment variable == null 
//printf(" cant find////////enviroment variable that they put in ")


// args need to be a pointer varibale that goes into t 
char extractor(char input[],char args[],char pid){

      int i = 0;
      fork();
    if(pid < 0){
         perror("fork error ")
    }

if(pid == 0){
                //snipping  wings 
            args[i] = strtok(input, " \n\t");
    // while its not null i loop through the array and stripping its wings
 while(args[i] != NULL){
        i++;  
        // had to add a null becuase execvp is kinda stupid 
        args[i] = strtok(NULL, " \n\t");//snip snip 
        
    // goal is finished
        execvp(args[0], args);}

}


}

//if the secined argument is not anything there then u just run home 
char cd(char input[], *char args[],pid_t pid){
extractor()

    if(arg[1] == NULL){
    chdir(home);
}

if(arg[1] == "~")
chdir(home);

// i should maybe put zero here ngl i dont think null will cover a case 
if(arg[1 != 0]){
perror("Cd");
}

}



char ls(char input[], *char args[],pid_t pid){
    // break steing store the arguemnsts run into execvp profit 
    //not like javava i have to like
       if(pid < 0){
perror(" WOMP WOMP err")
    if(pid == 0){
exctractor();
        execvp(input,args)


    }
}
}


char pwd(char input[],*char args[], pid_t pid){
    if(pid < 0){
perror(" WOMP WOMP err")
    if(pid == 0){
exctractor();
        execvp(input,args)
     }
}
}

char cat (char input[],*char args[], pid_t pid){
    if(pid < 0){
perror(" WOMP WOMP err")
    if(pid == 0){
exctractor();
        execvp(input,args)
     }
}
}


char mkdir(char input[],*char args[], pid_t pid){
    if(pid < 0){
perror(" WOMP WOMP err")
    if(pid == 0){
exctractor();
        execvp(input,args)
     }
}
}
return 0;

}


