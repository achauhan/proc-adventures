package com.oracle.demo.stub;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Very small in-memory "DB" used to eliminate the need for a real database.
 *
 * This is not a SQL engine. It provides just enough behavior for the migrated
 * Pro*C demos to run and print something meaningful.
 */
public final class DbStub {
    private DbStub() {}

    public static final class Table {
        public final String name;
        public final List<Map<String, Object>> rows = new ArrayList<>();

        public Table(String name) {
            this.name = name;
        }
    }

    private static final Table EMP = seedEmp();

    private static Table seedEmp() {
        var t = new Table("EMP");

        // Minimal EMP-like rows
        t.rows.add(row(
                "EMPNO", 7369,
                "ENAME", "SMITH",
                "JOB", "CLERK",
                "MGR", 7902,
                "HIREDATE", "1980-12-17",
                "SAL", 800
        ));
        t.rows.add(row(
                "EMPNO", 7499,
                "ENAME", "ALLEN",
                "JOB", "SALESMAN",
                "MGR", 7698,
                "HIREDATE", "1981-02-20",
                "SAL", 1600
        ));
        t.rows.add(row(
                "EMPNO", 7839,
                "ENAME", "KING",
                "JOB", "PRESIDENT",
                "MGR", null,
                "HIREDATE", "1981-11-17",
                "SAL", 5000
        ));

        return t;
    }

    private static Map<String, Object> row(Object... kv) {
        var m = new LinkedHashMap<String, Object>();
        for (int i = 0; i < kv.length; i += 2) {
            m.put(String.valueOf(kv[i]), kv[i + 1]);
        }
        return m;
    }

    /**
     * Used by cursor/dynamic SQL demos: SELECT ... FROM EMP WHERE JOB = :job
     */
    public static List<Map<String, Object>> selectEmpByJob(String job) {
        var out = new ArrayList<Map<String, Object>>();
        for (var r : EMP.rows) {
            var rJob = r.get("JOB");
            if (job == null || job.isBlank()) {
                out.add(r);
            } else if (rJob != null && job.equalsIgnoreCase(String.valueOf(rJob))) {
                out.add(r);
            }
        }
        return out;
    }

    /**
     * Used by generic procdemo "SQL interpreter": we only recognize a small subset.
     */
    public static QueryResult execute(String sql, List<String> bindVars) {
        var normalized = sql == null ? "" : sql.trim();
        var upper = normalized.toUpperCase();

        if (upper.startsWith("SELECT") && upper.contains("FROM EMP")) {
            // Recognize a simple JOB bind if present.
            String job = null;
            if (upper.contains("WHERE") && upper.contains("JOB") && upper.contains(":V1")) {
                if (bindVars != null && !bindVars.isEmpty()) {
                    job = bindVars.get(0);
                }
            }
            var rows = selectEmpByJob(job);
            return QueryResult.rows(rows);
        }

        // Everything else: treat as non-query statement
        return QueryResult.ok("(stub) Statement accepted: " + normalized);
    }

    public record QueryResult(boolean isQuery, String message, List<Map<String, Object>> rows) {
        public static QueryResult rows(List<Map<String, Object>> rows) {
            return new QueryResult(true, null, rows);
        }

        public static QueryResult ok(String message) {
            return new QueryResult(false, message, List.of());
        }
    }
}
