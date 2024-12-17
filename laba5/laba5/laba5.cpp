#include <iostream>
#include <vector>
#include <string>

using namespace std;

class HospitalWorker {
public:
    virtual void ManageWork(int hours) = 0;
};

class Doctor : public HospitalWorker {
private:
    int workedHours;
public:
    Doctor() : workedHours(0) {}

    void ManageWork(int hours) override {
        workedHours += hours;
        cout << "Doctor worked " << hours << " hours.\n";
    }
};

class Nurse : public HospitalWorker {
private:
    int workedHours;
public:
    Nurse() : workedHours(0) {}

    void ManageWork(int hours) override {
        workedHours += hours;
        cout << "Nurse worked " << hours << " hours.\n";

    }
};

class Administrator : public HospitalWorker {
private:
    int workedHours;
public:
    Administrator() : workedHours(0) {}

    void ManageWork(int hours) override {
        workedHours += hours;
        cout << "Administrator worked " << hours << " hours.\n";
    }
};

int main() {

    vector<HospitalWorker*> workers;

    workers.push_back(new Doctor());
    workers.push_back(new Nurse());
    workers.push_back(new Administrator());
    workers[0]->ManageWork(5); // Врач работает 5 часов
    workers[1]->ManageWork(8); // Медсестра работает 8 часов
    workers[2]->ManageWork(4); // Администратор работает 4 часа

    return 0;
}