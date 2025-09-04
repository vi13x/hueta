#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QWidget>
#include "datastore.h"

class QTableWidget;
class QPushButton;
class QLineEdit;

class AdminView : public QWidget {
    Q_OBJECT
public:
    AdminView(DataStore* store, QWidget* parent=nullptr);

private slots:
    void refresh();
    void removeUser();
    void editSchedule();

private:
    DataStore* m_store;
    QTableWidget* m_table;
    QPushButton* m_removeBtn;
    QPushButton* m_scheduleBtn;
};

#endif
