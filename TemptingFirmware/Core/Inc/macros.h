/*
 * macros.h
 *
 *  Created on: May 13, 2025
 *      Author: nick
 */

#ifndef INC_MACROS_H_
#define INC_MACROS_H_

#define RETURN_IF_ERROR(x)          \
    do {                            \
        if ((x) != HAL_OK) {        \
            return x;               \
        }                           \
    } while (0)

#endif /* INC_MACROS_H_ */
