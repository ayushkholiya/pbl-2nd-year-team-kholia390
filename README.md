# UniRoute — Smart Bus Verification System

UniRoute is a command-line student bus-pass verification application written in
C++. It keeps student pass records, checks a student's fee, route, shift, and
daily trip allowance, and stores verification history between runs.

## Features

- Register, update, delete, display, sort, and search student records.
- Verify students by student ID or pass ID.
- Check fee status, selected route and shift, pass type, and daily trip limits.
- View aggregate verification statistics and per-student history.
- Persist student records and audit logs as pipe-delimited text files.
- Load a sample database when no student data has been saved yet.

## Build and run

### CMake

Requires CMake 3.10 or later and a C++11 compiler.

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\uniroute.exe
```

For single-configuration generators, the executable is usually
`build\uniroute.exe`.

### Direct compiler command

With GCC or MinGW:

```powershell
g++ -std=c++11 -Wall -Wextra -pedantic -Iinclude src\main.cpp src\common.cpp -o uniroute.exe
.\uniroute.exe
```

The program reads console input and displays a numbered menu. Choose `0` to
save data and exit.

## Data files

The program reads and writes `students.txt` and `logs.txt` in its current
working directory. Both files are created on save and are intentionally
excluded from version control because they contain application data.

Student rows contain the student ID, name, route, shift, pass ID, fee status,
and pass type. Log rows contain the student ID, selected route and shift,
result, and reason. Fields are separated by `|`.

## Project modules

| Module | Files | Responsibility |
| --- | --- | --- |
| Shared helpers | `include/uniroute/common.hpp`, `src/common.cpp` | Input parsing, case conversion, line parsing, and shared file/state definitions |
| 1 — Student registry | `include/uniroute/student_registry.hpp` | Student records, linked-list registry, persistence, and sample data |
| 2 — Route and shift | `include/uniroute/route_shift.hpp` | Route/shift selection and validation |
| 3 — Verification | `include/uniroute/verification.hpp` | Pass checks, daily trip limits, and statistics |
| 4 — Audit and application | `include/uniroute/audit_log.hpp`, `src/main.cpp` | Verification history, menu, and application flow |

The domain modules are kept in focused headers; `src/main.cpp` composes them
into the command-line application.

## Team module ownership

These are the team's agreed module areas, corresponding to the original
single-file sections. They describe planned ownership and do not claim that
every listed person authored the existing code or its Git history.

| Module | Area | Assigned maintainer |
| --- | --- | --- |
| 1 | Student registry and sample data (original lines 1–402) | [@ayushkholiya](https://github.com/ayushkholiya) |
| 2 | Route and shift selection (original lines 404–486) | [@Priyankabisht12](https://github.com/Priyankabisht12) |
| 3 | Pass verification (original lines 494–643) | [@Aasthakala](https://github.com/Aasthakala) |
| 4 | Audit log and application flow (original lines 647–1270) | [@khushibtech18](https://github.com/khushibtech18) |

The three listed GitHub accounts currently have write access to this
repository. Collaborator access is managed separately from module ownership.

## Documentation

See [docs/architecture.md](docs/architecture.md) for module boundaries,
responsibilities, and the verification flow.
