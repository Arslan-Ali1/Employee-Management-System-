#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <cstdlib>
using namespace std;


class Person {
protected:
    string name;

public:
    Person() { name = ""; }

    void setName(string n) { name = n; }
    string getName() const { return name; }

    string getDetails() const {
        return "Name: " + name;
    }
};


class Position : public Person {
protected:
    string position;

public:
    Position() { position = ""; }

    void setPosition(string p) { position = p; }
    string getPosition() const { return position; }

    string getDetails() const {
        return Person::getDetails() + "\nPosition: " + position;
    }
};

class Employee : public Position {
private:
    int id;
    double salary;

public:
    Employee() {
        id = 0;
        salary = 0.0;
    }

    void setId(int i) { id = i; }
    void setSalary(double s) { salary = s; }

    int getId() const { return id; }
    double getSalary() const { return salary; }

    string getDetails() const {
        return Position::getDetails() +
               "\nID: " + to_string(id) +
               "\nSalary: $" + to_string(salary) +
               "\n-------------------------\n";
    }
};

class EmployeeDatabase {
private:
    vector<Employee> employees;

public:
    int generateId() {
        int maxId = 0;
        for (int i = 0; i < employees.size(); i++) {
            if (employees[i].getId() > maxId)
                maxId = employees[i].getId();
        }
        return maxId + 1;
    }

    void addEmployee(string name, string position, double salary) {
        Employee emp;
        emp.setId(generateId());
        emp.setName(name);
        emp.setPosition(position);
        emp.setSalary(salary);
        employees.push_back(emp);
    }

    vector<Employee> getAllEmployees() {
        return employees;
    }

    int findEmployeeIndex(int id) {
        for (int i = 0; i < employees.size(); i++) {
            if (employees[i].getId() == id)
                return i;
        }
        return -1;
    }

    bool updateEmployee(int id, string name, string position, double salary) {
        int index = findEmployeeIndex(id);
        if (index == -1) return false;

        if (name != "") employees[index].setName(name);
        if (position != "") employees[index].setPosition(position);
        if (salary > 0) employees[index].setSalary(salary);
        return true;
    }

    bool deleteEmployee(int id) {
        int index = findEmployeeIndex(id);
        if (index == -1) return false;
        employees.erase(employees.begin() + index);
        return true;
    }

    vector<Employee> searchByName(string name) {
        vector<Employee> result;
        for (int i = 0; i < employees.size(); i++) {
            if (employees[i].getName() == name)
                result.push_back(employees[i]);
        }
        return result;
    }

    Employee getEmployee(int id) {
        int index = findEmployeeIndex(id);
        if (index != -1)
            return employees[index];
        else
            return Employee();
    }

    bool saveToFile(string filename) {
        ofstream out(filename);
        if (!out) return false;

        for (int i = 0; i < employees.size(); i++) {
            out << employees[i].getId() << ","
                << employees[i].getName() << ","
                << employees[i].getPosition() << ","
                << employees[i].getSalary() << "\n";
        }
        out.close();
        return true;
    }

    bool loadFromFile(string filename) {
        ifstream in(filename);
        if (!in) return false;

        employees.clear();
        string line;
        while (getline(in, line)) {
            int id;
            string name, position;
            double salary;

            int p1 = line.find(',');
            int p2 = line.find(',', p1 + 1);
            int p3 = line.find(',', p2 + 1);

            if (p1 == -1 || p2 == -1 || p3 == -1) continue;

            id = stoi(line.substr(0, p1));
            name = line.substr(p1 + 1, p2 - p1 - 1);
            position = line.substr(p2 + 1, p3 - p2 - 1);
            salary = stod(line.substr(p3 + 1));

            Employee e;
            e.setId(id);
            e.setName(name);
            e.setPosition(position);
            e.setSalary(salary);
            employees.push_back(e);
        }
        in.close();
        return true;
    }

    bool isEmpty() {
        return employees.empty();
    }

    void sortBySalary() {
        for (int i = 0; i < employees.size() - 1; i++) {
            for (int j = i + 1; j < employees.size(); j++) {
                if (employees[i].getSalary() < employees[j].getSalary()) {
                    swap(employees[i], employees[j]);
                }
            }
        }
    }

    void generateReport() {
        if (employees.empty()) {
            cout << "No data to report.\n";
            return;
        }

        double total = 0;
        Employee highest = employees[0];
        for (int i = 0; i < employees.size(); i++) {
            total += employees[i].getSalary();
            if (employees[i].getSalary() > highest.getSalary())
                highest = employees[i];
        }

        cout << "Total Employees: " << employees.size() << "\n";
        cout << "Average Salary: $" << (total / employees.size()) << "\n";
        cout << "Highest Paid Employee:\n" << highest.getDetails();
    }
};
void menu() {
    system("cls");  
    cout << "\nEMPLOYEE MANAGEMENT SYSTEM\n";
    cout << "1. Add Employee\n";
    cout << "2. View All Employees\n";
    cout << "3. Update Employee\n";
    cout << "4. Delete Employee\n";
    cout << "5. Search by ID\n";        
    cout << "6. Search by Name\n";
    cout << "7. Sort by Salary\n";
    cout << "8. Report\n";
    cout << "9. Save to File\n";
    cout << "10. Exit\n";
    cout << "Enter your choice: ";
}


int main() {
    EmployeeDatabase db;
    db.loadFromFile("emp.txt");

    int choice;
    do {
        menu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            string name, pos;
            double sal;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter position: ";
            getline(cin, pos);
            cout << "Enter salary: ";
            cin >> sal;
            db.addEmployee(name, pos, sal);
        } else if (choice == 2) {
            vector<Employee> list = db.getAllEmployees();
            for (Employee e : list)
                cout << e.getDetails();
        } else if (choice == 3) {
            int id;
            string name, pos;
            double sal;
            cout << "Enter ID to update: ";
            cin >> id;
            cout << "New name (leave empty to skip): ";
            cin.ignore();
            getline(cin, name);
            cout << "New position: ";
            getline(cin, pos);
            cout << "New salary (0 to skip): ";
            cin >> sal;
            if (db.updateEmployee(id, name, pos, sal))
                cout << "Updated!\n";
            else
                cout << "Employee not found!\n";
        } else if (choice == 4) {
            int id;
            cout << "Enter ID to delete: ";
            cin >> id;
            if (db.deleteEmployee(id))
                cout << "Deleted!\n";
            else
                cout << "Not found!\n";
        }  else if (choice == 5) {
    int id;
    cout << "Enter Employee ID: ";
    cin >> id;

    Employee emp = db.getEmployee(id);
    if (emp.getId() == 0)  
        cout << "Employee not found!\n";
    else
        cout << emp.getDetails();
}

		
		
		
		 else if (choice == 6) {
            string name;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            vector<Employee> found = db.searchByName(name);
            if (found.empty()) cout << "Not found!\n";
            else for (Employee e : found) cout << e.getDetails();
        } else if (choice == 7) {
            db.sortBySalary();
            cout << "Sorted by salary!\n";
        } else if (choice == 8) {
            db.generateReport();
        } else if (choice == 9) {
            if (db.saveToFile("emp.txt")) cout << "Saved to file.\n";
            else cout << "Save failed.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    } while (choice != 10);

    return 0;
}
