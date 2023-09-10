# ShellUserInterface

Implementation of a shell user interface, including the relationship between child and parent processes, the steps needed to create a new process, including a search of $PATH environment variable, and an introduction to user-input parsing and verification. Also input/output redirection, piping, and background processes are implemented.

Group Members: Sophia Keezel and Tristan Hale

---------------------------------------------
parser.c

This file includes the parsing of the command string into meaningful tokens, in which the user enters commands where tokens are separated by a single space. Environment variables are also assigned by replace every token that starts with a dollar sign character into its corresponding value. The prompt is printed during each loop by expanding the $USER, $MACHINE, $PWD environment variables. Tilde expansion is implemented by expanding the $HOME variable with the token attached to a tilde. Built-in function are also implemented including exit, cd, jobs, and echo.

Part 1: Parsing - Sophia Keezel

Part 2: Environmental Variables - Tristan Hale

Part 3: Prompt - Sophia Keezel

Part 4: Tilde Expansion - Sophia Keezel

Part 10: Built-In Functions - Sophia Keezel

----------------------------------------------------
path-search.c

This file includes the path search which requires a search of a predefined list of directories in order to execute the command in the $PATH variable. External command execution is implemented by calling fork() and executing the command in the child process using execv(). For I/O redirection, the shell receives input from the keyboard and writes outputs to screen. Input redirection (<) replaces the keyboard and output redirection (>) replaces the screen with a specified file. Piping is implemented by redirecting the output of the first command with the input of the second command.

Part 5: $PATH Search - Tristan Hale

Part 6: External Command Execution - Tristan Hale

Part 7: I/O Redirection - Tristan Hale and Sophia Keezel

Part 8: Piping - Sophia Keezel and Tristan Hale

-----------------------------------------------------------------------
functionality.c and functionality.h

This file includes the a struct which holds the information used to track the completion of the commands sent to background processing. The purpose is to tell the shell to not wait for an external command to finish.

Part 9: Background Processing - Sophia Keezel and Tristan Hale

---------------------------------------------------------
makefile instructions

Run the makefile by typing "make", this will create an executable called "shell". Run this executable to run the program.

-----------------------------------------------------------------------
known bugs and errors

Piping implemenation is incorrect and runs the background process. We attempted to debug this error but couldn't figure out the issue.
