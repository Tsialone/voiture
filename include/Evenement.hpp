#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include "UtilDb.hpp" // Assure-toi que ce fichier contient la gestion de la connexion à la base de données

using namespace std;

class Evenement
{
private:
    int id;
    int id_voiture;
    double vitesse_initial;
    double gamma;
    string temp_debut;  // Format : HH:MM:SS

public:
    // Constructeurs
    Evenement();
    Evenement(int id, int id_voiture, double vitesse_initial, double gamma, string temp_debut);

    // Getters et Setters
    void setId(int id);
    int getId();

    void setIdVoiture(int id_voiture);
    int getIdVoiture();

    void setVitesseInitial(double vitesse);
    double getVitesseInitial();

    void setGamma(double gamma);
    double getGamma();

    void setTempDebut(string temp_debut);
    string getTempDebut();

    bool save();
    bool update();
    bool remove();

    static vector<Evenement> getAll(); // Récupère tous les événements
    static Evenement getById(int id); // Récupère un événement par ID
};
