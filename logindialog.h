#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "datastore.h"

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(DataStore* store, QWidget* parent = nullptr);
    User loggedUser() const { return m_user; }

private slots:
    void tryLogin();

private:
    DataStore* m_store;
    User m_user;

    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QPushButton* m_loginBtn;
    QLabel* m_statusLabel;
};
