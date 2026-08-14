# Techshell Project
Techshell is a bash like Linux shell written in C for LaTech's CSC2223 Systems Programming course

## Breakdown
Techshell can broken down into 3 main parts. Taking in input, Parsing that input, and then executing, this is done continuously until the user decides to exit.
The user is user is first promoted for the commands they wish to run, this is then stored as one continuous string in the heap. 
A pointer to this string is then passed to be split into an array of string to then be recursively parsed into a command. 
This is done by first scanning for any special operators (i.e. >, <, &&), if any are found then the split into commands function is called again for
both the left and right half of the input and then the output of those calls are used to form the special operator command. 
If no special operator is found then a regular command is formed using the input.
This produces one command struct which is then passed to the execution function, where it is then conditionally operated on. If the command passed is not built in
it is passed to a function that first forks the process before then running the command via `execvp()` the child process then checks for error before exiting, causing the parent to finish waiting and return the status of the command run (i.e. Success, Failure, etc.) for processing before then going back to take in input. If the command passed is a special command or built in command, the execute function then runs commands
specific logic before either running such commands via recursion or simply returning to await input. 
command.
Techshell handles shell input/output by storing a copy of the source/destination of a redirect in the command structure, then when that struct is executed the source/destination is then recalled and a file descriptor is made from it. This file descriptor is then used to replace the default input/output stream before the command is called.
Piping is handled in similar manor, a pipe is opened, then the process is forked, the child then replaces its output with one side of the pipe before calling the first command. Then, once the child exits, the process is forked again, and the child of this fork swaps their input with the other side of the pipe, before calling the second command and exiting like normal. 

## Features
- [x] Calling not built in commands
- [x] Passing command arguments
- [x] Error handling
- [X] I/O redirections
- [X] Current Path Before Prompt

## Running
Techshell can be build via gcc (or any other C compiler)like so:
```bash
gcc Techshell.c -o Techshell 
```

It can then be ran like any other binary:
```bash
./Techshell 
```

## Members:

| Name | Work Summary |
|------|--------------|
| Anthony Chester | Set basic skeleton for project, Wrote Split by String function, Made minimal working version of Split Commands function |
| Inioluwa Ebofin | {Type what you did here} |
| Madison Leblanc  | Expanded the shell parser by adding support for command operators, redirection filenames, invalid command checks, cd command parsing, and empty input handling |
| Jordan Owens | Implemented cd and the exec implentation of fork and run and pipe part of built in part. |
| Ayomide Oyekunle | {Type what you did here} |
| Calvin Vanner | Implemented working I/O command Redirections ( '<', '>', '>>', and '2>') |

