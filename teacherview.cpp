#include "teacherview.h"
#include "markdialog.h"
#include "datastore.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QDate>
TeacherView::TeacherView(const QString &username, QWidget *parent) : QWidget(parent), username(username) {
    setWindowTitle(QString("Учитель: %1").arg(username));
    resize(800, 500);

    QVBoxLayout *main = new QVBoxLayout(this);
    QLabel *lbl = new QLabel(QString("<b>Вошёл учитель:</b> %1").arg(username), this);
    main->addWidget(lbl);

    QHBoxLayout *h = new QHBoxLayout();
    classList = new QListWidget(this);
    classList->addItem("1A");
    classList->addItem("1B");
    classList->addItem("2A");
    classList->addItem("2B");
    h->addWidget(classList);

    studentList = new QListWidget(this);
    // For demo, static students per class
    studentList->addItem("Пусто — выберите класс");
    h->addWidget(studentList);

    main->addLayout(h);

    QHBoxLayout *bot = new QHBoxLayout();
    logoutBtn = new QPushButton("Выйти", this);
    bot->addStretch();
    bot->addWidget(logoutBtn);
    main->addLayout(bot);

    connect(classList, &QListWidget::itemClicked, this, &TeacherView::onClassSelected);
    connect(studentList, &QListWidget::itemClicked, this, &TeacherView::onStudentClicked);
    connect(logoutBtn, &QPushButton::clicked, this, &TeacherView::onLogout);
}

void TeacherView::onClassSelected() {
    QString cls = classList->currentItem()->text();
    studentList->clear();
    // sample students — in production load from datastore
    if (cls == "1A") {
        studentList->addItem("Ivanov");
        studentList->addItem("Petrov");
        studentList->addItem("Sidorov");
    } else if (cls == "1B") {
        studentList->addItem("Anna");
        studentList->addItem("Olga");
    } else {
        studentList->addItem("Student1");
        studentList->addItem("Student2");
    }
}

void TeacherView::onStudentClicked() {
    if (!classList->currentItem()) return;
    QString cls = classList->currentItem()->text();
    QString student = studentList->currentItem()->text();

    // choose subject — for demo provide few
    MarkDialog dlg(cls, student, this);
    if (dlg.exec() == QDialog::Accepted) {
        QString subject = dlg.subject();
        QString grade = dlg.grade();
        DataStore ds;
        ds.addGrade(cls, student, subject, QDate::currentDate().toString(Qt::ISODate), grade);
        QMessageBox::information(this, "OK", QString("Поставлена оценка %1 ученику %2 по %3").arg(grade, student, subject));
    }
}

void TeacherView::onLogout() {
    close();
}
