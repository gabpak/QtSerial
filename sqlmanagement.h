#ifndef SQLMANAGEMENT_H
#define SQLMANAGEMENT_H

#include <QObject>
//#include <QSqlDatabase>
//#include <QSqlQuery>
#include <QtSql>
#include <QDebug>

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

private:
    QSqlDatabase _database;
};

#endif // SQLMANAGEMENT_H
