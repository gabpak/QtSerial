#include "sqlmanagement.h"
#include <QSqlError>

SQLManagement::SQLManagement(QObject *parent)
    : QObject{parent}
{}

SQLManagement::~SQLManagement(){
	// Closing connexions :)
    closeConnection();
}

// Public method
bool SQLManagement::createConnection(){
    // Remplacez les paramètres par vos propres informations de connexion

    qDebug() << QSqlDatabase::drivers() << "\n";

    //_database = QSqlDatabase::addDatabase("QMYSQL"); // Changer avec SQLITE
    _database = QSqlDatabase::addDatabase("QSQLITE"); // Changer avec SQLITE
    if (!_database.open()) {
        qDebug() << _database.lastError();
        return false;
    }

    _database.setDatabaseName("database/rfid.db");
    QDir dir; qDebug() << dir.absolutePath();

    if (!_database.open()){
        qDebug() << "Error: connection with database failed";
        return false;
    }

    qDebug() << "Database: connection ok";

    showDatabase();

    return true;
}

bool SQLManagement::closeConnection(){
    if (_database.isOpen()) {
        _database.close();
		return true;
    }

    return false;
}

bool SQLManagement::showDatabase(){
    QList<QVariantMap> usersData;
    QSqlQuery query;

    if(query.prepare("SELECT id, rfid_id, prenom, nom, droit FROM users")){
        qDebug() << "PREPARE OK";
        if (!query.exec()) {
            qDebug() << "Error executing last query : " << query.lastError().text();
            return false;
        }
        while(query.next()){
            QVariantMap userData;
            userData["id"] = query.value("id");
            userData["rfid_id"] = query.value("rfid_id");
            userData["prenom"] = query.value("prenom");
            userData["nom"] = query.value("nom");
            userData["droit"] = query.value("droit");
            usersData.append(userData);
        }
        if (!query.next()) {
            qDebug() << "No rows returned";
        }
    }
    else{
        qDebug() << "PREPARE NOT OK";
        qDebug() << "Error preparing last query : " << query.lastError().text();
        return false;
    }

    qDebug() << "\nSHOW DATA";
    for(const QVariantMap &userData : usersData){
        qDebug() << "ID: " << userData["id"].toString();
        qDebug() << "RFID ID: " << userData["rfid_id"].toString();
        qDebug() << "Prenom: " << userData["prenom"].toString();
        qDebug() << "Nom: " << userData["nom"].toString();
        qDebug() << "Droit: " << userData["droit"].toString();
        qDebug() << "-----------";
    }

    return true;
}

// Get the informations based on the rfid_id
QString SQLManagement::getID(QString rfid_id){
    QList<QVariantMap> usersData;
    QSqlQuery query;

    if(query.prepare("SELECT id, rfid_id, prenom, nom, droit FROM users")){
        //qDebug() << "PREPARE OK";
        if (!query.exec()) {
            qDebug() << "Error executing last query : " << query.lastError().text();
            return "NULL";
        }
        while(query.next()){
            QVariantMap userData;
            userData["id"] = query.value("id");
            userData["rfid_id"] = query.value("rfid_id");
            userData["prenom"] = query.value("prenom");
            userData["nom"] = query.value("nom");
            userData["droit"] = query.value("droit");
            usersData.append(userData);
        }
        /*
        if (!query.next()) {
            qDebug() << "No rows returned";
        }
        */
    }
    else{
        qDebug() << "PREPARE NOT OK";
        qDebug() << "Error preparing last query : " << query.lastError().text();
        return "NULL";
    }

    for(const QVariantMap &userData : usersData){
        if(userData["rfid_id"].toString() == rfid_id){
            qDebug() << "UID FOUND IN THE DATABASE";
            return userData["id"].toString() + "|" +
                   userData["prenom"].toString() + "|" +
                   userData["nom"].toString() + "|" +
                   userData["droit"].toString();
        }
    }

    qDebug() << "UID NOT FOUND IN THE DATABASE";
    return "0|NULL|NULL|NULL";
    //return "NULL";
}














