/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *                                                                            
 *  Version:    C, Version 2.1
 *                                                                            
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */
 
#define NUMBER_OF_RUNS	1000000

//#include "dhry.h"

/* Global Variables: */
#define REG register

Rec_Type        g_Glob,
                g_Next_Glob;
                
Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

//extern char     *malloc ();
Enumeration     Func_1 (Capital_Letter   Ch_1_Par_Val,Capital_Letter   Ch_2_Par_Val);
Boolean Func_2 (Str_30  Str_1_Par_Ref, Str_30  Str_2_Par_Ref);
Boolean Func_3 (Enumeration Enum_Par_Val);
void Proc_1 (REG Rec_Pointer Ptr_Val_Par);
void Proc_2 (One_Fifty   *Int_Par_Ref);
void Proc_3 (Rec_Pointer *Ptr_Ref_Par);
void Proc_4 (void);
void Proc_5 (void);
void Proc_6 (Enumeration  Enum_Val_Par, Enumeration *Enum_Ref_Par);
void Proc_7 (One_Fifty       Int_1_Par_Val, One_Fifty       Int_2_Par_Val, One_Fifty      *Int_Par_Ref);
void Proc_8 (Arr_1_Dim       Arr_1_Par_Ref, Arr_2_Dim       Arr_2_Par_Ref,
 int             Int_1_Par_Val, int             Int_2_Par_Val);

/* forward declaration necessary since Enumeration may not simply be int */
  
#ifndef REG
        Boolean Reg = false;
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
        Boolean Reg = true;
#endif

/* variables for time measurement: */

//extern float GettTimerSec(); 

float           Begin_Time,
                End_Time,
                User_Time;
float           Microseconds,
                Dhrystones_Per_Second;

/* end of variables for time measurement */

//#define STRCPY(d,s) mem_cpy(d, s, sizeof(s) + 1)

//#define STRCPY str_cpy 

#define STRCPY strcpy 

//#define STRCPY string_cpy 

//#define STRCPY(d,s) memcpy(d, s, sizeof(s) + 1)

