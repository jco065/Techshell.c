#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

enum CMD {
  PIPE,   // |
  ANDAND, // &&
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

  int length = strlen(input); //sizeof(*input) / sizeof(input[0]);
  for (int i = 0; i < length; i ++) {
    char c = input[i];
    //printf("i: %d, c: %c\n", i, c);
    if ((c == ' ') || (c == '\n')) {
      //check new word
      //cur_word_index += 1;
      cur_word[cur_word_index] += '\0';
      //printf("|%s|\n", cur_word);
      //push cur word to arr of words
      strcpy(words_by_space[wbsi], cur_word);
      wbsi += 1;
      //reset cur word
      memset(cur_word, '\0', sizeof(cur_word));
      cur_word_index = 0;
    } else {
      cur_word[cur_word_index] = c;
      cur_word_index += 1;
    }
  }

  //printf(">%d<", wbsi);
  words_by_space[wbsi] = NULL;

  return words_by_space;
}



ShellCommand* SpiltIntoCmd(char** input) {
  int ti = 0;
  printf("###\n");
  while(input[ti] != NULL) {
  printf("%s ", input[ti]);
  ti += 1;
  }
  printf("\n###\n");
  const char *speial_ops[][5] = { {"<", ">", ">>", "2>", NULL}, {"|", "|&", NULL, NULL}, {"&&", "||", NULL, NULL} };

  ShellCommand *CommandInfo = calloc(1, sizeof(ShellCommand));
 
  int size_of_wbs = 0;
  while (input[size_of_wbs] != NULL) {
    size_of_wbs += 1;
    //printf("--> %d\n", size_of_wbs);
  }

  for (int i = 0; i < 3; i++ ) {
    int ii = 0;
    while (speial_ops[i][ii] != NULL) { //for (int ii = 0; ii <= 4; ii++) {
     
      for (int j = 0; j < size_of_wbs; j++) {
        //printf("*%s* is *%s* == %d\n", speial_ops[i][ii], input[j], strcmp(speial_ops[i][ii], input[j]));
        if (strcmp(speial_ops[i][ii], input[j]) == 0) {
          //split
          //CommandInfo->cmd = speial_ops;
          //printf("test \n"); 
          if ( strcmp(input[j], "&&") == 0 ) {
              printf("and\n");
              CommandInfo->cmd = ANDAND;
          }

          int lhs = j + 1;
          int rhs = size_of_wbs - j;

          char **left_half = calloc( lhs, sizeof(char*));
          char **right_half = calloc( rhs, sizeof(char*));
          printf("|%d, %d|\n", lhs, rhs);

          for(int i=0;i<=lhs;i++) {
            left_half[i] = (char*) calloc(1, 50);
          }
          for(int i=0;i<=rhs;i++) {
            right_half[i] = (char*) calloc(1, 50);
          }

          for( int si = 0; si < size_of_wbs; si++ ) {
            printf("si: %d\n", si);
            if (si == j) {

            } else if (si < j) {
              strcpy(left_half[si], input[si]);
              printf("LH[%d]: %s\n", si, input[si]);
            } else {
              strcpy(right_half[si-j-1], input[si]);
              printf("RH[%d]: %s\n", si-j-1, input[si]);
            }
          }
        
          left_half[lhs - 1] = NULL;
          right_half[rhs - 1] = NULL;

          //printf("_%d_", size_of_wbs);
          
          ShellCommand *CommandLeft = calloc(1, sizeof(ShellCommand));
          ShellCommand *CommandRight = calloc(1, sizeof(ShellCommand));

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
 
  printf("    __%s__\n", input[0]);

  if (strcmp(input[0], "exit") == 0) {
    printf("EXIT CODE\n");
    CommandInfo->cmd = EXIT;
  } else {
    CommandInfo->cmd = NOTBUILTIN;
  }

  CommandInfo->args = input;

  return CommandInfo;
}

void fork_and_run(ShellCommand* command) {
  printf(">  %s\n", command->args[0]);
//Fork Process
//Parent -> Check for Success -> Wait
//Child  -> execvp() -> Check
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

  //printf("<%s>", spilt_by_space[0]);

  CommandInfo = SpiltIntoCmd(spilt_by_space);

  

  return CommandInfo;
}


void ExecuteCommand(ShellCommand* command) {
  printf("*****{%d}****\n", command->cmd);
  switch (command->cmd) {
    case ANDAND:
      printf("AA\n");
      ExecuteCommand(command->left_cmd);
      //Check for Success before running right
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
    //printf("||%s||\n", input);
    // parse the command line
    command = ParseCommandLine(input);
    // execute the command
    ExecuteCommand(command);
  }

  exit(0);
}
