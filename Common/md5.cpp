/*
 * Simple MD5 implementation
 *
 * Compile with: gcc -o md5 md5.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
 
#define N 100000

// leftrotate function definition

#define F(x, y, z) (z ^ ( x & (y ^ z) ))
#define G(x, y, z) (y ^ ( z & (y ^ x) ))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

#define FF(a, b, c, d, x, s, ac) { \
	(a) += F((b), (c), (d)) + (x) + (uint32_t)(ac); \
	(a) = LEFTROTATE((a), (s)); \
	(a) += (b); \
}
 
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G((b), (c), (d)) + (x) + (uint32_t)(ac); \
	(a) = LEFTROTATE((a), (s)); \
	(a) += (b); \
}

#define HH(a, b, c, d, x, s, ac) { \
	(a) += H((b), (c), (d)) + (x) + (uint32_t)(ac); \
	(a) = LEFTROTATE((a), (s)); \
	(a) += (b); \
}

#define II(a, b, c, d, x, s, ac) { \
	(a) += I((b), (c), (d)) + (x) + (uint32_t)(ac); \
	(a) = LEFTROTATE((a), (s)); \
	(a) += (b); \
}

void to_bytes(uint32_t val, uint8_t *bytes)
{
    bytes[0] = (uint8_t) val;
    bytes[1] = (uint8_t) (val >> 8);
    bytes[2] = (uint8_t) (val >> 16);
    bytes[3] = (uint8_t) (val >> 24);
}
 
uint32_t to_int32(const uint8_t *bytes)
{
    return (uint32_t) bytes[0]
        | ((uint32_t) bytes[1] << 8)
        | ((uint32_t) bytes[2] << 16)
        | ((uint32_t) bytes[3] << 24);
}
 
void md5(const uint8_t *initial_msg, int initial_len, uint8_t *digest) {
 
    // These vars will contain the hash
    register uint32_t h0, h1, h2, h3;
 
    // Message (to prepare)
    uint8_t *msg = NULL;
 
    int new_len, offset;
    uint32_t w[16];
    register uint32_t a, b, c, d, i, f, temp;
 
    // Initialize variables - simple count in nibbles:
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
 
    //Pre-processing:
    //append "1" bit to message  
    //append "0" bits until message length in bits is 448 (mod 512)
    //append length mod (2^64) to message
 

	new_len = initial_len + 1;

	int mod = new_len & 0x3F;

	if(mod > 56) new_len += 120 - mod;
	else if(mod < 56) new_len += 56 - mod;

 
	// allocate a msg with new length
    msg = (uint8_t*)malloc(new_len + 8);
	// copy the original msg to the new one
    memcpy(msg, initial_msg, initial_len);
	// append "1" bit. Note that for a computer, 8bit is the minimum length of a datatype
    msg[initial_len] = 0x80; 

	if(initial_len + 1 < new_len)
		memset(&msg[initial_len + 1], 0, sizeof(uint8_t) * (new_len - 1 - initial_len));

    for (offset = initial_len + 1; offset < new_len; ++offset)
        msg[offset] = 0; // append "0" bits
 
    // append the lower 32 bits of len in bits at the end of the buffer.
    to_bytes(initial_len<<3, msg + new_len);
    // append the higher 32 bits of len in bits at the end of the buffer.
    to_bytes(initial_len >> 29, msg + new_len + 4);
 
    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    for(offset=0; offset<new_len; offset += 64) {
 
        // break chunk into sixteen 32-bit words w[j], from 0 to 15
        for (i = 0; i < 16; ++i)
            w[i] = *((uint32_t*)(msg + offset + (i<<2)));
 
        // Initialize hash value for this chunk:
        a = h0;
        b = h1;
        c = h2;
        d = h3;
 
		FF(a, b, c, d, w[ 0],  7, 0xD76AA478);
		FF(d, a, b, c, w[ 1], 12, 0xE8C7B756);
		FF(c, d, a, b, w[ 2], 17, 0x242070DB);
		FF(b, c, d, a, w[ 3], 22, 0xC1BDCEEE);
		FF(a, b, c, d, w[ 4],  7, 0xF57C0FAF);
		FF(d, a, b, c, w[ 5], 12, 0x4787C62A);
		FF(c, d, a, b, w[ 6], 17, 0xA8304613);
		FF(b, c, d, a, w[ 7], 22, 0xFD469501);
		FF(a, b, c, d, w[ 8],  7, 0x698098D8);
		FF(d, a, b, c, w[ 9], 12, 0x8B44F7AF);
		FF(c, d, a, b, w[10], 17, 0xFFFF5BB1);
		FF(b, c, d, a, w[11], 22, 0x895CD7BE);
		FF(a, b, c, d, w[12],  7, 0x6B901122);
		FF(d, a, b, c, w[13], 12, 0xFD987193);
		FF(c, d, a, b, w[14], 17, 0xA679438E);
		FF(b, c, d, a, w[15], 22, 0x49B40821);
		GG(a, b, c, d, w[ 1],  5, 0xF61E2562);
		GG(d, a, b, c, w[ 6],  9, 0xC040B340);
		GG(c, d, a, b, w[11], 14, 0x265E5A51);
		GG(b, c, d, a, w[ 0], 20, 0xE9B6C7AA);
		GG(a, b, c, d, w[ 5],  5, 0xD62F105D);
		GG(d, a, b, c, w[10],  9, 0x02441453);
		GG(c, d, a, b, w[15], 14, 0xD8A1E681);
		GG(b, c, d, a, w[ 4], 20, 0xE7D3FBC8);
		GG(a, b, c, d, w[ 9],  5, 0x21E1CDE6);
		GG(d, a, b, c, w[14],  9, 0xC33707D6);
		GG(c, d, a, b, w[ 3], 14, 0xF4D50D87);
		GG(b, c, d, a, w[ 8], 20, 0x455A14ED);
		GG(a, b, c, d, w[13],  5, 0xA9E3E905);
		GG(d, a, b, c, w[ 2],  9, 0xFCEFA3F8);
		GG(c, d, a, b, w[ 7], 14, 0x676F02D9);
		GG(b, c, d, a, w[12], 20, 0x8D2A4C8A);
		HH(a, b, c, d, w[ 5],  4, 0xFFFA3942);
		HH(d, a, b, c, w[ 8], 11, 0x8771F681);
		HH(c, d, a, b, w[11], 16, 0x6D9D6122);
		HH(b, c, d, a, w[14], 23, 0xFDE5380C);
		HH(a, b, c, d, w[ 1],  4, 0xA4BEEA44);
		HH(d, a, b, c, w[ 4], 11, 0x4BDECFA9);
		HH(c, d, a, b, w[ 7], 16, 0xF6BB4B60);
		HH(b, c, d, a, w[10], 23, 0xBEBFBC70);
		HH(a, b, c, d, w[13],  4, 0x289B7EC6);
		HH(d, a, b, c, w[ 0], 11, 0xEAA127FA);
		HH(c, d, a, b, w[ 3], 16, 0xD4EF3085);
		HH(b, c, d, a, w[ 6], 23, 0x04881D05);
		HH(a, b, c, d, w[ 9],  4, 0xD9D4D039);
		HH(d, a, b, c, w[12], 11, 0xE6DB99E5);
		HH(c, d, a, b, w[15], 16, 0x1FA27CF8);
		HH(b, c, d, a, w[ 2], 23, 0xC4AC5665);
		II(a, b, c, d, w[ 0],  6, 0xF4292244);
		II(d, a, b, c, w[ 7], 10, 0x432AFF97);
		II(c, d, a, b, w[14], 15, 0xAB9423A7);
		II(b, c, d, a, w[ 5], 21, 0xFC93A039);
		II(a, b, c, d, w[12],  6, 0x655B59C3);
		II(d, a, b, c, w[ 3], 10, 0x8F0CCC92);
		II(c, d, a, b, w[10], 15, 0xFFEFF47D);
		II(b, c, d, a, w[ 1], 21, 0x85845DD1);
		II(a, b, c, d, w[ 8],  6, 0x6FA87E4F);
		II(d, a, b, c, w[15], 10, 0xFE2CE6E0);
		II(c, d, a, b, w[ 6], 15, 0xA3014314);
		II(b, c, d, a, w[13], 21, 0x4E0811A1);
		II(a, b, c, d, w[ 4],  6, 0xF7537E82);
		II(d, a, b, c, w[11], 10, 0xBD3AF235);
		II(c, d, a, b, w[ 2], 15, 0x2AD7D2BB);
		II(b, c, d, a, w[ 9], 21, 0xEB86D391);
 
        // Add this chunk's hash to result so far:
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
 
    }
 
    // cleanup
    free(msg);
 
    //var char digest[16] := h0 append h1 append h2 append h3 //(Output is in little-endian)
	*((uint32_t*)digest) = h0;
	*((uint32_t*)(digest + 4)) = h1;
	*((uint32_t*)(digest + 8)) = h2;
	*((uint32_t*)(digest + 12)) = h3;
}
 
