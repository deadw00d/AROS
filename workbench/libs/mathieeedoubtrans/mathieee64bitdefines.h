/*
 *  Some shortcuts that provide the highest flexibility
 *  for 64bit and 32bit Systems.
 */

#ifndef __MATHIEEE64BIT_DEFINES_H__
#define __MATHIEEE64BIT_DEFINES_H__



#if defined(AROS_64BIT_TYPE) || defined(__GNUC__)
#    define Get_High32of64(val)       ((val) >> 32L )
#    define Get_Low32of64(val)        ((val) & 0xFFFFFFFF)
#else
#    define Get_High32of64(val)       ((val).high)
#    define Set_High32of64(Dest,Src)  ((Dest).high) = Src
#    define Get_Low32of64(val)        ((val).low)  
#    define Set_Low32of64(Dest,Src)   ((Dest).low) = Src
#endif

/* XOR */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define XOR64C(Dest64, Var1_64, Const64_Hi, Const64_Lo, Const64) \
     Dest64 = Var1_64 ^ Const64 ;
#else
#define XOR64C(Dest64, Var1_64, Const64_Hi, Const64_Lo, Const64) \
     Set_High32of64(Dest64, Get_High32of64(Var1_64) ^ Const64_Hi); \
     Set_Low32of64( Dest64, Get_Low32of64(Var1_64)  ^ Const64_Lo); 
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define XOR64QC(Dest64, Const64_Hi, Const64_Lo, Const64) \
     Dest64 ^= Const64 ;
#else
#define XOR64QC(Dest64, Const64_Hi, Const64_Lo, Const64) \
     Set_High32of64(Dest64, Get_High32of64(Dest64) ^ Const64_Hi); \
     Set_Low32of64( Dest64, Get_Low32of64(Dest64)  ^ Const64_Lo); 
#endif

/* OR */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define OR64(Dest64, Var1_64, Var2_64) \
  Dest64 = Var1_64 | Var2_64 ;
#else
#define OR64(Dest64, Var1_64, Var2_64) \
  Set_High32of64(Dest64, (Get_High32of64(Var1_64) | Get_High32of64(Var2_64))); \
  Set_Low32of64( Dest64, (Get_Low32of64 (Var1_64) | Get_Low32of64 (Var2_64))); 
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define OR64Q(Dest64, Var64) \
  Dest64 |= Var2_64 ;
#else
#define OR64Q(Dest64, Var64) \
  Set_High32of64(Dest64, (Get_High32of64(Dest64) | Get_High32of64(Var64))); \
  Set_Low32of64( Dest64, (Get_Low32of64 (Dest64) | Get_Low32of64 (Var64))); 
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define OR64C(Dest64, Var64, Const64_Hi, Const64_Lo, Const64) \
  Dest64 = Var64 | Const64;
#else
#define OR64C(Dest64, Var64, Const64_Hi, Const64_Lo, Const64) \
  Set_High32of64(Dest64, Get_High32of64(Var64) | Const64_Hi); \
  Set_Low32of64( Dest64, Get_Low32of64(Var64)  | Const64_Lo); 
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define OR64QC(Dest64, Const64_Hi, Const64_Lo, Const64) \
  Dest64 |= | Const64;
#else
#define OR64QC(Dest64, Const64_Hi, Const64_Lo, Const64) \
  Set_High32of64(Dest64, Get_High32of64(Dest64) | Const64_Hi); \
  Set_Low32of64( Dest64, Get_Low32of64(Dest64)  | Const64_Lo); 
#endif

/* AND */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define AND64C(Dest64, Var64, Const64_Hi, Const64_Lo, Const64) \
  Dest64 = Var64 & Const64 ;
#else
#define AND64C(Dest64, Var64, Const64_Hi, Const64_Lo, Const64) \
  Set_High32of64(Dest64, Get_High32of64(Var64) & Const64_Hi); \
  Set_Low32of64( Dest64, Get_Low32of64(Var64)  & Const64_Lo); 
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define AND64QC(Dest64, Const64_Hi, Const64_Lo, Const64) \
  Dest64 &= Const64 ;
#else
#define AND64QC(Dest64, Const64_Hi, Const64_Lo, Const64) \
  Set_High32of64(Dest64, Get_High32of64(Dest64) & Const64_Hi); \
  Set_Low32of64( Dest64, Get_Low32of64(Dest64)  & Const64_Lo); 
#endif


/* SHL (shift left n bits)*/

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define SHL64(Dest64, Src64, n) \
   Dest64 = Src64 << (n);
