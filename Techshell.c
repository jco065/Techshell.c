#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

enum CMD {
  EXIT,
  NOTBUILTIN
};

typedef struct{
  enum CMD cmd;
  struct ShellCommand *left_cmd;
  struct ShellCommand *right_cmd;
  char *args[];
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
    printf("i: %d, c: %c\n", i, c);
    if ((c == ' ') || (c == '\n')) {
      //check new word
      //cur_word_index += 1;
      cur_word[cur_word_index] += '\0';
      printf("|%s|\n", cur_word);
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

  return words_by_space;
}

void fork_and_run(ShellCommand* command) {
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

  printf("<%s>", spilt_by_space[0]);

  if (strcmp(spilt_by_space[0], "exit") == 0) {
    CommandInfo->cmd = EXIT;
  } else {
    CommandInfo->cmd = NOTBUILTIN;
  }

  return CommandInfo;
}


void ExecuteCommand(ShellCommand* command) {
  switch (command->cmd) {
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
    printf("||%s||\n", input);
    // parse the command line
    command = ParseCommandLine(input);
    // execute the command
    ExecuteCommand(command);
  }

  exit(0);
}
