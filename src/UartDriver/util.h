/*
 * util.h
 *
 *  Created on: Jan 16, 2020
 *      Author: dmitriy
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "math.h"
#include "main.h"
char* utoa_builtin_div(uint32_t *value, char *buffer);
uint8_t getSizeByParameter(char *buffer, char parameter);
void addToBuffer(char *element, char *buffer, uint8_t *index, uint8_t *size);
void copyToSymbol(char *from, char *to, const char *delimiter,
		uint8_t max_size);
void init_buffer(char *buffer, uint16_t size, char param);
void copy_buffer(char *from, char *to, uint8_t max_size);
void concat(char *s1, char *s2, char *_result);
void concat_len(char *s1, uint8_t len1, char *s2, uint8_t len2, char *_result);

uint8_t hex_to_dec(char *buff, uint8_t size);
uint32_t _pow(uint16_t a, uint8_t c);
double f_pow(float a, uint8_t c);
float radians(float deg);
float calc_dist(float a_lat1, float a_lon1, float a_lat2, float a_lon2);

uint32_t crc_32(uint8_t *buf, uint32_t len);
uint32_t crc_32_simple(uint8_t *buf, uint32_t len);
uint8_t crc_8_simple(uint8_t *adr_buffer, uint16_t byte_cnt);
uint8_t crc_8(uint8_t *adr_buffer, uint16_t byte_cnt);

void __wait(uint32_t timeout);
uint8_t rtc_bcd2_to_byte(uint8_t value);
uint16_t modul16(signed short a);
uint32_t modul32(signed int a);

void littleendian_to_bigendian(void *x, uint16_t size);
int8_t compare_float(float a, float b);

#endif /* INC_UTIL_H_ */
