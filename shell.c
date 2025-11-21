// shell.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#define MAX_WD 256
#define MAX_CMD 256
#define MAX_TOK 16

int running = 1;
char *inFile = NULL;
char *outFile = NULL;
static int runInBackground = 0;
char *prevCommands[100];
int numCommands = 0;

static const char *commandAliases[] = {"exit", "cd", "ls", "ps", "cat", "addNum", "sleep", "history", "!5"};

static void runCommand(char **);

int tokenize(char *raw, char **out, int max_tok)
{
    int n = 0;
    for (char *t = strtok(raw, " \t\r\n"); t && n < max_tok; t = strtok(NULL, " \t\r\n"))
    {
        out[n++] = t;
    }
    out[n] = NULL;
    return n;
}

static void proc_exit(int sig)
{
    int saved_errno = errno;
    int status;

    while (waitpid(-1, &status, WNOHANG) > 0)
    {
    }
    errno = saved_errno;
}

static void cmd_exit(char **tokens)
{
    inFile = NULL;
    outFile = NULL;
    running = 0;
}

static void cmd_cd(char **tokens)
{
    inFile = NULL;
    outFile = NULL;
    if (chdir(tokens[1]) != 0)
    {
        printf("%s is not a valid directory\n", tokens[1]);
    }
}

static void cmd_ls(char **tokens)
{
    int status;
    char *args[3];

    args[0] = "/bin/ls";

    if (tokens[1] == NULL)
    {
        args[1] = NULL;
    }
    else if (tokens[2] == NULL)
    {
        args[1] = tokens[1];
        args[2] = NULL;
    }
    else
    {
        printf("Too many flags given, %s \n", tokens[2]);
        inFile = NULL;
        outFile = NULL;
        return;
    }

    int childPid = fork();

    if (childPid == 0)
    {
        if (inFile)
            freopen(inFile, "r", stdin);
        if (outFile)
            freopen(outFile, "w", stdout);
        execv(args[0], args);
    }
    else
    {
        if (!runInBackground)
        {
            if (waitpid(childPid, &status, 0) < 0)
                perror("waitpid");
        }
        else
        {
            printf("[bg] started pid %d\n", childPid);
        }
    }
    inFile = NULL;
    outFile = NULL;
}

static void cmd_ps(char **tokens)
{
    int status;
    char *args[3];

    args[0] = "/bin/ps";

    if (tokens[1] == NULL)
    {
        args[1] = NULL;
    }
    else if (tokens[2] == NULL)
    {
        args[1] = tokens[1];
        args[2] = NULL;
    }
    else
    {
        printf("Too many flags given, %s \n", tokens[2]);
        inFile = NULL;
        outFile = NULL;
        return;
    }

    int childPid = fork();

    if (childPid == 0)
    {
        if (inFile)
            freopen(inFile, "r", stdin);
        if (outFile)
            freopen(outFile, "w", stdout);
        execv(args[0], args);
    }
    else
    {
        if (!runInBackground)
        {
            if (waitpid(childPid, &status, 0) < 0)
                perror("waitpid");
        }
        else
        {
            printf("[bg] started pid %d\n", childPid);
        }
    }
    inFile = NULL;
    outFile = NULL;
}

static void cmd_cat(char **tokens)
{
    int status;
    char *args[3];

    args[0] = "/bin/cat";

    if (tokens[1] == NULL)
    {
        args[1] = NULL;
    }
    else if (tokens[2] == NULL)
    {
        args[1] = tokens[1];
        args[2] = NULL;
    }
    else
    {
        printf("Too many flags given, %s \n", tokens[2]);
        inFile = NULL;
        outFile = NULL;
        return;
    }

    int childPid = fork();

    if (childPid == 0)
    {
        if (inFile)
            freopen(inFile, "r", stdin);
        if (outFile)
            freopen(outFile, "w", stdout);
        execv(args[0], args);
    }
    else
    {
        if (!runInBackground)
        {
            if (waitpid(childPid, &status, 0) < 0)
                perror("waitpid");
        }
        else
        {
            printf("[bg] started pid %d\n", childPid);
        }
    }
    inFile = NULL;
    outFile = NULL;
}

