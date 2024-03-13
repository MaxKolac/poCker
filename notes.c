/*
* -- Notes for self. This file is not included in any builds. --
*
* #include declarations are basically copying those file contents here. Their order matters!
* Also, I'm not sure of this 100% but I believe redeclaration issues can be avoided if:
* - the header1.h requires header2.h
* - header1.h does NOT include header2.h BUT-
* - main.c includes first the header2.h then header1.h
* Or just use guard macros - #ifndef header_name, #define header_name and then #endif at the EOF. They might work too.
*
* Elements order: https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide/
*   #include <>
*   #include ""
*   #defines
*   Data Types (structures, types etc.)
*   Globals (constants outside of function scopes)
*   Prototypes & Function Declarations
*   Code
*
* Take a look at commit 686690df388e5e523d4a20ff21db2c1daeb0dde3. Things to learn from it:
* - Arrays without initialized elements might include random gibberish that
*   happened to be there when array claimed its spots in memory. In the case of this commit, random integers.
* - Integers greater than 1 are also treated as true. Likely because booleans in stdlib.h are just integers in disguise.
*
* Prototype of a function which takes in one array of pointers to structs as parameter looks as follows:
* return_type functionName(struct Type*[]);
* In case I ever forget.
*
*   There are 4 storage classes: auto, extern, static, register
*   Type    |   Location    |    Lifetime    |   initial value   |
*   auto    |   stack       | function scope |   garbage         |
*   The default one.
*
*   extern  |   heap        | end of program |   zero            |
*   Short for external, tells us that it is declared elsewhere and not within the same block it is written.
*   Kind of like a global variable. Accessible from any block/function. Even between two files.
*
*   static  |   heap        | end of program |   zero            |
*   Preserve their value even once the program goes outside of their scope.
*   Initialized only once and live until EOP. Accessible from within their scope only.
*   NOTE: using static on a function is like making it private in C#, it's only available within the same file.
*
*   register| CPU registry  | function scope |   garbage        |
*   Same as auto, but it's in CPU's registry block. MUCH faster to access.
*   Commonly used for data that is accessed frequently. If no registry place is available, they will
*   reside inside RAM. Unable to create pointers to them.
*
*   Apparently the compiler is fine with accidentally assigning values in the IF instructions as a condition.
*/
