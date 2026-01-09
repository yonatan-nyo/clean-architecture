Hey - I'm Yonatan.

# Clean Architecture

This document focuses on the purpose behind key design principles, not just their definitions. The goal is to show how each principle helps produce code that is easier to extend, test, and maintain - so adding features or fixing bugs costs less time and effort.

## Programming paradigms - what they aim for

Programming paradigms rob some capabilities from programmers in order to improve maintainability, readability, and testability.

- **Structured programming** - discipline for direct control flow

  - Example: remove unrestricted `goto` statements and replace them with structured constructs (functions, loops, clear block scopes). This makes control flow easier to reason about and simplifies verification.

- **Object-oriented programming** - discipline for indirect control flow

  - Example: objects encapsulate state and behavior; code interacts via well-defined method calls rather than low-level pointer manipulation. Passing objects by reference (or by pointer) enables shared, composable designs for building complex systems.

- **Functional programming** - discipline on assignment and side effects

  - Example: favor immutable data and pure functions. Instead of mutating a transaction list in place, compute the result (e.g., balance) from the list, which simplifies reasoning and helps with concurrency and parallelism. Practical systems balance immutability with performance concerns.

## SOLID Principles

read more in [solid/README.md](solid/README.md),

I recommend reading the SOLID principes here even though u might have learned about it in school/uni before, there are some stuff i read from the clean architecture a craftsman guide to software structure that i found useful and was not mentioned in the usual definitions of the SOLID principles.

## Components

A _component_ is a deployable unit: the smallest part of a system you can independently release (for example, a shared library, a microservice, or a package).

**Why components matter**

- Group related classes and modules into cohesive packages that map to deployment units.
- Keep changes localized: when an implementation changes, prefer redeploying a single component, not the whole system.

**Brief history (concise)**
Early large programs had long compile/link cycles. To reduce build time and support modular development, code was separated into reusable libraries and load-time or link-time binding was introduced - the roots of modern components.

### Component principles

- **REP - Reuse/Release Equivalence Principle**

  - A component is the unit of release: classes and modules that are released together should be grouped into the same component. Group by likely reasons to change and deploy together.

- **CCP - Common Closure Principle**

  - Classes that change for the same reasons should be packaged together. This improves maintainability: a change to one concern affects only its component, not many.

  - CCP is similar to SRP but applied at the component (package) level: separate things that change for different reasons.

- **CRP - Common Reuse Principle**
  - Classes that are reused together should be in the same component. If you depend on a component, you should be prepared to use most of what it provides. Avoid depending on a component that drags in lots of unrelated code.
  - CRP relates to ISP: prefer small, focused components so clients don’t inherit large, irrelevant dependency trees.

**notes**

- We need to balance cohesion and coupling at the component level. High cohesion within components and low coupling between them improves maintainability and flexibility.
- You need to prevent cyclical dependencies between classes or even components. Make a concise foldering and packaging structure between components and make a clear Directed Acyclic Graph (DAG).
- If you depend on some component, its better to have the component you depend on stable (doesnt change a lot).

---

## Architecture

A good architecture must support the system's use cases, operation, development, maintenance, and deployment while keeping options open for future change.

Good architectures are centered on use cases so that architects can safely describe the structures that support those use cases without committing to frameworks, tools, and environments.

### Independence & decoupling

- **Use cases visible:** structure the system so primary use cases are first-class elements (clear modules, descriptive names). This makes intent obvious and behavior easy to find.
- **Operation-ready:** design for the operational needs of each use case (throughput, latency). Keep components isolated so you can scale or move them between threads, processes, or servers as needed.
- **Independent developability:** partition the system into components teams can own; this reduces coordination friction (Conway’s law) and enables parallel work.
- **Immediate deployability:** prefer an architecture that can be deployed without fragile manual steps; isolate wiring/config so components can be started and supervised independently.
- **Decoupling modes:** decouple at the appropriate level, source, deployment, or service, and prefer the minimal mode that leaves the **service** option open if needed later.
- **Duplication caution:** avoid premature consolidation; only remove duplication when it is true duplication (changes follow the same pattern).

### Boundary

