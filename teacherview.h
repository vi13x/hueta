#ifndef TEACHERVIEW_H
#define TEACHERVIEW_H

#include "user.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QHeaderView>

class Teacher : public User {
public:
    Teacher(const QString& username, const QString& password);
    QStringList getPermissions() const override;
};

class TeacherView : public QWidget {
    Q_OBJECT

public:
    explicit TeacherView(QWidget *parent = nullptr);
    void setTeacher(std::shared_ptr<Teacher> teacher);
    void refreshData();

private slots:
    void onRefreshClicked();
    void onAddMarkClicked();

private:
    void setupUI();
    void loadStudents();
    void loadMarks();

    std::shared_ptr<Teacher> currentTeacher;
    QVBoxLayout *mainLayout;
    QLabel *welcomeLabel;
    QTableWidget *marksTable;
    QComboBox *studentCombo;
    QComboBox *subjectCombo;
    QSpinBox *markSpinBox;
    QPushButton *addMarkButton;
    QPushButton *refreshButton;
};

#endif // TEACHERVIEW_H
