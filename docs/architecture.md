# Architecture

UniRoute is a single-process console application organized around four domain
modules. The module boundaries follow the responsibilities of the original
`main.cpp` sections, rather than depending on their old line numbers.

## Modules

### Shared helpers

`include/uniroute/common.hpp` provides inline input and serialization helpers:
case normalization, pipe-delimited field splitting, integer input, and
line-based input. `src/common.cpp` defines the shared input-EOF state and the
two data-file names.

### 1. Student registry

`include/uniroute/student_registry.hpp` defines `StudentRecord`,
`StudentRegistry`, and the sample-data loader. The registry owns a singly
linked list and implements student lookup, insertion, deletion, updates,
sorting, searching, display, and file persistence.

### 2. Route and shift

`include/uniroute/route_shift.hpp` defines `RouteShift`. It presents available
routes and shifts and keeps track of the current selection used by the
verification flow.

### 3. Verification

`include/uniroute/verification.hpp` defines `Verification` and the daily trip
record. Verification checks the student record, fee status, selected route and
shift, pass type, and per-day trip limit before granting access. The module
also tracks attempt statistics.

### 4. Audit and application

`include/uniroute/audit_log.hpp` defines the verification-log records, the
linked-list-backed stack, and `AuditLog` persistence and display operations.
`src/main.cpp` coordinates the modules, prompts the user, dispatches menu
actions, and saves data when the application exits.

## Verification flow

1. The application loads saved student and audit records. If no student
   records are available, it loads the sample database.
2. The operator selects a route and shift.
3. The operator verifies a student by student ID or pass ID.
4. `Verification` applies the pass rules and returns an outcome and reason.
5. `AuditLog` records the attempt and can display the latest or
   student-specific history.
6. Student and audit data are saved to the current working directory.

## Persistence

`students.txt` and `logs.txt` use one pipe-delimited record per line. The
application creates or overwrites these files when saving. They are runtime
data and should not be committed to the repository.

## Build

CMake builds the `uniroute` executable from `src/main.cpp` and
`src/common.cpp`; the domain modules are included from `include/uniroute`.
The code targets C++11.
