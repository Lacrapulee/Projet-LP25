# Documentation du projet de sauvegarde incrémentale

## Contexte

Le projet **LP25 - A2024** consiste à développer une solution de sauvegarde incrémentale d'un répertoire source vers un répertoire cible, en utilisant un programme écrit en langage C. Le système de sauvegarde est inspiré de Borg Backup et met l'accent sur la déduplication des données. Le projet inclut également la possibilité de sauvegarder sur un serveur distant via des sockets, bien que cette fonctionnalité n'ait pas été implémentée à cause de contraintes organisationnelles. 

Le projet a été mené de manière individuelle, après que les autres membres du groupe n'ont pas pu participer de manière significative. Malgré cela, l'outil de sauvegarde a été développé de manière fonctionnelle et complète, en se concentrant principalement sur la gestion des fichiers locaux, la déduplication et l'optimisation de l'espace de stockage.

---

## Objectifs principaux

Les objectifs du projet étaient les suivants :

- **Sauvegarde incrémentale** : Sauvegarder uniquement les fichiers ayant changé depuis la dernière sauvegarde.
- **Déduplication des données** : Identifier et supprimer les doublons dans les sauvegardes en utilisant des techniques de hachage (MD5).
- **Sauvegarde locale ou réseau** : Permettre la sauvegarde et la restauration de fichiers à la fois localement et potentiellement sur un serveur distant via des connexions réseau.
  
---

## Architecture du projet

Le projet est organisé en plusieurs modules, chacun ayant une fonction bien définie.

### Modules

1. **`file_handler`** : 
    - Gère les opérations de fichier telles que la lecture, l'écriture, l'énumération des fichiers dans un répertoire et la gestion des répertoires.
2. **`deduplication`** : 
    - Effectue la déduplication des fichiers en les découpant en chunks, calculant leurs sommes de contrôle MD5, et en comparant les hachages pour identifier les données redondantes.
3. **`backup_manager`** : 
    - Implémente la logique principale de la sauvegarde incrémentale, notamment la gestion de la création, de la suppression et de la mise à jour des sauvegardes.
4. **`network`** : 
    - Bien que non implémenté dans ce projet, ce module était destiné à gérer les opérations réseau, telles que l'envoi de fichiers et la récupération de sauvegardes à partir d'un serveur distant via des sockets TCP.

```bash
projet_lp25/
│
├── src/
│   ├── main.c
│   ├── file_handler.c
│   ├── file_handler.h
│   ├── deduplication.c
│   ├── deduplication.h
│   ├── backup_manager.c
│   ├── backup_manager.h
│   ├── network.c
│   └── network.h
├── Makefile
└── README.md
```

---

## Fonctionnalités principales

### Sauvegarde incrémentale

La sauvegarde incrémentale consiste à ne sauvegarder que les fichiers modifiés depuis la dernière sauvegarde, optimisant ainsi l'utilisation de l'espace de stockage. Le programme suit les étapes suivantes pour réaliser une sauvegarde :

- **Création de la sauvegarde** : Si aucune sauvegarde précédente n'existe, une sauvegarde complète est effectuée. Dans les sauvegardes suivantes, seules les modifications depuis la dernière sauvegarde sont prises en compte.
- **Comparaison des fichiers** : Les fichiers de la source et de la destination sont comparés en fonction de leur timestamp, de leur taille et de leur contenu pour déterminer si une mise à jour est nécessaire.
- **Déduplication** : Lors de chaque sauvegarde, les fichiers sont découpés en blocs de données (chunks) et les blocs identiques sont évités pour économiser de l'espace. Seuls les blocs uniques sont sauvegardés, et les autres sont référencés.

### Restauration

L'option `--restore` permet de restaurer les fichiers à partir de la sauvegarde, en les copiant dans le répertoire de destination.

1. Le programme vérifie si la sauvegarde spécifiée existe.
2. Les fichiers sont extraits et copiés dans la destination.
3. Des comparaisons sont effectuées pour s'assurer que les fichiers sont correctement restaurés (en fonction de la date de modification et de la taille).

### Liste des sauvegardes

L'option `--list-backups` permet d'afficher toutes les sauvegardes disponibles, que ce soit localement ou sur un serveur distant.

