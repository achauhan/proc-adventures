// Minimal stubs to allow linking Pro*C-generated code without Oracle client libraries.
// These functions DO NOT perform any database work.
// They satisfy unresolved external symbols like sqlcxt/sqlcx2/sqlorat used by Pro*C.

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Pro*C runtime entrypoints (signatures vary by version; we accept generic pointers).
void sqlcxt(void **ctxp, unsigned int *ctxl, void *exdp, void *dp)
{
    (void)ctxp; (void)ctxl; (void)exdp; (void)dp;
}

void sqlcx2(void **ctxp, unsigned int *ctxl, void *exdp, void *dp)
{
    (void)ctxp; (void)ctxl; (void)exdp; (void)dp;
}

void sqlorat(void **ctxp, unsigned int *ctxl, void *exdp, void *dp)
{
    (void)ctxp; (void)ctxl; (void)exdp; (void)dp;
}

// Some samples call sqlgls/sqlglm for statement and error text.
// Provide no-op implementations so code compiles/links.
void sqlgls(char *stmt, long *stmtl, long *func)
{
    if (stmt && stmtl && *stmtl > 0) {
        stmt[0] = 0;
    }
    if (func) {
        *func = 0;
    }
}

// Variant signature used by some generated/sample code on Windows.
void sqlglm(unsigned char *msg, int *bufl, int *msgl)
{
    if (msg && bufl && *bufl > 0) {
        msg[0] = 0;
    }
    if (msgl) {
        *msgl = 0;
    }
}

#ifdef __cplusplus
}
#endif
