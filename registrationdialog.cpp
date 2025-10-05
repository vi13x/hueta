#include "registrationdialog.h"
#include "datastore.h"
#include "studentview.h"
#include "teacherview.h"
#include "adminview.h"

RegistrationDialog::RegistrationDialog(QWidget *parent)
    : QDialog(parent) {
    setupUI();
    setWindowTitle("Регистрация");
    setModal(true);
    setFixedSize(350, 250);
}

void RegistrationDialog::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("Регистрация нового пользователя");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);
    
    mainLayout->addWidget(new QLabel("Имя пользователя:"));
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Введите имя пользователя");
    mainLayout->addWidget(usernameEdit);
    
    mainLayout->addWidget(new QLabel("Пароль:"));
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Введите пароль");
    mainLayout->addWidget(passwordEdit);
    
    mainLayout->addWidget(new QLabel("Подтвердите пароль:"));
    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setPlaceholderText("Подтвердите пароль");
    mainLayout->addWidget(confirmPasswordEdit);
    
    mainLayout->addWidget(new QLabel("Роль:"));
    roleCombo = new QComboBox();
    roleCombo->addItems({"Студент", "Преподаватель", "Администратор"});
    mainLayout->addWidget(roleCombo);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    registerButton = new QPushButton("Зарегистрироваться");
    registerButton->setDefault(true);
    connect(registerButton, &QPushButton::clicked, this, &RegistrationDialog::onRegisterClicked);
    buttonLayout->addWidget(registerButton);
    
    cancelButton = new QPushButton("Отмена");
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect Enter key to register
    connect(usernameEdit, &QLineEdit::returnPressed, this, &RegistrationDialog::onRegisterClicked);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &RegistrationDialog::onRegisterClicked);
    connect(confirmPasswordEdit, &QLineEdit::returnPressed, this, &RegistrationDialog::onRegisterClicked);
}

void RegistrationDialog::onRegisterClicked() {
    if (validateInput()) {
        registerUser();
    }
}

bool RegistrationDialog::validateInput() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();
    QString confirmPassword = confirmPasswordEdit->text().trimmed();
    
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return false;
    }
    
    if (username.length() < 3) {
        QMessageBox::warning(this, "Ошибка", "Имя пользователя должно содержать минимум 3 символа");
        return false;
    }
    
    if (password.length() < 6) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен содержать минимум 6 символов");
        return false;
    }
    
    if (password != confirmPassword) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают");
        return false;
    }
    
    return true;
}

void RegistrationDialog::registerUser() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();
    UserRole role;
    
    switch (roleCombo->currentIndex()) {
        case 0: role = UserRole::Student; break;
        case 1: role = UserRole::Teacher; break;
        case 2: role = UserRole::Admin; break;
        default: role = UserRole::Student; break;
    }
    
    std::shared_ptr<User> user;
    switch (role) {
        case UserRole::Student:
            user = std::make_shared<Student>(username, password);
            break;
        case UserRole::Teacher:
            user = std::make_shared<Teacher>(username, password);
            break;
        case UserRole::Admin:
            user = std::make_shared<Admin>(username, password);
            break;
    }
    
    if (DataStore::getInstance().addUser(user)) {
        QMessageBox::information(this, "Успех", "Пользователь успешно зарегистрирован");
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким именем уже существует");
    }
}
