/*
 * util.c
 *
 *  Created on: Jan 16, 2020
 *      Author: dmitriy
 */
#include "util.h"

const uint_least32_t crc_table[256] = { 0x00000000, 0x77073096, 0xEE0E612C,
		0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3, 0x0EDB8832,
		0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07,
		0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D,
		0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A,
		0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8,
		0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD,
		0xA50AB56B, 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3,
		0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC, 0x51DE003A, 0xC8D75180,
		0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F, 0x2802B89E,
		0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB,
		0xB6662D3D, 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589,
		0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E,
		0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01, 0x6B6B51F4,
		0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1,
		0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF,
		0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, 0x4DB26158, 0x3AB551CE, 0xA3BC0074,
		0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A,
		0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F,
		0xDD0D7CC9, 0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525,
		0x206F85B3, 0xB966D409, 0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822,
		0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320,
		0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615,
		0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B,
		0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344, 0x8708A3D2, 0x1E01F268,
		0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76,
		0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43,
		0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1,
		0xA6BC5767, 0x3FB506DD, 0x48B2364B, 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6,
		0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79, 0xCB61B38C,
		0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9,
		0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7,
		0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C,
		0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82,
		0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7,
		0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD,
		0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6, 0xFF0F6A70, 0x66063BCA,
		0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45, 0xA00AE278,
		0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D,
		0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53,
		0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9, 0xBDBDF21C, 0xCABAC28A, 0x53B39330,
		0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E,
		0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B,
		0x2D02EF8D };

static const uint16_t crctable[256] = { 0x0000, 0x1189, 0x2312, 0x329B, 0x4624,
		0x57AD, 0x6536, 0x74BF, 0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5,
		0xE97E, 0xF8F7, 0x0919, 0x1890, 0x2A0B, 0x3B82, 0x4F3D, 0x5EB4, 0x6C2F,
		0x7DA6, 0x8551, 0x94D8, 0xA643, 0xB7CA, 0xC375, 0xD2FC, 0xE067, 0xF1EE,
		0x1232, 0x03BB, 0x3120, 0x20A9, 0x5416, 0x459F, 0x7704, 0x668D, 0x9E7A,
		0x8FF3, 0xBD68, 0xACE1, 0xD85E, 0xC9D7, 0xFB4C, 0xEAC5, 0x1B2B, 0x0AA2,
		0x3839, 0x29B0, 0x5D0F, 0x4C86, 0x7E1D, 0x6F94, 0x9763, 0x86EA, 0xB471,
		0xA5F8, 0xD147, 0xC0CE, 0xF255, 0xE3DC, 0x2464, 0x35ED, 0x0776, 0x16FF,
		0x6240, 0x73C9, 0x4152, 0x50DB, 0xA82C, 0xB9A5, 0x8B3E, 0x9AB7, 0xEE08,
		0xFF81, 0xCD1A, 0xDC93, 0x2D7D, 0x3CF4, 0x0E6F, 0x1FE6, 0x6B59, 0x7AD0,
		0x484B, 0x59C2, 0xA135, 0xB0BC, 0x8227, 0x93AE, 0xE711, 0xF698, 0xC403,
		0xD58A, 0x3656, 0x27DF, 0x1544, 0x04CD, 0x7072, 0x61FB, 0x5360, 0x42E9,
		0xBA1E, 0xAB97, 0x990C, 0x8885, 0xFC3A, 0xEDB3, 0xDF28, 0xCEA1, 0x3F4F,
		0x2EC6, 0x1C5D, 0x0DD4, 0x796B, 0x68E2, 0x5A79, 0x4BF0, 0xB307, 0xA28E,
		0x9015, 0x819C, 0xF523, 0xE4AA, 0xD631, 0xC7B8, 0x48C8, 0x5941, 0x6BDA,
		0x7A53, 0x0EEC, 0x1F65, 0x2DFE, 0x3C77, 0xC480, 0xD509, 0xE792, 0xF61B,
		0x82A4, 0x932D, 0xA1B6, 0xB03F, 0x41D1, 0x5058, 0x62C3, 0x734A, 0x07F5,
		0x167C, 0x24E7, 0x356E, 0xCD99, 0xDC10, 0xEE8B, 0xFF02, 0x8BBD, 0x9A34,
		0xA8AF, 0xB926, 0x5AFA, 0x4B73, 0x79E8, 0x6861, 0x1CDE, 0x0D57, 0x3FCC,
		0x2E45, 0xD6B2, 0xC73B, 0xF5A0, 0xE429, 0x9096, 0x811F, 0xB384, 0xA20D,
		0x53E3, 0x426A, 0x70F1, 0x6178, 0x15C7, 0x044E, 0x36D5, 0x275C, 0xDFAB,
		0xCE22, 0xFCB9, 0xED30, 0x998F, 0x8806, 0xBA9D, 0xAB14, 0x6CAC, 0x7D25,
		0x4FBE, 0x5E37, 0x2A88, 0x3B01, 0x099A, 0x1813, 0xE0E4, 0xF16D, 0xC3F6,
		0xD27F, 0xA6C0, 0xB749, 0x85D2, 0x945B, 0x65B5, 0x743C, 0x46A7, 0x572E,
		0x2391, 0x3218, 0x0083, 0x110A, 0xE9FD, 0xF874, 0xCAEF, 0xDB66, 0xAFD9,
		0xBE50, 0x8CCB, 0x9D42, 0x7E9E, 0x6F17, 0x5D8C, 0x4C05, 0x38BA, 0x2933,
		0x1BA8, 0x0A21, 0xF2D6, 0xE35F, 0xD1C4, 0xC04D, 0xB4F2, 0xA57B, 0x97E0,
		0x8669, 0x7787, 0x660E, 0x5495, 0x451C, 0x31A3, 0x202A, 0x12B1, 0x0338,
		0xFBCF, 0xEA46, 0xD8DD, 0xC954, 0xBDEB, 0xAC62, 0x9EF9, 0x8F70 };

