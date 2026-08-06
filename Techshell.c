#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
enum CMD {

  THEN,      //  &&
  ELSE,      //  ||
  OUTPUT,    //  >
  INPUT,     //  <
  APPEND,    //  >>
  ERRAPPEND, //  2>,
  PIPE,      //  |
  EXIT,
  NOTBUILTIN
};

typedef struct ShellCommand{
  enum CMD cmd;
  struct ShellCommand *left_cmd;
  struct ShellCommand *right_cmd;
  char **args;
} ShellCommand;


char** SpiltBySpace(char* input) {
  char **words_by_space = calloc(20, sizeof(char*));

  for(int i=0;i<=10;i++) {
    words_by_space[i] = (char*) calloc(1, 50);
  }

  int wbsi = 0;
  char cur_word[50] = {'\0'};
  int cur_word_index = 0;
  int length = strlen(input);

  for (int i = 0; i < length; i ++) {
    char c = input[i];
    if ((c == ' ') || (c == '\n')) {
      //check new word
      cur_word[cur_word_index] += '\0';

      //push cur word to arr of words
      strcpy(words_by_space[wbsi], cur_word);
      wbsi += 1;

      //reset cur word
      memset(cur_word, '\0', sizeof(cur_word));
      cur_word_index = 0;

    } else { //keep adding
      cur_word[cur_word_index] = c;
      cur_word_index += 1;
    }
  }

  //Null is added so we know when the words arr ends
  words_by_space[wbsi] = NULL;

  return words_by_space;
}

void print_string_arr(char** input) {
  int ti = 0;
  while(input[ti] != NULL) {
    printf("%s ", input[ti]);
    ti += 1;
  }
  printf("\n");
}


ShellCommand* SpiltIntoCmd(char** input) {
  //Split up to allow for presidance (Handle < before &&)
  const char *special_ops[][5] = { {"<", ">", ">>", "2>", NULL}, {"|", NULL}, {"&&", "||", NULL} };

  ShellCommand *CommandInfo = calloc(1, sizeof(ShellCommand));

  //Find the size of the input/words by space
  int size_of_wbs = 0;
  while (input[size_of_wbs] != NULL) {
    size_of_wbs += 1;
  }

  //Loop over all of the speical operators 
  for (int i = 0; i < 3; i++ ) {
    int ii = 0;
    while (special_ops[i][ii] != NULL) { //for (int ii = 0; ii <= 4; ii++) {
     
      //Loop over input 
      for (int j = 0; j < size_of_wbs; j++) {

        //Check for match
        if (strcmp(special_ops[i][ii], input[j]) == 0) {
          // If we find a match we split the two sides of the match and parse both of them, before assign to a parent command

          //Check what the match was then assign the matching enum value
          if ( strcmp(input[j], "&&") == 0 ) {
              CommandInfo->cmd = THEN;
          }
          //TODO Add remainging enum types

          //Calc size of each half
          int lhs = j + 1;
          int rhs = size_of_wbs - j;

          char **left_half = calloc( lhs, sizeof(char*));
          char **right_half = calloc( rhs, sizeof(char*));

          //Preassign size for each word in each half
          for(int i=0;i<=lhs;i++) {
            left_half[i] = (char*) calloc(1, 50);
          }
          for(int i=0;i<=rhs;i++) {
            right_half[i] = (char*) calloc(1, 50);
          }

          //Loop over all words, and split input into 2 array from before and after the match
          for( int si = 0; si < size_of_wbs; si++ ) {
            if (si < j) {
              strcpy(left_half[si], input[si]);
            } else if (si > j) {
              strcpy(right_half[si-j-1], input[si]);
            }
          }
       
          //Add NULL to end so we can detect the end of the array later on
          left_half[lhs - 1] = NULL;
          right_half[rhs - 1] = NULL;

          ShellCommand *CommandLeft = calloc(1, sizeof(ShellCommand));
          ShellCommand *CommandRight = calloc(1, sizeof(ShellCommand));

          //Recursive call to SpiltIntoCmd to keep going untill it returns only one command
          CommandLeft = SpiltIntoCmd( left_half );
          CommandRight = SpiltIntoCmd( right_half );

          CommandInfo->left_cmd = CommandLeft;
          CommandInfo->right_cmd = CommandRight;

          return CommandInfo;
        }
      }

      ii += 1;
    }
  }

  //return only one (NO More/Any speicals ops)
 
  //TODO Add other built in commands
  if (strcmp(input[0], "exit") == 0) {
    CommandInfo->cmd = EXIT;
  } else {
    CommandInfo->cmd = NOTBUILTIN;
  }

  CommandInfo->args = input;

  return CommandInfo;
}

void fork_and_run(ShellCommand *command) {
  char **args = command->args;
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return;
  }
  if (pid == 0) {
    execvp(args[0], args);
    fprintf(stderr, "Error %d (%s)\n", errno, strerror(errno));
    exit(1);
  }
  wait(NULL);
}

char* CommandPrompt() {
  char *raw_input = calloc(100, sizeof(char));
  //Print current dir, and username
  printf("$ ");
fgets(raw_input, 100, stdin);
  return raw_input;
}


ShellCommand* ParseCommandLine(char* input) {
  ShellCommand *CommandInfo = calloc(1, sizeof(ShellCommand));
 
  char** spilt_by_space = SpiltBySpace(input);

  CommandInfo = SpiltIntoCmd(spilt_by_space);

  return CommandInfo;
}


void ExecuteCommand(ShellCommand* command) {
  switch (command->cmd) {
    //TODO Add other built-ins to be handled
    case THEN:
      ExecuteCommand(command->left_cmd);
      //TODO: Check for Success before running right
      ExecuteCommand(command->right_cmd);
    break;
    case EXIT:
      printf("done");
      exit(0);
    break;
    case NOTBUILTIN:
      fork_and_run(command);
    break;
  }
}


int main() {
  char* input;
  ShellCommand* command;

  for (;;) {
    input = CommandPrompt();
    // parse the command line
    command = ParseCommandLine(input);
    // execute the command
    ExecuteCommand(command);
  }

  exit(0);
}
