#include <QDebug>
#include "seriallink.h"

// Constructor
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

// Destructor
seriallink::~seriallink(){
    closeConnection();
}

// Public
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


// Public
void seriallink::closeConnection(){
    _serial.close();
}

// Public
void seriallink::write(const char* messageToWrite){
    _serial.write(messageToWrite);
}

// Public
bool seriallink::isOpen(){
    return _serial.isOpen();
}

// Public
bool seriallink::isWritable(){
    return _serial.isWritable();
}

// Public
bool seriallink::isReadable(){
    return _serial.isReadable();
}

// Public
QByteArray seriallink::getSerialData(){
    return this->dataReceived;
}

// Private
void seriallink::read(){
    dataReceived.clear();
    dataReceived = _serial.readAll();
    qDebug() << dataReceived;
    emit gotNewData(dataReceived);
}


