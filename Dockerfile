# Utiliser une image de base avec Apache et PHP
FROM php:8.2-apache

# Copier les fichiers de l'application dans le conteneur
COPY . /var/www/html/

# Exposer le port 80
EXPOSE 80

# Définir le répertoire de travail
WORKDIR /var/www/html/

# Optionnel : Installer des extensions PHP si nécessaire
RUN docker-php-ext-install mysqli pdo pdo_mysql