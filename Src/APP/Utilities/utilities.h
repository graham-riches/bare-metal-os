/*! \file utilities.h
*
*  \brief utilities module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"

/*********************************** Consts ********************************************/
#define UTILITIES_MAX_STRSPLIT_LENGTH  512

/************************************ Types ********************************************/
/**
 * \brief generic ring buffer structure
 */
typedef struct
{
    uint16_t head;  //!< index of the head pointer
    uint16_t tail;  //!< index of the tail pointer
    uint16_t size;  //!< capacity of the ring buffer
    uint8_t *data;  //!< pointer to the data array
} UTILITIES_ringBuffer_t;

/**
 * \brief return values for the ring buffer structure
 */
typedef enum
{
    UTILITIES_RINGBUFFER_OK = 0, //!< ring buffer operation was successful
    UTILITIES_RINGBUFFER_FULL,   //!< ring buffer operation failed as the buffer was full
    UTILITIES_RINGBUFFER_NULL,   //!< invalid argument passed to function
    UTILITIES_RINGBUFFER_EMPTY,  //!< not enough data is available

} UTILITIES_ringBufferStatus_t;

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
UTILITIES_ringBufferStatus_t UTILITIES_ringBufferAdd( UTILITIES_ringBuffer_t *buffer, uint8_t *data, uint16_t size );
uint16_t UTILITIES_ringBufferGet( UTILITIES_ringBuffer_t *buffer, uint8_t *data, uint16_t size );
uint16_t UTILITIES_ringBufferGetFreeSpace( UTILITIES_ringBuffer_t *buffer );
uint16_t UTILITIES_ringBufferGetUsedSpace( UTILITIES_ringBuffer_t *buffer );
int32_t UTILITIES_strSplit ( char *inputString, char delimiter, int maxStrings, char *argv[] );

#ifdef __cplusplus
}
#endif

#endif /* __UTILITIES_H__ */