const uint8_t crc8_table[] = { 0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126,
		32, 163, 253, 31, 65, 157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227,
		189, 62, 96, 130, 220, 35, 125, 159, 193, 66, 28, 254, 160, 225, 191,
		93, 3, 128, 222, 60, 98, 190, 224, 2, 92, 223, 129, 99, 61, 124, 34,
		192, 158, 29, 67, 161, 255, 70, 24, 250, 164, 39, 121, 155, 197, 132,
		218, 56, 102, 229, 187, 89, 7, 219, 133, 103, 57, 186, 228, 6, 88, 25,
		71, 165, 251, 120, 38, 196, 154, 101, 59, 217, 135, 4, 90, 184, 230,
		167, 249, 27, 69, 198, 152, 122, 36, 248, 166, 68, 26, 153, 199, 37,
		123, 58, 100, 134, 216, 91, 5, 231, 185, 140, 210, 48, 110, 237, 179,
		81, 15, 78, 16, 242, 172, 47, 113, 147, 205, 17, 79, 173, 243, 112, 46,
		204, 146, 211, 141, 111, 49, 178, 236, 14, 80, 175, 241, 19, 77, 206,
		144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238, 50, 108, 142, 208,
		83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115, 202, 148, 118,
		40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139, 87, 9, 235,
		181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22, 233, 183,
		85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168, 116, 42,
		200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53 };

static uint8_t _hex_to_dec(char elem);

uint8_t hex_to_dec(char *buff, uint8_t size) {

	uint8_t res = 0;

	for (uint8_t i = 0, j = size - 1; i < size; i++, j--) {
		res += _hex_to_dec(buff[i]) * _pow(16, i);
	}

	return res;
}

static uint8_t _hex_to_dec(char elem) { /* @suppress("Name convention for function")*/
	if (elem == 'A' || elem == 'a')
		return 10;
	if (elem == 'B' || elem == 'b')
		return 11;
	if (elem == 'C' || elem == 'c')
		return 12;
	if (elem == 'D' || elem == 'd')
		return 13;
	if (elem == 'E' || elem == 'e')
		return 13;
	if (elem == 'F' || elem == 'f')
		return 15;

	return elem - 0x30;

}

uint32_t _pow(uint16_t a, uint8_t c) { /* @suppress("Name convention for function")*/
	uint32_t res = 1;

	for (uint8_t i = 0; i < c; i++) {
		res *= a;
	}

	return res;
}

double f_pow(float a, uint8_t c) {
	double res = 1;

	for (uint8_t i = 0; i < c; i++) {
		res *= a;
	}

	return res;
}

/**
 * util int to char[]
 * */
char* utoa_builtin_div(uint32_t *value, char *buffer) {
	buffer += 11;
	/* 11 байт достаточно для десятичного представления 32-х байтного числа
	 и  завершающего нуля*/
	*--buffer = 0;
	do {
		*--buffer = *value % 10 + '0';
		*value /= 10;
	} while (*value != 0);
	return buffer;
}

uint8_t getSizeByParameter(char *buffer, char parameter) {
	uint8_t size = 0;
	while (*buffer != parameter && size < 255 && *buffer != '\0') {
		size++;
		buffer++;
	}

	return size;
}

void addToBuffer(char *element, char *buffer, uint8_t *index, uint8_t *size) {
	if (*index >= *size - 1) {
		return;
	}

	*(buffer + *index) = *element;
	*index = *index + 1;
}

