package com.oracle.demo;

import com.oracle.demo.stub.DbStub;

import java.util.List;
import java.util.Map;

/**
 * Java stubbed equivalent of DynamicSQL-3.c / DynamicSQL-3.pc.
 *
 * Original intent: demonstrate dynamic SQL with a bind variable (JOB).
 * Stub behavior: accepts a JOB arg and runs a fixed "dynamic" query against
 * an in-memory EMP table.
 */
public final class DynamicSql3Stub {
    public static void main(String[] args) {
        String job = args.length > 0 ? args[0] : "CLERK";

        String dynSql = "SELECT EMPNO, ENAME, MGR, HIREDATE, SAL FROM EMP WHERE JOB = :v1";
        System.out.println("(stub) Preparing dynamic statement:\n  " + dynSql);
        System.out.println("(stub) Binding :v1 = '" + job + "'\n");

        var result = DbStub.execute(dynSql, List.of(job));
        if (!result.isQuery()) {
            System.out.println(result.message());
            return;
        }

        printRows(result.rows());
    }

    private static void printRows(List<Map<String, Object>> rows) {
        if (rows.isEmpty()) {
            System.out.println("(stub) 0 rows");
            return;
        }

        System.out.printf("%-8s %-10s %-6s %-12s %-6s%n",
                "EMPNO", "ENAME", "MGR", "HIREDATE", "SAL");

        for (var r : rows) {
            System.out.printf("%-8s %-10s %-6s %-12s %-6s%n",
                    r.get("EMPNO"),
                    r.get("ENAME"),
                    r.get("MGR"),
                    r.get("HIREDATE"),
                    r.get("SAL"));
        }
    }
}