- Draw explicit boundaries between concerns: separate what matters to different stakeholders and avoid cross-cutting dependencies.
- Concrete examples:
  - **UI vs Business Rules** - the UI should not contain or depend on domain logic.
  - **Business Rules vs Database** - treat the database as a replaceable detail behind an interface or adapter.
  - **UI vs Database** - avoid direct UI–DB coupling; use view models or adapters to translate between layers.
- Enforce boundaries with interfaces, ports/adapters, and clear data translation layers so each side can evolve independently.

### Business Rules

- Critical business rules (e.g. calculation N% interest for a loan)
- Critical business data (e.g. loan balance, an interest rate, and a payment schedule)

### Entities

- object (or instance of a class) within our computer system that embodies a small set of critical business rules operating on Critical Business Data

### Use Cases

- When the business rules functions inside entities are invoked

### Presenters and Views/humble object

- Presenter: API
- View: GUI
- GUI/View is hard to unit test, but presenters are easy to unit test, therefore GUI should be simple, does not process the data.

### Partial boundaries

Why use a partial boundary? Full boundaries with reciprocal interfaces and independent deploys are costly; a partial boundary lets you prepare for separation without extra release overhead.

- **Common approaches**

  - **Skip-the-last-step:** create the interfaces and I/O contracts but keep components packaged together. Pros: readiness for later separation; cons: same upfront work and risk of boundary erosion.
  - **One-directional / Strategy:** expose a client-facing interface implemented by service classes (dependency inversion in one direction). Sets the stage for a full boundary but needs discipline to avoid backchannels.
  - **Facade:** provide a simple facade class that delegates to internal services. Cheap and practical, but it produces transitive dependencies and makes future separation harder.

- **Trade-offs & guidance**
  - Use partial boundaries when separation is likely but uncertain; prepare abstractions early but avoid adding release/administrative burden prematurely.
  - Enforce boundaries with tests, code reviews, and CI checks to prevent dependency creep.
  - If a partial boundary is never used, remove or consolidate it rather than letting it rot and become a maintenance liability.

### Services / microservices

Microservices can help scale and isolate concerns, but they introduce operational and design complexity. Use them when the architecture and team/operational model genuinely benefit from independently deployable services; otherwise prefer simpler, well-partitioned components.

Key pitfalls:

- **The decoupling fallacy:** services can still be tightly coupled by shared data, network latencies, or platform constraints; decoupling at the code level is not sufficient.
- **The deployment fallacy:** services require more infrastructure, release management, and runtime operational expertise (observability, rolling upgrades, resilience patterns).
- **The development fallacy:** independent development often relies on stubs/mocks and contract tests—without these, integration becomes brittle.

Guidance:

- Start with clear API contracts and contract tests (consumer-driven contracts) before splitting into services.
- Keep service boundaries aligned with business use cases and team organization (Conway’s law).
- Prefer coarse-grained, stable APIs and avoid fine-grained chatty RPCs between services.

---

### Test boundary

Tests that depend on volatile layers (GUIs, external services, hardware) are fragile. Design for testability by keeping business rules and use-case logic independent of volatile details.

Practical testing rules:

- Write fast unit tests for core use cases using in-memory or fake adapters.
- Use integration tests to validate real adapters and protocol-level behavior; keep these slower and more focused.
- Use end-to-end tests only to validate whole-system behavior and user flows; accept they will be fragile and run them less frequently.
- For services, adopt contract tests to catch integration breaks early.
- For embedded systems, provide host-side simulation adapters so unit tests can run in CI without hardware.

---

### Clean Embedded Architecture

Embedded systems have constraints (memory, CPU, real-time requirements) but the same architectural goals apply: keep business logic independent of hardware and firmware.

Principles:

- **Separate concerns:** put domain/use-case logic at the core and hardware/OS specifics at the outer layers.
- **Ports & Adapters / HAL & OSAL:** define minimal interfaces (ports) for peripherals and OS services; implement platform-specific adapters (HAL, OSAL) that satisfy those interfaces.
- **Determinism & resource control:** isolate dynamic allocation and non-deterministic behavior to outer layers; prefer static pools or bounded allocators in real-time paths.
- **Host simulation:** provide fake adapters so the core can be unit-tested and run on a host machine or CI without real hardware.
- **Minimal runtime dependencies:** minimize use of heavy runtime features (exceptions, RTTI, dynamic frameworks) in timing-critical core code.

