/**
* \file command.c
* \brief Fichier contenant les fonctions liés à l'interpretation et l'éxécution des commandes.
* \author Alexis VELTEN & Quentin DE POTTER
* \version 1.0
* \date 15 janvier 2021
*
* Programme regroupant les fonctionnalités du Mini-Shell
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
#include <time.h>

#include "../include/command.h"

/**
* @brief Fonction d'initialisation d'une nouvelle instance d'un objet pcmd_t.
* @return Nouvelle instance de type pcmd_t.
*/
pcmd_t create_cmd(){
  pcmd_t cmd = malloc(sizeof(struct cmd_t));
  cmd->argc = 0;
  cmd->argv = calloc(CMD_PARAMETERS,sizeof(char*));
  cmd->previousCMD = NULL;
  cmd->background = 0;
  return cmd;
}

/**
* @brief Fonction de suppression d'une instance d'un objet pcmd_t.
* @param cmd Instance d'un objet de type pcmd_t voulant être supprimer.
*/
void destroy_cmd(pcmd_t cmd){
  if(cmd!=NULL){
    destroy_cmd(cmd->previousCMD);
    free(cmd->previousCMD);
  }
}

/**
* @brief Fonction permettant de compter le nombre de commande dans un objet de type pcmd_t
* @param cmd Instance d'un objet de type pcmd_t voulant être compter
* @return Retourne le nombre de commande.
*/
int count_cmd(pcmd_t cmd){
  if(cmd == NULL) return 0;
  return count_cmd(cmd->previousCMD) +1;
}

/**
* @brief Fonction permettant d'interpréter une ligne de commande rentrer
* par l'utilisateur pour la transformer en plusieurs commandes.
* @param cmd Instance d'un objet de type pcmd_t voulant être interpréter.
* @param raw_cmd Chaîne de caractère contenant la ligne de commande.
*/
void translate_cmd(pcmd_t *cmd, char *raw_cmd){
  pcmd_t nextCMD ;

  (*cmd)->argv[0] = raw_cmd;
  (*cmd)->argc = 0;

  char entreGuillmets = 0 ;

  int i=0 ;

  while(raw_cmd[i] != 0){
    switch(raw_cmd[i]){
      case ' ':
      if(entreGuillmets == 0){
        raw_cmd[i] = 0;
        if((*cmd)->argc == 0){
          (*cmd)->argv[(*cmd)->argc] = raw_cmd+i+1 ;
        }else{
          if((*cmd)->argv[(*cmd)->argc-1][0] == 0)
          (*cmd)->argc = (*cmd)->argc-1 ;
          (*cmd)->argv[(*cmd)->argc] = raw_cmd+i+1;
          (*cmd)->argc = (*cmd)->argc+1 ;
        }
      }
      break ;
      case '"' :
      entreGuillmets = entreGuillmets == 0 ;
      raw_cmd[i] = 0;
      if(entreGuillmets != 0)
      (*cmd)->argv[(*cmd)->argc-1] = raw_cmd+i+1;
      break ;
      case '\n':
      raw_cmd[i] = 0;
      if((*cmd)->argc != 0 && (*cmd)->argv[(*cmd)->argc-1][0] == 0){
        (*cmd)->argc = (*cmd)->argc-1 ;
        (*cmd)->argv[(*cmd)->argc] = NULL ;
      }
      break ;
      case ';':
      raw_cmd[i] = 0;
      if((*cmd)->argc != 0 && (*cmd)->argv[(*cmd)->argc-1][0] == 0){
        (*cmd)->argc = (*cmd)->argc-1 ;
        (*cmd)->argv[(*cmd)->argc] = NULL ;
      }

      nextCMD = create_cmd();
      nextCMD->previousCMD = *cmd;
      nextCMD->previousCMDAccess = EXECUTE ;

      *cmd = nextCMD;
      (*cmd)->argv[0] = raw_cmd+i+1 ;

      raw_cmd[i] = 0;

      break ;
      case '&':
      if(i > 0 && raw_cmd[i-1] == '&'){
        raw_cmd[i] = 0;
        raw_cmd[i-1] = 0;
        if((*cmd)->argc != 0 && (*cmd)->argv[(*cmd)->argc-1][0] == 0){
          (*cmd)->argc = (*cmd)->argc-1 ;
          (*cmd)->argv[(*cmd)->argc] = NULL ;
        }

        nextCMD = create_cmd();
        nextCMD->previousCMD = *cmd;
        nextCMD->previousCMDAccess = EXECUTE_AND ;

        *cmd = nextCMD;
        (*cmd)->argv[0] = raw_cmd+i+1 ;
      }else if(raw_cmd[i+1] != '&'){
        raw_cmd[i] = 0;
        (*cmd)->background = 1 ;

        if((*cmd)->argc==0)
        (*cmd)->argv[0] = raw_cmd+i+1 ;
      }
      break;
      case '|' :
      if(i > 0 && raw_cmd[i-1] == '|'){
        raw_cmd[i] = 0;
        raw_cmd[i-1] = 0;
        if((*cmd)->argc != 0 && (*cmd)->argv[(*cmd)->argc-1][0] == 0){
          (*cmd)->argc = (*cmd)->argc-1 ;
          (*cmd)->argv[(*cmd)->argc] = NULL ;
        }

        nextCMD = create_cmd();
        nextCMD->previousCMD = *cmd;
        nextCMD->previousCMDAccess = EXECUTE_OR ;

        *cmd = nextCMD;
        (*cmd)->argv[0] = raw_cmd+i+1 ;

        raw_cmd[i] = 0;
        raw_cmd[i-1] = 0;
      }
      break ;
      default:
      if((*cmd)->argc == 0)
      (*cmd)->argc = (*cmd)->argc+1 ;
    }
    i++;
  }
}

