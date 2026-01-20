
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[69];
};
static const struct sqlcxp sqlfpn =
{
    68,
    "C:\\Users\\593091\\Documents\\code\\proc-adventures\\sqlcaErrorHandling.pc"
};


static unsigned int sqlctx = 1732643117;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
              int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned int   sqlpfmem;
            void  *sqhstv[6];
   unsigned int   sqhstl[6];
            int   sqhsts[6];
            void  *sqindv[6];
            int   sqinds[6];
   unsigned int   sqharm[6];
   unsigned int   *sqharc[6];
   unsigned short  sqadto[6];
   unsigned short  sqtdso[6];
} sqlstm = {13,6};

/* SQLLIB Prototypes */
extern void sqlcxt (void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlcx2t(void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlbuft(void **, char *);
extern void sqlgs2t(void **, char *);
extern void sqlorat(void **, unsigned int *, void *);

/* Forms Interface */
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern void sqliem(unsigned char *, signed int *);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{13,4128,1,27,0,
5,0,0,0,0,0,27,94,0,0,4,4,0,2,9,68,66,95,83,69,67,85,82,69,1,9,0,0,1,9,0,0,1,
10,0,0,1,10,0,0,
45,0,0,2,141,0,4,114,0,0,6,1,0,2,9,68,66,95,83,69,67,85,82,69,2,9,0,0,2,9,0,0,
2,3,0,0,2,9,0,0,2,4,0,0,1,3,0,0,
93,0,0,3,0,0,30,181,0,0,0,0,0,2,9,68,66,95,83,69,67,85,82,69,
};


/***********************************************************************************
* errorhdlngsqlca.pc - This sample program explains how to implement ErrorHandling *
*                      using SQLCA                                                 *
*                                                                                  *
************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* EXEC SQL BEGIN DECLARE SECTION; */ 

  /* VARCHAR host_username[21]; */ 
struct { unsigned short len; unsigned char arr[21]; } host_username;

  short host_username_i;

  /* VARCHAR host_password[21]; */ 
struct { unsigned short len; unsigned char arr[21]; } host_password;

  short host_password_i;

  int host_empno;
  short host_empno_i;

  /* VARCHAR host_ename[11]; */ 
struct { unsigned short len; unsigned char arr[11]; } host_ename;

  short host_ename_i;

  /* VARCHAR host_job[10]; */ 
struct { unsigned short len; unsigned char arr[10]; } host_job;

  short host_job_i;

  int host_mgr;
  short host_mgr_i;

  /* VARCHAR host_hiredate[23]; */ 
struct { unsigned short len; unsigned char arr[23]; } host_hiredate;

  short host_hiredate_i;	

  double host_sal;
  short host_sal_i;

/* EXEC SQL END DECLARE SECTION; */ 


/* EXEC SQL DECLARE DB_SECURE DATABASE; */ 

/* EXEC SQL INCLUDE sqlca;
 */ 
/*
 * $Header: precomp/public/sqlca.h /nt/2 2020/07/19 18:53:20 dgoddard Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */


int main()
{
  int input_empno = 0;
  
  char c_ename[11];
  char c_job[10];
  int  i_mgr;
  char c_hiredate[23];
  double d_sal;

  int lsqlerrmsg;
  char sqlerrmsg[71];
  
  /*****************************************************************************
   * Initialize the host variables                                             *
   *****************************************************************************/
  memset(&host_username,'\0',sizeof(host_username));
  host_username_i = -1;

  memset(&host_password,'\0',sizeof(host_password));
  host_password_i = -1;

  host_empno = 0;
  host_empno_i = -1;

  memset(&host_ename,'\0',sizeof(host_ename));
  host_ename_i = -1;

  memset(&host_job,'\0',sizeof(host_job));
  host_job_i = -1;

  host_mgr = 0;
  host_mgr_i = -1;

  memset(&host_hiredate,'\0',sizeof(host_hiredate));
  host_hiredate_i = -1;

  host_sal = 0;
  host_sal_i = -1;

  printf("This is a sample program on Error Handling operation ... \n");
   
  /*****************************************************************************
   * Copy User Name and Password to connect to Oracle                          *
   *****************************************************************************/
  strcpy((char *)host_username.arr,"scott");
  host_username.len = strlen("scott");
  host_username_i = 0;

  strcpy((char *)host_password.arr,"tiger");
  host_password.len = strlen("tiger");
  host_password_i = 0;

  /* Make conneciton to the DB */
  /* EXEC SQL CONNECT :host_username IDENTIFIED BY :host_password AT DB_SECURE; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 13;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )10;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&host_username;
  sqlstm.sqhstl[0] = (unsigned int  )23;
  sqlstm.sqhsts[0] = (         int  )23;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&host_password;
  sqlstm.sqhstl[1] = (unsigned int  )23;
  sqlstm.sqhsts[1] = (         int  )23;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlstm.sqlcmax = (unsigned int )100;
  sqlstm.sqlcmin = (unsigned int )2;
  sqlstm.sqlcincr = (unsigned int )1;
  sqlstm.sqlctimeout = (unsigned int )0;
  sqlstm.sqlcnowait = (unsigned int )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


  
  if(sqlca.sqlcode != 0)
  {
    printf("The connection to the DB is not successfull \n");
  }
  else
  {
    printf("Connection is successfull \n");
  }

  printf("Please provide an employee id:"); 
  scanf("%d", &input_empno );

  if (input_empno > 0)
  {
    host_empno = input_empno;
    host_empno_i = 0;
  }

  /* EXEC SQL AT DB_SECURE SELECT ENAME,
                                 JOB,
				 MGR,
                                 TO_CHAR(HIREDATE,'MM/DD/YYYY HH24:MI:SS'),
                                 SAL
       INTO :host_ename:host_ename_i,
            :host_job:host_job_i,
            :host_mgr:host_mgr_i,
            :host_hiredate:host_hiredate_i,
            :host_sal:host_sal_i
       FROM EMP
       WHERE EMPNO = :host_empno:host_empno_i; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 13;
  sqlstm.arrsiz = 6;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "select ENAME ,JOB ,MGR ,TO_CHAR(HIREDATE,'MM/DD/YYYY HH24:M\
I:SS') ,SAL into :b0:b1,:b2:b3,:b4:b5,:b6:b7,:b8:b9  from EMP where EMPNO=:b10\
:b11";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )45;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.sqlpfmem = (unsigned int  )0;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&host_ename;
  sqlstm.sqhstl[0] = (unsigned int  )13;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)&host_ename_i;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&host_job;
  sqlstm.sqhstl[1] = (unsigned int  )12;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)&host_job_i;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&host_mgr;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)&host_mgr_i;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&host_hiredate;
  sqlstm.sqhstl[3] = (unsigned int  )25;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)&host_hiredate_i;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&host_sal;
  sqlstm.sqhstl[4] = (unsigned int  )sizeof(double);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)&host_sal_i;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)&host_empno;
  sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)&host_empno_i;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   switch(sqlca.sqlcode)
   {
      case 0:
             printf("Select query executed successfully for EMPOYEE ID: %d \n",input_empno);
             
             printf("Selected Rows: <%d> \n",sqlca.sqlerrd[2]);

             if (host_ename_i == 0)
             {
                strncpy(c_ename,host_ename.arr,host_ename.len);
                printf("EMPLOYEE NAME: %s \n",c_ename);
             }
             
             if(host_job_i == 0)
             {
                strncpy(c_job,host_job.arr,host_job.len);
                printf("JOB: %s \n",c_job);
             }
             
             if (host_mgr_i == 0)
             {
               i_mgr = host_mgr;
               printf("MANAGER: %d \n",i_mgr);
             }

             if (host_hiredate_i == 0)
             {
                strncpy(c_hiredate,host_hiredate.arr,host_hiredate.len);
                printf("HIRE DATE: %s \n",c_hiredate);
             }

             if(host_sal_i == 0)
             {
                d_sal = host_sal;
                printf("SALARY: %0.2f \n",d_sal);
             }

             break;

      case 1403:
               printf("No row found for the provided employee id");
               break;

      default:
              printf("Selection is haivng some problem. sqlca.sqlcode=<%d> \n",sqlca.sqlcode);
              
              lsqlerrmsg = sqlca.sqlerrm.sqlerrml;
              strncpy(sqlerrmsg, sqlca.sqlerrm.sqlerrmc, lsqlerrmsg);
              sqlerrmsg[lsqlerrmsg] = '\0';
              printf("Error description for the transaction=<%s> \n",sqlerrmsg);                           
   }


  /* Closing the DB conneciton */
  /* EXEC SQL AT DB_SECURE COMMIT WORK RELEASE; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 13;
  sqlstm.arrsiz = 6;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )93;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



  if(sqlca.sqlcode != 0)
  {
    printf("Connection to the DB did not close successfully \n");
  }
  else
  {
    printf("Connection to the DB closed successfully \n");
  }

  return 0;
}
