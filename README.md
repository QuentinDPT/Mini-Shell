# Mini-Shell

Développé par Quentin de POTTER & Alexis VELTEN.

## Cahier des charges

Contraintes du cahier des charges (document "Sujet Projet Mini-Shell")

### Fonctions métiers attendus

- [x] FM01 - Le binaire est capable d’exécuter une commande simple (ie:ls –l; ps; who)
- [x] FM02 - Le binaire est capable d’exécuter un sous-ensemble de plusieurs commandes de sorte à prendre en compte
  - [x] Les opérateurs de controle : **&&** et **||**
  - [ ] Les redirections de flux simple : **|**, **>**, **<**, **>>**, **<<**
  - [x] L'execution en arrière plan : **&**
- [x] FM03 - L’exécution des commandes internes (fonctionnalités built-in) suivantes
  - [x] **cd**
  - [x] **pwd**
  - [x] **exit**
  - [x] **echo**
- [x] FM04 - La persistance des commandes saisie dans un fichier (historique)

### Fonctionnalités optionnels

- [ ] FO01 – La réalisation d’un mode batch (ie: ./my_shell –c «ls –al | grep toto»)
- [ ] FO02 – La création de variables d’environnement
- [ ] FO03 – La prise en charge d’alias

### Exigences techniques

- [x] CT01 – La compilation du projet doit se faire via un Makefile.
- [x] CT02 – La définitions des structures doit se faire dans un fichier typedef.h.
- [x] CT03 – La définition des méthodes protoype (.h) & implémentation (.c) doit se faire de manière séparéeautant que faire se peut.
- [x] CT04 – Le code produit doit être documenté.
- [x] CT05 – La gestion des erreurs doitse faire via «les mécanismes proposés parerrno».
- [x] CT06 – Les éventuels résultats temporaires ne seront pas persistés sur le système de fichier.

### Contraintes techniques optionnelles

- [x] CTO01 – La documentation du code générée via l’utilitaire doxygen.
- [ ] CTO02 – Le code est soumis à un contrôle de couverture via l’utilitaire gcov.
- [ ] CTO03 – Une page de manuel Linux est rédigée pour détailler l’exécution du shell.
