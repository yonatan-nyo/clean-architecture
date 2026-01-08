# A Craftsman's Guide to Software Structure

## SOLID Principles

### SRP - Single Responsibility Principle

- A module should have one, and only one, reason to change.
- A module should be responsible to one, and only one, user or stakeholder.
- A module should be responsible to one, and only one, actor.
- A module should encapsulate a single concern - changes from one actor should affect one module only (implementation may still be split across multiple functions/files for its respective actor to prevent excessive code length).

### OCP - Open-Closed Principle

- A software artifact should be open for extension but closed for modification.
- New functionality should be added by adding new code, not by changing existing code (or with minimal changes).

### LSP - Liskov Substitution Principle

- Subtypes must be substitutable for their base types without surprising callers.
- Avoid subtype behaviors that break callers' expectations (e.g., `Square` that overrides `setWidth`/`setHeight` and surprises `Rectangle`-using code).

### ISP - Interface Segregation Principle

- Clients should not be forced to depend on interfaces they do not use.
- Prefer many small, specific interfaces over one large, general-purpose interface.

### DIP - Dependency Inversion Principle

- Depend on **abstractions** (interfaces/abstract types), not on concrete implementations. This reduces coupling and improves flexibility (there's some exclusion to what rarely changes, we can depend on the concrete implementation).
- Main reason: interfaces change less often than implementations, so depend on them.
- Avoid direct dependencies on _volatile_ concrete modules (those under active development or likely to change).
- Use factories or dependency injection to create concrete instances without introducing source-level dependencies on those concrete classes.
- Practical rules of thumb:
  - Don’t refer to volatile concrete classes from high-level code.
  - Don’t derive from volatile concrete classes; prefer inheriting from interfaces or abstract classes instead.
  - Avoid overriding concrete functions that introduce hidden dependencies; prefer abstract methods with multiple implementations.

**Examples:** see `srp.cpp`, `ocp.cpp`, `lsp.cpp`, `isp.cpp`, `dip.cpp` for short "violation vs refactor" demos.

back to [top README](../README.md)
