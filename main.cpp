#include <QApplication>
#include "datastore.h"
#include "mainwindow.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataStore store;
    store.load(); // загрузка данных, если есть

    LoginDialog login(&store);
    if(login.exec() != QDialog::Accepted) return 0;

    User currentUser = login.loggedUser();

    MainWindow w(currentUser, &store);
    w.show();

    return a.exec();
}
