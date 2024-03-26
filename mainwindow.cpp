#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Listing all the COMs available on the computer
    refresh_com_detection();

    // Differents type of baud rates for the serial communication
    ui->baud_comboBox->addItem("9600");
    ui->baud_comboBox->addItem("1200");
    ui->baud_comboBox->addItem("2400");
    ui->baud_comboBox->addItem("4800");
    ui->baud_comboBox->addItem("19200");
    ui->baud_comboBox->addItem("38400");
    ui->baud_comboBox->addItem("57600");
    ui->baud_comboBox->addItem("115200");

    //Refresh the port dectection
    qtimerCom = new QTimer(this);
    connect(qtimerCom, &QTimer::timeout, this, &MainWindow::refresh_com_detection);
    qtimerCom->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh_com_detection()
{
    const auto availablePorts = QSerialPortInfo::availablePorts();
    QStringList availablePortNames;
    for (const QSerialPortInfo &info : availablePorts) {
        availablePortNames.append(info.portName());
    }

    // Remove unavailable COM ports from the com_comboBox
    for (int i = 0; i < ui->com_comboBox->count(); ++i) {
        QString currentPortName = ui->com_comboBox->itemText(i);
        if (!availablePortNames.contains(currentPortName)) {
            ui->com_comboBox->removeItem(i);
            --i;
        }
    }

    // Add new COM ports to the com_comboBox
    for (const QSerialPortInfo &info : availablePorts) {
        QString portName = info.portName();
        if (ui->com_comboBox->findText(portName) == -1) {
            ui->com_comboBox->addItem(portName);
        }
    }

    if (availablePortNames.isEmpty()) {
        qDebug() << "Warning: Aucun port série n'est disponible";
        // EXIT_FAILURE; // Peut-être excessif :)
    } else {
        for (const QSerialPortInfo &info : availablePorts) {
            qDebug() << "  Nom : " << info.portName();
            qDebug() << "  Description : " << info.description();
            qDebug() << "  Fabricant : " << info.manufacturer();
            qDebug() << "  Identifiant du produit : " << info.productIdentifier();
            qDebug() << "  Identifiant du vendeur : " << info.vendorIdentifier();
            qDebug() << "  Emplacement système : " << info.systemLocation();
            qDebug() << "---------------------------------";
        }
    }
}

/*
void MainWindow::on_p_ledOn_clicked()
{
    if(arduinoSerial->isWritable())
        arduinoSerial->write("o");
    else
        qDebug() << "Couldn't write to serial";
}

void MainWindow::on_p_ledOff_clicked()
{
    if(arduinoSerial->isWritable())
        arduinoSerial->write("n");
    else
        qDebug() << "Couldn't write to serial";
}
*/

void MainWindow::on_connect_button_clicked()
{
    QString COMSelectionned = ui->com_comboBox->currentText();

    QSerialPort::BaudRate baudRateCombotBox = QSerialPort::BaudRate::Baud9600;
    QString selectedBaudRate = ui->baud_comboBox->currentText();
    if(selectedBaudRate == "9600")
        baudRateCombotBox = QSerialPort::BaudRate::Baud9600;
    if(selectedBaudRate == "1200")
        baudRateCombotBox = QSerialPort::BaudRate::Baud1200;
    if(selectedBaudRate == "2400")
        baudRateCombotBox = QSerialPort::BaudRate::Baud2400;
    if(selectedBaudRate == "4800")
        baudRateCombotBox = QSerialPort::BaudRate::Baud4800;
    if(selectedBaudRate == "19200")
        baudRateCombotBox = QSerialPort::BaudRate::Baud19200;
    if(selectedBaudRate == "38400")
        baudRateCombotBox = QSerialPort::BaudRate::Baud38400;
    if(selectedBaudRate == "57600")
        baudRateCombotBox = QSerialPort::BaudRate::Baud57600;
    if(selectedBaudRate == "115200")
        baudRateCombotBox = QSerialPort::BaudRate::Baud115200;

    qDebug() << "Connexion with COM: " << COMSelectionned << " / Baud Rate: " << selectedBaudRate << '\n';
    arduinoSerial = new seriallink(COMSelectionned, baudRateCombotBox);

    // CONNECTION SERIE OK :)
    if(arduinoSerial->openConnection()){
        ui->label_connexion_status->setText("Connected");
        ui->label_connexion_status->setStyleSheet("QLabel { color : green; }");
        ui->connect_button->setEnabled(false);
        ui->plainTextEdit->setEnabled(true);
        ui->send_button->setEnabled(true);
    }
    // CONNECTION SERIE NON OK :(
    else{
        ui->label_connexion_status->setStyleSheet("QLabel { color : red; }");
        ui->label_connexion_status->setText("Connexion fail !");
    }
}

void MainWindow::on_TESTBUTTON_clicked()
{
    QByteArray data;
    arduinoSerial->gotNewData(data);
    ui->textBrowser->append(data);
}

