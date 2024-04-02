#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QObject>
#include <QSerialPort>
#include <QSqlQuery> // TODO

class Seriallink : public QObject
{
    Q_OBJECT
public:
    explicit Seriallink(const QString &com = "COM1", QSerialPort::BaudRate baudrate = QSerialPort::BaudRate::Baud9600, QObject *parent = nullptr);
    ~Seriallink();

    bool openConnection();
    void closeConnection();
    void write(const char* messageToWrite);
    bool isOpen();
    bool isWritable();
    bool isReadable();

    // Getter
    QByteArray getSerialData();

signals:
    QByteArray gotNewData(QByteArray data);

private:
    void read(); // Called when a signal is received

    QSerialPort _serial;
    QByteArray dataReceived;

};

#endif // SERIALLINK_H
