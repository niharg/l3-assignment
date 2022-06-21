// Input: Pointer to store value(s) upon successful read.
// Format will be:
// - bits 0 to 3, Left Operand value in binary
// - bit 4, Addition Operator selected
// - bit 5, Multiplication Operator selected
// - bit 6, Subtraction Operator selected
// - bit 7, Division Operator selected
// - bits 8 to 11, Right Operand value in binary
// Output: true if read was successful, false otherwise
extern bool readKnobInputs(unsigned *dataPtr);
// Input: Value to write to display LEDs.
// Each bit set will turn an LED ON, each bit clear will turn an LED OFF.
// Format will be:
// - bit 0: Result LED Error
// - bit 1: Result LED Bit 0 (LSB)
// - bit 2: Result LED Bit 1
// - bit 3: Result LED Bit 2
// - bit 4: Result LED Bit 3
// - bit 5: Result LED Bit 4
// - bit 6: Result LED Bit 5
// - bit 7: Result LED Bit 6
// - bit 8: Result LED Bit 7 (MSB)
// Output: N/A
extern void writeLedOutput(unsigned data);

