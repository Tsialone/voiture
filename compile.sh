#!/bin/bash

# Nom du script: run.sh
# Usage: ./run.sh [clean] [rebuild]

PROJECT_NAME="Voiture"
BUILD_DIR="build"
SRC_DIR="src"
INCLUDE_DIR="include"

# Fonction pour nettoyer le projet
clean_project() {
    echo "Nettoyage du projet..."
    rm -rf "$BUILD_DIR"
    echo "Nettoyage terminé."
}

# Fonction pour compiler le projet
compile_project() {
    echo "Compilation du projet..."
    
    # Créer le dossier build s'il n'existe pas
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR" || exit
    
    # Générer les fichiers de build avec CMake
    cmake ..
    
    # Compiler le projet
    if make -j$(nproc); then
        echo "Compilation réussie!"
    else
        echo "Échec de la compilation!"
        exit 1
    fi
    
    cd ..
}

# Fonction pour exécuter le projet
run_project() {
    echo "Exécution du programme..."
    "./$BUILD_DIR/$PROJECT_NAME"
}

# Gestion des arguments
if [[ "$1" == "clean" ]]; then
    clean_project
    exit 0
elif [[ "$1" == "rebuild" ]]; then
    clean_project
fi

# Compilation standard
compile_project

# Exécution si la compilation réussit
if [[ $? -eq 0 ]]; then
    run_project
fi