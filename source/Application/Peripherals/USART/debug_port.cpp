/*! \file debug_port.c
*
*  \brief class for managing the debug port.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "debug_port.h"
#include "hal_gpio.h"
#include "hal_interrupt.h"
#include "hal_rcc.h"
#include <cstdio>
#include <cstring>

/*********************************** Consts ********************************************/
constexpr uint16_t debug_port_buffer_size = 256;

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************** Local Variables **************************************/
/* create the GPIO pins */
static HAL::AlternateModePin tx_pin( GPIOA, HAL::Pins::pin_2, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af7 );
static HAL::AlternateModePin rx_pin( GPIOA, HAL::Pins::pin_3, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af7 );


/******************************* Global Variables **************************************/
DebugPort debug_port( USART2, debug_port_buffer_size, debug_port_buffer_size );


/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new Debug Port:: Debug Port object
 * 
 * \param usart the uart peripheral address pointer
 * \param tx_size size of the tx ring buffer
 * \param rx_size size of the rx ring buffer
 */
DebugPort::DebugPort( USART_TypeDef *usart, size_t tx_size, size_t rx_size )
: HAL::USARTInterrupt( usart, tx_size, rx_size )
{

}


/**
 * \brief initialize the debug port with the correct HW settings
 */
void DebugPort::initialize( void )
{
   using namespace HAL;

   /* enable the GPIO clocks and the USART clocks */
   reset_control_clock.set_apb1_clock( APB1Clocks::usart_2, true );
   reset_control_clock.set_ahb1_clock( AHB1Clocks::gpio_a, true );

   /* configure the usart with the application specific settings */
   this->write_control_register( USARTControlRegister1::parity_selection, 0x00 );
   this->write_control_register( USARTControlRegister1::word_length, 0x00 );
   this->write_control_register( USARTControlRegister2::stop_bits, 0x00 );
   this->write_control_register( USARTControlRegister3::cts_enable, 0x00 );
   this->write_control_register( USARTControlRegister3::rts_enable, 0x00 );
   this->set_baudrate( Clocks::APB1, 115200 );

   /* enable the usart and interrupts */
   this->write_control_register( USARTControlRegister1::receiver_enable, 0x01 );
   this->write_control_register( USARTControlRegister1::transmitter_enable, 0x01 );
   this->write_control_register( USARTControlRegister1::receive_interrupt_enable, 0x01 );

   /* register the interrupt in the hal interrupts table */
   interrupt_manager.register_callback( InterruptName::usart_2, this, 0, 2 );

   /* enable the UART */
   this->write_control_register( USARTControlRegister1::usart_enable, 0x01 );
}

/**
 * \brief log a debug message
 * 
 * \param message the message to send
 */
void DebugPort::debug( const char *message )
{
   this->send( "DEBUG: " );
   this->send( message );
   this->send( "\r\n" );
}

/**
 * \brief log an info message
 * 
 * \param message the message to send
 */
void DebugPort::info( const char *message )
{
   this->send( "INFO: " );
   this->send( message );
   this->send( "\r\n" );
}

/**
 * \brief log a warning
 * 
 * \param message the warning message
 */
void DebugPort::warning( const char *message )
{
   this->send( "WARNING: " );
   this->send( message );
   this->send( "\r\n" );
}

/**
 * \brief log an error
 * 
 * \param message the error to log
 */
void DebugPort::error( const char *message )
{
   this->send( "ERROR: " );
   this->send( message );
   this->send( "\r\n" );
}