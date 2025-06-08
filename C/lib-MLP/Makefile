# Nom de l'exécutable
EXEC = mlp

# Compilateur à utiliser
CC = gcc

# Options de compilation
CFLAGS = -Wall -g -O2

# Bibliothèques à lier
LIBS = -lm

# Fichiers sources
SOURCES = mlp.c main.c

# Fichiers objets
OBJECTS = $(SOURCES:.c=.o)

# Règle par défaut : compiler l'exécutable
all: $(EXEC)

# Règle pour créer l'exécutable
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)

# Règle pour compiler les fichiers sources en objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers générés
clean:
	rm -f $(OBJECTS) $(EXEC)

# Exécuter le programme
run: $(EXEC)
	./$(EXEC)

# Dépendances des fichiers objets
main.o: main.c mlp.h
mlp.o: mlp.c mlp.h

.PHONY: all clean run
