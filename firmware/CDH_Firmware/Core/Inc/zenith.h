/*
 * zenith.h
 *
 *  Created on: Jan 28, 2022
 *      Author: leocelente
 */

#ifndef INC_ZENITH_H_
#define INC_ZENITH_H_


#include <stdio.h>
extern osMutexId_t UART_MutexHandle;


#ifndef DEBUG
    #define debug(fmt, ...) (void)0;
#else
#define debug(fmt, ...) \
		if (osMutexAcquire(UART_MutexHandle, osWaitForever) == osOK) { \
			printf("%s:%d: \t"fmt"\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
			osMutexRelease(UART_MutexHandle); \
		}
#endif



#endif /* INC_ZENITH_H_ */
