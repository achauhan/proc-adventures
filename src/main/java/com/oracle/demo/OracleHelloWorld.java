package com.oracle.demo;

import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Optional;

public class OracleHelloWorld {

    public static final int INPUT_OK = 0;
    public static final int NO_INPUT = 1;
    public static final int TOO_LONG = 2;

    public static final int SID_MAX_LENGTH = 16;
    public static final int USERNAME_MAX_LENGTH = 30;
    public static final int PASSWORD_MAX_LENGTH = 30;

    private final Console console;
    private final BufferedReader reader;
    private final PrintStream out;

    public OracleHelloWorld() {
        this(ConsoleAvailable() ? System.console() : null, null, System.out);
    }

    public OracleHelloWorld(Console console, BufferedReader reader, PrintStream out) {
        this.console = console;
        this.reader = reader;
        this.out = out;
    }

    public static class InputResult {
        public final int status;
        public final String value;
        public InputResult(int status, String value) {
            this.status = status;
            this.value = value;
        }
    }

    public InputResult getLine(String prompt, int maxLength) {
        try {
            String line = readLine(prompt);
            if (line == null || line.isBlank()) return new InputResult(NO_INPUT, null);
            if (line.length() > maxLength) return new InputResult(TOO_LONG, null);
            return new InputResult(INPUT_OK, line);
        } catch (IOException e) {
            out.println("Input error: " + e.getMessage());
            return new InputResult(NO_INPUT, null);
        }
    }

    public String readPassword(String prompt) {
        // No secure password needed; keep compatibility with existing interface.
        return readLineUnchecked(prompt + " (not used)");
    }

    private static boolean ConsoleAvailable() {
        try { return System.console() != null; } catch (Throwable t) { return false; }
    }

    private String readLine(String prompt) throws IOException {
        out.print(prompt);
        out.flush();
        if (console != null) {
            return console.readLine();
        } else {
            BufferedReader br = (reader != null) ? reader :
                new BufferedReader(new java.io.InputStreamReader(System.in));
            return br.readLine();
        }
    }

    private String readLineUnchecked(String prompt) {
        try { return readLine(prompt); } catch (IOException e) { return null; }
    }

    // ---- Replaced DB logic with local file/hardcoded data ----

    // Reads first line of the given file if it exists; otherwise returns fallback.
    private String readFirstLineOrFallback(Path path, String fallback) {
        try {
            if (Files.exists(path)) {
                Optional<String> line = Files.lines(path).findFirst();
                if (line.isPresent() && !line.get().isBlank()) {
                    return line.get();
                }
            }
        } catch (IOException ignored) {}
        return fallback;
    }

    // Emulates "SELECT 'Hello World' FROM DUAL"
    private String getHelloMessage() {
        return readFirstLineOrFallback(Path.of("hello.txt"), "Hello World");
    }

    // Emulates "SELECT banner FROM v$version WHERE banner LIKE 'Oracle%'"
    private String getVersionBanner() {
        return readFirstLineOrFallback(
            Path.of("version.txt"),
            "Oracle Database 21c (Simulated)"
        );
    }

    // Main flow: prompt and print without any DB connection.
    public int run() {
        out.println("Running in offline mode (no Oracle connection).");
        
        InputResult sid = getLine("Enter SID (optional, ignored): ", SID_MAX_LENGTH);
        InputResult user = getLine("Enter username (optional, ignored): ", USERNAME_MAX_LENGTH);
        String pass = readPassword("Enter password");

        // Input is not used for any connection; show sanitized info.
        if (sid.status == TOO_LONG || user.status == TOO_LONG) {
            out.println("Input too long.");
            return 1;
        }
        
        out.println("Reading messages...");
        String hello = getHelloMessage();
        String version = getVersionBanner();

        out.println("Query result: " + hello);
        out.println("Version banner: " + version);
        out.println("Done.");
        return 0;
    }

    public static void main(String[] args) {
        OracleHelloWorld app = new OracleHelloWorld();
        int code = app.run();
        System.exit(code);
    }
}
