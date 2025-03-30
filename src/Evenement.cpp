#include "Evenement.hpp"

Evenement::Evenement() : id(0), id_voiture(0), vitesse_initial(0.0), gamma(0.0), temp_debut("") {}

Evenement::Evenement(int id, int id_voiture, double vitesse_initial, double gamma, string temp_debut)
    : id(id), id_voiture(id_voiture), vitesse_initial(vitesse_initial), gamma(gamma), temp_debut(temp_debut) {}

void Evenement::setId(int id)
{
    if (id < 0)
        throw invalid_argument("L'ID ne peut pas être négatif.");
    this->id = id;
}

int Evenement::getId()
{
    return id;
}

void Evenement::setIdVoiture(int id_voiture)
{
    if (id_voiture < 0)
        throw invalid_argument("L'ID de la voiture ne peut pas être négatif.");
    this->id_voiture = id_voiture;
}

int Evenement::getIdVoiture()
{
    return id_voiture;
}

void Evenement::setVitesseInitial(double vitesse)
{
    if (vitesse <= 0)
        throw invalid_argument("La vitesse initiale doit être positive.");
    this->vitesse_initial = vitesse;
}

double Evenement::getVitesseInitial()
{
    return vitesse_initial;
}

void Evenement::setGamma(double gamma)
{
    if (gamma <= 0)
        throw invalid_argument("Le gamma doit être positif.");
    this->gamma = gamma;
}

double Evenement::getGamma()
{
    return gamma;
}

void Evenement::setTempDebut(string temp_debut)
{
    this->temp_debut = temp_debut;
}

string Evenement::getTempDebut()
{
    return temp_debut;
}

// ========================== MÉTHODE `save()` ==========================
bool Evenement::save()
{
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    // Ajout de l'ID dans la requête SQL
    string query = "INSERT INTO evenement (id, id_voiture, vitesse_initial, gamma, temp_debut) "
                   "VALUES (?, ?, ?, ?, ?)";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        // Bind des valeurs
        sqlite3_bind_int(stmt, 1, id);           // L'ID de l'événement
        sqlite3_bind_int(stmt, 2, id_voiture);   // ID de la voiture associée à l'événement
        sqlite3_bind_double(stmt, 3, vitesse_initial); // Vitesse initiale
        sqlite3_bind_double(stmt, 4, gamma);     // Gamma
        sqlite3_bind_text(stmt, 5, temp_debut.c_str(), -1, SQLITE_STATIC); // Temps de début

        // Exécution de la requête
        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            throw runtime_error("Erreur lors de l'insertion de l'événement.");
        }

        // Si l'insertion est réussie, on met à jour l'ID de l'événement
        this->id = sqlite3_last_insert_rowid(db); // Récupérer l'ID généré
        sqlite3_finalize(stmt);
        return true;
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans save(): " << e.what() << endl;
        return false;
    }
}

// ========================== MÉTHODE `update()` ==========================
bool Evenement::update()
{
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "UPDATE evenement SET id_voiture = ?, vitesse_initial = ?, gamma = ?, temp_debut = ? WHERE id = ?";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        sqlite3_bind_int(stmt, 1, id_voiture);
        sqlite3_bind_double(stmt, 2, vitesse_initial);
        sqlite3_bind_double(stmt, 3, gamma);
        sqlite3_bind_text(stmt, 4, temp_debut.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            throw runtime_error("Erreur lors de la mise à jour de l'événement.");
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
bool Evenement::remove()
{
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "DELETE FROM evenement WHERE id = ?";

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
            throw runtime_error("Erreur lors de la suppression de l'événement.");
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

// ========================== MÉTHODE `getAll()` ==========================
vector<Evenement> Evenement::getAll()
{
    vector<Evenement> evenements;
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "SELECT * FROM evenement";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Evenement evenement;
            evenement.setId(sqlite3_column_int(stmt, 0));
            evenement.setIdVoiture(sqlite3_column_int(stmt, 1));
            evenement.setVitesseInitial(sqlite3_column_double(stmt, 2));
            evenement.setGamma(sqlite3_column_double(stmt, 3));
            evenement.setTempDebut(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
            evenements.push_back(evenement);
        }

        sqlite3_finalize(stmt);
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans getAll(): " << e.what() << endl;
    }

    return evenements;
}

// ========================== MÉTHODE `getById()` ==========================
Evenement Evenement::getById(int id)
{
    Evenement evenement;
    sqlite3 *db = UtilDb::getCon();
    sqlite3_stmt *stmt;

    string query = "SELECT * FROM evenement WHERE id = ?";

    try
    {
        if (!db)
            throw runtime_error("Connexion à la base de données non établie.");

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            throw runtime_error("Erreur SQLite: " + string(sqlite3_errmsg(db)));

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            evenement.setId(sqlite3_column_int(stmt, 0));
            evenement.setIdVoiture(sqlite3_column_int(stmt, 1));
            evenement.setVitesseInitial(sqlite3_column_double(stmt, 2));
            evenement.setGamma(sqlite3_column_double(stmt, 3));
            evenement.setTempDebut(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
        }

        sqlite3_finalize(stmt);
    }
    catch (exception &e)
    {
        cerr << "Exception capturée dans getById(): " << e.what() << endl;
    }

    return evenement;
}
