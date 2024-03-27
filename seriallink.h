#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QObject>
#include <QSerialPort>
#include <QComboBox>
#include <QPushButton>
#include <QSqlQuery> // TODO

class seriallink : public QObject
{
    Q_OBJECT
public:
    explicit seriallink(const QString &com = "COM1", QSerialPort::BaudRate baudrate = QSerialPort::BaudRate::Baud9600, QObject *parent = nullptr);
    ~seriallink();

    bool openConnection();
    void closeConnection();
    void write(const char* messageToWrite);
    bool isOpen();
    bool isWritable();
    bool isReadable();
    void read(); // Called when a signal is received

signals:
    void gotNewData(QByteArray data);

private:
    QSerialPort _serial;

};

#endif // SERIALLINK_H
