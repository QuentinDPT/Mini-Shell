#ifndef COMMAND_H
#define COMMAND_H

#define HISTORY_MAX_LENGTH 50
#define CWD_MAX_LENGHT 100
#define CMD_LENGTH 2000
#define CMD_PARAMETERS 60
#define HISTORY_FILE "/tmp/minishell.txt"
#include "typedef.h"

pcmd_t create_cmd();

void destroy_cmd(pcmd_t cmd);

int count_cmd(pcmd_t cmd);

void print_cmd(pcmd_t cmd);

void translate_cmd(pcmd_t *cmd, char *raw_cmd);

int execute_expression(pcmd_t cmd);

int execute_single_command(char *const argv[], char background);

int child_execute_cmd(char *const argv[]);

void add_cmd(const char *cmd);

int execute_history(char background);

void change_directory(char* argv[]);

void print_help();

#endif
