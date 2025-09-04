#include "teacherview.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>

TeacherView::TeacherView(const User& u, DataStore* store, QWidget* parent)
    : QWidget(parent), m_user(u), m_store(store)
{
    auto layout = new QVBoxLayout(this);

    auto form = new QFormLayout;
    m_classBox = new QComboBox(this);
    m_studentBox = new QComboBox(this);
    m_subjectEdit = new QLineEdit(this);
    m_markEdit = new QLineEdit(this);
    m_addMarkBtn = new QPushButton("Выставить оценку", this);
    m_absentBtn = new QPushButton("Отметить пропуск", this);

    form->addRow("Класс:", m_classBox);
    form->addRow("Ученик:", m_studentBox);
    form->addRow("Предмет:", m_subjectEdit);
    form->addRow("Оценка (1-10):", m_markEdit);
    layout->addLayout(form);
    layout->addWidget(m_addMarkBtn);
    layout->addWidget(m_absentBtn);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({"Предмет", "Оценка", "Дата"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_table);

    connect(m_addMarkBtn, &QPushButton::clicked, this, &TeacherView::addMark);
    connect(m_absentBtn, &QPushButton::clicked, this, &TeacherView::toggleAbsent);
    connect(m_classBox, &QComboBox::currentTextChanged, this, &TeacherView::refresh);
    connect(m_studentBox, &QComboBox::currentTextChanged, this, &TeacherView::refresh);
    connect(m_store, &DataStore::changed, this, &TeacherView::refresh);

    // Заполнение классов
    QSet<QString> classes;
    for (auto u : m_store->allUsers()) {
        if (u.role == Role::Student && !u.classId.isEmpty())
            classes.insert(u.classId);
    }
    m_classBox->addItems(classes.values());

    refresh();
}

void TeacherView::refresh() {
    m_studentBox->clear();
    QString cls = m_classBox->currentText();
    for (auto u : m_store->allUsers()) {
        if (u.role == Role::Student && u.classId == cls) {
            m_studentBox->addItem(u.fullName, u.username);
        }
    }

    QString login = m_studentBox->currentData().toString();
    if (login.isEmpty()) return;

    auto marks = m_store->getMarks(login);
    m_table->setRowCount(marks.size());
    for (int i=0; i<marks.size(); ++i) {
        m_table->setItem(i, 0, new QTableWidgetItem(marks[i].subject));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(marks[i].value)));
        m_table->setItem(i, 2, new QTableWidgetItem(marks[i].date.toString("dd.MM.yyyy")));
    }
}

void TeacherView::addMark() {
    QString login = m_studentBox->currentData().toString();
    if (login.isEmpty()) return;

    QString subject = m_subjectEdit->text();
    int val = m_markEdit->text().toInt();
    if (val < 1 || val > 10) {
        QMessageBox::warning(this, "Ошибка", "Оценка должна быть от 1 до 10");
        return;
    }

    m_store->addMark(login, subject, val, QDate::currentDate());
    refresh();
}

void TeacherView::toggleAbsent() {
    QString login = m_studentBox->currentData().toString();
    if (login.isEmpty()) return;

    bool absent = !m_store->isAbsent(login, QDate::currentDate());
    m_store->setAbsent(login, QDate::currentDate(), absent);
    QMessageBox::information(this, "OK", absent ? "Отмечен пропуск" : "Пропуск снят");
}
