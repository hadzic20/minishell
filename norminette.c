#include "minishell.h"

int main(int argc, char **argv, char **envp) {
    char *command[15];
    command[0] = "norminette";
    command[1] = "builtin";
    command[2] = "libft";
    command[3] = "parse";
    command[4] = "prompt";
    command[5] = "redirect";
    command[6] = "utils";
    command[7] = "main.c";
    command[8] = "minishell.h";
    command[9] = NULL;
    execve("/Users/amillahadzic/Library/Python/3.8/bin/norminette", command, envp);
}