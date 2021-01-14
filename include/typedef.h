#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * \enum CMD_ACCESS
 * \brief Type de commande.
 * CMD_ACCESS est une série de constantes prédéfinie
 * sur le type de commande que l'utilisateur écrit dans le shell.
 */
typedef enum CMD_ACCESS{
  EXECUTE,
  EXECUTE_AND,
  EXECUTE_OR,
  PIPE
} CMD_ACCESS;

/**
 * \struct cmd_t
 * \brief Objet Commande.
 *
 * Structure récursive contenant des informations sur la commande.
 */
typedef struct cmd_t{
  int argc;
  char **argv;
  struct cmd_t *previousCMD ;
  CMD_ACCESS previousCMDAccess;
  char background;
} *pcmd_t;

#endif
