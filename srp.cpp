#include <bits/stdc++.h>

using namespace std;

// Example: SRP Violation
// The class below violates the Single Responsibility Principle because

// it handles multiple reasons to change:
// payroll calculation (HR/Finance),
// persistence (DB admin), and
// logging (Ops).

// This makes it hard to maintain.
class EmployeeBad {
public:
    EmployeeBad(string name, double hourlyRate)
        : name(std::move(name)), hourlyRate(hourlyRate) {}

    double calculatePay(int hours) {
        return hours * hourlyRate;
    }

    void saveToFile() {
        ofstream f(name + ".txt");
        f << "Employee: " << name << "\n";
        f << "Rate: " << hourlyRate << "\n";
    }

    void log(const string &message) {
        cout << "[LOG] " << message << endl;
    }

private:
    string name;
    double hourlyRate;
};

// SRP-friendly design: separate responsibilities into focused classes.
class PayrollCalculator {
public:
    double calculatePay(double hourlyRate, int hours) {
        return hourlyRate * hours;
    }
};

class EmployeeRepository {
public:
    void save(const string &name, double rate) {
        ofstream f(name + ".txt");
        f << "Employee: " << name << "\n";
        f << "Rate: " << rate << "\n";
    }
};

class Logger {
public:
    void info(const string &msg) { cout << "[INFO] " << msg << endl; }
    void error(const string &msg) { cerr << "[ERROR] " << msg << endl; }
};

class Employee {
public:
    Employee(string name, double rate) : name(std::move(name)), hourlyRate(rate) {}
    string getName() const { return name; }
    double getRate() const { return hourlyRate; }

private:
    string name;
    double hourlyRate;
};

// Small demo showing how responsibilities are separated.
inline void demoSRP() {
    Employee emp("Alice", 25.0);
    PayrollCalculator calc;
    EmployeeRepository repo;
    Logger logger;

    int hours = 40;
    double pay = calc.calculatePay(emp.getRate(), hours);
    logger.info(emp.getName() + " earned $" + to_string(pay));
    repo.save(emp.getName(), emp.getRate());
}

/*
Explanation (short):
- EmployeeBad mixes concerns: payroll logic, persistence, logging.
  Stakeholders with different reasons to change:
  - HR/Finance may change payroll rules (affects calculatePay)
  - DB admin may change storage format (affects saveToFile)
  - Ops may change logging (affects log)

- In the SRP-friendly refactor, each class has one reason to change:
  - PayrollCalculator: payroll rules
  - EmployeeRepository: persistence
  - Logger: logging format/destination
  - Employee: the domain entity

This reduces coupling and makes code easier to test,
reducing merge conflicts and improving maintainability.
*/

int main() {
    demoSRP();
    return 0;
}