#else
#define SHL64(Dest64, Src64, n) \
   if( n < 32) \
   { \
     Set_High32of64(Dest64,       (Get_High32of64(Src64)  << n) | \
                           (ULONG)(Get_Low32of64 (Src64)) >> (32-(n)) ); \
     Set_Low32of64(Dest64,        (Get_Low32of64 (Src64)  << n)       ); \
   } \
   else \
     if (n < 64) \
     { \
       Set_High32of64(Dest64, (ULONG)Get_Low32of64(Src64) << ( (n)-32) );\
       Set_Low32of64(Dest64,  0                                        ); \
     } \
     else \
       if (n > 64) \
       { \
         Set_High32of64(Dest64, 0); \
         Set_Low32of64(Dest64,  0); \
       } 
#endif


#if defined AROS_64BIT_TYPE || defined __GNUC__
#define SHL32(Dest64, Src32, Shift) \
   Dest64 = ((QUAD)Src32) << (Shift);
#else
#define SHL32(Dest64, Src32, Shift) \
	 if( Shift < 32) \
   { \
     Set_High32of64(Dest64, (Src32 >> (32-(Shift)) ) ); \
     Set_Low32of64 (Dest64, (Src32 << (Shift)      ) ); \
   } \
   else \
     { \
       if (Shift < 64) \
       {  \
         Set_High32of64(Dest64, (Src32 << ((Shift)-32)) ); \
         Set_Low32of64 (Dest64, 0                       ); \
       } \
       else \
         Set_Value64C(Dest64, 0 , 0, 0); \
     } 
#endif


/* SHR (shifting to the right **signed/unsigned**)  */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define SHRU64(Dest64, Src64, Shift) \
   Dest64 = (UQUAD)Src64 << (Shift);
#else
#define SHRU64(Dest64, Src64, Shift) \
   if( (Shift) < 32) \
   { \
     Set_Low32of64 (Dest64,((ULONG)Get_Low32of64 (Src64) >> (Shift)     )|   \
                           (       Get_High32of64(Src64) <<(32-(Shift)))  ); \
     Set_High32of64(Dest64, (ULONG)Get_High32of64(Src64) >> (Shift)       ); \
   } \
   else \
   	 {\
       if ((Shift) < 64) \
       { \
         Set_Low32of64 (Dest64, (ULONG)Get_High32of64(Src64) >> ((Shift)-32) ); \
         Set_High32of64(Dest64, 0); \
       } \
       else \
         { \
           Set_High32of64(Dest64, 0); \
           Set_Low32of64(Dest64,  0); \
         } \
     }           
#endif


#if defined AROS_64BIT_TYPE || defined __GNUC__
#define SHRU32(Dest32, Src64, Shift) \
   Dest32 = (UQUAD)Src64 >> (Shift);
#else
#define SHRU32(Dest32, Src64, Shift) \
   if( (Shift) < 32) \
   { \
     Dest32 = (ULONG)Get_Low32of64(Src64)  >> (Shift) |  \
                     Get_High32of64(Src64) << (32-(Shift)) \
   } \
   else \
   	 {\
       if ((Shift) < 64) \
       { \
         Dest32 = (ULONG)Get_High32of64(Src64) >> ((Shift)-32) ; \
       } \
       else \
         { \
           Dest32 = 0; \
         } \
     }           
#endif


/* SHR ** signed ** */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define SHRS64(Dest64, Src64, n) \
   Dest64 = (UQUAD)Src64 << n;
#else
#define SHRS64(Dest64, Src64, n) \
   if( n < 32) \
   { \
     Set_Low32of64 (Dest64, (LONG)Get_Low32of64(Src64)  >> (n) |       \
                                  Get_High32of64(Src64) << ((n)-32) ); \
     Set_High32of64(Dest64, (LONG)Get_High32of64(Src64) >> n        ); \
   } \
   else \
     if (n < 64) \
     { \
       Set_Low32of64 (Dest64,  (LONG)Get_High32of64(Src64) >> ((n)-32) );\
       Set_High32of64(Dest64,  (LONG)Get_High32of64(Src64) >>  32      ); \
     } \
     else \
       { \
       Set_High32of64(Dest64, (LONG)Get_High32of64(Src64) >>  32      ); \
       Set_Low32of64 (Dest64, (LONG)Get_High32of64(Src64) >>  32      ); \
       } 
#endif

/* Set_Value */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define Set_Value64(Dest64, Var64) \
  Dest64 = Var64;
#else
#define Set_Value64(Dest64, Var64) \
  Set_High32of64(Dest64, Get_High32of64(Var64));\
  Set_Low32of64(Dest64,  Get_Low32of64(Var64)); 
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define Set_Value64C(Dest64, Const64_Hi, Const64_Lo, Const64) \
  Dest64 = Const64;
