#include "Voiture.hpp"

Voiture::Voiture() : id(0), vitesse_maximal(0.0), capacite_accelere(0.0), capacite_freinage(0.0), carburant_maximal(0.0), consommation(0.0) {}

Voiture::Voiture(int id, string model, double vitesse_maximal, double capacite_accelere, double capacite_freinage, double carburant_maximal, double consommation)
    : id(id), model(model), vitesse_maximal(vitesse_maximal), capacite_accelere(capacite_accelere), capacite_freinage(capacite_freinage), carburant_maximal(carburant_maximal), consommation(consommation) {}

void Voiture::setId(int id)
{
    if (id < 0)
        throw invalid_argument("L'ID ne peut pas être négatif.");
    this->id = id;
}

int Voiture::getId()
{
    return id;
}
vector<Evenement> Voiture::getEventTuplet()
{
    return this->events;
};
void Voiture::addEvents(Evenement event)
{
    events.push_back(event);
}

double Voiture::getVitesseInital()
{
    return this->vitesse_initial;
}
void Voiture::setVitesseInitial(double vitesse)
{
    this->vitesse_initial = vitesse;
}

double Voiture::getGamma()
{
    return gamma;
}
void Voiture::setGamma(double gamma)
{
    this->gamma = gamma;
}

double Voiture::getVitesse()
{
    return Voiture::vitesse;
}
void Voiture::setVitesse(double vitesse)
{
    Voiture::vitesse = vitesse;
}

void Voiture::setModel(string model)
{
    if (model.empty())
        throw invalid_argument("Le modèle ne peut pas être vide.");
    this->model = model;
}

string Voiture::getModel()
{
    return model;
}

void Voiture::setVitesseMaximal(double vitesse)
{
    if (vitesse <= 0)
        throw invalid_argument("La vitesse maximale doit être positive.");
    this->vitesse_maximal = vitesse;
}

double Voiture::getVitesseMaximal()
{
    return vitesse_maximal;
}

void Voiture::setCapaciteAccelere(double capacite)
{
    if (capacite <= 0)
        throw invalid_argument("La capacité d'accélération doit être positive.");
    this->capacite_accelere = capacite;
}

double Voiture::getCapaciteAccelere()
{
    return capacite_accelere;
}

void Voiture::setCapaciteFreinage(double capacite)
{
    if (capacite <= 0)
        throw invalid_argument("La capacité de freinage doit être positive.");
    this->capacite_freinage = capacite;
}

double Voiture::getCapaciteFreinage()
{
    return capacite_freinage;
}

void Voiture::setCarburantMaximal(double carburant)
{
    if (carburant <= 0)
        throw invalid_argument("Le carburant maximal doit être positif.");
    this->carburant_maximal = carburant;
}

double Voiture::getCarburantMaximal()
{
    return carburant_maximal;
}

void Voiture::setConsommation(double consommation)
{
    if (consommation <= 0)
        throw invalid_argument("La consommation doit être positive.");
    this->consommation = consommation;
}

double Voiture::getConsommation()
{
    return consommation;
}

// ========================== MÉTHODE `save()` ==========================
bool Voiture::save()
{
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "INSERT INTO voiture (id, model, vitesse_maximal, capacite_accelere, capacite_freinage, carburant_maximal, consommation) "
                   "VALUES (?, ?, ?, ?, ?, ?, ?)";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        sqlite3_bind_int(stmt, 1, id);
        sqlite3_bind_text(stmt, 2, model.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, vitesse_maximal);
        sqlite3_bind_double(stmt, 4, capacite_accelere);
        sqlite3_bind_double(stmt, 5, capacite_freinage);
        sqlite3_bind_double(stmt, 6, carburant_maximal);
        sqlite3_bind_double(stmt, 7, consommation);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            throw runtime_error("Erreur lors de l'insertion de la voiture.");
        }

        this->id = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);
        return true;
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans save(): " << e.what() << endl;
        return false;
    }
}

// ========================== MÉTHODE `getById()` ==========================
Voiture Voiture::getById(int id)
{
    Voiture voiture;
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "SELECT * FROM voiture WHERE id = ?";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            voiture.setId(sqlite3_column_int(stmt, 0));
            voiture.setModel(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
            voiture.setVitesseMaximal(sqlite3_column_double(stmt, 2));
            voiture.setCapaciteAccelere(sqlite3_column_double(stmt, 3));
            voiture.setCapaciteFreinage(sqlite3_column_double(stmt, 4));
            voiture.setCarburantMaximal(sqlite3_column_double(stmt, 5));
            voiture.setConsommation(sqlite3_column_double(stmt, 6));
        }

        sqlite3_finalize(stmt);
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans getById(): " << e.what() << endl;
    }

    return voiture;
}

// ========================== MÉTHODE `getAll()` ==========================
vector<Voiture> Voiture::getAll()
{
    vector<Voiture> voitures;
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "SELECT * FROM voiture";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Voiture voiture;
            voiture.setId(sqlite3_column_int(stmt, 0));
            voiture.setModel(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
            voiture.setVitesseMaximal(sqlite3_column_double(stmt, 2));
            voiture.setCapaciteAccelere(sqlite3_column_double(stmt, 3));
            voiture.setCapaciteFreinage(sqlite3_column_double(stmt, 4));
            voiture.setCarburantMaximal(sqlite3_column_double(stmt, 5));
            voiture.setConsommation(sqlite3_column_double(stmt, 6));
            voitures.push_back(voiture);
        }

        sqlite3_finalize(stmt);
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans getAll(): " << e.what() << endl;
    }

    return voitures;
}

// ========================== MÉTHODE `update()` ==========================
bool Voiture::update()
{
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "UPDATE voiture SET model = ?, vitesse_maximal = ?, capacite_accelere = ?, capacite_freinage = ?, carburant_maximal = ?, consommation = ? WHERE id = ?";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        sqlite3_bind_text(stmt, 1, model.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, vitesse_maximal);
        sqlite3_bind_double(stmt, 3, capacite_accelere);
        sqlite3_bind_double(stmt, 4, capacite_freinage);
        sqlite3_bind_double(stmt, 5, carburant_maximal);
        sqlite3_bind_double(stmt, 6, consommation);
        sqlite3_bind_int(stmt, 7, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            throw runtime_error("Erreur lors de la mise à jour de la voiture.");
        }

        sqlite3_finalize(stmt);
        return true;
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans update(): " << e.what() << endl;
        return false;
    }
}

// ========================== MÉTHODE `remove()` ==========================
bool Voiture::remove()
{
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "DELETE FROM voiture WHERE id = ?";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            throw runtime_error("Erreur lors de la suppression de la voiture.");
        }

        sqlite3_finalize(stmt);
        return true;
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans remove(): " << e.what() << endl;
        return false;
    }
}
