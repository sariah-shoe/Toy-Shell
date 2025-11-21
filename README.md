# Sariah Shoemaker COMP-2362 Toy Shell Cover Sheet

A Makefile is included in the ZIP. This Makefile will compile both my shell and addNum (a program that shell can execute.)

Nums.txt is included in the files to help test redirection with addNum

My shell has 9 commands
```
    Exit - Exits my shell, accepts no flags
    Cd - Changes the directory in my shell to whatever comes after cd, accepts no flags
    Ls - Lists what's in the directory, accepts one flag
    Ps - Lists what processes are running, accepts one flag
    Cat - Outputs what is in a file, accepts one flag
    addNum - After running the command, it will ask for two numbers and add them
    Sleep - Has the shell sleep, the default is 60 seconds but you can change it to something else, it accepts one flag
    History - Shows the last 100 commands, accepts no flags
    !5 - Runs the 5th command run, accepts no flags
```

If an invalid command is run, the shell will tell you it is not a valid command
