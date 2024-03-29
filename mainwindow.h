#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "seriallink.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
    class QTimer;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isConnected; // Connected to a serial instance

public slots:
    //void updateGUI(QByteArray data);

private slots:
    /*
    void on_p_ledOn_clicked();
    void on_p_ledOff_clicked();
    */

    void refresh_com_detection();
    void on_connect_button_clicked();
    void on_send_button_clicked();
    void print_serial();


private:
    Ui::MainWindow *ui;
    seriallink *arduinoSerial;
    QTimer *qtimerCom;
};
#endif // MAINWINDOW_H
