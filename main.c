#include <stdio.h>


#define LEFT_OP_MASK        (0xf)
#define ADD_MASK            (1 << 4)
#define MUL_MASK            (1 << 5)
#define SUB_MASK            (1 << 6)
#define DIV_MASK            (1 << 7)
#define RIGHT_OP_MASK       (0xf00)

#define LEFT_OP_SHIFT       (0)
#define ADD_SHIFT           (4)
#define MUL_SHIFT           (5)
#define SUB_SHIFT           (6)
#define DIV_SHIFT           (7)
#define RIGHT_OP_SHIFT      (8)

#define GET_KNOB_LEFT_OP(data)  (((data) & LEFT_OP_MASK) >> LEFT_OP_SHIFT)
#define GET_KNOB_RIGHT_OP(data) (((data) & RIGHT_OP_MASK) >> RIGHT_OP_SHIFT)
#define GET_KNOB_ADD(data)      ((data) & ADD_MASK) >> ADD_SHIFT)
#define GET_KNOB_MUL(data)      (((data) & MUL_MASK) >> MUL_SHIFT)
#define GET_KNOB_DIV(data)      (((data) & DIV_MASK) >> DIV_SHIFT)
#define GET_KNOB_SUB(data)      (((data) & SUB_MASK) >> SUB_SHIFT)

#define BYTE_MAX            (127)
#define BYTE_MIN            (-128)

extern bool readKnobInputs(unsigned *dataPtr);
extern void writeLedOutput(unsigned data);

int main(void)
{
    while(1)
    {
        unsigned knob_data;
        int result  = 0;
        bool     b_error = false;

        if (!readKnobInputs(&knob_data))
        {
            printf("\nERROR: Failed to read knob inputs. Exiting.");
            return -1;
        }

        // Knobs read successfully. Decode it now.
        unsigned int left_operand  = GET_KNOB_LEFT_OP(knob_data);
        unsigned int right_operand = GET_KNOB_RIGHT_OP(knob_data);
        bool b_add                 = (bool) GET_KNOB_ADD(knob_data);
        bool b_sub                 = (bool) GET_KNOB_SUB(knob_data);
        bool b_mul                 = (bool) GET_KNOB_MUL(knob_data);
        bool b_div                 = (bool) GET_KNOB_DIV(knob_data);

        // Compute the result.
        if (b_add) 
        {
            result = left_operand + right_operand;
        }
        else if (b_sub)
        {
            result = left_operand - right_operand;
        }
        else if (b_mul)
        {
            result = left_operand * right_operand;
        }
        else if (b_div)
        {
            result = left_operand / right_operand;
        }

        // Handle overflow situation.
        if ((result > BYTE_MAX)) || (result < BYTE_MIN))
        {
            b_error = true;
            result = 0;
        }

        // Finally, program the hardware to reflect the result.
        writeLedOutput(((result << 1) | (unsigned int) b_error));
    }
}
