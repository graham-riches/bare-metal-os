/*! \file common.h
*
*  \brief common module includes
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <stdbool.h>
#include <stdint.h>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/
#define PARAMETER_NOT_USED(X) (void)(X)

#ifndef NULL
#    define NULL ((void*)0)
#endif

/* setup arm math */
#ifndef ARM_MATH_CM4
#    define ARM_MATH_CM4
#endif

#ifndef __STATIC_INLINE
#    define __STATIC_INLINE static inline
#endif

#ifndef __GNUC__
#    define __GNUC__
#endif
