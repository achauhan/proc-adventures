package com.oracle.demo;

import static org.junit.jupiter.api.Assertions.*;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.List;

import org.junit.jupiter.api.*;

class OracleHelloWorldTest {

    private ByteArrayOutputStream outBytes;
    private PrintStream out;

    @BeforeEach
    void setup() {
        outBytes = new ByteArrayOutputStream();
        out = new PrintStream(outBytes, true, StandardCharsets.UTF_8);
    }

    @AfterEach
    void cleanup() {
        out.close();
    }

    @Test
    void run_usesFallbacks_whenFilesMissing() {
        // No hello.txt or version.txt present
        OracleHelloWorld app = new OracleHelloWorld(null, new BufferedReader(new StringReader("\n\n\n")), out);
        int code = app.run();

        String output = outBytes.toString(StandardCharsets.UTF_8);
        assertEquals(0, code);
        assertTrue(output.contains("Running in offline mode"), "Should indicate offline mode");
        assertTrue(output.contains("Query result: Hello World"), "Fallback hello should be used");
        assertTrue(output.contains("Version banner: Oracle Database 21c (Simulated)"), "Fallback version should be used");
    }

    @Test
    void run_readsFromFiles_whenPresent() throws Exception {
        Path hello = Path.of("hello.txt");
        Path version = Path.of("version.txt");
        try {
            Files.write(hello, List.of("Hello From File"), StandardCharsets.UTF_8);
            Files.write(version, List.of("Oracle Database 23ai (Simulated)"), StandardCharsets.UTF_8);

            OracleHelloWorld app = new OracleHelloWorld(null, new BufferedReader(new StringReader("\n\n\n")), out);
            int code = app.run();

            String output = outBytes.toString(StandardCharsets.UTF_8);
            assertEquals(0, code);
            assertTrue(output.contains("Query result: Hello From File"));
            assertTrue(output.contains("Version banner: Oracle Database 23ai (Simulated)"));
        } finally {
            Files.deleteIfExists(hello);
            Files.deleteIfExists(version);
        }
    }

    @Test
    void getLine_flagsTooLongInput() {
        String longSid = "X".repeat(OracleHelloWorld.SID_MAX_LENGTH + 1);
        OracleHelloWorld app = new OracleHelloWorld(null, new BufferedReader(new StringReader(longSid + "\n")), out);

        OracleHelloWorld.InputResult res = app.getLine("Enter SID: ", OracleHelloWorld.SID_MAX_LENGTH);
        assertEquals(OracleHelloWorld.TOO_LONG, res.status);
        assertNull(res.value);
    }

    @Test
    void getLine_acceptsValidInput() {
        String sid = "ORCL";
        OracleHelloWorld app = new OracleHelloWorld(null, new BufferedReader(new StringReader(sid + "\n")), out);

        OracleHelloWorld.InputResult res = app.getLine("Enter SID: ", OracleHelloWorld.SID_MAX_LENGTH);
        assertEquals(OracleHelloWorld.INPUT_OK, res.status);
        assertEquals(sid, res.value);
    }

    @Test
    void readPassword_returnsInputLine() {
        String pwd = "secret";
        OracleHelloWorld app = new OracleHelloWorld(null, new BufferedReader(new StringReader(pwd + "\n")), out);

        String v = app.readPassword("Enter password");
        assertEquals(pwd, v);
    }
}
