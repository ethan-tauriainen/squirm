# squirm
The squirm program is effectively a 'mouse jiggler'. This program will keep Teams green.

## Further Development Plans
This is a simple, but functional, prototype. I have big plans for squirm, baby. Eventually there will be the ability to record mouse movement macros and replay them. Also I plan to make a GUI.

## How to use:
```
# Easy to compile.
gcc -o squirm main.c

# Must be run as root
ethan@localhost:~/scratch/squirm> ./squirm 
Root privileges are required to run this command.

# Help menu.
ethan@localhost:~/scratch/squirm> sudo ./squirm --help
Usage: squirm [OPTION...]
squirm -- a program that periodically jiggles the mouse cursor

  -i, --interval=TIME        Jiggle mouse left and right every TIME seconds
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to <taurethan@gmail.com>.

# Default interval is 5 seconds; however, one may specify
# an interval of his/her choice. Use Ctrl-C to stop the
# program.
than@localhost:~/scratch/squirm> sudo ./squirm --interval 5
^C
Jiggle success.
```
