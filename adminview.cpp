#include "adminview.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

AdminView::AdminView(DataStore* store, QWidget* parent)
    : QWidget(parent), m_store(store)
{
    auto layout = new QVBoxLayout(this);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Логин", "ФИО", "Роль", "Класс"});
    m_table->horizontalHeader()->setStretchLastSection(true);

    m_removeBtn = new QPushButton("Удалить пользователя", this);
    m_scheduleBtn = new QPushButton("Редактировать расписание", this);

    layout->addWidget(m_table);
    layout->addWidget(m_removeBtn);
    layout->addWidget(m_scheduleBtn);

    connect(m_removeBtn, &QPushButton::clicked, this, &AdminView::removeUser);
    connect(m_scheduleBtn, &QPushButton::clicked, this, &AdminView::editSchedule);
    connect(m_store, &DataStore::changed, this, &AdminView::refresh);

    refresh();
}

void AdminView::refresh() {
    auto users = m_store->allUsers();
    m_table->setRowCount(users.size());
    for (int i=0; i<users.size(); ++i) {
        auto u = users[i];
        QString roleStr = (u.role==Role::Student?"Ученик":u.role==Role::Teacher?"Учитель":"Админ");
        m_table->setItem(i, 0, new QTableWidgetItem(u.username));
        m_table->setItem(i, 1, new QTableWidgetItem(u.fullName));
        m_table->setItem(i, 2, new QTableWidgetItem(roleStr));
        m_table->setItem(i, 3, new QTableWidgetItem(u.classId));
    }
}

void AdminView::removeUser() {
    auto item = m_table->currentItem();
    if (!item) return;
    QString login = m_table->item(item->row(), 0)->text();
    if (QMessageBox::question(this, "Удаление", "Удалить пользователя "+login+"?") == QMessageBox::Yes) {
        m_store->removeUser(login);
    }
}

void AdminView::editSchedule() {
    bool ok=false;
    int day = QInputDialog::getInt(this, "День недели", "1=Пн ... 7=Вс", 1, 1, 7, 1, &ok);
    if (!ok) return;

    QStringList lessons = m_store->getSchedule(day);
    QString text = lessons.join(", ");
    text = QInputDialog::getText(this, "Уроки", "Введите уроки через запятую:", QLineEdit::Normal, text, &ok);
    if (!ok) return;

    lessons = text.split(",", Qt::SkipEmptyParts);
    for (QString &s : lessons) s = s.trimmed();
    m_store->setSchedule(day, lessons);
}
