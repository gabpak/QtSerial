#include <QDebug>
#include "seriallink.h"

seriallink::seriallink(const QString &com, QSerialPort::BaudRate baudrate, QObject *parent)
    : QObject{parent}
{
    // Initialisation of the serial communication
    _serial.setPortName(com);
    _serial.setBaudRate(baudrate);
    // Per default
    _serial.setDataBits(QSerialPort::Data8);
    _serial.setParity(QSerialPort::NoParity);
    _serial.setStopBits(QSerialPort::OneStop);
    _serial.setFlowControl(QSerialPort::NoFlowControl);

                            // Signal                       // Slot
    connect(&_serial, &QSerialPort::readyRead, this, &seriallink::read);
}

seriallink::~seriallink(){
    closeConnection();
}

bool seriallink::openConnection(){
    if(!_serial.open(QIODevice::ReadWrite)){
        qDebug() << "Connexion not possible";
        return false;
    }
    else{
        qDebug() << "Connexion ok";
        return true;
    }
}


void seriallink::closeConnection(){
    _serial.close();
}

void seriallink::write(const char* messageToWrite){
    _serial.write(messageToWrite);
}

bool seriallink::isOpen(){
    return _serial.isOpen();
}

bool seriallink::isWritable(){
    return _serial.isWritable();
}

bool seriallink::isReadable(){
    return _serial.isReadable();
}

void seriallink::read(){
    QByteArray dataReceived = _serial.readAll();
    qDebug() << dataReceived;
    emit gotNewData(dataReceived);
}
