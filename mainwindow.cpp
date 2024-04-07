#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isConnected(false)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->label_database_connect->setText("Not connected !");
    ui->label_database_connect->setStyleSheet("QLabel { color : red; }");

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

    //Binding the enter key to the send_browser
    connect(ui->send_browser, &QLineEdit::returnPressed, this, &MainWindow::on_send_button_clicked);

    // Database
    database = new SQLManagement(this);
    if(database->createConnection()){
        // Indicateur sur IHM
        ui->label_database_connect->setText("Connected");
        ui->label_database_connect->setStyleSheet("QLabel { color : green; }");
    }
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
            if(isConnected == true){
                disconnect(arduinoSerial, &Seriallink::gotNewData, this, &MainWindow::print_serial);
                qDebug() << "COM HAS BEEN DECONNECTED";
                ui->label_serial_connexion->setText("Disconnected !");
                ui->label_serial_connexion->setStyleSheet("QLabel { color : red; }");

                isConnected = false;
                if(arduinoSerial){
                    delete arduinoSerial;
                    arduinoSerial = nullptr;
                }

                // On débloque les boutons pour pouvoir relancer une connexion
                ui->connect_serial_button->setEnabled(true);
                ui->com_comboBox->setEnabled(true);
                ui->baud_comboBox->setEnabled(true);
                ui->send_browser->setEnabled(false);
                // On supprime le droit à l'utilisateur d'envoyer des messages sur le port serie
                ui->send_button->setEnabled(false);
                ui->disconnect_serial_button->setEnabled(false);
            }
        }
    }

    // Add new COM ports to the com_comboBox
    for (const QSerialPortInfo &info : availablePorts) {
        QString portName = info.portName();
        if (ui->com_comboBox->findText(portName) == -1) {
            ui->com_comboBox->addItem(portName);
        }
    }
    /*
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
    */
}

void MainWindow::on_connect_serial_button_clicked()
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
    arduinoSerial = new Seriallink(COMSelectionned, baudRateCombotBox, this);

    // Ajouter verification de validité de pointeur ici

    // CONNECTION SERIE OK :)
    if(arduinoSerial->openConnection()){
        isConnected = true;
        ui->label_serial_connexion->setText("Connected");
        ui->label_serial_connexion->setStyleSheet("QLabel { color : green; }");
        // On bloque les boutons pour eviter de se connecter plusieurs fois
        ui->connect_serial_button->setEnabled(false);
        ui->com_comboBox->setEnabled(false);
        ui->baud_comboBox->setEnabled(false);
        ui->send_browser->setEnabled(true);
        // On permet à l'utilisateur d'envoyer des messages sur le port serie
        ui->send_button->setEnabled(true);
        ui->disconnect_serial_button->setEnabled(true);

        connect(arduinoSerial, &Seriallink::gotNewData, this, &MainWindow::print_serial);
    }
    // CONNECTION SERIE NON OK :(
    else{
        ui->label_serial_connexion->setStyleSheet("QLabel { color : red; }");
        ui->label_serial_connexion->setText("Connexion fail !");
    }
}

void MainWindow::on_send_button_clicked()
{
    if(arduinoSerial){ // Normalement le bouton est grisé, mais au cas ou :)
        if(arduinoSerial->isWritable()){
            // Recuperation du text du send_browser
            QString stringText = ui->send_browser->text();
             // Permet la conversion entre QString et string.
            std::string str = stringText.toStdString();
            // Affichage sur le text_browser
            ui->textBrowser->append("< " + stringText);
             // Envoi sur le port serie
            arduinoSerial->write(str.c_str());
            // On clean le send_browser
            ui->send_browser->clear();
        }
    }
}

void MainWindow::on_disconnect_serial_button_clicked()
{
    if(isConnected == true){
        isConnected = false;

        disconnect(arduinoSerial, &Seriallink::gotNewData, this, &MainWindow::print_serial);

        if(arduinoSerial){
            delete arduinoSerial;
            arduinoSerial = nullptr;
        }

        qDebug() << "DISCONNECTION: COM HAS BEEN DECONNECTED";
        ui->label_serial_connexion->setText("Disconnected !");
        ui->label_serial_connexion->setStyleSheet("QLabel { color : red; }");

        // On débloque les boutons pour pouvoir relancer une connexion
        ui->connect_serial_button->setEnabled(true);
        ui->com_comboBox->setEnabled(true);
        ui->baud_comboBox->setEnabled(true);
        // On supprime le droit à l'utilisateur d'envoyer des messages sur le port serie
        ui->send_browser->setEnabled(false);
        ui->send_button->setEnabled(false);
        ui->disconnect_serial_button->setEnabled(false);
    }
}


void MainWindow::print_serial(){
    QString id = arduinoSerial->getSerialData();
    ui->textBrowser->append("> " + id);
    QString stringText = database->getInfos(id);
    database->decrementCredit(id); // Decrement the credit
    std::string str = stringText.toStdString();

    if(str != "NULL"){
        qDebug() << stringText;
         ui->textBrowser->append("< " + stringText + '\n');
        // We send the information back to the serial
        if(arduinoSerial->isWritable()){
            arduinoSerial->write(str.c_str());
        }
    }
    else{
        ui->textBrowser->append("******** [INFO] USER DOES NOT EXIST [INFO] ********\n");
        if(arduinoSerial->isWritable()){
            arduinoSerial->write("NULL");
        }
    }
}

void MainWindow::on_clear_pushButton_clicked()
{
    ui->textBrowser->clear();
}

