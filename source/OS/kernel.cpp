/*! \file kernel.c
*
*  \brief contains the main kernel functions for running the operating system.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "kernel.h"
#include "stm32f4xx.h"

namespace OS
{
/********************************** Constants *******************************************/
constexpr uint32_t pendSV_pending_bit_offset = 0x01U << 28;


/********************************** Function Declarations *******************************************/
void set_pending_context_switch(void);
bool is_context_switch_pending(void);

/********************************** Global Variables *******************************************/
SystemClock core_clock;
Scheduler scheduler(core_clock, SYSTEM_MAX_THREADS, set_pending_context_switch, is_context_switch_pending);
TaskControlBlock* system_active_task = scheduler.get_active_tcb_ptr();
TaskControlBlock* system_pending_task = scheduler.get_pending_tcb_ptr();


/********************************** Function Definitions *******************************************/
/**
 * \brief launch the kernel
 */
__attribute__((naked)) void enter_kernel(void) {
    using namespace OS;

    __asm("CPSID      I                        \n" /* disable interrupts */
          "LDR        R0, =system_active_task  \n" /* load the active task pointer into r0*/
          "LDR        R1, [R0]                 \n" /* load the stack pointer from the contents of task into R1 */
          "LDR        R4, [R1]                 \n" /* copy the saved stack pointer into R4 */
          "MOV        SP, R4                   \n" /* update the stack pointer */
          "POP        {R4-R11}                 \n" /* pop R4-R11 off the stack */
          "POP        {R0-R3}                  \n" /* pop R0-R3 off the stack */
          "POP        {R4}                     \n" /* pop the last Cortex-saved register */
          "MOV        R12, R4                  \n" /* restore R12 */
          "ADD        SP,SP,#4                 \n" /* skip over the saved LR as it is invalid on startup */
          "POP        {R4}                     \n" /* grab the task function pointer */
          "MOV        LR, R4                   \n" /* restore the LR state */
          "ADD        SP,SP,#4                 \n" /* increment the stack pointer */
          "CPSIE      I                        \n" /* re-enable interrupts */
          "BX         LR                       \n" /* branch to the link register */
    );
}

/**
 * \brief Set the systick timer frequency
 * 
 * \param ticks of the main sysclock per interrupt
 */
void set_systick_frequency(uint32_t ticks) {
    SysTick_Config(ticks);
    NVIC_SetPriority(SysTick_IRQn, 15);
}

/**
 * \brief sleep the calling thread for a set amount of ticks.
 * \note this is not in ms because the user can configure the systick interrupt to a variable amount
 *       so there is not a 1:1 correspondence between ticks and ms
 * 
 * \param ticks how many ticks to sleep
 */
void sleep_thread(uint32_t ticks){
    scheduler.sleep_thread(ticks);
}

/**
 * \brief Set the PendSV interrupt flag in the NVIC to trigger a context switch
 */
void set_pending_context_switch(void) {
    NVIC_SetPendingIRQ(PendSV_IRQn);
}

/**
 * \brief check if an interrupt is already pending for a context switch
 * 
 * \retval true/false 
 */
bool is_context_switch_pending(void) {
    return static_cast<bool>(SCB->ICSR & pendSV_pending_bit_offset);
}


};  // namespace OS
