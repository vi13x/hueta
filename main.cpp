#include <QApplication>
#include "datastore.h"
#include "logindialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DataStore store;
    store.load();

    LoginDialog login(&store);
    if (login.exec() == QDialog::Accepted) {
        MainWindow w(login.currentUser(), &store);
        w.show();
        return app.exec();
    }
    return 0;
}
