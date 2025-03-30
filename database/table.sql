CREATE TABLE Voiture (
    id INT PRIMARY KEY ,
    model VARCHAR(255) NOT NULL,
    vitesse_maximal DECIMAL (10,2) NOT NULL,
    capacite_accelere DECIMAL (10,2) NOT NULL,
    capacite_freinage DECIMAL (10 ,2) NOT NULL,
    carburant_maximal DECIMAL (10 ,2) NOT NULL,
    consommation DECIMAL (10 ,2 ) NOT NULL
);

CREATE TABLE Evenement (
    id INT PRIMARY KEY ,
    id_voiture INT ,
    vitesse_initial DECIMAL (10 ,2),
    gamma DECIMAL(10 ,2),
    temp_debut TIME NOT NULL,
    FOREIGN KEY (id_voiture) REFERENCES Voiture(id)
);