static void cmd_addNum(char **tokens)
{
    char *args[2];
    int status;
    args[0] = "./addNum";
    args[1] = NULL;

    int childPid = fork();

    if (childPid == 0)
    {
        if (inFile)
            freopen(inFile, "r", stdin);
        if (outFile)
            freopen(outFile, "w", stdout);
        execv(args[0], args);
    }
    else
    {
        if (!runInBackground)
        {
            if (waitpid(childPid, &status, 0) < 0)
                perror("waitpid");
        }
        else
        {
            printf("[bg] started pid %d\n", childPid);
        }
    }
    inFile = NULL;
    outFile = NULL;
}

static void cmd_sleep(char **tokens)
{
    char *args[3];
    int status;

    args[0] = "/bin/sleep";

    if (tokens[1] == NULL)
    {
        args[1] = "60";
        args[2] = NULL;
    }
    else if (tokens[2] == NULL)
    {
        args[1] = tokens[1];
        args[2] = NULL;
    }
    else
    {
        printf("Too many flags given, %s \n", tokens[2]);
        inFile = NULL;
        outFile = NULL;
        return;
    }

    int childPid = fork();

    if (childPid == 0)
    {
        if (inFile)
            freopen(inFile, "r", stdin);
        if (outFile)
            freopen(outFile, "w", stdout);
        execv(args[0], args);
    }
    else
    {
        if (!runInBackground)
        {
            if (waitpid(childPid, &status, 0) < 0)
                perror("waitpid");
        }
        else
        {
            printf("[bg] started pid %d\n", childPid);
        }
    }
    inFile = NULL;
    outFile = NULL;
}

static void cmd_history(char **tokens)
{
    printf("Last %d commands\n", numCommands);
    for (int i = 0; i < numCommands; i++)
    {
        printf("Command %d: %s\n", i + 1, prevCommands[i]);
    }
}

static void cmd_five(char **tokens)
{
    if (numCommands < 5 || !prevCommands[4])
    {
        printf("No 5th command in history yet.\n");
        return;
    }
    char temp[MAX_CMD];
    strncpy(temp, prevCommands[4], sizeof temp);
    temp[sizeof temp - 1] = '\0';

    char *toks[MAX_TOK + 1];
    int ntok = tokenize(temp, toks, MAX_TOK);
    if (ntok > 0)
        runCommand(toks);
}

static void (*commands[])(char **) = {cmd_exit, cmd_cd, cmd_ls, cmd_ps, cmd_cat, cmd_addNum, cmd_sleep, cmd_history, cmd_five};

static int strip_background(char **tokens)
{
    int i = 0;
    while (tokens[i])
        i++;
    if (i > 0 && strcmp(tokens[i - 1], "&") == 0)
    {
        tokens[i - 1] = NULL;
        return 1;
    }
    return 0;
}

static void redirection(char **tokens)
{
    inFile = NULL;
    outFile = NULL;
    for (int i = 0; tokens[i] != NULL; i++)
    {
        if (strcmp(tokens[i], "<") == 0)
        {
            inFile = tokens[i + 1];
            for (int j = i; tokens[j] != NULL; j++)
            {
                tokens[j] = tokens[j + 2];
            }
            i--;
        }
        else if (strcmp(tokens[i], ">") == 0)
        {
            outFile = tokens[i + 1];
            for (int j = i; tokens[j] != NULL; j++)
            {
                tokens[j] = tokens[j + 2];
            }
            i--;
        }
    }
}

static void runCommand(char **tokens)
{
    if (!tokens || !tokens[0])
        return;

    runInBackground = strip_background(tokens);
    redirection(tokens);

    int len = (int)(sizeof(commandAliases) / sizeof(commandAliases[0]));
    for (int i = 0; i < len; i++)
    {
        if (strcmp(commandAliases[i], tokens[0]) == 0)
        {
            commands[i](tokens);
            return;
        }
    }
    printf("%s is not a valid command\n", tokens[0]);
}

int main(void)
{
    char wd[MAX_WD];
    char rawCommand[MAX_CMD];

    if (signal(SIGCHLD, proc_exit) == SIG_ERR)
        perror("signal");

    while (running == 1)
    {
        getcwd(wd, sizeof wd);
        printf("%s> ", wd);
        fflush(stdout);

        if (!fgets(rawCommand, sizeof rawCommand, stdin))
            break;

        rawCommand[strcspn(rawCommand, "\n")] = '\0';

        if (numCommands < 100 && rawCommand[0] != '\0')
        {
            prevCommands[numCommands++] = strdup(rawCommand);
        }

        char *tokens[MAX_TOK + 1];
        int ntok = tokenize(rawCommand, tokens, MAX_TOK);
        if (ntok > 0)
            runCommand(tokens);
    }
    return 0;
}
