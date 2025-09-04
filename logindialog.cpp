#include "logindialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QStackedWidget>
#include <QLabel>
#include <QSpacerItem>

LoginDialog::LoginDialog(DataStore* store, QWidget* parent)
    : QDialog(parent), m_store(store)
{
    setWindowTitle("Электронный дневник");
    resize(400, 300);

    m_stack = new QStackedWidget(this);

    // ===== Welcome page =====
    QWidget* welcome = new QWidget(this);
    auto wLayout = new QVBoxLayout(welcome);
    QLabel* title = new QLabel("Добро пожаловать в электронный дневник!", welcome);
    title->setStyleSheet("font-size:18px; font-weight:bold; margin:20px; text-align:center;");
    wLayout->addWidget(title, 0, Qt::AlignCenter);

    QPushButton* regBtn = new QPushButton("Регистрация", welcome);
    QPushButton* loginBtn = new QPushButton("Уже есть аккаунт? Войти", welcome);

    wLayout->addStretch();
    wLayout->addWidget(regBtn);
    wLayout->addWidget(loginBtn);
    wLayout->addStretch();

    connect(regBtn, &QPushButton::clicked, this, &LoginDialog::showRegister);
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::showLogin);

    // ===== Register page =====
    QWidget* regPage = new QWidget(this);
    auto rLayout = new QVBoxLayout(regPage);
    auto regForm = new QFormLayout;

    m_regUserEdit = new QLineEdit(this);
    m_regPassEdit = new QLineEdit(this);
    m_regPassEdit->setEchoMode(QLineEdit::Password);
    m_regFullNameEdit = new QLineEdit(this);
    m_regClassEdit = new QLineEdit(this);
    m_regRoleBox = new QComboBox(this);
    m_regRoleBox->addItem("Ученик", (int)Role::Student);
    m_regRoleBox->addItem("Учитель", (int)Role::Teacher);
    m_regRoleBox->addItem("Администратор", (int)Role::Admin);

    regForm->addRow("Логин:", m_regUserEdit);
    regForm->addRow("Пароль:", m_regPassEdit);
    regForm->addRow("ФИО:", m_regFullNameEdit);
    regForm->addRow("Класс:", m_regClassEdit);
    regForm->addRow("Роль:", m_regRoleBox);
    rLayout->addLayout(regForm);

    auto regBtn2 = new QPushButton("Зарегистрироваться", this);
    auto backBtn1 = new QPushButton("Назад", this);
    rLayout->addWidget(regBtn2);
    rLayout->addWidget(backBtn1);

    connect(regBtn2, &QPushButton::clicked, this, &LoginDialog::registerUser);
    connect(backBtn1, &QPushButton::clicked, this, &LoginDialog::showWelcome);

    // ===== Login page =====
    QWidget* loginPage = new QWidget(this);
    auto lLayout = new QVBoxLayout(loginPage);
    auto loginForm = new QFormLayout;

    m_loginUserEdit = new QLineEdit(this);
    m_loginPassEdit = new QLineEdit(this);
    m_loginPassEdit->setEchoMode(QLineEdit::Password);

    loginForm->addRow("Логин:", m_loginUserEdit);
    loginForm->addRow("Пароль:", m_loginPassEdit);
    lLayout->addLayout(loginForm);

    auto loginBtn2 = new QPushButton("Войти", this);
    auto backBtn2 = new QPushButton("Назад", this);
    lLayout->addWidget(loginBtn2);
    lLayout->addWidget(backBtn2);

    connect(loginBtn2, &QPushButton::clicked, this, &LoginDialog::tryLogin);
    connect(backBtn2, &QPushButton::clicked, this, &LoginDialog::showWelcome);

    // Add pages to stack
    m_stack->addWidget(welcome);   // index 0
    m_stack->addWidget(regPage);   // index 1
    m_stack->addWidget(loginPage); // index 2

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stack);

    showWelcome();
}

// ===== Slots =====
void LoginDialog::showWelcome() {
    m_stack->setCurrentIndex(0);
}
void LoginDialog::showRegister() {
    m_stack->setCurrentIndex(1);
}
void LoginDialog::showLogin() {
    m_stack->setCurrentIndex(2);
}

void LoginDialog::tryLogin() {
    QString login = m_loginUserEdit->text();
    QString pass = m_loginPassEdit->text();

    if (m_store->verify(login, pass, &m_user)) {
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}

void LoginDialog::registerUser() {
    QString login = m_regUserEdit->text();
    QString pass = m_regPassEdit->text();
    QString name = m_regFullNameEdit->text();
    QString cls = m_regClassEdit->text();
    Role role = (Role)m_regRoleBox->currentData().toInt();

    if (login.isEmpty() || pass.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }

    if (m_store->userExists(login)) {
        QMessageBox::warning(this, "Ошибка", "Пользователь уже существует");
        return;
    }

    User u;
    u.username = login;
    u.passwordHash = DataStore::hash(pass);
    u.fullName = name;
    u.classId = cls;
    u.role = role;

    if (m_store->addUser(u)) {
        QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");
        m_user = u;
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить пользователя");
    }
}
