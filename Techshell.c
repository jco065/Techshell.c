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


void fork_and_run(ShellCommand* command) {
//Fork Process
//Parent -> Check for Success -> Wait
//Child  -> execvp() -> Check
}

char* CommandPrompt() {
  char *raw_input = calloc(50, sizeof(char));
  //Print current dir, and username
  printf("$ ");
  scanf("%s", raw_input);
  return raw_input;
}


ShellCommand* ParseCommandLine(char* input) {
  ShellCommand *CommandInfo = calloc(1, sizeof(ShellCommand));
  if (strcmp(input, "exit") == 0) {
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
    // parse the command line
    command = ParseCommandLine(input);
    // execute the command
    ExecuteCommand(command);
  }

  exit(0);
}