#else
#define Set_Value64C(Dest64, Const64_Hi, Const64_Lo, Const64) \
  Set_High32of64(Dest64, Const64_Hi);\
  Set_Low32of64 (Dest64, Const64_Lo); 
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define Set_Value64from32(Dest64, Var32) \
  Dest64 = (QUAD)Var32;
#else
#define Set_Value64from32(Dest64, Var32) \
  Set_Low32of64 (Dest64, Var32); \ 
  Set_High32of64(Dest64, 0);
#endif


/* Comparisons */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_eq(Var1_64, Var2_64) \
  (Var1_64 == Var2_64)
#else
#define is_eq(Var1_64, Var2_64) \
  (Get_Low32of64(Var1_64) == Get_Low32of64(Var2_64)  && \
   Get_High32of64(Var1_64) == Get_High32of64(Var2_64))
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_eqC(Var1_64, Const64_Hi, Const64_Lo, Const64) \
  (Var1_64 == Const64)
#else
#define is_eqC(Var64, Const64_Hi, Const64_Lo, Const64) \
  (Get_Low32of64(Var64) == Const64_Lo  && \
   Get_High32of64(Var64) == Const64_Hi )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_neq(Var1_64, Var2_64) \
  (Var1_64 != Var2_64)
#else
#define is_neq(Var1_64, Var2_64) \
  (Get_Low32of64 (Var1_64) != Get_Low32of64 (Var2_64) || \
   Get_High32of64(Var1_64) != Get_High32of64(Var2_64)    )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_greater(Var1_64, Var2_64 ) \
  (Var1_64 > Var2_64)
#else
#define is_greater(Var1_64, Var2_64 ) \
  ( (Get_High32of64(Var1_64) >  Get_High32of64(Var2_64)) || \
    (Get_High32of64(Var1_64) == Get_High32of64(Var2_64) &&  \
     Get_Low32of64(Var1_64)  >  Get_Low32of64(Var2_64) ) )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_greaterC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  (Var1_64 > Const64)
#else
#define is_greaterC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  ( (Get_High32of64(Var1_64) >  Const64_Hi || \
    (Get_High32of64(Var1_64) == Const64_Hi &&  \
     Get_Low32of64(Var1_64)  >  Const64_Hi) )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_geq(Var1_64, Var2_64 ) \
  (Var1_64 >= Var2_64)
#else
#define is_geq(Var1_64, Var2_64 ) \
  ( (Get_High32of64(Var1_64) >=  Get_High32of64(Var2_64)) || \
    (Get_High32of64(Var1_64) ==  Get_High32of64(Var2_64) &&  \
     Get_High32of64(Var1_64) >=  Get_High32of64(Var2_64)  ) )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_geqC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  (Var1_64 > Const64)
#else
#define is_geqC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  ( (Get_High32of64(Var1_64) >=  Const64_Hi) || \
    (Get_High32of64(Var1_64) ==  Const64_Hi  &&  \
     Get_Low32of64(Var1_64)  >=  Const64_Lo)  )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_less(Var1_64, Var2_64 ) \
  (Var1_64 < Var2_64)
#else
#define is_less(Var1_64, Var2_64 ) \
  ( ((ULONG)Get_High32of64(Var1_64) <  (ULONG)Get_High32of64(Var2_64)) || \
    ((ULONG)Get_High32of64(Var1_64) == (ULONG)Get_High32of64(Var2_64)  &&  \
     (ULONG)Get_Low32of64(Var1_64)  <  (ULONG)Get_Low32of64(Var2_64)  ) )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_lessC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  ((UQUAD)Dest < (UQUAD)Const64)
#else
#define is_lessC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  ( ((ULONG)Get_High32of64(Var1_64) <  (ULONG)Const64_Hi)  || \
    ((ULONG)Get_High32of64(Var1_64) == (ULONG)Const64_Hi   &&  \
     (ULONG)Get_Low32of64(Var1_64)  <  (ULONG)Const64_Lo) )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_lessSC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  ((QUAD)Var1_64 < (QUAD)Const64 )
#else
#define is_lessSC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  ( ((LONG)Get_High32of64(Var1_64) <  (LONG)Const64_Hi)  || \
    ((LONG)Get_High32of64(Var1_64) == (LONG)Const64_Hi   &&  \
     (LONG)Get_Low32of64(Var1_64)  <  (LONG)Const64_Lo) )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_leq(Var1_64, Var2_64 ) \
  ((UQUAD)Var1_64 <= (UQUAD)Var2_64)
