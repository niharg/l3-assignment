# Project overview
main.c and ext_func.h files contain the code expected in Part A and Part B of the assignment.
test_cases.txt contain additional test cases as expected in Part C.

IMPORTANT NOTE: Running the code as is will result in a SIGSEGV since the register HW addresses do not really exist. 
But the code can be tested with some hacks like overriding the register variables with custom value and uncommenting the printf() inside writeLedOutput().
I have tested the code with some fake inputs for verification.

