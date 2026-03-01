#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define INPUT_MAX 256
#define ARGS_MAX 64
#include <fcntl.h>

char directory[PATH_MAX];
char input[INPUT_MAX];
char *args[ARGS_MAX];
// 4. Forbidden functions include system()and popen()
/// displays money sign plus what ever
// ~$ if they ` it goes to home and if
// they make itso what ever direcotry your in it display is and what ever is in
// it
// prints wirking direcotry

void cd(char *args[]);
void superman(char *args[]);
void extractor(char *input, char *args[]);
void PACKMAN(char *INNNY, char *OUTTY);

pid_t pid;
char *home;
char *INNNY;
char *OUTTY;

int main(void) {
  char directory[PATH_MAX];
  char input[INPUT_MAX];
  /// char *args[4];

  char *home = getenv("HOME");
  char *root = getenv("ROOT");

  home = getenv("HOME");

  if (home != NULL) {
    chdir(home);
  }
  while (1) {
    INNNY = NULL;
    OUTTY = NULL;

    char directory[PATH_MAX];

    if (getcwd(directory, sizeof(directory)) != NULL) {
      printf("%s$ ", directory);
      fflush(stdout);
    }

    // this reads my keyboard
    fgets(input, INPUT_MAX, stdin);
    extractor(input, args);
    int check_FORPACK = 0;
    if (args[0] == NULL)
      continue;

    for (int i = 0; args[i] != NULL; i++) {
      if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0) {
        check_FORPACK = 1;
        break;
      }
    }
    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    }
    for (int i = 0; args[i] != NULL; i++) {
      if (strcmp(args[i], "<") == 0 && args[i + 1])
        INNNY = args[i + 1];
      if (strcmp(args[i], ">") == 0 && args[i + 1])
        OUTTY = args[i + 1];
    }
    if (strcmp(args[0], "cd") == 0) {
      cd(args);
    } else {
      superman(args);
    }
  }
}

void extractor(char *input, char *args[]) {
  int i = 0;
  args[i] = strtok(input, " \n\t");
  while (args[i] != NULL && i < ARGS_MAX - 1) {
    i++;
    args[i] = strtok(NULL, " \n\t");
  }
  args[i] = NULL;
}
void superman(char *args[]) {
  pid = fork();
  if (pid < 0) {
    perror("child failed its mission ");
    return;
  }
  if (pid == 0) {
    if (args[1] == NULL) {
    }
    if (args[1] && strcmp(args[1], "/root") == 0)

    {
      fprintf(stderr, "root access DENIED\n");
      exit(1);
    }
    PACKMAN(INNNY, OUTTY);
    int i = 0;

    while (args[i] != NULL) {
      if (strcmp(args[i], "<") == 0) {
        args[i] = NULL;
        break;
      }
      if (strcmp(args[i], ">") == 0) {
        args[i] = NULL;
        break;
      }
      i++;
    }
    execvp(args[0], args);
    perror("execvp");
    exit(1);
  }
  wait(NULL);
}
void cd(char *args[]) {
  home = getenv("HOME");
  if (args[1] == NULL || strcmp(args[1], "~") == 0) {
    if (home)
      chdir(home);
    return;
  }
  if (args[1] && strcmp(args[1], "/root") == 0) {
    perror("cannot open directory /root: Permission denied");
  }
  if (chdir(args[1]) != 0) {
    fprintf(stderr, "Error %d (%s)\n", errno, strerror(errno));
    return;
  }
}
void PACKMAN(char *INNNY, char *OUTTY) {
  if (INNNY) {
    int IN = open(INNNY, O_RDONLY);
    if (IN < 0) {
      perror(INNNY);
      exit(1);
    }
    dup2(IN, STDIN_FILENO);
    close(IN);
  }
  if (OUTTY) {
    int OUT = open(OUTTY, O_CREAT | O_WRONLY | O_TRUNC, 0664);
    if (OUT < 0) {
      perror(OUTTY);
      exit(1);
    }
    dup2(OUT, STDOUT_FILENO);
    close(OUT);
  }
}
