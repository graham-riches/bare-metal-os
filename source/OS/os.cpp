/*! \file kernel.c
*
*  \brief contains the main kernel functions for running the operating system.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "os.h"
#include "hal_interrupt.h"
#include "stm32f4xx.h"
#include "cm4_port.h"


namespace os
{
/********************************** Global Objects and Variables *******************************************/
scheduler::TaskControlBlock* system_active_task;
scheduler::TaskControlBlock* system_pending_task;

/********************************** Local Objects and Variables *******************************************/

/********************************** Function Definitions *******************************************/

namespace kernel
{
/**
 * \brief initialize the kernel
 */
void setup(void) {
    DISABLE_INTERRUPTS();

    //!< initialize the task pointers to initialize the kernel
    system_active_task = scheduler::get_active_task_control_block();
    system_pending_task = scheduler::get_pending_task_control_block();

    //!< activate the first task
    system_active_task->thread_ptr->set_status(thread::status::active);

    //!< start the system clock
    system_clock::initialize();

    //!< setup core interrupt priorities
    HAL::interrupt_manager.set_priority(HAL::InterruptName::systick_handler, HAL::PreemptionPriority::level_16);
    HAL::interrupt_manager.set_priority(HAL::InterruptName::pendsv_handler, HAL::PreemptionPriority::level_16);

    ENABLE_INTERRUPTS();
}

/**
 * \brief pick up the first task control block pointer
 */
// clang-format off
__attribute__((naked))  void enter(void) {
    using namespace os;

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
// clang-format on

/**
 * \brief Set the systick timer frequency
 * 
 * \param ticks of the main sysclock per interrupt
 */
void set_systick_frequency(uint32_t ticks) {
    SysTick_Config(ticks);
}

};  // namespace kernel

};  // namespace os
