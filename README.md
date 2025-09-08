# UOC Covid-19 Vaccination Programme

This project simulates a Covid-19 vaccination programme with data structures implemented in C.

## Overview
The project is split into two modules:

- **code/UOCCovid19Vaccine/** — Core library with data types and operations:
  - Countries
  - Vaccines and batches
  - Patients and vaccination queues

- **code/UOCProgrammeEC/** — Programme entry point and automated test suite covering PR1, PR2 and PR3.

## Features
- **PR1 — Lists and Tables**
  - Manage countries, vaccines, developers.
  - Store and update vaccination batches (linked list).
- **PR2 — Queues and Vaccination Process**
  - Patient queue by country.
  - Inoculation of first and second doses.
  - Eligibility rules by group (e.g., AstraZeneca restrictions, Janssen single dose).
  - Percentage of fully vaccinated patients.
- **PR3 — Search and Sorting**
  - Count patients vaccinated with a specific batch (queue traversal).
  - Sort vaccination batches by `lotID` (asc) and vaccine name (desc) using Quicksort.

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
See `docs/PR1.pdf`, `docs/PR2.pdf`, `docs/PR3.pdf` for assignment statements and requirements.