#else
#define is_leq(Var1_64, Var2_64 ) \
  /* first comparison has to be a "<" !!! */ \
  ( ((ULONG)Get_High32of64(Var1_64) <  (ULONG)Get_High32of64(Var2_64)) || \
    ((ULONG)Get_High32of64(Var1_64) == (ULONG)Get_High32of64(Var2_64)  &&  \
     (ULONG) Get_Low32of64(Var1_64) <= (ULONG) Get_Low32of64(Var2_64)     ) )
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define is_leqC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  ((UQUAD)Var1_64 <= (UQUAD)Const64)
#else
#define is_leqC(Var1_64, Const64_Hi, Const64_Lo, Const64 ) \
  /* first comparison has to be a "<" !!! */ \
  ( ((ULONG)Get_High32of64(Var1_64) <  (ULONG)Const64_Hi) || \
    ((ULONG)Get_High32of64(Var1_64) == (ULONG)Const64_Hi  &&  \
     (ULONG)Get_Low32of64(Var1_64)  <= (ULONG)Const64_Lo)  )
#endif

/* arithmetic */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define ADD64(Dest64, Var1_64, Var2_64) \
   Dest64 = Var1_64 + Var2_64;
#else
#define ADD64(Dest64, Var1_64, Var2_64) \
   if ((LONG) Get_Low32of64(Var1_64) < 0  && \
       (LONG) Get_Low32of64(Var2_64) < 0  )  \
   { \
     Set_Low32of64 (Dest64, Get_Low32of64 (Var1_64) + Get_Low32of64 (Var2_64) ); \
     Set_High32of64(Dest64, Get_High32of64(Var1_64) + Get_High32of64(Var2_64) + 1 ); \
   } \
   else \
     if ((LONG) Get_Low32of64(Var1_64) >= 0  && \
         (LONG) Get_Low32of64(Var2_64) >= 0  )  \
     { \
       Set_Low32of64 (Dest64, Get_Low32of64 (Var1_64) + Get_Low32of64 (Var2_64) ); \
       Set_High32of64(Dest64, Get_High32of64(Var1_64) + Get_High32of64(Var2_64) ); \
     } \
     else \
     { \
       Set_Low32of64 (Dest64, Get_Low32of64 (Var1_64) + Get_Low32of64 (Var2_64) ); \
       if ((LONG)Get_Low32of64(Dest64) >= 0 ) \
         Set_High32of64(Dest64, Get_High32of64(Var1_64) + Get_High32of64(Var2_64) + 1 ); \
       else \      
         Set_High32of64(Dest64, Get_High32of64(Var1_64) + Get_High32of64(Var2_64) ); \
     }      
#endif

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define ADD64Q(Dest64, Var64) \
   Dest64 += Var64;
#else
#define ADD64Q(Dest64, Var64) \
   if ((LONG) Get_Low32of64(Dest64) < 0  && \
       (LONG) Get_Low32of64(Var64)  < 0  )  \
   { \
     Set_Low32of64 (Dest64, Get_Low32of64 (Dest64) + Get_Low32of64 (Var64) ); \
     Set_High32of64(Dest64, Get_High32of64(Dest64) + Get_High32of64(Var64) + 1 ); \
   } \
   else \
     if ((LONG) Get_Low32of64(Dest64) >= 0  && \
         (LONG) Get_Low32of64(Var64)  >= 0  )  \
     { \
       Set_Low32of64 (Dest64, Get_Low32of64 (Dest64) + Get_Low32of64 (Var64) ); \
       Set_High32of64(Dest64, Get_High32of64(Dest64) + Get_High32of64(Var64) ); \
     } \
     else \
     { \
       Set_Low32of64 (Dest64, Get_Low32of64 (Dest64) + Get_Low32of64 (Var64) ); \
       if ((LONG)Get_Low32of64(Dest64) >= 0 ) \
         Set_High32of64(Dest64, Get_High32of64(Dest64) + Get_High32of64(Var64) + 1 ); \
       else \      
         Set_High32of64(Dest64, Get_High32of64(Dest64) + Get_High32of64(Var64) ); \
     }      
#endif


#if defined AROS_64BIT_TYPE || defined __GNUC__
#define SUB64(Dest64, Var1_64, Var2_64) \
   Dest64 = Var1_64 - Var2_64;
#else
#define SUB64(Dest64, Var1_64, Var2_64) \
   printf("using SUB! still incomplete define!!\n"); 
#endif

/* NEG might still produce a wrong result for certain arguments */

#if defined AROS_64BIT_TYPE || defined __GNUC__
#define NEG64(Dest64) \
   Dest64 = -Dest64;
#else
#define NEG64(Dest64) \
  Set_High32of64( Dest64, Get_High32of64(Dest64) ^ 0xFFFFFFFF ); \
  Set_Low32of64 ( Dest64, -Get_High32of64(Dest64)); 
#endif


#endif /* __MATHIEEE64BIT_DEFINES_H__ */

