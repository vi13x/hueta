#include "studentview.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>

StudentView::StudentView(const User& u, DataStore* store, QWidget* parent)
    : QWidget(parent), m_user(u), m_store(store)
{
    auto layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Оценки и статистика (1-10): " + u.fullName, this));

    m_table = new QTableWidget(this);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({"Предмет", "Оценка", "Дата"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_table);

    refresh();

    connect(m_store, &DataStore::changed, this, &StudentView::refresh);
}

void StudentView::refresh() {
    auto marks = m_store->getMarks(m_user.username);
    m_table->setRowCount(marks.size());
    for (int i=0; i<marks.size(); ++i) {
        m_table->setItem(i, 0, new QTableWidgetItem(marks[i].subject));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(marks[i].value)));
        m_table->setItem(i, 2, new QTableWidgetItem(marks[i].date.toString("dd.MM.yyyy")));
    }
}
