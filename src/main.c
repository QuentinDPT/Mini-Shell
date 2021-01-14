/**
* \file main.c
* \brief Programme d'éxécution du Mini-Shell
* \author Alexis VELTEN & Quentin DE POTTER
* \version 1.0
* \date 15 janvier 2021
*
* Programme permettant l'éxécution du Mini-Shell
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "../include/command.h"

struct cmd_t;

int main(){
  printf("\033[H\033[J");
  printf("\e[1;35m                  __  __ _       _  _____ _          _ _ \n                 |  \\/  (_)     (_)/ ____| |        | | |\n                 | \\  / |_ _ __  _| (___ | |__   ___| | |\n                 | |\\/| | | '_ \\| |\\___ \\| '_ \\ / _ \\ | |\n                 | |  | | | | | | |____) | | | |  __/ | |\n                 |_|  |_|_|_| |_|_|_____/|_| |_|\\___|_|_|\e[0m\n");
  printf("\n");
  printf("\e[1;31m*****************\e[0m     \e[1;33mBienvenue dans le Mini-Shell !\e[0m     \e[1;31m*****************\e[0m\n");
  printf("\n");
  printf("\e[1;32mPour plus d'information sur l'utilisation du Mini-Shell, tapez 'help'\e[0m\n");

  char cwd[CWD_MAX_LENGHT];
  char raw_cmd[CMD_LENGTH];
  int lastProcessReturn = 0 ;

  do{
    //Récupération du pwd
    getcwd(cwd,CWD_MAX_LENGHT);

    printf("\e[1;34m~%s\e[0m$ ", cwd);

    fgets(raw_cmd, CMD_LENGTH, stdin) ;

    if(strcmp(raw_cmd, "help\n") == 0) print_help();

    if((strcmp(raw_cmd, "exit\n") == 0) || (strcmp(raw_cmd, "quit\n") == 0)) break;

    pcmd_t command = create_cmd();

    //Historique des commandes
    add_cmd(raw_cmd);

    //Traduction de la commande (systeme standard pour envoie de commande)
    translate_cmd(&command, raw_cmd);

    execute_expression(command);

    destroy_cmd(command);
  } while(1);

  printf("\e[1;31m*****************\e[0m               \e[1;33mTerminé !\e[0m                \e[1;31m*****************\e[0m\n");
  return EXIT_SUCCESS;
}

/**
* @brief Fonction d'aide pour guider l'utilisateur dans l'utilisation du Mini-Shell.
*/
void print_help(){
  printf("Pour voir l'historique de vos commandes, tapez 'history'.\n");
  printf("Pour quitter le Mini-Shell, tapez 'exit' ou 'quit'.\n");
}
