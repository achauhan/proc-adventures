package com.oracle.demo;

/**
 * Java stubbed equivalent of sqlcaErrorHandling.c / sqlcaErrorHandling.pc.
 *
 * The original demo shows SQLCA-based error handling. In Java, JDBC exceptions
 * would be used. For a stubbed/no-DB build, we just demonstrate structured
 * error handling and consistent error output.
 */
public final class SqlcaErrorHandlingStub {

    public static void main(String[] args) {
        System.out.println("(stub) Demonstrating error handling (no DB).\n");

        try {
            simulateSqlOperation("SELECT * FROM EMP WHERE EMPNO = -1");
            System.out.println("(stub) Operation completed successfully.");
        } catch (StubSqlException ex) {
            System.err.println("SQLSTATE: " + ex.sqlState);
            System.err.println("Message : " + ex.getMessage());
        }
    }

    private static void simulateSqlOperation(String sql) throws StubSqlException {
        // In the real Pro*C sample this would run SQL and populate sqlca/sqlstate.
        // Here we fail deterministically to show the flow.
        throw new StubSqlException("42000", "(stub) Simulated SQL error while executing: " + sql);
    }

    private static final class StubSqlException extends Exception {
        final String sqlState;

        StubSqlException(String sqlState, String message) {
            super(message);
            this.sqlState = sqlState;
        }
    }
}
