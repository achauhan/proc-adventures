package com.oracle.demo;

import com.oracle.demo.stub.DbStub;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 * Java stubbed equivalent of procdemo.c / procdemo.pc.
 *
 * Original intent: ANSI dynamic SQL "interpreter" that can accept statements
 * and (optionally) bind variables.
 *
 * Stub limitations:
 *  - Recognizes a small subset of queries against an in-memory EMP table.
 *  - Everything else is accepted as a no-op statement.
 */
public final class ProcDemoStub {
    public static void main(String[] args) throws Exception {
        System.out.println("(stub) ANSI Dynamic SQL interpreter (no DB). Type EXIT to quit.");
        System.out.println("Try: SELECT * FROM EMP;\n");

        try (var in = new BufferedReader(new InputStreamReader(System.in))) {
            while (true) {
                System.out.print("SQL> ");
                var stmt = readStatement(in);
                if (stmt == null) {
                    break;
                }
                var trimmed = stmt.trim();
                if (trimmed.equalsIgnoreCase("EXIT")) {
                    break;
                }
                if (trimmed.equalsIgnoreCase("HELP") || trimmed.equals("?")) {
                    printHelp();
                    continue;
                }

                // Very small "bind variable" demo: if the statement contains :v1, prompt for it.
                var binds = new ArrayList<String>();
                if (trimmed.toUpperCase().contains(":V1")) {
                    System.out.print("Enter value for :v1: ");
                    binds.add(in.readLine());
                }

                var result = DbStub.execute(trimmed, binds);
                if (!result.isQuery()) {
                    System.out.println(result.message());
                    continue;
                }

                printRows(result.rows());
            }
        }

        System.out.println("\nHave a good day! (stub)");
    }

    private static String readStatement(BufferedReader in) throws Exception {
        // Read lines until ';' terminator (like the C demo). No PL/SQL '/' support here.
        StringBuilder sb = new StringBuilder();
        while (true) {
            String line = in.readLine();
            if (line == null) {
                return sb.isEmpty() ? null : sb.toString();
            }

            sb.append(line).append('\n');
            if (line.contains(";")) {
                var s = sb.toString();
                int idx = s.indexOf(';');
                return s.substring(0, idx);
            }

            System.out.print("... ");
        }
    }

    private static void printRows(java.util.List<java.util.Map<String, Object>> rows) {
        if (rows.isEmpty()) {
            System.out.println("(stub) 0 rows");
            return;
        }
        // Print columns from the first row.
        var cols = rows.get(0).keySet();
        for (String c : cols) {
            System.out.printf("%-12s", c);
        }
        System.out.println();

        for (var r : rows) {
            for (String c : cols) {
                Object v = r.get(c);
                System.out.printf("%-12s", v == null ? "NULL" : String.valueOf(v));
            }
            System.out.println();
        }
    }

    private static void printHelp() {
        System.out.println("\nEnter a SQL statement terminated with ';'.");
        System.out.println("Type EXIT to quit.");
        System.out.println("Supported stub query: SELECT ... FROM EMP [WHERE JOB = :v1];\n");
    }
}
