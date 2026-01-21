# Requirements — `hello_noinput.pc` (Minimal Pro\*C Hello World, no user input)

## 1. Purpose
Provide the smallest possible Pro\*C example that demonstrates embedded SQL in C **without requiring any user input** and that can be built/run in the stubbed (no real Oracle DB) environment.

## 2. Scope
- Declares a host variable and executes one embedded SQL statement.
- Executes `SELECT 'Hello World' INTO :hello FROM DUAL`.
- Prints the resulting string to standard output.
- Does **not** prompt for SID/username/password and does **not** connect.

Out of scope:
- Database connectivity, authentication, and session lifecycle.
- Any reads from stdin.
- Version queries and multi-statement demos.

## 3. Functional Requirements

### FR-NI-1: No user input
- The program **shall not** require any user input.
- The program **shall not** read from stdin (no prompts).

### FR-NI-2: No database connect
- The program **shall not** attempt to connect to an Oracle database.

### FR-NI-3: Execute Hello World query
- The program **shall** execute an embedded SQL statement that selects the literal string `Hello World` from `DUAL` into a host variable.

### FR-NI-4: Output
- The program **shall** print `Hello World` followed by a newline to standard output.

### FR-NI-5: Exit code
- The program **shall** exit with code `0`.

## 4. Non-Functional Requirements

### NFR-NI-1: Stub-friendly build
- The program **shall** be buildable in a stubbed environment (no real Oracle runtime present at execution time), when linked against the project’s stub runtime.

### NFR-NI-2: Precompile without DB connectivity
- The program **shall** be able to be precompiled using Pro\*C with syntax-only checking (e.g., `SQLCHECK=SYNTAX`) such that no DB connection is required during precompile.

### NFR-NI-3: Minimal dependencies
- The program **shall** require only:
  - a C compiler
  - the Oracle Pro\*C precompiler (`proc`)
  - Oracle client runtime libraries **or** the provided stub runtime

### NFR-NI-4: Simplicity
- The program **should** remain a minimal demonstration of embedded SQL and host variables.

## 5. Assumptions
- The `DUAL` table exists (or is treated as valid by the Pro\*C syntax-only precompile).
- In stubbed mode, the stubbed runtime provides sufficient symbols for linking and execution.

## 6. Acceptance Criteria
- **AC-NI-1**: Running the program prints exactly `Hello World` (with a trailing newline) and exits with code `0`.
- **AC-NI-2**: Running the program does not prompt the user and does not block waiting for input.
- **AC-NI-3**: A stubbed build (Pro\*C precompile + compile/link) completes successfully.
