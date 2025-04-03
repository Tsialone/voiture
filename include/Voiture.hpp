#pragma once
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include "UtilDb.hpp"
#include <Evenement.hpp>
#include <vector>
#include <wx/wx.h>
#include <wx/timer.h>

using namespace std;

class Voiture
{
private:
    int id;
    string model;
    double vitesse = 0.0;

    double gamma = 0.0;

    double carburant_actuel;
    vector<Evenement> events;
    double vitesse_initial = 0.0;
    double vitesse_maximal;
    double capacite_accelere;
    double capacite_freinage;
    double carburant_maximal;
    double consommation;

public:
    double fixe_gamma_ac = 0.0;
    double fixe_gamma_fg = 0.0;
    double consommation_t = 0.0;

    double carburant_consomme = 0.0;

    double conso_fixe = 0.0;

    double position = 0.0;
    double position_initial = 0.0;
    // Constructeurs
    Voiture();
    Voiture(int id, string model, double vitesse_maximal, double capacite_accelere, double capacite_freinage, double carburant_maximal, double consommation);

    // Getters et Setters
    double getGamma();
    void setGamma(double gamma);
    double getCarburantActuel() const { return carburant_actuel; }
    void setCarburantActuel(double niveau)
    {
        carburant_actuel = std::max(0.0, std::min(carburant_maximal, niveau));
    }

    // Méthode pour consommer du carburant
    void consommerCarburant(double tempsEcoule)
    {
        double consommation = this->consommation * tempsEcoule * vitesse;
        carburant_actuel = std::max(0.0, carburant_actuel - consommation);
    }

    vector<Evenement> getEventTuplet();
    vector<wxStaticText *> event_text;

    // void

    void addEvents(Evenement event);

    void setId(int id);
    int getId();
    void setModel(string model);
    string getModel();
    void setVitesseMaximal(double vitesse);
    double getVitesseMaximal();
    double getVitesse();
    void setVitesse(double vitesse);
    void setCapaciteAccelere(double capacite);
    double getCapaciteAccelere();
    void setCapaciteFreinage(double capacite);
    double getCapaciteFreinage();
    void setCarburantMaximal(double carburant);
    double getCarburantMaximal();
    void setConsommation(double consommation);
    double getConsommation();

    double getVitesseInital();
    void setVitesseInitial(double vitesse);

    // CRUD Methods
    bool save();                     // Create
    static Voiture *getById(int id); // Read by ID
    static vector<Voiture> getAll(); // Read all
    bool update();                   // Update
    bool remove();                   // Delete
};
