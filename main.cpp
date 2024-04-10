#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":ressources/logo.ico")); // Chemin relatif au fichier de ressources
    MainWindow w;
    w.show();
    return a.exec();
}
