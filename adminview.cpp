#include "adminview.h"
#include "datastore.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDir>

AdminView::AdminView(const QString &username, QWidget *parent) : QWidget(parent), username(username) {
    setWindowTitle(QString("Администратор: %1").arg(username));
    resize(900, 600);
    QVBoxLayout *main = new QVBoxLayout(this);
    main->addWidget(new QLabel(QString("<b>Администратор:</b> %1").arg(username)));

    QHBoxLayout *top = new QHBoxLayout();

    // Teachers
    QVBoxLayout *tcol = new QVBoxLayout();
    tcol->addWidget(new QLabel("Учителя:"));
    teachersList = new QListWidget(this);
    // load from file
    QFile tf(QDir::currentPath() + "/teachers.txt");
    if (tf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&tf);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) teachersList->addItem(line.split(':').first());
        }
    }
    tcol->addWidget(teachersList);
    QPushButton *addT = new QPushButton("Добавить учителя", this);
    QPushButton *remT = new QPushButton("Удалить учителя", this);
    tcol->addWidget(addT);
    tcol->addWidget(remT);

    top->addLayout(tcol);

    // Admins
    QVBoxLayout *acol = new QVBoxLayout();
    acol->addWidget(new QLabel("Администраторы:"));
    adminsList = new QListWidget(this);
    QFile af(QDir::currentPath() + "/admins.txt");
    if (af.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&af);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) adminsList->addItem(line.split(':').first());
        }
    }
    acol->addWidget(adminsList);
    QPushButton *addA = new QPushButton("Добавить админа", this);
    QPushButton *remA = new QPushButton("Удалить админа", this);
    acol->addWidget(addA);
    acol->addWidget(remA);

    top->addLayout(acol);

    // Schedule editor
    QVBoxLayout *scol = new QVBoxLayout();
    scol->addWidget(new QLabel("Расписание (каждая строка — запись):"));
    scheduleEdit = new QTextEdit(this);
    // load schedule
    DataStore ds;
    auto lines = ds.loadSchedule();
    for (auto &l : lines) {
        scheduleEdit->append(l);
    }
    scol->addWidget(scheduleEdit);
    QPushButton *loadS = new QPushButton("Загрузить (обновить)", this);
    QPushButton *saveS = new QPushButton("Сохранить расписание", this);
    scol->addWidget(loadS);
    scol->addWidget(saveS);

    top->addLayout(scol);

    main->addLayout(top);

    QHBoxLayout *bot = new QHBoxLayout();
    QPushButton *viewGradesBtn = new QPushButton("Просмотреть оценки", this);
    QPushButton *clearGradesBtn = new QPushButton("Очистить все оценки", this);
    QPushButton *logoutBtn = new QPushButton("Выйти", this);

    bot->addWidget(viewGradesBtn);
    bot->addWidget(clearGradesBtn);
    bot->addStretch();
    bot->addWidget(logoutBtn);

    main->addLayout(bot);

    connect(addT, &QPushButton::clicked, this, &AdminView::onAddTeacher);
    connect(remT, &QPushButton::clicked, this, &AdminView::onRemoveTeacher);
    connect(addA, &QPushButton::clicked, this, &AdminView::onAddAdmin);
    connect(remA, &QPushButton::clicked, this, &AdminView::onRemoveAdmin);
    connect(loadS, &QPushButton::clicked, this, &AdminView::onLoadSchedule);
    connect(saveS, &QPushButton::clicked, this, &AdminView::onSaveSchedule);
    connect(viewGradesBtn, &QPushButton::clicked, this, &AdminView::onViewGrades);
    connect(clearGradesBtn, &QPushButton::clicked, this, &AdminView::onClearGrades);
    connect(logoutBtn, &QPushButton::clicked, this, &AdminView::onLogout);
}

void AdminView::onLogout() { close(); }

void AdminView::onAddTeacher() {
    bool ok;
    QString username = QInputDialog::getText(this, "Добавить учителя", "Имя пользователя:", QLineEdit::Normal, "", &ok);
    if (!ok || username.trimmed().isEmpty()) return;
    QString pwd = QInputDialog::getText(this, "Пароль", "Пароль для учителя:", QLineEdit::Password, "", &ok);
    if (!ok) return;
    DataStore ds;
    ds.addTeacher(username.trimmed(), pwd);
    teachersList->addItem(username.trimmed());
}

void AdminView::onRemoveTeacher() {
    if (!teachersList->currentItem()) return;
    QString u = teachersList->currentItem()->text();
    DataStore ds;
    ds.removeTeacher(u);
    delete teachersList->takeItem(teachersList->currentRow());
}

void AdminView::onAddAdmin() {
    bool ok;
    QString username = QInputDialog::getText(this, "Добавить администратора", "Имя пользователя:", QLineEdit::Normal, "", &ok);
    if (!ok || username.trimmed().isEmpty()) return;
    QString pwd = QInputDialog::getText(this, "Пароль", "Пароль для администратора:", QLineEdit::Password, "", &ok);
    if (!ok) return;
    DataStore ds;
    ds.addAdmin(username.trimmed(), pwd);
    adminsList->addItem(username.trimmed());
}

void AdminView::onRemoveAdmin() {
    if (!adminsList->currentItem()) return;
    QString u = adminsList->currentItem()->text();
    DataStore ds;
    ds.removeAdmin(u);
    delete adminsList->takeItem(adminsList->currentRow());
}

void AdminView::onLoadSchedule() {
    DataStore ds;
    auto lines = ds.loadSchedule();
    scheduleEdit->clear();
    for (auto &l : lines) scheduleEdit->append(l);
}

void AdminView::onSaveSchedule() {
    QString all = scheduleEdit->toPlainText();
    QVector<QString> lines = all.split('\n').toVector();
    DataStore ds;
    if (ds.saveSchedule(lines)) QMessageBox::information(this, "OK", "Расписание сохранено.");
    else QMessageBox::warning(this, "Ошибка", "Не удалось сохранить расписание.");
}

void AdminView::onViewGrades() {
    DataStore ds;
    auto grades = ds.loadGrades();
    QString txt;
    for (auto &g : grades) txt += g + "\n";
    QMessageBox::information(this, "Оценки", txt.isEmpty() ? "Нет оценок" : txt);
}

void AdminView::onClearGrades() {
    if (QMessageBox::question(this, "Подтвердите", "Удалить все оценки?") != QMessageBox::Yes) return;
    DataStore ds;
    ds.overwriteGrades({});
    QMessageBox::information(this, "OK", "Оценки удалены.");
}
