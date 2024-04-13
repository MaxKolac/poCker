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
*
*   https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers
*   Function pointers need to be inside the extra brackets. Otherwise it just look like we are declaring a new function.
*   The syntax is: return_type (*pointer_name)(arg_type_one, arg_type_two, ...) = &function_to_point_at
*
*   https://gsamaras.wordpress.com/code/caution-when-reading-char-with-scanf-c/
*   When using scanf() to get user's input as a single character, remember to put a space before it like this:
*   scanf(" %c", &input);
*
*   https://stackoverflow.com/a/57221903/21342746
*   Defining global consts to make sure they are redefined in other files that include them should
*   be done as in following example:
*    - file.h includes:
*   extern const type var_name; (this is only declaration)
*    - file.c includes:
*   const type var_name = value; (this is the definition)
*   To quote Eugene Sh. from the answer above the linked one:
*   "Don't define variables (even const) in h files. extern them there.
*   The rule of thumb - if it is creating an object in memory - it should not be in the header."
*
*   AddressSanitizer is for detection of memory leaks, dangling pointers and all that.
*   Apparently this lib does not come with the GCC GNU compiler for Windows???
*   In case you ever figure out how to use this, make sure that:
*   -fsanitize=address -static-libasan -g
*   is added to both compiler AND linker options + you rebuild the whole project once you remove ASan.
*   Object files created this way still have SANA in them and things will not compile if you don't rebuild the whole project.
*
*   Apparently, it is a better practice for source files to include their own header.
*   This way the compiler will catch if you change a function implementation parameter list and
*   forget to do the same with its declaration in the header.
*   https://stackoverflow.com/a/30817652/21342746
*
*   Apparently, consts in C are not truly constant. They can't be used to create
*   a struct member of array type with non-variadic length. A macro needs to be used instead, since
*   macros are preprocessed before compilation. If a need ever arises to reference a const's addres, make use
*   of the const instead of a macro, which has the same name with a _OBJ suffix (as a convention).
*   https://stackoverflow.com/a/30583588/21342746
*
*   Although these two might seem the same:
*   char* mytext = "Hello World!"
*   char mytest[] = "Hello World!"
*   The first one is a string literal. It is a --constant-- string, meaning if you attempt to modify it, you'll ger
*   a segmentation fault.
*   The second one will instruct the compiler to first create an actual array, then copy the chars into it,
*   allowing you to modify them.
*   https://stackoverflow.com/questions/11554262/converting-char-to-lower-case-throws-segfault-error
*/
