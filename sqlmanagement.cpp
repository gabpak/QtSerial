#include "sqlmanagement.h"
#include <QSqlError>

SQLManagement::SQLManagement(QObject *parent)
    : QObject{parent}
{}

SQLManagement::~SQLManagement(){
	// Closing connexions :)
    closeConnection();
}

// Public methods
bool SQLManagement::createConnection(){
    // Remplacez les paramètres par vos propres informations de connexion
    qDebug() << QSqlDatabase::drivers();
    _database = QSqlDatabase::addDatabase("QMYSQL");
    _database.setHostName("localhost:3306");
    _database.setDatabaseName("rfid");
    _database.setUserName("root");
    _database.setPassword("");

    if (!_database.open()) {
        qDebug() << _database.lastError();
        return false;
    }

    qDebug() << "Connected to the database successfully!";
    return true;
}

bool SQLManagement::closeConnection(){
    if (_database.isOpen()) {
        _database.close();
		return true;
    }

    return false;
}


