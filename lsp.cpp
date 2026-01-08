#include <bits/stdc++.h>

using namespace std;

// LSP Violation example: Square inherits Rectangle and overrides setters
// A function that expects a Rectangle's behavior can be surprised by Square.

class Rectangle {
protected:
    int width, height;

public:
    Rectangle(int w, int h) : width(w), height(h) {}
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int area() const { return width * height; }
};

class Square : public Rectangle {
public:
    Square(int size) : Rectangle(size, size) {}
    void setWidth(int w) override { width = height = w; }
    void setHeight(int h) override { width = height = h; }
};

// This function mutates a Rectangle and expects area = 5 * 10 = 50 afterwards.
void testArea(Rectangle &r) {
    r.setWidth(5);
    r.setHeight(10);
    cout << "Expected area = 50, got " << r.area() << "\n";
}

// LSP-friendly designs:
// 1) Prefer immutable shapes (no independent setters) that expose only behaviors
struct IShape {
    virtual ~IShape() = default;
    virtual int area() const = 0;
};

class Rectangle2 : public IShape {
    int width, height;

public:
    Rectangle2(int w, int h) : width(w), height(h) {}
    int area() const override { return width * height; }
};

class Square2 : public IShape {
    int size;

public:
    Square2(int s) : size(s) {}
    int area() const override { return size * size; }
};

// 2) Or provide a resizing interface that only true rectangles implement
struct IResizableRect {
    virtual ~IResizableRect() = default;
    virtual void setWidth(int w) = 0;
    virtual void setHeight(int h) = 0;
    virtual int area() const = 0;
};

class RectResizable : public IResizableRect {
    int width, height;

public:
    RectResizable(int w, int h) : width(w), height(h) {}
    void setWidth(int w) override { width = w; }
    void setHeight(int h) override { height = h; }
    int area() const override { return width * height; }
};

int main() {
    cout << "-- Bad example (violates LSP):" << endl;
    Rectangle rc(2, 3);
    testArea(rc); // Expected area = 50, got 50

    Square sq(5);
    testArea(sq); // Expected area = 50, got 100 -> LSP violated

    cout << "\n-- Refactored (follows LSP):" << endl;
    Rectangle2 r2(5, 10);
    Square2 s2(5);
    cout << "Rectangle2 area expected 50, got " << r2.area() << "\n";
    cout << "Square2 area expected 25, got " << s2.area() << "\n";

    cout << "\n-- Using a resizable rectangle (Square does not implement this):" << endl;
    RectResizable rr(2, 3);
    rr.setWidth(5);
    rr.setHeight(10);
    cout << "RectResizable area expected 50, got " << rr.area() << "\n";

    return 0;
}
