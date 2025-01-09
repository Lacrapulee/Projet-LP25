# Nom de l'exécutable
EXEC = bin/main

# Répertoire des sources des objets et du binaire 
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I$(SRC_DIR)
LDFLAGS = -lssl -lcrypto

# Fichiers source
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/file_handler.c $(SRC_DIR)/backup_manager.c $(SRC_DIR)/deduplication.c $(SRC_DIR)/tools.c

# Fichiers objets
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Règle par défaut
all: $(EXEC)

# Créer le répertoire obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Créer le répertoire bin s'il n'existe pas
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
 
# Lien de l'exécutable à partir des objets
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers .c en fichiers .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

# Supprime les fichiers générés par `make`
distclean: clean
	rm -f *~

# Refaire la compilation et le lien de l'exécutable
rebuild: clean all

# Afficher le message d'utilisation
help:
	@echo "Usage :"
	@echo "  make       - Compiler le programme"
	@echo "  make clean - Supprimer les fichiers générés"
	@echo "  make distclean - Supprimer les fichiers générés et les sauvegardes"
	@echo "  make rebuild - Recompiler le programme après avoir nettoyé"
