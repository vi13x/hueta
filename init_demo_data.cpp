#include "datastore.h"
#include "studentview.h"
#include "teacherview.h"
#include "adminview.h"

void initDemoData() {
    DataStore& store = DataStore::getInstance();
    
    // Clear existing data
    // store.clearAllData(); // Would need to implement this method
    
    // Add demo users
    store.addUser(std::make_shared<Admin>("admin", "admin123"));
    store.addUser(std::make_shared<Teacher>("teacher1", "teacher123"));
    store.addUser(std::make_shared<Teacher>("teacher2", "teacher123"));
    store.addUser(std::make_shared<Student>("student1", "student123"));
    store.addUser(std::make_shared<Student>("student2", "student123"));
    store.addUser(std::make_shared<Student>("student3", "student123"));
    
    // Add demo marks
    store.addMark("student1", "Математика", 5);
    store.addMark("student1", "Математика", 4);
    store.addMark("student1", "Физика", 5);
    store.addMark("student1", "Информатика", 5);
    
    store.addMark("student2", "Математика", 3);
    store.addMark("student2", "Физика", 4);
    store.addMark("student2", "Химия", 3);
    
    store.addMark("student3", "Математика", 5);
    store.addMark("student3", "Химия", 4);
    store.addMark("student3", "Русский язык", 5);
    store.addMark("student3", "Информатика", 4);
}