/**
* @brief Fonction récursive permettant l'éxécution d'une ligne de commande.
* @param cmd Instance d'un objet de type pcmd_t voulant être éxécuter.
* @return Retourne le code retour du processus éxécutant la commande en question.
*/
int execute_expression(pcmd_t cmd){
  if(cmd == NULL) return 0;

  if(cmd->previousCMDAccess == EXECUTE){
    execute_expression(cmd->previousCMD);
    return execute_single_command(cmd->argv, cmd->background);
  }
  if(cmd->previousCMDAccess == EXECUTE_AND){
    int retour = execute_expression(cmd->previousCMD);
    if(retour == 0) return execute_single_command(cmd->argv, cmd->background);
    return retour;
  }
  if(cmd->previousCMDAccess == EXECUTE_OR){
    int retour = execute_expression(cmd->previousCMD);
    if(retour != 0) return execute_single_command(cmd->argv, cmd->background);
    return retour;
  }
}

/**
* @brief Fonction permettant l'éxécution d'une commande.
* @param cmd Instance d'un objet de type pcmd_t voulant être éxécuter.
* @param cmdBg
* @return Retourne le code retour du processus éxécutant la commande en question.
*/
int execute_single_command(char *const argv[], char background){
  if(strcmp(argv[0],"history") == 0){
    execute_history(background);
  } else if (strcmp(argv[0],"cd") == 0){
    change_directory(argv);
  } else {
    pid_t childPID = fork();
    if(childPID == 0){
      // enfant
      child_execute_cmd(argv);
    } else {
      // parent
      if(background){
        return childPID;
      } else {
        int child_status;
        waitpid(childPID, &child_status, 0);
        return child_status ;
      }
    }
  }
}

/**
* @brief Fonction permettant l'éxécution d'une commande par un processus fils.
* @param argv Liste de chaîne de caractère contenant la commande a éxécuter.
* @return Retourne le code retour du processus fils éxécutant la commande en question.
*/
int child_execute_cmd(char *const argv[]){
  execvp(argv[0],argv);
  exit(1);
}

/**
* @brief Fonction permettant l'écriture d'une commande dans le fichier historique.
* @param cmd Commande voulant être ajouter dans l'hi
*/
void add_cmd(const char *cmd){
  FILE* history = fopen(HISTORY_FILE,"a");
  if(cmd != NULL && cmd[0] != 0)
  fprintf(history,cmd);
  fclose(history);
}

/**
* @brief Fonction permettant l'éxécution de la commande "history"
* @param background Caractère permettant l'éxécution en arrière plan
* @return Retourne le code retour du processus fils éxécutant la commande en question.
*/
int execute_history(char background){
  // enfant
  pid_t childPID = fork();
  if(childPID == 0){
    // enfant
    execlp("cat","cat",HISTORY_FILE,(char *)NULL);
  }else{
    // parent
    if(background){
      return childPID;
    }else{
      int child_status;
      waitpid(childPID, &child_status, 0);
      return child_status ;
    }
  }
}

/**
* @brief Fonction permettant l'éxécution de la commande cd.
* @param argv Liste de chaîne de caractère contenant la commande cd et ses arguments.
*/
void change_directory(char* argv[]){
  if (argv[1] == NULL) {
    chdir(getenv("HOME"));
  } else {
    if (chdir(argv[1]) == -1) {
      printf(" %s: No such directory\n", argv[1]);
    }
  }
}
