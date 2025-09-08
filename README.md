# Vaccination Management System

A C-based system that simulates a complete vaccination programme, from managing countries and vaccine developers to administering doses and tracking patient data. It implements custom data structures (lists, queues, tables) and algorithms (recursion, search, sorting) to provide realistic vaccination process management.

## Features

### Countries & Vaccines
- Manage **countries** (EU and non-EU).
- Register **developers** and associate them with vaccines.
- Manage **vaccines** with attributes:
  - Technology (Adenovirus, Inactivated, Peptidic, mRNA).
  - Current clinical trial phase.
  - Authorized countries.

### Patients & Queues
- Manage **patients** with name, ID, assigned vaccine, and doses received.
- Store patients in **queues by country**, processed in order of arrival.
- Enforce eligibility rules:
  - AstraZeneca not allowed for groups 2 & 3 (elderly).
  - Janssen requires only a single dose.

### Vaccination Batches
- Manage **vaccination batches** (lot ID, vaccine type, number of doses).
- Store batches in a **linked list**.
- Support insertion, deletion, retrieval, and traversal of batches.
- Process first and second doses for patients in queues.
- Track percentage of fully vaccinated patients.

### Algorithms & Advanced Operations
- **Recursion**: compare patient queues, count vaccinated by vaccine/technology.
- **Search**: count patients vaccinated with a given batch.
- **Sorting**: quicksort algorithm to order vaccination batches by `lotID` (asc) and vaccine name (desc).
- **Statistics**: identify most used vaccine technology per country.

## Project Structure
```
code/
├── UOCCovid19Vaccine/         # Core library
│   ├── include/               # Header files
│   └── src/                   # Source files
├── UOCProgrammeEC/            # Programme + test suite
│   ├── src/                   # main.c
│   └── test/                  # Test framework and cases
│       ├── include/
│       └── src/
docs/
├── PR1.pdf                    # Assignment PR1
├── PR2.pdf                    # Assignment PR2
└── PR3.pdf                    # Assignment PR3
```
## Build & Run
# Build application (with main)
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 \
  -Icode/UOCCovid19Vaccine/include \
  -Icode/UOCProgrammeEC/test/include \
  code/UOCCovid19Vaccine/src/*.c \
  code/UOCProgrammeEC/src/main.c \
  code/UOCProgrammeEC/test/src/*.c \
  -o uocvaccine

# Run
./uocvaccine

## Documentation
- docs/PR1.pdf — Countries, vaccines, developers management.
- docs/PR2.pdf — Patient queues, vaccination process and eligibility.
- docs/PR3.pdf — Vaccine batches, recursion, search & sorting.
