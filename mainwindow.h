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

private slots:
    void refresh_com_detection();
    void on_connect_serial_button_clicked();
    void on_send_button_clicked();
    void on_disconnect_serial_button_clicked();
    void print_serial();

private:
    Ui::MainWindow *ui;
    Seriallink *arduinoSerial;
    QTimer *qtimerCom;
};
#endif // MAINWINDOW_H
