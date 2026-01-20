# Oracle stubs (link-only)

These stubs exist only to allow **linking** Pro*C-generated C code (`hello.c`) when Oracle client libraries are unavailable.

- They **do not** connect to any database.
- Any `EXEC SQL ...` calls will become no-ops. Output values returned from SQL will be empty/uninitialized.
- Use only for smoke testing that the binary builds/runs.

Build example (MinGW gcc):

- Compile:
  - `gcc -O2 -IC:\Tools\oracle\precomp\public -c hello.c -o build\hello.o`
  - `gcc -O2 -c stubs\oracle_stub.c -o build\oracle_stub.o`
- Link:
  - `gcc -o bin\hello_stub.exe build\hello.o build\oracle_stub.o`

## procdemo

- Generate C from Pro*C:
  - `proc "in=procdemo.pc" "out=build\procdemo.c" MODE=ANSI CODE=ANSI_C SQLCHECK=SEMANTICS`
- Compile:
  - `gcc -O2 -IC:\Tools\oracle\precomp\public -c build\procdemo.c -o build\procdemo.o`
  - `gcc -O2 -c stubs\oracle_stub.c -o build\oracle_stub.o`
- Link:
  - `gcc -o bin\procdemo_stub.exe build\procdemo.o build\oracle_stub.o`
