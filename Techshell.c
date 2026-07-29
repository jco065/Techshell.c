#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// i needed something to recongize whats built in verues whats not
char *nonBuiltins[] = {"ls", "pwd", "cat", "grep", "mkdir"};

enum CMD {
  EXIT,
  NOTBUILTIN,
  BUILTIN

};

typedef struct {
  enum CMD cmd;
  int command_index;
  struct ShellCommand *left_cmd;
  struct ShellCommand *right_cmd;
  char *args[];
} ShellCommand;

// added a fucntion that handles the non execvp runners
// void ExecuteBuiltin(ShellCommand *command)
// //lol figure out latter that non built ins isnt really need for redirection
// and only cover two cases but what ever
void fork_and_run(ShellCommand *command) {
  // Fork Process
  // Parent -> Check for Success -> Wait
  // Child  -> execvp() -> Check
  pid_t pid = fork();

  if (pid == 0) {
    // runs the command
    //
    // stored it int the struct wered way to do this if problems arise i might
    // change
    execvp(command->args[0], command->args);
    perror("execvp");
    exit(1);

  }

  else if (pid > 0) {
    wait(NULL);
  }

  else {
    perror("fork");
  }
}

char *CommandPrompt() {
  char *raw_input = calloc(50, sizeof(char));
  // Print current dir, and username
  printf("$ ");
  scanf("%s", raw_input);
  return raw_input;
}
// chaning the command parse to dnity whens a command form execvp ISNT a command
// and IS a command

ShellCommand *ParseCommandLine(char *input) {
  int numCommands = 5;
  ShellCommand *CommandInfo = calloc(1, sizeof(ShellCommand));
  if (strcmp(input, "exit") == 0) {
    CommandInfo->cmd = EXIT;
  }
  // added this for loop to clasify the differnce bettwen an execvp usable and a
  // non I added the variable so i didnt have to loop again in the execute if it
  // is
  for (int i = 0; i < numCommands; i++) {
    if (strcmp(input, nonBuiltins[i]) == 0) {
      CommandInfo->cmd = NOTBUILTIN;
      CommandInfo->command_index = i;
      break;
    }

    else {
      CommandInfo->cmd = BUILTIN;
    }
  }
  return CommandInfo;
}

void ExecuteCommand(ShellCommand *command) {
  switch (command->cmd) {
  case EXIT:
    printf("done");
    exit(0);
    break;
  case NOTBUILTIN:
    fork_and_run(command);
    break;
  case BUILTIN:
    // ExecuteBuiltin(command);
    break;
  }
}

int main() {
  char *input;
  ShellCommand *command;

  for (;;) {
    input = CommandPrompt();
    // parse the command line
    command = ParseCommandLine(input);
    // execute the command
    ExecuteCommand(command);
  }

  exit(0);
}
