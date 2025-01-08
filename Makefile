<<<<<<< Updated upstream
CC = gcc
CFLAGS = -Wall -Wextra -I./src
SRC = src/main.c src/file_handler.c src/deduplication.c src/backup_manager.c src/network.c
OBJ = $(SRC:.c=.o)
=======
# Nom de l'exécutable
EXEC = main

# Répertoires pour les sources, objets et binaires
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Compilateur et options de compilation
CC = gcc
CPPFLAGS = -I$(SRC_DIR)
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lssl -lcrypto
>>>>>>> Stashed changes

all: lp25_borgbackup

cborgbackup: $(OBJ)
    $(CC) -o $@ $^

<<<<<<< Updated upstream
clean:
    rm -f $(OBJ) lp25_borgbackup
=======
# Chemin complet de l'exécutable
TARGET = $(BIN_DIR)/$(EXEC)

# Règle par défaut
all: $(TARGET)

# Création des répertoires nécessaires
prepare_dirs: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@if [ ! -d $@ ]; then rm -f $@; mkdir -p $@; fi

# Lien de l'exécutable à partir des objets
$(TARGET): $(OBJ) | prepare_dirs
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Compilation réussie. Exécutable généré : $@"

# Compilation des fichiers .c en fichiers .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)

# Supprime les fichiers générés et les répertoires
distclean: clean
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Refaire la compilation et le lien de l'exécutable
rebuild: clean all

# Afficher le message d'utilisation
help:
	@echo "Usage :"
	@echo "  make         - Compiler le programme"
	@echo "  make clean   - Supprimer les fichiers générés"
	@echo "  make distclean - Supprimer les fichiers générés et les répertoires"
	@echo "  make rebuild - Recompiler le programme après avoir nettoyé"

.PHONY: all clean distclean rebuild help prepare_dirs
>>>>>>> Stashed changes
