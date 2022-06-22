#include <stdio.h>
#include <stdbool.h>

// Macros for API spec.

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
#define GET_KNOB_ADD(data)      (((data) & ADD_MASK) >> ADD_SHIFT)
#define GET_KNOB_MUL(data)      (((data) & MUL_MASK) >> MUL_SHIFT)
#define GET_KNOB_DIV(data)      (((data) & DIV_MASK) >> DIV_SHIFT)
#define GET_KNOB_SUB(data)      (((data) & SUB_MASK) >> SUB_SHIFT)

#define BYTE_MAX            (127)
#define BYTE_MIN            (-128)


// HW spec defines.

#define USER_REG_INP_1          ((unsigned int *)0xa0000000)
#define USER_REG_INP_2          ((unsigned int *)0xa0000004)
#define USER_REG_OUT_1          ((unsigned int *)0xa0000008)

volatile const unsigned int *userInpReg1 = USER_REG_INP_1;
volatile const unsigned int *userInpReg2 = USER_REG_INP_2;

int *userOutReg1 = USER_REG_OUT_1;

#define HW_POS_ADD      (0)
#define HW_POS_SUB      (1)
#define HW_POS_MUL      (2)
#define HW_POS_DIV      (3) 

bool readKnobInputs(unsigned *dataPtr)
{
    int inp_reg1_val = *userInpReg1;
    int inp_reg2_val = *userInpReg2;
    int left_operand = 0;
    int right_operand = 0;

    if (dataPtr == NULL)
        return false;

    // get left operand.
    int value = inp_reg1_val >> 16;
    for (int i = 0; i < 16; i++)
    {
        if (value & 1 == 1) 
        {
            left_operand = i;
            break;
        }
        value = value >> 1;
    }
    

    // get right operand. 
    value = inp_reg1_val & 0xffff;
    for (int i = 0; i < 16; i++)
    {
        if (value & 1 == 1)
        {
            right_operand = i;
            break;
        }
        value = value >> 1;
    }

    *dataPtr = 0;
    *dataPtr = *dataPtr | ((*dataPtr & LEFT_OP_MASK) | (left_operand << LEFT_OP_SHIFT));
    *dataPtr = *dataPtr | ((*dataPtr & RIGHT_OP_MASK) | (right_operand << RIGHT_OP_SHIFT));
    
    // get operation type
    if (inp_reg2_val & (1 << HW_POS_ADD))
    {
        *dataPtr = *dataPtr | ((*dataPtr & ADD_MASK) | (1 << ADD_SHIFT));
    }
    if (inp_reg2_val & (1 << HW_POS_SUB))
    {
        *dataPtr = *dataPtr | ((*dataPtr & SUB_MASK) | (1 << SUB_SHIFT));
    }
    if (inp_reg2_val & (1 << HW_POS_MUL))
    {
        *dataPtr = *dataPtr | ((*dataPtr & MUL_MASK) | (1 << MUL_SHIFT));
    }
    if (inp_reg2_val & (1 << HW_POS_DIV))
    {
        *dataPtr = *dataPtr | ((*dataPtr & DIV_MASK) | (1 << DIV_SHIFT));
    }

    return true;
    
}

void writeLedOutput(unsigned data)
{
//    printf("Answer is : %d", data >> 1);
//    printf("LED Error is : %u", data & 1);

    *userOutReg1 = data;
}

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
            if (right_operand == 0)
            {
                b_error = true;
                result = 0;
            }
            else
            {
                result = left_operand / right_operand;
            }
        }

        // Handle overflow situation.
        if ((result > BYTE_MAX) || (result < BYTE_MIN))
        {
            b_error = true;
            result = 0;
        }

        // Finally, program the hardware to reflect the result.
        writeLedOutput(((result << 1) | (unsigned int) b_error));
    }

    return 0;

}
