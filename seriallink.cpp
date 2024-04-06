#include <QDebug>
#include "Seriallink.h"

// Constructor
Seriallink::Seriallink(const QString &com, QSerialPort::BaudRate baudrate, QObject *parent)
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
    connect(&_serial, &QSerialPort::readyRead, this, &Seriallink::read);
}

// Destructor
Seriallink::~Seriallink(){
    closeConnection();
}

// Public
bool Seriallink::openConnection(){
    if(!_serial.open(QIODevice::ReadWrite)){
        qDebug() << "Connexion not possible";
        return false;
    }
    else{
        qDebug() << "Connexion ok";
        return true;
    }
}


// Public
void Seriallink::closeConnection(){
    _serial.close();
}

// Public
void Seriallink::write(const char* messageToWrite){
    _serial.write(messageToWrite);
}

// Public
bool Seriallink::isOpen(){
    return _serial.isOpen();
}

// Public
bool Seriallink::isWritable(){
    return _serial.isWritable();
}

// Public
bool Seriallink::isReadable(){
    return _serial.isReadable();
}

// Public
QByteArray Seriallink::getSerialData(){
    return this->dataReceived;
}

// Private
void Seriallink::read(){
    dataReceived.clear();
    if(_serial.bytesAvailable() >= 7){
        dataReceived = _serial.readAll();
        //qDebug() << dataReceived;
        emit gotNewData(dataReceived);
    }
}


