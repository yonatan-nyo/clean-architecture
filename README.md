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
