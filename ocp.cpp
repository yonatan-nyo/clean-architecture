#include <bits/stdc++.h>

using namespace std;

// OCP Violation: Report generator with a switch/enum
// Adding a new format requires modifying generateReport (open code modification required).
class ReportGeneratorBad {
public:
    enum Format { PDF,
                  CSV };

    void generateReport(Format f) {
        if (f == PDF) {
            cout << "Generating PDF report (bad)\n";
        } else if (f == CSV) {
            cout << "Generating CSV report (bad)\n";
        }
    }
};

// OCP-friendly design: define an abstract Exporter and add new exporters
// by implementing the interface. Existing code doesn't need modification.

struct Exporter {
    virtual ~Exporter() = default;
    virtual void exportReport(const string &data) = 0;
};

struct PDFExporter : Exporter {
    void exportReport(const string &data) override {
        cout << "PDF export: " << data << "\n";
    }
};

struct CSVExporter : Exporter {
    void exportReport(const string &data) override {
        cout << "CSV export: " << data << "\n";
    }
};

class ReportService {
public:
    void addExporter(unique_ptr<Exporter> e) { exporters.push_back(move(e)); }

    void generate(const string &data) {
        for (auto &e : exporters)
            e->exportReport(data);
    }

private:
    vector<unique_ptr<Exporter>> exporters;
};

inline void demoOCP() {
    cout << "-- Bad implementation (violates OCP):" << endl;
    ReportGeneratorBad bad;
    bad.generateReport(ReportGeneratorBad::PDF);

    cout << "\n-- Refactored (follows OCP):" << endl;
    ReportService svc;
    svc.addExporter(make_unique<PDFExporter>());
    svc.addExporter(make_unique<CSVExporter>());
    svc.generate("Quarterly sales data");

    cout << "\nAdd a new exporter (e.g., XML) by creating a new class that implements Exporter - no changes to ReportService are needed." << endl;
}

int main() {
    demoOCP();
    return 0;
}