Practical checklist:

- Identify use cases and write host-executable unit tests for each.
- Define small, stable hardware/service interfaces with clear async/sync behavior.
- Implement adapters for each target platform and a fake adapter for tests.
- Audit and isolate all blocking and heap allocations to adapter layers.
- Document timing and memory budgets and enforce them with CI checks and tests.

A clean embedded architecture makes the core portable, testable, and maintainable while keeping runtime and hardware specifics changeable behind adapters.

---

## Databases

Databases are an implementation detail — keep business rules independent of how data is stored.

- **Databases are details, not domain rules.** The shape of domain data (entities, value objects, invariants) is architecturally significant; the storage technology (files, SQL, NoSQL, or memory) is replaceable.
- **Hide persistence behind ports/adapters.** Define repository/DAO interfaces in the core (use cases/domain) and implement adapters that interact with a specific persistence mechanism.
- **Use in-memory/fake adapters for tests.** Fast, deterministic unit tests should use an in-memory or fake adapter that implements the same interface as the real adapter.
- **Keep mapping explicit.** Translate between domain models and persistence schemas with dedicated mappers or translators, not inside business logic.
- **Let adapters handle operational concerns.** Transactions, batching, indexing, and performance tuning belong in adapter layers — use cases should express business intent only.

**Common patterns & trade-offs**

- **File system:** treat files as a storage adapter behind a small storage interface (useful for simple apps or special persistence needs).
- **RDBMS / NoSQL:** model domain semantics first, then map to the DB design that fits query patterns, consistency, and relationships.
- **In-memory:** great for tests and prototypes; avoid leaking in-memory assumptions into production code.

**Practical recommendations**

- Define small, explicit repository interfaces in the core/domain layer.
- Implement persistence adapters in outer layers; keep them replaceable and well-tested.
- Provide in-memory or fake adapters for unit tests and focused integration tests for real adapters.
- Document operational constraints (transactions, indexes, quotas) in the adapter layer, not in use-case logic.

> The data model is significant; the database is a detail.

## GUI

GUI is a detail - keep business rules independent of how data is presented.

U don't know what marketing will want in the future, so dont couple your business rules to the GUI. decouple it!
Keep GUI as stupid as possible, and put all the logic in the presenter (or view model).

## Frameworks

Framework is a detail - keep business rules independent of frameworks and tools.

Business rules should be ignorant of frameworks. Frameworks come and go, but business rules endure. Treat frameworks as tools to implement details, not as drivers of core logic.

Keep the framework behind an architectural boundary if at all possible, for as long as possible. Perhaps you can find a way to get the milk without buying the cow.

## Architectures & packaging

- **Four common styles**:
  - **Package by Layer** (horizontal layers: web, services, persistence) is easy to start but hides domain intent and scales poorly;
  - **Package by Feature** (vertical slices) improves discoverability;
  - **Ports & Adapters (hexagonal)** separates an inside (domain) from an outside (infrastructure) with dependencies pointing inward;
  - **Package by Component** groups business logic and persistence into a single component (a deployable unit) to keep related code together.
- **Risks**: overly liberal use of `public` breaks encapsulation and enables cross-boundary violations (e.g., controllers accessing repositories directly), creating a "relaxed layered" architecture that undermines intended rules.
- minimize public surface area, enforce boundaries with **interfaces**, tests, and static analysis (or compiler checks), and prefer component-focused organization when you want clearer ownership and easier evolution toward services.

## Takeaways

- **Prioritize maintainability, testability, and flexibility over initial convenience.** Favor designs that are easier to change and verify.
- **Keep business rules isolated from implementation details (UI, DB, frameworks).** Treat databases, GUIs, and frameworks as replaceable details behind clear interfaces.
- **Apply SOLID and modularization incrementally.** Avoid premature modularization — introduce abstractions when they solve real problems (but dont ignore them, this will result in a tangled codebase, especially if you are working in a team).
- **Enforce boundaries with tests, reviews, and automation.** Use unit tests, contract tests, CI checks, and static analysis to prevent boundary erosion.

---

## References

Martin, R. C. (2017). Clean architecture: A craftsman's guide to software structure and design.

---
