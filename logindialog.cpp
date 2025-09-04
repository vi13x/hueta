#include "logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(DataStore* store, QWidget* parent)
    : QDialog(parent), m_store(store)
{
    setWindowTitle("Вход в систему");
    resize(300, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* userLabel = new QLabel("Логин:", this);
    m_usernameEdit = new QLineEdit(this);

    QLabel* passLabel = new QLabel("Пароль:", this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_loginBtn = new QPushButton("Войти", this);
    m_statusLabel = new QLabel("", this);

    layout->addWidget(userLabel);
    layout->addWidget(m_usernameEdit);
    layout->addWidget(passLabel);
    layout->addWidget(m_passwordEdit);
    layout->addWidget(m_loginBtn);
    layout->addWidget(m_statusLabel);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginDialog::tryLogin);
}

void LoginDialog::tryLogin() {
    QString username = m_usernameEdit->text();
    QString password = m_passwordEdit->text();

    User user;
    if(!m_store->verify(username, password, &user)) {
        QMessageBox::warning(this, "Ошибка", "Неправильный логин или пароль");
        return;
    }

    m_user = user;
    accept(); // Закрыть диалог и вернуть управление
}