---

## Options du programme

Le programme propose plusieurs options pour gérer les sauvegardes :

- **`--backup`** : Crée une nouvelle sauvegarde.
- **`--restore`** : Restaure une sauvegarde existante.
- **`--list-backups`** : Affiche toutes les sauvegardes disponibles.
- **`--dry-run`** : Effectue un test de sauvegarde ou de restauration sans modification réelle.
- **`--d-server`** : Spécifie l'adresse IP du serveur de destination pour la sauvegarde.
- **`--s-server`** : Spécifie l'adresse IP du serveur source pour la restauration.
- **`--dest`** : Spécifie le chemin de destination de la sauvegarde ou de la restauration.
- **`--source`** : Spécifie le chemin source de la sauvegarde ou de la restauration.
- **`--verbose`** : Affiche des informations détaillées sur l'exécution du programme.

### Exemples d'utilisation

```bash
# Créer une sauvegarde
./main --backup --source /path/to/source --dest /path/to/destination

# Restaurer une sauvegarde
./main --restore aaaa.mm.jj.hh.min.ss.mmmmm --source /path/to/source --dest /path/to/destination

# Lister les sauvegardes disponibles
./main --list-backups --dest /path/to/destination

# Effectuer une sauvegarde en mode "dry run" (test sans effectuer de modifications)
./main --backup --source /path/to/source --dest /path/to/destination --dry-run
```

---

## Problèmes rencontrés et solutions

### Manque d'organisation pour le réseau
Le module réseau pour la sauvegarde distante n'a pas pu être implémenté en raison d'un manque d'organisation au sein de l'équipe. Bien que cette fonctionnalité ait été planifiée, la mise en place d'une communication réseau via des sockets TCP n'a pas pu être réalisée dans les délais.

Cependant, la version actuelle du projet permet la gestion locale des sauvegardes et peut être étendue avec des fonctionnalités réseau dans de futures versions.

### Déduplication des données
Le processus de déduplication a été mis en œuvre en découpant les fichiers en chunks et en calculant leur somme MD5. Bien que cette méthode permette d'éviter les doublons, elle peut entraîner une surcharge mémoire et des coûts de performance élevés pour des fichiers volumineux. À l'avenir, il serait utile d'optimiser l'algorithme pour gérer les fichiers de très grande taille de manière plus efficace.

### Problème de fichier de hashage 
Dans le projet, le système est conçu de sorte que si un chunk a déjà été sauvegardé, son hachage MD5 est enregistré dans un fichier nommé hashes.dat. Cependant, lorsqu'un utilisateur restaure un fichier, puis le resauvegarde, il peut arriver que le fichier ne soit pas correctement sauvegardé, car son hachage est déjà présent dans hashes.dat.

---

## Points notables de l'implémentation

- **Utilisation de `gettimeofday`, `localtime`, et `strftime` pour gérer les dates** : Le programme utilise ces fonctions pour générer des noms de sauvegardes uniques basés sur la date et l'heure.
- **Suppression sécurisée avec `unlink`** : Les fichiers obsolètes sont supprimés en utilisant `unlink`, ce qui assure la suppression correcte des fichiers.

---

## Conclusion

Bien que le module réseau n'ait pas pu être implémenté à cause de contraintes organisationnelles, le projet présente une solution complète de gestion des sauvegardes incrémentales avec déduplication des données. Le programme est capable de réaliser des sauvegardes locales efficaces, tout en optimisant l'espace de stockage. De futures améliorations pourraient inclure l'extension de la gestion des sauvegardes distantes, ainsi que l'optimisation de la déduplication pour les fichiers volumineux.

---

## Annexes

### Convention de code

Le projet suit un certain nombre de conventions de codage, dont les principales sont :

- **Indentation** : Utilisation d'un nombre cohérent d'espaces pour l'indentation dans tout le projet.
- **Nommage des variables et fonctions** : Utilisation du *snake_case* pour nommer les variables, fonctions et types.
- **Gestion des pointeurs** : Les pointeurs sont déclarés avec une étoile séparée du type et collée au nom de la variable, par exemple `int *a`.
