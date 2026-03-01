# Techshell.c
A Csc 222 shell like implentaition Jordan Owens
```sh
How to compile 
gcc Techshell.c -o techshell


How to run 
./techshell


Techshell is a simple Unix shell that mimics the behavior of bash. When run, it displays the current working directory as a prompt and waits for the user to type a command. It supports:

Running any program available on the system (ls, ps, wc, cat, chmod, etc.)

Changing directories with cd, including cd ~ to go home
Input redirection with < (read stdin from a file)

Output redirection with > (write stdout to a file)

Combining both: wc -l < input.txt > output.txt

~~sh




