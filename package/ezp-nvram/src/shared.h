#ifndef __SHARED_H__
#define __SHARED_H__

#include <netinet/in.h>
#include <stdint.h>

#define Y2K			946684800UL		// seconds since 1970

#define ASIZE(array)	(sizeof(array) / sizeof(array[0]))

// process.c
extern char *psname(int pid, char *buffer, int maxlen);
extern int pidof(const char *name);
extern int killall(const char *name, int sig);

// files.c
#define FW_CREATE	0
#define FW_APPEND	1
#define FW_NEWLINE	2

extern unsigned long f_size(const char *path);
extern int f_exists(const char *file);
extern int f_read(const char *file, void *buffer, int max);												// returns bytes read
extern int f_write(const char *file, const void *buffer, int len, unsigned flags, unsigned cmode);		//
extern int f_read_string(const char *file, char *buffer, int max);										// returns bytes read, not including term; max includes term
extern int f_write_string(const char *file, const char *buffer, unsigned flags, unsigned cmode);		//
extern int f_read_alloc(const char *path, char **buffer, int max);
extern int f_read_alloc_string(const char *path, char **buffer, int max);


// base64.c
extern int base64_encode(unsigned char *in, char *out, int inlen);			// returns amount of out buffer used
extern int base64_decode(const char *in, unsigned char *out, int inlen);	// returns amount of out buffer used
extern int base64_encoded_len(int len);
extern int base64_decoded_len(int len);										// maximum possible, not actual


// strings.c
extern int find_word(const char *buffer, const char *word);

// crc32.o
extern unsigned long crc32(unsigned long, const unsigned char *, unsigned int);

#endif
