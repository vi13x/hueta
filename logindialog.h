#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "datastore.h"

class QStackedWidget;
class QLineEdit;
class QPushButton;
class QComboBox;

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    LoginDialog(DataStore* store, QWidget* parent=nullptr);
    User currentUser() const { return m_user; }

private slots:
    void showWelcome();
    void showRegister();
    void showLogin();
    void tryLogin();
    void registerUser();

private:
    DataStore* m_store;
    User m_user;

    QStackedWidget* m_stack;

    // Login
    QLineEdit* m_loginUserEdit;
    QLineEdit* m_loginPassEdit;

    // Register
    QLineEdit* m_regUserEdit;
    QLineEdit* m_regPassEdit;
    QLineEdit* m_regFullNameEdit;
    QLineEdit* m_regClassEdit;
    QComboBox* m_regRoleBox;
};

#endif
