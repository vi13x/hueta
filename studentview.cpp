#include "studentview.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>

StudentView::StudentView(const QString &username, QWidget *parent) : QWidget(parent), username(username) {
    setWindowTitle(QString("Студент: %1").arg(username));
    resize(600, 400);
    QVBoxLayout *v = new QVBoxLayout(this);
    QLabel *userLbl = new QLabel(QString("<b>Вы вошли как:</b> %1").arg(username), this);
    v->addWidget(userLbl);

    tabs = new QTabWidget(this);

    // Расписание
    QListWidget *scheduleList = new QListWidget(this);
    scheduleList->addItem("Понедельник: Математика, Русский");
    scheduleList->addItem("Вторник: Физика, История");
    scheduleList->addItem("Среда: Литература");
    tabs->addTab(scheduleList, "Расписание");

    // Успеваемость
    QListWidget *gradesList = new QListWidget(this);
    gradesList->addItem("Математика — 8");
    gradesList->addItem("Русский — 9");
    tabs->addTab(gradesList, "Успеваемость");

    v->addWidget(tabs);

    QHBoxLayout *h = new QHBoxLayout();
    backBtn = new QPushButton("Назад", this);
    logoutBtn = new QPushButton("Выйти из аккаунта", this);
    h->addWidget(backBtn);
    h->addWidget(logoutBtn);
    v->addLayout(h);

    connect(logoutBtn, &QPushButton::clicked, this, &StudentView::onLogout);
    connect(backBtn, &QPushButton::clicked, this, &StudentView::onBack);
}

void StudentView::onLogout() {
    close();
}

void StudentView::onBack() {
    // просто закрываем — пользователь вернётся к главному окну
    close();
}
