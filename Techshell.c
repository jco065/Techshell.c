#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
enum CMD {

  THEN,      //  &&
  ELSE,      //  ||
  OUTPUT,    //  >
  INPUT,     //  <
  APPEND,    //  >>
  ERRAPPEND, //  2>,
  PIPE,      //  |
  EXIT,
  CD,
  INVALID,
  NOTBUILTIN
};

typedef struct ShellCommand {
  enum CMD cmd;
  struct ShellCommand *left_cmd;
  struct ShellCommand *right_cmd;
  char **args;

  // redirection file names
  char *input_file;
  char *output_file;
  char *error_file;

  // directory used for cd
  char *directory;

} ShellCommand;

char **SpiltBySpace(char *input) {
  char **words_by_space = calloc(20, sizeof(char *));

  for (int i = 0; i <= 19; i++) {
    words_by_space[i] = (char *)calloc(1, 50);
  }

  int wbsi = 0;
  char cur_word[50] = {'\0'};
  int cur_word_index = 0;
  int length = strlen(input);

  for (int i = 0; i < length; i++) {
    char c = input[i];
    if ((c == ' ') || (c == '\n')) {
      // check new word
      // had to add this to fix the command    thing  if  we dont have this it
      // wouldnt run
      if (cur_word_index > 0) {
        cur_word[cur_word_index] += '\0';

        // push cur word to arr of words
        strcpy(words_by_space[wbsi], cur_word);
        wbsi += 1;
      }
      // reset cur word
      memset(cur_word, '\0', sizeof(cur_word));
      cur_word_index = 0;

    } else { // keep adding
      cur_word[cur_word_index] = c;
      cur_word_index += 1;
    }
  }

  // Null is added so we know when the words arr ends
  words_by_space[wbsi] = NULL;

  return words_by_space;
}

void print_string_arr(char **input) {
  int ti = 0;
  while (input[ti] != NULL) {
    printf("%s ", input[ti]);
    ti += 1;
  }
  printf("\n");
}

ShellCommand *SpiltIntoCmd(char **input) {
  // Split up to allow for presidance (Handle < before &&)
  const char *special_ops[][5] = {
      {"<", ">", ">>", "2>", NULL}, {"|", NULL}, {"&&", "||", NULL}};

  ShellCommand *CommandInfo = calloc(1, sizeof(ShellCommand));

  // Find the size of the input/words by space
  int size_of_wbs = 0;
  while (input[size_of_wbs] != NULL) {
    size_of_wbs += 1;
  }

  // Loop over all of the speical operators
  for (int i = 0; i < 3; i++) {
    int ii = 0;
    while (special_ops[i][ii] != NULL) { // for (int ii = 0; ii <= 4; ii++) {

      // Loop over input
      for (int j = 0; j < size_of_wbs; j++) {

        // Check for match
        if (strcmp(special_ops[i][ii], input[j]) == 0) {
          // If we find a match we split the two sides of the match and parse
          // both of them, before assign to a parent command

          // make sure the operator has something on both sides
          if (j == 0 || j == size_of_wbs - 1) {
            CommandInfo->cmd = INVALID;
            return CommandInfo;
          }

          // Check what the match was then assign the matching enum value
          if (strcmp(input[j], "&&") == 0) {
            CommandInfo->cmd = THEN;
          } else if (strcmp(input[j], "||") == 0) {
            CommandInfo->cmd = ELSE;
          } else if (strcmp(input[j], ">") == 0) {
            CommandInfo->cmd = OUTPUT;
            CommandInfo->output_file = strdup(input[j + 1]);
          } else if (strcmp(input[j], "<") == 0) {
            CommandInfo->cmd = INPUT;
            CommandInfo->input_file = strdup(input[j + 1]);
          } else if (strcmp(input[j], ">>") == 0) {
            CommandInfo->cmd = APPEND;
            CommandInfo->output_file = strdup(input[j + 1]);
          } else if (strcmp(input[j], "2>") == 0) {
            CommandInfo->cmd = ERRAPPEND;
            CommandInfo->error_file = strdup(input[j + 1]);
          } else if (strcmp(input[j], "|") == 0) {
            CommandInfo->cmd = PIPE;
          }

          // Calc size of each half
          int lhs = j + 1;
          int rhs = size_of_wbs - j;

          char **left_half = calloc(lhs, sizeof(char *));
          char **right_half = calloc(rhs, sizeof(char *));

          // Preassign size for each word in each half
          for (int i = 0; i <= lhs; i++) {
            left_half[i] = (char *)calloc(1, 50);
          }
          for (int i = 0; i <= rhs; i++) {
            right_half[i] = (char *)calloc(1, 50);
          }

          // Loop over all words, and split input into 2 array from before and
          // after the match
          for (int si = 0; si < size_of_wbs; si++) {
            if (si < j) {
              strcpy(left_half[si], input[si]);
            } else if (si > j) {
              strcpy(right_half[si - j - 1], input[si]);
            }
          }

          // Add NULL to end so we can detect the end of the array later on
          left_half[lhs - 1] = NULL;
          right_half[rhs - 1] = NULL;

          ShellCommand *CommandLeft = calloc(1, sizeof(ShellCommand));
          ShellCommand *CommandRight = calloc(1, sizeof(ShellCommand));

          // Recursive call to SpiltIntoCmd to keep going untill it returns only
          // one command
          CommandLeft = SpiltIntoCmd(left_half);
          CommandRight = SpiltIntoCmd(right_half);

          CommandInfo->left_cmd = CommandLeft;
          CommandInfo->right_cmd = CommandRight;

          return CommandInfo;
        }
      }

      ii += 1;
    }
  }

  // return only one (NO More/Any speicals ops)

  // TODO Add other built in commands
  if (strcmp(input[0], "exit") == 0) {
    CommandInfo->cmd = EXIT;
  } else if (strcmp(input[0], "cd") == 0) {

    // cd should only have one directory argument
    // J.O comment must check input[1] first  but your tokenizer prefills slots
    // meaning that input[2] wouldnt be null destroying cd
    if (input[1] != NULL && input[2] != NULL) {
      CommandInfo->cmd = INVALID;
    } else {
      CommandInfo->cmd = CD;

      if (input[1] != NULL) {
        CommandInfo->directory = strdup(input[1]);
      }
    }
  } else {
    CommandInfo->cmd = NOTBUILTIN;
  }

  CommandInfo->args = input;

  return CommandInfo;
}

