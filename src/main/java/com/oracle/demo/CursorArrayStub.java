package com.oracle.demo;

import com.oracle.demo.stub.DbStub;

import java.util.Scanner;

/**
 * Java stubbed equivalent of cursorArray.c / cursorArray.pc.
 *
 * Original intent: prompt for a JOB and print matching EMP rows.
 * Stub behavior: query an in-memory EMP table.
 */
public final class CursorArrayStub {
    public static void main(String[] args) {
        String job;
        if (args.length > 0 && !args[0].isBlank()) {
            job = args[0];
        } else {
            System.out.print("Enter JOB to search in EMP (e.g. CLERK): ");
            // Don't close System.in; keep the demo simple.
            @SuppressWarnings("resource")
            var sc = new Scanner(System.in);
            job = sc.nextLine();
        }

        var rows = DbStub.selectEmpByJob(job);
        if (rows.isEmpty()) {
            System.out.println("No rows for JOB='" + job + "' (stub)." );
            return;
        }

        System.out.printf("%-8s %-10s %-10s %-6s %-12s %-6s%n",
                "EMPNO", "ENAME", "JOB", "MGR", "HIREDATE", "SAL");

        for (var r : rows) {
            System.out.printf("%-8s %-10s %-10s %-6s %-12s %-6s%n",
                    r.get("EMPNO"),
                    r.get("ENAME"),
                    r.get("JOB"),
                    r.get("MGR"),
                    r.get("HIREDATE"),
                    r.get("SAL"));
        }
    }
}
