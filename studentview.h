#ifndef STUDENTVIEW_H
#define STUDENTVIEW_H

#include <QWidget>
#include "datastore.h"

class QTableWidget;

class StudentView : public QWidget {
    Q_OBJECT
public:
    StudentView(const User& u, DataStore* store, QWidget* parent=nullptr);

private:
    void refresh();

    User m_user;
    DataStore* m_store;
    QTableWidget* m_table;
};

#endif
