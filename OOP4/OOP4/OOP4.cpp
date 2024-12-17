#include <iostream>
#include <vector>
#include <string>

using namespace std;

class MedicalRecord {
public:
    string history;

    MedicalRecord(const string& history) : history(history) {}
};

class Patient {
private:
    string name;
    int age;
    MedicalRecord medicalRecord;

public:
    Patient(const string& name, int age, const string& history)
        : name(name), age(age), medicalRecord(history) {}

    void displayInfo() const {
        cout << "Patient Name: " << name << ", Age: " << age << endl;
        cout << "Medical History: " << medicalRecord.history << endl;
    }

    string getName() const {
        return name;
    }
};

class Doctor {
private:
    string name;

public:
    Doctor(const string& name) : name(name) {}

    void treatPatient(const Patient& patient) const {
        cout << "Doctor " << name << " is treating patient " << patient.getName() << "." << endl;
    }
};

class Department {
private:
    string name;
    vector<Patient> patients;   

public:
    Department(const string& name) : name(name) {}

    void addPatient(const Patient& patient) {
        patients.push_back(patient);
    }

    void displayPatients() const {
        cout << "Department: " << name << endl;
        for (const auto& patient : patients) {
            patient.displayInfo();
        }
    }
};

class Hospital {
private:
    vector<Department> departments;

public:
    void addDepartment(const Department& department) {
        departments.push_back(department);
    }

    void displayDepartments() const {
        for (const auto& department : departments) {
            department.displayPatients();
        }
    }
};

int main() {
    // Создание пациентов
    Patient patient1("Ivanov Ivan", 30, "Flu");
    Patient patient2("Petrov Petr", 45, "Cold");

    // Создание врача
    Doctor doctor1("Dr. Smith");

    // Создание отделения и добавление пациентов
    Department department1("Therapy");
    department1.addPatient(patient1); 
    department1.addPatient(patient2);

    // Создание больницы и добавление отделения
    Hospital hospital;
    hospital.addDepartment(department1);

    // Отображение информации о пациентах в больнице
    hospital.displayDepartments();

    // Врач лечит пациента
    doctor1.treatPatient(patient1);

    return 0;
}