char * string_cpy(    register char * dest,    register char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

void str_cpy (register char   *d,
    	      register char   *s)
{
    *(((long *)d)) = *(((long *)s));
    *(((long *)d)+1) = *(((long *)s)+1);
    *(((long *)d)+2) = *(((long *)s)+2);
    *(((long *)d)+3) = *(((long *)s)+3);
    *(((long *)d)+4) = *(((long *)s)+4);
    *(((long *)d)+5) = *(((long *)s)+5);
    *(((long *)d)+6) = *(((long *)s)+6);
    *(((long *)d)+7) = *(((long *)s)+7);
}

/*
void str_cpy2 (d, s)
    register char   *d;
    register char   *s;
{
  int ch;
  __asm
	{
  loop:
	ldrb ch,[s],#1
	strb ch,[s],#1	
	cmp ch,#0;
	bne loop	
	}	
}
*/
   
/*
 *
 */
/*
void mem_cpy (d, s, l)
    register char   *d;
    register char   *s;
    register int    l;
{
    if (((long)d | (long)s) & 0x3)
    {
        while (l--) *d++ = *s++;
    }
    else
    {
        while (l >= 16)
        {
            *(((long *)d)) = *(((long *)s));
            *(((long *)d)+1) = *(((long *)s)+1);
            *(((long *)d)+2) = *(((long *)s)+2);
            *(((long *)d)+3) = *(((long *)s)+3);
            l -= 16;
            d += 16;
            s += 16;
        }
        while (l > 3)
        {
            *(((long *)d)++) = *(((long *)s)++);
            l -= 4;
        }
        while (l)
        {
           *d++ = *s++;
           l--;
        }

    }
}
*/

extern void rtc_time_start(void);
extern void gxos_get_rtc_time_ms(unsigned int *cur_time);

static unsigned int dhry_cur_time_start=0,dhry_cur_time_finish=0;
	
static void DhryStartTimer(void)
{
/*
	start_rtc();
*/

	rtc_time_start();//start rtc timer
	gxos_get_rtc_time_ms(&dhry_cur_time_start);  //get RTC time
}

static float DhryGetTimerSec(void)
{
	
	float time_token;	
/*
	stop_rtc();
        time_token = rRTC_SEC * 1000000 + rRTC_MS * 1000 + rRTC_US*10;
        time_token /= 100000;	
*/
	gxos_get_rtc_time_ms(&dhry_cur_time_finish);  //get RTC time
	        
	  time_token = ((float)(dhry_cur_time_finish-dhry_cur_time_start))/1000.0;
	        
        return time_token;
}


void dhry_main (void)
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{
        One_Fifty       Int_1_Loc;
  REG   One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
  REG   char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
  REG   int             Run_Index;
  REG   int             Number_Of_Runs;

  /* Initializations */
  
// Add your UART initializing code here

  //Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  //Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));

  Next_Ptr_Glob = &g_Next_Glob;
  Ptr_Glob = &g_Glob;

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  STRCPY (Ptr_Glob->variant.var_1.Str_Comp, 
          "DHRYSTONE PROGRAM, SOME STRING");
  STRCPY (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */

  SHELL_PRINT ("\n");
  SHELL_PRINT ("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  SHELL_PRINT ("\n");
  if (Reg)
  {
    SHELL_PRINT ("Program compiled with 'register' attribute\n");
    SHELL_PRINT ("\n");
  }
  else
  {
    SHELL_PRINT ("Program compiled without 'register' attribute\n");
    SHELL_PRINT ("\n");
  }
  SHELL_PRINT ("Please give the number of runs through the benchmark: ");

  Number_Of_Runs = NUMBER_OF_RUNS;
  SHELL_PRINT ("%d\n", Number_Of_Runs);

  SHELL_PRINT ("\n");

  SHELL_PRINT ("Execution starts, %d runs through Dhrystone\n", Number_Of_Runs);

  /***************/
  /* Start timer */
  /***************/
  
// Add your timer initializing code here
 
  Begin_Time = 0.0;
	
	DhryStartTimer();

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {

    Proc_5();
    Proc_4();
      /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    STRCPY (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
      /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
        /* Int_3_Loc == 7 */
      Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
        /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
      /* Int_Glob == 5 */
    Proc_1 (Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
                             /* loop body executed twice */
    {
      if (Enum_Loc == Func_1 (Ch_Index, 'C'))
          /* then, not executed */
        {
        Proc_6 (Ident_1, &Enum_Loc);
        STRCPY (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
        }
    }
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
      /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2 (&Int_1_Loc);
      /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/

// Add your timer stopping code here
  
  End_Time = DhryGetTimerSec();	// Get timer value in second
  
  SHELL_PRINT ("Execution ends\n");
  SHELL_PRINT ("\n");
  SHELL_PRINT ("Final values of the variables used in the benchmark:\n");
  SHELL_PRINT ("\n");
  SHELL_PRINT ("Int_Glob:            %d\n", Int_Glob);
  SHELL_PRINT ("        should be:   %d\n", 5);
  SHELL_PRINT ("Bool_Glob:           %d\n", Bool_Glob);
  SHELL_PRINT ("        should be:   %d\n", 1);
  SHELL_PRINT ("Ch_1_Glob:           %c\n", Ch_1_Glob);
  SHELL_PRINT ("        should be:   %c\n", 'A');
  SHELL_PRINT ("Ch_2_Glob:           %c\n", Ch_2_Glob);
  SHELL_PRINT ("        should be:   %c\n", 'B');
  SHELL_PRINT ("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
  SHELL_PRINT ("        should be:   %d\n", 7);
  SHELL_PRINT ("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
  SHELL_PRINT ("        should be:   Number_Of_Runs + 10\n");
  SHELL_PRINT ("Ptr_Glob->\n");
  SHELL_PRINT ("  Ptr_Comp:          %d\n", (int) Ptr_Glob->Ptr_Comp);
  SHELL_PRINT ("        should be:   (implementation-dependent)\n");
  SHELL_PRINT ("  Discr:             %d\n", Ptr_Glob->Discr);
  SHELL_PRINT ("        should be:   %d\n", 0);
  SHELL_PRINT ("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
  SHELL_PRINT ("        should be:   %d\n", 2);
  SHELL_PRINT ("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
  SHELL_PRINT ("        should be:   %d\n", 17);
  SHELL_PRINT ("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
  SHELL_PRINT ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  SHELL_PRINT ("Next_Ptr_Glob->\n");
  SHELL_PRINT ("  Ptr_Comp:          %d\n", (int) Next_Ptr_Glob->Ptr_Comp);
  SHELL_PRINT ("        should be:   (implementation-dependent), same as above\n");
  SHELL_PRINT ("  Discr:             %d\n", Next_Ptr_Glob->Discr);
  SHELL_PRINT ("        should be:   %d\n", 0);
  SHELL_PRINT ("  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  SHELL_PRINT ("        should be:   %d\n", 1);
  SHELL_PRINT ("  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  SHELL_PRINT ("        should be:   %d\n", 18);
  SHELL_PRINT ("  Str_Comp:          %s\n",
                                Next_Ptr_Glob->variant.var_1.Str_Comp);
  SHELL_PRINT ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  SHELL_PRINT ("Int_1_Loc:           %d\n", Int_1_Loc);
  SHELL_PRINT ("        should be:   %d\n", 5);
  SHELL_PRINT ("Int_2_Loc:           %d\n", Int_2_Loc);
  SHELL_PRINT ("        should be:   %d\n", 13);
  SHELL_PRINT ("Int_3_Loc:           %d\n", Int_3_Loc);
  SHELL_PRINT ("        should be:   %d\n", 7);
  SHELL_PRINT ("Enum_Loc:            %d\n", Enum_Loc);
  SHELL_PRINT ("        should be:   %d\n", 1);
  SHELL_PRINT ("Str_1_Loc:           %s\n", Str_1_Loc);
  SHELL_PRINT ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  SHELL_PRINT ("Str_2_Loc:           %s\n", Str_2_Loc);
  SHELL_PRINT ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  SHELL_PRINT ("\n");

  User_Time = End_Time - Begin_Time;

  Microseconds = (float) User_Time * Mic_secs_Per_Second 
                      / (float) Number_Of_Runs;
  Dhrystones_Per_Second = (float) Number_Of_Runs / (float) User_Time;

  SHELL_PRINT ("User_Time for Dhrystone: ");
  SHELL_PRINT ("%d.%02d \n", (int)(User_Time),((int)(User_Time*100)%100));
  
  SHELL_PRINT ("Microseconds for one run through Dhrystone: ");
  //SHELL_PRINT ("%6.1f \n", Microseconds);
  SHELL_PRINT ("%d.%02d \n", (int)(Microseconds),((int)(Microseconds*100)%100));
  SHELL_PRINT ("Dhrystones per Second:                      ");
  //SHELL_PRINT ("%6.1f \n", Dhrystones_Per_Second);
  SHELL_PRINT ("%d.%d  \n", (int)(Dhrystones_Per_Second),((int)(Dhrystones_Per_Second*10)%10));
  SHELL_PRINT ("Dhrystones MIPS:                            ");
  //SHELL_PRINT ("%6.1f \n", Dhrystones_Per_Second / 1757.0);
  SHELL_PRINT ("%d.%d  \n", (int)(Dhrystones_Per_Second/1757.0),((int)(Dhrystones_Per_Second*10/1757.0)%10));
  SHELL_PRINT ("\n");
  
}


void Proc_1 (Ptr_Val_Par)
/******************/

REG Rec_Pointer Ptr_Val_Par;
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;  
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */
  
  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob); 
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp 
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void Proc_2 (Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;  
  Enumeration   Enum_Loc=0;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 (void) /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5 (void) /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


        /* Procedure for the assignment of structures,          */
        /* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char   *d;
register char   *s;
register int    l;
{
        while (l--) *d++ = *s++;
}
#endif


void dhry_test(void)
{
	//gxos_interrupt_disable();
	dhry_main();
	//gxos_interrupt_enable();	
	
}

