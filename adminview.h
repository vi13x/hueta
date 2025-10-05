#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include "user.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>

class Admin : public User {
public:
    Admin(const QString& username, const QString& password);
    QStringList getPermissions() const override;
};

class AdminView : public QWidget {
    Q_OBJECT

public:
    explicit AdminView(QWidget *parent = nullptr);
    void setAdmin(std::shared_ptr<Admin> admin);
    void refreshData();

private slots:
    void onRefreshClicked();
    void onAddUserClicked();
    void onDeleteUserClicked();

private:
    void setupUI();
    void loadUsers();
    void loadMarks();

    std::shared_ptr<Admin> currentAdmin;
    QVBoxLayout *mainLayout;
    QLabel *welcomeLabel;
    QTabWidget *tabWidget;
    
    // Users tab
    QTableWidget *usersTable;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QComboBox *roleCombo;
    QPushButton *addUserButton;
    QPushButton *deleteUserButton;
    
    // Marks tab
    QTableWidget *marksTable;
    QPushButton *refreshButton;
};

#endif // ADMINVIEW_H
