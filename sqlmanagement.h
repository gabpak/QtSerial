#ifndef SQLMANAGEMENT_H
#define SQLMANAGEMENT_H

#include <QObject>
#include <QtSql>
#include <QDebug>

//#include <QSqlDatabase>
//#include <QSqlQuery>

/*
*   Databse information:
*   username: root
*   password:
*   structure: rfid
*   table: users
*   add user: INSERT INTO users (id_rfid, prenom, nom, droit, nbr_absences) VALUES ('ABGEYF9Y', 'Gabin', 'Paques', true, 4);
*/

class SQLManagement : public QObject
{
    Q_OBJECT
public:
    explicit SQLManagement(QObject *parent = nullptr);
    ~SQLManagement();

	// Methods
    bool createConnection();
    bool closeConnection();
    bool showDatabase(); // Just for debug :)
    QString getID(QString id); // Get the informations based on the rfid_id

private:
    QSqlDatabase _database;
};

#endif // SQLMANAGEMENT_H
