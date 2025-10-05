#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <memory>

class User;

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    std::shared_ptr<User> getAuthenticatedUser() const;

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    void setupUI();
    bool authenticateUser(const QString& username, const QString& password);

    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *cancelButton;
    
    std::shared_ptr<User> authenticatedUser;
};

#endif // LOGINDIALOG_H
