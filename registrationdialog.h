#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>

class RegistrationDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);

private slots:
    void onRegisterClicked();

private:
    void setupUI();
    bool validateInput();
    void registerUser();

    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QComboBox *roleCombo;
    QPushButton *registerButton;
    QPushButton *cancelButton;
};

#endif // REGISTRATIONDIALOG_H
