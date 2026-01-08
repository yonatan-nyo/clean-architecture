#include <bits/stdc++.h>

using namespace std;

// Simpler DIP example: Payment processing
// Bad: high-level code depends on a concrete payment gateway and must change
// whenever that gateway's API changes.

// Concrete, volatile gateway (changes often)
class StripeGatewayV1 {
public:
    bool charge(int cents) {
        cout << "StripeV1 charging " << cents << " cents\n";
        return true;
    }
};

// New version of the gateway: API changed
class StripeGatewayV2 {
public:
    // Now requires currency and returns a transaction id
    string charge(int cents, const string &currency) {
        cout << "StripeV2 charging " << cents << " " << currency << "\n";
        return "tx-123";
    }
};

// Bad app: depends directly on the concrete
class ShopBadV1 {
    StripeGatewayV1 gw;

public:
    void checkout(int cents) {
        gw.charge(cents);
        cout << "checkout complete (bad v1)\n";
    }
};
class ShopBadV2 {
    StripeGatewayV2 gw;

public:
    void checkout(int cents) {
        gw.charge(cents, "USD");
        cout << "checkout complete (bad v2)\n";
    }
};

// --- DIP-friendly design ---
struct IPaymentGateway {
    virtual ~IPaymentGateway() = default;
    virtual bool charge(int cents) = 0; // stable, simple abstraction
};

// Adapter for the volatile StripeGatewayV2 to the stable interface
class StripeV2Adapter : public IPaymentGateway {
    StripeGatewayV2 inner;
    string currency = "USD";

public:
    bool charge(int cents) override {
        inner.charge(cents, currency);
        return true;
    }
};

// Another concrete implementation (e.g., PayPal) implementing the same abstraction
class PayPalGateway : public IPaymentGateway {
public:
    bool charge(int cents) override {
        cout << "PayPal charging " << cents << " cents\n";
        return true;
    }
};

// Shop depends only on the abstraction
class Shop {
    IPaymentGateway &gateway;

public:
    Shop(IPaymentGateway &g) : gateway(g) {}
    void checkout(int cents) {
        gateway.charge(cents);
        cout << "checkout complete (good)\n";
    }
};

int main() {
    cout << "-- Tight coupling (bad):" << endl;
    ShopBadV1 s1;
    s1.checkout(1000);
    ShopBadV2 s2;
    s2.checkout(2000);

    cout << "\n-- DIP-friendly (good):" << endl;
    StripeV2Adapter stripeAdapter;
    Shop shopStripe(stripeAdapter);
    shopStripe.checkout(1500); // no Shop changes despite gateway API differences

    PayPalGateway paypal;
    Shop shopPaypal(paypal);
    shopPaypal.checkout(500); // swap impl without changing Shop

    cout << "\nObservation: Shop depends only on IPaymentGateway; adapting volatile concretes via adapters keeps high-level code stable." << endl;

    return 0;
}
