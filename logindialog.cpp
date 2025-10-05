#include "logindialog.h"
#include "datastore.h"
#include "registrationdialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), authenticatedUser(nullptr) {
    setupUI();
    setWindowTitle("Вход в систему");
    setModal(true);
    setFixedSize(300, 200);
}

void LoginDialog::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("Вход в Academic Ledger");
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
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("Войти");
    loginButton->setDefault(true);
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    buttonLayout->addWidget(loginButton);
    
    registerButton = new QPushButton("Регистрация");
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
    buttonLayout->addWidget(registerButton);
    
    cancelButton = new QPushButton("Отмена");
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect Enter key to login
    connect(usernameEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
}

void LoginDialog::onLoginClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();
    
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }
    
    if (authenticateUser(username, password)) {
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверное имя пользователя или пароль");
        passwordEdit->clear();
        passwordEdit->setFocus();
    }
}

void LoginDialog::onRegisterClicked() {
    RegistrationDialog regDialog(this);
    if (regDialog.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "Успех", "Регистрация прошла успешно. Теперь вы можете войти в систему.");
    }
}

bool LoginDialog::authenticateUser(const QString& username, const QString& password) {
    if (DataStore::getInstance().authenticateUser(username, password)) {
        authenticatedUser = DataStore::getInstance().findUser(username);
        return true;
    }
    return false;
}

std::shared_ptr<User> LoginDialog::getAuthenticatedUser() const {
    return authenticatedUser;
}