// arguments for types of files


// i  had to change your fork and run cause who did the parsing did it in a way where it wants to know if the left 
// succed if so it does right which is logically true 
int fork_and_run(ShellCommand *command, char *input_file, char *output_file,
                  char *error_file, enum CMD output_option) {
  char **args = command->args;
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return -1;
  }
  // child process
  if (pid == 0) {
    // Input File
    if (input_file != NULL) {
      // grabs the fd(file desciptor)
      int input_fd = open(input_file, O_RDONLY);
      if (input_fd < 0) {
        perror("Can't open input file");
        exit(1);
      }
      // STDIN is replaced by the input file
      if (dup2(input_fd, STDIN_FILENO) < 0) {
        perror("dup2");
        close(input_fd);
        exit(1);
      }
      // Duplicated descriptor is used as stdin
      close(input_fd);
    }
    // Output File ( > or >> )
    if (output_file != NULL) {
      // grabs the fd(file descriptor)
      int output_fd;
      // Checks if '>'
      if (output_option == OUTPUT) {
        output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      }
      // Checks if '>>'
      else if (output_option == APPEND) {
        output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
      } else {
        fprintf(stderr, "Not correct output option");
        exit(1);
      }
      if (output_fd < 0) {
        perror("Can't open output file");
        exit(1);
      }
      // STOUT is replaced by the output file
      if (dup2(output_fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(output_fd);
        exit(1);
      }
      // Duplicated descriptor is used as stdout
      close(output_fd);
    }
    // Error File ( 2> )
    if (error_file != NULL) {
      int error_fd = open(error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (error_fd < 0) {
        perror("Can't open error file");
        exit(1);
      }
      // STDERR is replaced by the output file
      if (dup2(error_fd, STDERR_FILENO) < 0) {
        perror("dup2");
        close(error_fd);
        exit(1);
      }
      // Duplicated decripter is used as stderr
      close(error_fd);
    }
    // Replace the child process with the program
    execvp(args[0], args);
    fprintf(stderr, "Error %d (%s)\n", errno, strerror(errno));
    exit(1);
  }
 int parentWAIT;
  waitpid(pid, &parentWAIT, 0);
  if (WIFEXITED(parentWAIT))
    return WEXITSTATUS(parentWAIT);
  if (WIFSIGNALED(parentWAIT))
    return 128 + WTERMSIG(parentWAIT);
  return -1;
}

// so many error handlings cause i wanted to learn a little more about waitpid
// and the sytems with that
int fork_and_run_pipe(ShellCommand *left_cmd, ShellCommand *right_cmd) {
  // error file descriptor
  int fds[2];
  if (pipe(fds) == -1) {
    perror("something wrong with thee pipe");
    return -1;
  }
  pid_t pid = fork();
  // first command ran of the
  if (pid == 0) {
    // read part of pipe
    // duppp the file of the first and send it  o end pip then send it to the
    // othe process
    dup2(fds[1], STDOUT_FILENO);
    close(fds[0]);
    close(fds[1]);
    execvp(left_cmd->args[0], left_cmd->args);
    perror(left_cmd->args[0]); // if it reached here exec failed
    exit(EXIT_FAILURE);        // macro this is bacisaly exit(1)
  }

  pid_t pid2 = fork();
  if (pid2 == 0) {
    dup2(fds[0], STDIN_FILENO);
    close(fds[0]);
    close(fds[1]);
    execvp(right_cmd->args[0], right_cmd->args);
    perror(right_cmd->args[0]); // if it reached here exec failed
    exit(EXIT_FAILURE);
  }
  // closes parent 
  close(fds[0]);
  close(fds[1]);
  // i need  a way  to have the parent staus wait for the process to run
  int parentWAIT;
  waitpid(pid, NULL, 0); // this is the first cmd
  waitpid(pid2, &parentWAIT, 0);
  if (WIFEXITED(parentWAIT))
    return WEXITSTATUS(parentWAIT);
  if (WIFSIGNALED(parentWAIT))
    return 128 + WTERMSIG(parentWAIT);
  return -1;
}

char *CommandPrompt(char *cwd) {
  char *raw_input = calloc(100, sizeof(char));
  printf("%s$ ", cwd);
  if (fgets(raw_input, 100, stdin) == NULL) {
    printf("\n");
    exit(0);
  }
  return raw_input;
}

ShellCommand *ParseCommandLine(char *input) {
  ShellCommand *CommandInfo = calloc(1, sizeof(ShellCommand));

  char **spilt_by_space = SpiltBySpace(input);

  CommandInfo = SpiltIntoCmd(spilt_by_space);

  return CommandInfo;
}
// who ever did the exectuecommand  stuff wanted  a sucess to see if  the left to succed which is logical so the function cant 
//reutrn void 
//   cases || and && depdns on it  
int  ExecuteCommand(ShellCommand *command) {
  switch (command->cmd) {
  // TODO Add other built-ins to be handled
    //added your staus check
  case THEN:{
    int left = ExecuteCommand(command->left_cmd);
        // TODO: Check for Success before running right
    //Sucess check 
    if(left == 0){
      return ExecuteCommand(command->right_cmd);
    }
          return left;   

}
case ELSE: {
    int left = ExecuteCommand(command->left_cmd);
    if (left != 0){
      return ExecuteCommand(command->right_cmd);
    }
    return left;
  }

  case EXIT:
    printf("done");
    exit(0);
    return 0; // doesnt get here if  but complers crying
  case CD: {
    char *dir = command->directory;
    if (dir == NULL || dir[0] == '\0') {
      dir = getenv("HOME");
    }
    if (chdir(dir) != 0) {
      fprintf(stderr, "Error %d (%s)\n", errno, strerror(errno));
    return -1;
    }
    return 0;
  }
  case INVALID:
    printf("Invalid command\n");
    return -1;

  case NOTBUILTIN:
    return fork_and_run(command, NULL, NULL, NULL, INVALID);
    
  case INPUT:
    return fork_and_run(command->left_cmd, command->input_file, NULL, NULL, INVALID);
    
    // '>'
  case OUTPUT:
    return fork_and_run(command->left_cmd, NULL, command->output_file, NULL, OUTPUT);
    
    // '>>'
  case APPEND:
   return  fork_and_run(command->left_cmd, NULL, command->output_file, NULL, APPEND);
    
    // '2>'
  case ERRAPPEND:
    return fork_and_run(command->left_cmd, NULL, NULL, command->error_file, ERRAPPEND);
    
  case PIPE: {
    return fork_and_run_pipe(command->left_cmd, command->right_cmd);
    }
    return -1;
}
  

  
  }


int main() {
  char cwd[1024];
  char *input;
  ShellCommand *command;


  for (;;) {
    //error case 
    if(getcwd(cwd,sizeof(cwd)) == NULL){
      perror("getcwd failed");
    }
    input = CommandPrompt(cwd);
    // parse the command line
    command = ParseCommandLine(input);
    // execute the command
    ExecuteCommand(command);
  }

  exit(0);
}