void copyToSymbol(char *from, char *to, const char *delimiter,
		uint8_t max_size) {
	for (uint8_t i = 0; i < max_size; i++, from++) {
		if (*from == '\0' || *from == *delimiter) {
			to[i] = '\0';
			return;
		}
		if (*from == '\r') {
			from++;
			continue;
		}
		to[i] = *from;
	}
}

void copy_buffer(char *from, char *to, uint8_t max_size) {
	for (uint8_t i = 0; i < max_size; i++) {
		to[i] = from[i];
	}
}

void init_buffer(char *buffer, uint16_t size, char param) {

	for (int i = 0; i < size; i++) {
		*(buffer + i) = param;
	}
}

void concat(char *s1, char *s2, char *_result) {

	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);

	for (uint8_t i = 0; i < len1; i++, s1++) {
		_result[i] = *s1;
	}

	for (uint8_t i = len1; i < len1 + len2; i++, s2++) {
		_result[i] = *s2;
	}
}

void concat_len(char *s1, uint8_t len1, char *s2, uint8_t len2, char *_result) {

	for (uint8_t i = 0; i < len1; i++, s1++) {
		_result[i] = *s1;
	}

	for (uint8_t i = len1; i < len1 + len2; i++, s2++) {
		_result[i] = *s2;
	}
}

/**
 * calculate distance in km
 * */
float calc_dist(float a_lat1, float a_lon1, float a_lat2, float a_lon2) {

	float dist_calc = 0;
	float dist_calc2 = 0;
	float diflat = 0;
	float diflon = 0;

	diflat = radians(a_lat2 - a_lat1);
	a_lat1 = radians(a_lat1);
	a_lat2 = radians(a_lat2);
	diflon = radians((a_lon2) - (a_lon1));

	dist_calc = (sin(diflat / 2.0) * sin(diflat / 2.0));
	dist_calc2 = cos(a_lat1);
	dist_calc2 *= cos(a_lat2);
	dist_calc2 *= sin(diflon / 2.0);
	dist_calc2 *= sin(diflon / 2.0);
	dist_calc += dist_calc2;

	dist_calc = (2 * atan2(sqrt(dist_calc), sqrt(1.0 - dist_calc)));
	dist_calc *= 6371.0;
	return dist_calc;
}

float radians(float deg) {
	return (deg * 1000) / 57296;
}

/*
 Name  : CRC-32
 Poly  : 0x04C11DB7    x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11
 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
 Init  : 0xFFFFFFFF
 Revert: true
 XorOut: 0xFFFFFFFF
 Check : 0xCBF43926 ("123456789")
 MaxLen: 268 435 455 байт (2 147 483 647 бит) - обнаружение
 одинарных, двойных, пакетных и всех нечетных ошибок
 */
uint32_t crc_32(uint8_t *buf, uint32_t len) {
	uint32_t crc = 0;
	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);

	return crc ^ 0xFFFFFFFFUL;
}

uint32_t crc_32_simple(uint8_t *buf, uint32_t len) {
	uint32_t crc = 0;
	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);

	return crc;
}

uint16_t crc_16(const void *buf, uint32_t len) {
	uint16_t crc = 0;
	uint8_t *c = (uint8_t*) buf;
	while (len--)
		crc = crctable[(crc ^ *c++) & 0xFF] ^ (crc >> 8);

	return crc;
}

uint8_t crc_8_simple(uint8_t *adr_buffer, uint16_t byte_cnt) {
	return crc_8(adr_buffer, byte_cnt);
}

uint8_t crc_8(uint8_t *adr_buffer, uint16_t byte_cnt) {
	uint8_t crc8 = 0;
	for (uint16_t i = 0; i < (byte_cnt); i++) {
		crc8 = crc8_table[crc8 ^ adr_buffer[i]];
	}
	return crc8;
}

void __wait(uint32_t timeout) { /* @suppress("Name convention for function")*/
	for (uint32_t i = 0; i < timeout; i++) {
		__NOP();

	}
}

uint8_t rtc_bcd2_to_byte(uint8_t value) {
	uint32_t tmp = 0U;
	tmp = ((uint8_t) (value & (uint8_t) 0xF0) >> (uint8_t) 0x4) * 10;
	return (tmp + (value & (uint8_t) 0x0F));
}

uint16_t modul16(signed short a) {
	uint16_t tmp = (uint16_t) a;
	if (tmp & 0x8000) {
		tmp = 0xFFFF - tmp + 1;
	}
	return tmp;
}

uint32_t modul32(signed int a) {
	if (a & 0x80000000) {
		a = 0xFFFFFFFF - a + 1;
	}
	return a;
}

int8_t compare_float(float a, float b) {
	const float eps = 0.001;
	if (fabsf(a - b) > eps) {
		if ((a - b) > eps) {
			return 1;
		} // a>b
		if ((a - b) < eps) {
			return -1;
		} // a<b
	}
	return 0;
}

