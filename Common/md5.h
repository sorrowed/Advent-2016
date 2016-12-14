/*
 * md5.h
 *
 *  Created on: Dec 14, 2016
 *      Author: tom
 */

#ifndef COMMON_MD5_H_
#define COMMON_MD5_H_

#include <cinttypes>

void md5(const uint8_t *initial_msg, int initial_len, uint8_t *digest);


#endif /* COMMON_MD5_H_ */
