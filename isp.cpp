#include <bits/stdc++.h>

using namespace std;

// Fat interface (bad): forces implementers to support both printing and scanning
struct IFatDevice {
    virtual ~IFatDevice() = default;
    virtual void print(const string &doc) = 0;
    virtual string scan() = 0;
};

class PrinterOnlyBad : public IFatDevice {
public:
    void print(const string &doc) override { cout << "PrinterOnlyBad prints: " << doc << "\n"; }
    string scan() override { throw runtime_error("scan not supported"); }
};

// Segregated interfaces (good)
struct ICanPrint {
    virtual ~ICanPrint() = default;
    virtual void print(const string &doc) = 0;
};
struct ICanScan {
    virtual ~ICanScan() = default;
    virtual string scan() = 0;
};

class Printer : public ICanPrint {
public:
    void print(const string &doc) override { cout << "Printer prints: " << doc << "\n"; }
};
class Scanner : public ICanScan {
public:
    string scan() override { return "scanned data"; }
};

// Clients depend only on what they need
void clientNeedsPrint(ICanPrint &p) { p.print("Report"); }
void clientNeedsScan(ICanScan &s) { cout << "Client got: " << s.scan() << "\n"; }

// Device that implements both interfaces (printer + scanner)
class MultiFunctionDevice : public ICanPrint, public ICanScan {
public:
    void print(const string &doc) override { cout << "MultiFunctionDevice prints: " << doc << "\n"; }
    string scan() override { return "MultiFunctionDevice scanned content"; }
};

int main() {
    cout << "-- ISP-friendly (segregated interfaces):" << endl;
    Printer goodP;
    Scanner goodS;
    clientNeedsPrint(goodP);
    clientNeedsScan(goodS);

    cout << "\n-- Multi-function device that implements both interfaces:" << endl;
    MultiFunctionDevice mfd;
    clientNeedsPrint(mfd); // uses ICanPrint interface
    clientNeedsScan(mfd);  // uses ICanScan interface

    return 0;
}
