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
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QTabWidget>
#include <QSplitter>
#include <QDate>

AdminView::AdminView(const QString &username, QWidget *parent) : QWidget(parent), username(username) {
    setWindowTitle(QString("АДМИНИСТРАТОР: %1").arg(username));
    resize(1200, 800);
    setMinimumSize(1000, 700);

    // Устанавливаем брутальные стили
    setStyleSheet(
        "QWidget { background-color: #1a1a1a; color: white; } "
        "QListWidget { background-color: #2a2a2a; border: 2px solid #404040; } "
        "QListWidget::item { padding: 12px; border-bottom: 1px solid #404040; font-weight: bold; } "
        "QListWidget::item:selected { background-color: #505050; } "
        "QListWidget::item:hover { background-color: #404040; } "
        "QTextEdit { background-color: #2a2a2a; border: 2px solid #404040; color: white; font-family: monospace; } "
        "QPushButton { background-color: #404040; color: white; border: 2px solid #606060; padding: 12px 24px; font-weight: bold; letter-spacing: 1px; } "
        "QPushButton:hover { background-color: #505050; border: 2px solid #707070; } "
        "QPushButton:pressed { background-color: #303030; border: 2px solid #505050; } "
        "QLabel { color: white; font-weight: bold; } "
    );

    QVBoxLayout *main = new QVBoxLayout(this);
    main->setSpacing(20);
    main->setContentsMargins(20, 20, 20, 20);

    // Заголовок в брутальном стиле
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setStyleSheet("QFrame { background-color: #1a1a1a; border: 2px solid #404040; padding: 15px; }");
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    QLabel *userLbl = new QLabel(QString("АДМИНИСТРАТОР: %1").arg(username.toUpper()), this);
    userLbl->setStyleSheet("QLabel { color: white; font-size: 24px; font-weight: bold; letter-spacing: 2px; }");
    headerLayout->addWidget(userLbl);
    headerLayout->addStretch();
    
    QLabel *dateLabel = new QLabel(QDate::currentDate().toString("dd.MM.yyyy"), this);
    dateLabel->setStyleSheet("QLabel { color: #b0b0b0; font-size: 16px; font-weight: bold; }");
    headerLayout->addWidget(dateLabel);
    
    main->addWidget(headerFrame);

    // Основной контент с вкладками
    QTabWidget *mainTabs = new QTabWidget(this);
    mainTabs->setStyleSheet(
        "QTabWidget::pane { border: 2px solid #404040; background-color: #2a2a2a; } "
        "QTabBar::tab { background-color: #404040; color: white; padding: 12px 24px; margin-right: 2px; border: 1px solid #606060; font-weight: bold; letter-spacing: 1px; } "
        "QTabBar::tab:selected { background-color: #505050; border: 2px solid #707070; } "
        "QTabBar::tab:hover { background-color: #454545; } "
    );

    // Вкладка управления пользователями
    QWidget *usersTab = new QWidget();
    QHBoxLayout *usersLayout = new QHBoxLayout(usersTab);

    // Учителя
    QVBoxLayout *tcol = new QVBoxLayout();
    QLabel *teachersLabel = new QLabel("УЧИТЕЛЯ:", this);
    teachersLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; margin: 10px 0; letter-spacing: 1px; }");
    tcol->addWidget(teachersLabel);
    
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
    QPushButton *addT = new QPushButton("ДОБАВИТЬ УЧИТЕЛЯ", this);
    QPushButton *remT = new QPushButton("УДАЛИТЬ УЧИТЕЛЯ", this);
    tcol->addWidget(addT);
    tcol->addWidget(remT);

    usersLayout->addLayout(tcol);

    // Администраторы
    QVBoxLayout *acol = new QVBoxLayout();
    QLabel *adminsLabel = new QLabel("АДМИНИСТРАТОРЫ:", this);
    adminsLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; margin: 10px 0; letter-spacing: 1px; }");
    acol->addWidget(adminsLabel);
    
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
    QPushButton *addA = new QPushButton("ДОБАВИТЬ АДМИНА", this);
    QPushButton *remA = new QPushButton("УДАЛИТЬ АДМИНА", this);
    acol->addWidget(addA);
    acol->addWidget(remA);

    usersLayout->addLayout(acol);
    mainTabs->addTab(usersTab, "УПРАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯМИ");

    // Вкладка расписания
    QWidget *scheduleTab = new QWidget();
    QVBoxLayout *scheduleLayout = new QVBoxLayout(scheduleTab);
    
    QLabel *scheduleLabel = new QLabel("РАСПИСАНИЕ (КАЖДАЯ СТРОКА — ЗАПИСЬ):", this);
    scheduleLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; margin: 10px 0; letter-spacing: 1px; }");
    scheduleLayout->addWidget(scheduleLabel);
    
    scheduleEdit = new QTextEdit(this);
    // load schedule
    DataStore ds;
    auto lines = ds.loadSchedule();
    for (auto &l : lines) {
        scheduleEdit->append(l);
    }
    scheduleLayout->addWidget(scheduleEdit);
    
    QHBoxLayout *scheduleButtons = new QHBoxLayout();
    QPushButton *loadS = new QPushButton("ЗАГРУЗИТЬ (ОБНОВИТЬ)", this);
    QPushButton *saveS = new QPushButton("СОХРАНИТЬ РАСПИСАНИЕ", this);
    scheduleButtons->addWidget(loadS);
    scheduleButtons->addWidget(saveS);
    scheduleLayout->addLayout(scheduleButtons);

    mainTabs->addTab(scheduleTab, "РАСПИСАНИЕ");

    main->addWidget(mainTabs);
    
    // Сохраняем ссылку на mainTabs
    this->mainTabs = mainTabs;

    // Кнопки в брутальном стиле
    QHBoxLayout *bot = new QHBoxLayout();
    QPushButton *viewGradesBtn = new QPushButton("ПРОСМОТРЕТЬ ОЦЕНКИ", this);
    QPushButton *clearGradesBtn = new QPushButton("ОЧИСТИТЬ ВСЕ ОЦЕНКИ", this);
    QPushButton *logoutBtn = new QPushButton("ВЫЙТИ", this);

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
