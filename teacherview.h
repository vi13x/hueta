#ifndef TEACHERVIEW_H
#define TEACHERVIEW_H

#include <QWidget>
#include "datastore.h"

class QComboBox;
class QTableWidget;
class QPushButton;
class QLineEdit;

class TeacherView : public QWidget {
    Q_OBJECT
public:
    TeacherView(const User& u, DataStore* store, QWidget* parent=nullptr);

private slots:
    void refresh();
    void addMark();
    void toggleAbsent();

private:
    User m_user;
    DataStore* m_store;

    QComboBox* m_classBox;
    QComboBox* m_studentBox;
    QLineEdit* m_subjectEdit;
    QLineEdit* m_markEdit;
    QPushButton* m_addMarkBtn;
    QPushButton* m_absentBtn;

    QTableWidget* m_table;
};

#endif
