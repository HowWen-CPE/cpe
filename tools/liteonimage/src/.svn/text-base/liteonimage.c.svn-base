/*
 * (C) Copyright 2000-2004
 * DENX Software Engineering
 * Wolfgang Denk, wd@denx.de
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __WIN32__
#include <netinet/in.h>		/* for host / network byte order conversions	*/
#endif
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


#if defined(__BEOS__) || defined(__NetBSD__) || defined(__APPLE__)
#include <inttypes.h>
#endif

#ifdef __WIN32__
typedef unsigned int __u32;

#define SWAP_LONG(x) \
	((__u32)( \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24) ))
typedef		unsigned char	uint8_t;
typedef		unsigned short	uint16_t;
typedef		unsigned int	uint32_t;

#define     ntohl(a)	SWAP_LONG(a)
#define     htonl(a)	SWAP_LONG(a)
#endif	/* __WIN32__ */

#ifndef	O_BINARY		/* should be define'd on __WIN32__ */
#define O_BINARY	0
#endif

#include "image.h"

extern int errno;

#ifndef MAP_FAILED
#define MAP_FAILED (-1)
#endif

char *cmdname;


static	void	copy_file (int, const char *, int);
static  void    write_padding(int ifd, int pad_len, char pad_val);
static	void	usage	(void);
static	void	print_header (prefix_image_header_t *);

char	*datafile;
char	*imagefile;

int dflag    = 0;
int eflag    = 0;
int lflag    = 0;
int vflag    = 0;
int xflag    = 0;
int fflag    = 0;
int pflag    = 0;


prefix_image_header_t header;
prefix_image_header_t *hdr = &header;

/*******************************************
main function 	use the FW produced by mkimage to 
generate a new FW coverd with a new header
prefix_image_header, to verify the original FW's validity.
you do not have to anything cause everything was done 
in the Makefile.
*********************************************/
int main (int argc, char **argv)
{
	int ifd;
	uint32_t checksum;
	struct stat sbuf;
	unsigned char *ptr;
	char *name = "";
	char cat = '\0';
	char subcat = '\0';
	char major = '\0';
	char minor = '\0';
	char subminor = '\0';
	int fs_padding = 0;
	int kn_offset = 0;
	int fs_offset = 0;
	char *fs_name = "";
    char *tmp;

	cmdname = *argv;


	while (--argc > 0 && **++argv == '-') {
		while (*++*argv) {
			switch (**argv) {
			case 'l': //list image header information
				lflag = 1;
				break;
			case 'd': //use image data from 'datafile'\n"
				if (--argc <= 0)
					usage ();
				datafile = *++argv;
				dflag = 1;
				goto NXTARG;
			case 'v': // ????
				vflag++;
				break;
			case 'x': //set XIP (execute in place)\n"
				xflag++;
				break;
			case 'p': //????
				pflag++;
				break;
            case 'k': // set MTD start addr for kernel to 'kn_offset'
                if (--argc <= 0) {
                    usage ();
                }
                kn_offset = strtoul(*++argv, (char **)&ptr, 16);
                if (*ptr) {
                    fprintf (stderr,
                        "%s: invalid entry point %s\n",
                        cmdname, *argv);
                    exit (EXIT_FAILURE);
                }
				goto NXTARG;
            case 'c': //set category of the product to 'cat'\n"
                if (--argc <= 0)
                    usage ();
                cat = **++argv;
				goto NXTARG;
            case 's': //set subcategory of the product to 'subcat'\n"
                if (--argc <= 0)
                    usage ();
                subcat = **++argv;
				goto NXTARG;
            case 'w': //set major of the product to 'major'\n"
                if (--argc <= 0)
                    usage ();
                major = (char) atoi(*++argv);
				goto NXTARG;
            case 'y': // set minor of the product to 'minor'\n"
                if (--argc <= 0)
                    usage ();
                minor = (char) atoi(*++argv);
				goto NXTARG;
            case 'z': //set subminor of the product to 'subminor'\n"
                if (--argc <= 0)
                    usage ();
                subminor = (char) atoi(*++argv);
				goto NXTARG;
            case 'f': //use rootfs data from 'fs_name'\n"
                if (--argc <= 0)
                    usage ();
                fs_name = *++argv;
                fflag++;
				goto NXTARG;
            case 'o': //use MTD start addr for rootfs to 'fs_offset'\n"
                if (--argc <= 0)
                    usage ();
                fs_offset = strtoul(*++argv, (char **)&ptr, 16);
                if (*ptr) {
                    fprintf (stderr,
                        "%s: invalid entry point %s\n",
                        cmdname, *argv);
                    exit (EXIT_FAILURE);
                }
				goto NXTARG;
			default:
				usage ();
			}
		}
NXTARG:		;
	}

	if ((argc != 1) || ((lflag ^ dflag) == 0))
		usage();

    /* Check kn_offset is larger than the header length */
    if (dflag && kn_offset < sizeof(prefix_image_header_t)) {
        kn_offset = sizeof(prefix_image_header_t);
    } 
    
    /**
     * Once fflag is enabled:
     * 1. ensure fs_offset and kn_offset are given.
     * 2. ensure the value of fs_offset larger than the kernel length plus the
     *    header length.
     */
    if (fflag) {
        if (!fs_offset) {
		    usage();
            exit(EXIT_FAILURE);
        }

        if (stat(datafile, &sbuf) < 0) {
            fprintf (stderr, "%s: Can't stat %s: %s\n",
                cmdname, datafile, strerror(errno));
            exit (EXIT_FAILURE);
        }

        if (sbuf.st_size + sizeof(prefix_image_header_t) > fs_offset) {
            fprintf(stderr, 
                    "%s: The kernel will exceed to the rootfs's partition\n",
                    cmdname);
            exit (EXIT_FAILURE);
        }

        fs_padding = fs_offset - (sbuf.st_size + sizeof(prefix_image_header_t));
    }


	imagefile = *argv;

	if (lflag) {
		ifd = open(imagefile, O_RDONLY|O_BINARY);
	} else {
		ifd = open(imagefile, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0666);
	}

	if (ifd < 0) {
		fprintf (stderr, "%s: Can't open %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (lflag) {
		int len;
		char *data;
		/*
		 * list header information of existing image
		 */
		if (fstat(ifd, &sbuf) < 0) {
			fprintf (stderr, "%s: Can't stat %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}

		if ((unsigned)sbuf.st_size < sizeof(prefix_image_header_t)) {
			fprintf (stderr,
				"%s: Bad size: \"%s\" is no valid image\n",
				cmdname, imagefile);
			exit (EXIT_FAILURE);
		}

		ptr = (unsigned char *)mmap(0, sbuf.st_size,
					    PROT_READ, MAP_SHARED, ifd, 0);
		if ((caddr_t)ptr == (caddr_t)-1) {
			fprintf (stderr, "%s: Can't read %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}

		/*
		 * create copy of header so that we can blank out the
		 * checksum field for checking - this can't be done
		 * on the PROT_READ mapped data.
		 */
		memcpy (hdr, ptr, sizeof(prefix_image_header_t));
#if 0
		if (ntohl(hdr->pih_magic) != PIH_MAGIC) {
			fprintf (stderr,
				"%s: Bad Magic Number: \"%s\" is no valid image\n",
				cmdname, imagefile);
			exit (EXIT_FAILURE);
		}
#endif
		data = (char *)hdr;
		len  = sizeof(prefix_image_header_t);

		checksum = ntohl(hdr->pih_hcrc);
		hdr->pih_hcrc = htonl(0);	/* clear for re-calculation */

		if (crc32(0, data, len) != checksum) {
			fprintf (stderr,
				"*** Warning: \"%s\" has bad header checksum!\n",
				imagefile);
		}

		data = (char *)(ptr + sizeof(prefix_image_header_t));
		len  = sbuf.st_size - sizeof(prefix_image_header_t) ;

		if (crc32(0, data, len) != ntohl(hdr->pih_dcrc)) {
			fprintf (stderr,
				"*** Warning: \"%s\" has corrupted data!\n",
				imagefile);
		}

		/* for multi-file images we need the data part, too */
		print_header ((prefix_image_header_t *)ptr);

		(void) munmap((void *)ptr, sbuf.st_size);
		(void) close (ifd);

		exit (EXIT_SUCCESS);
	}

	/*
	 * Must be -w then: set major of the product to 'major'\n"
	 *
	 * write dummy header, to be fixed later
	 */
	memset (hdr, 0, sizeof(prefix_image_header_t));

	if (write(ifd, hdr, sizeof(prefix_image_header_t)) != sizeof(prefix_image_header_t)) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}


        /* EZP: add a given length of padding bytes before the kernel. */
        write_padding(ifd, kn_offset - sizeof(prefix_image_header_t), '\0');
		copy_file(ifd, datafile, 0);
	

	/* We're a bit of paranoid */
#if defined(_POSIX_SYNCHRONIZED_IO) && !defined(__sun__) && !defined(__FreeBSD__)
	(void) fdatasync (ifd);
#else
	(void) fsync (ifd);
#endif

	if (fstat(ifd, &sbuf) < 0) {
		fprintf (stderr, "%s: Can't stat %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	ptr = (unsigned char *)mmap(0, sbuf.st_size,
				    PROT_READ|PROT_WRITE, MAP_SHARED, ifd, 0);
	if (ptr == (unsigned char *)MAP_FAILED) {
		fprintf (stderr, "%s: Can't map %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	hdr = (prefix_image_header_t *)ptr;

	checksum = crc32(0,
			  (const char *)(ptr + sizeof(prefix_image_header_t)),
			  sbuf.st_size - sizeof(prefix_image_header_t)
			 );

	/* Build new header */
#ifdef PIH_MAGIC
	hdr->pih_magic = htonl(PIH_MAGIC);

#else
	hdr->pih_magic = htonl(0x11111111);
	
#endif
	hdr->pih_size  = htonl(sbuf.st_size - sizeof(prefix_image_header_t));

	hdr->pih_dcrc  = htonl(checksum);


	checksum = crc32(0,(const char *)hdr,sizeof(prefix_image_header_t));

	hdr->pih_hcrc = htonl(checksum);

	print_header (hdr);

    if (fflag) {
        int len;
        /* EZP: append rootfs with the given padding if it needs. */
        len = lseek(ifd, 0L, SEEK_SET);
        len = lseek(ifd, sbuf.st_size, SEEK_SET);
        write_padding(ifd, fs_padding, '\0');
        copy_file(ifd, fs_name, 0);
    }

	(void) munmap((void *)ptr, sbuf.st_size);

	/* We're a bit of paranoid */
#if defined(_POSIX_SYNCHRONIZED_IO) && !defined(__sun__) && !defined(__FreeBSD__)
	(void) fdatasync (ifd);
#else
	(void) fsync (ifd);
#endif

	if (close(ifd)) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}
 #if  0     /* EZP: calculation all image file(.bix) crc32 checksum */
    if (pflag) {
        int len;
        char *data;
        ifd = open(imagefile, O_RDWR|O_BINARY, 0666);
        if (ifd < 0) {
            fprintf (stderr, "open image file %s err \n", imagefile);
        }
        printf("ifd: %d\n", ifd);
        memset(&sbuf, 0, sizeof(sbuf));
        if (fstat(ifd, &sbuf) < 0) {
            fprintf (stderr, "%s: Can't stat %s: %s\n",
                    cmdname, imagefile, strerror(errno));
            exit (EXIT_FAILURE);
        }
       // printf("len: %d\n", sbuf.st_size);
        ptr = (unsigned char *)mmap(0, sbuf.st_size,
                PROT_READ|PROT_WRITE, MAP_SHARED, ifd, 0);
        if (ptr == (unsigned char *)MAP_FAILED) {
            fprintf (stderr, "%s: Can't map %s: %s file size:%d\n",
                    cmdname, imagefile, strerror(errno), (int)sbuf.st_size);
            exit (EXIT_FAILURE);
        }
        data = ptr + sizeof(prefix_image_header_t);
        len  = sbuf.st_size - sizeof(prefix_image_header_t);
        checksum = crc32(0, data, len);
        hdr = (prefix_image_header_t *)ptr;

        hdr->pih_magic = htonl(PIH_MAGIC);
//        hdr->ih_time  = htonl(sbuf.st_mtime);
//        tmp = (char *)&hdr->ih_time;
 //       tmp[0] = cat;
 //       tmp[1] = subcat;
  //      tmp[2] = (major << 4) | (minor & 0xF);
   //     tmp[3] = (subminor << 4);
        hdr->pih_size  = htonl(sbuf.st_size - sizeof(prefix_image_header_t));
   //     hdr->pih_load  = htonl(addr);
   //     hdr->pih_ep    = htonl(ep);
        hdr->pih_dcrc  = htonl(checksum);
 //       hdr->ih_os    = opt_os;
 //       hdr->ih_arch  = opt_arch;
 //       hdr->ih_type  = opt_type;
 //       hdr->ih_comp  = opt_comp;

        hdr->pih_hcrc = htonl(0);
 //       strncpy((char *)hdr->ih_name, name, IH_NMLEN);

        checksum = crc32(0,(const char *)hdr,sizeof(prefix_image_header_t));

        hdr->pih_hcrc = htonl(checksum);

        print_header (hdr);

        munmap((void *)ptr, sbuf.st_size);

        if (close(ifd)) {
            fprintf (stderr, "%s: Write error on %s: %s\n",
                    cmdname, imagefile, strerror(errno));
            exit (EXIT_FAILURE);
        }
    } /* EZP: */
#endif
	exit (EXIT_SUCCESS);
}

static void
write_padding(int ifd, int pad_len, char pad_val)
{
    int i;
    for (i = 0; i < pad_len; i++) {
        write(ifd, &pad_val, 1);
    }
}

static void
copy_file (int ifd, const char *datafile, int pad)
{
	int dfd;
	struct stat sbuf;
	unsigned char *ptr;
	int tail;
	int zero = 0;
	int offset = 0;
	int size;

	if (vflag) {
		fprintf (stderr, "Adding Image %s\n", datafile);
	}

	if ((dfd = open(datafile, O_RDONLY|O_BINARY)) < 0) {
		fprintf (stderr, "%s: Can't open %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (fstat(dfd, &sbuf) < 0) {
		fprintf (stderr, "%s: Can't stat %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	ptr = (unsigned char *)mmap(0, sbuf.st_size,
				    PROT_READ, MAP_SHARED, dfd, 0);
	if (ptr == (unsigned char *)MAP_FAILED) {
		fprintf (stderr, "%s: Can't read %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (xflag) {
		unsigned char *p = NULL;
		/*
		 * XIP: do not append the image_header_t at the
		 * beginning of the file, but consume the space
		 * reserved for it.
		 */

		if ((unsigned)sbuf.st_size < sizeof(prefix_image_header_t)) {
			fprintf (stderr,
				"%s: Bad size: \"%s\" is too small for XIP\n",
				cmdname, datafile);
			exit (EXIT_FAILURE);
		}

		for (p=ptr; p < ptr+sizeof(prefix_image_header_t); p++) {
			if ( *p != 0xff ) {
				fprintf (stderr,
					"%s: Bad file: \"%s\" has invalid buffer for XIP\n",
					cmdname, datafile);
				exit (EXIT_FAILURE);
			}
		}

		offset = sizeof(prefix_image_header_t);
	}

	size = sbuf.st_size - offset;
	if (write(ifd, ptr + offset, size) != size) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (pad && ((tail = size % 4) != 0)) {

		if (write(ifd, (char *)&zero, 4-tail) != 4-tail) {
			fprintf (stderr, "%s: Write error on %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}
	}

	(void) munmap((void *)ptr, sbuf.st_size);
	(void) close (dfd);
}

void
usage ()
{
	fprintf (stderr, "Usage: %s -l image\n"
			 "          -l ==> list image header information\n"
			 "       %s -d data_file[:data_file...] image\n",
		cmdname, cmdname);
	fprintf (stderr, 
			 "          -d ==> use image data from 'datafile'\n"			 
			 "          -k ==> set MTD start addr for kernel to 'kn_offset'\n"            
             "          -f ==> use rootfs data from 'fs_name'\n"
             "          -o ==> use MTD start addr for rootfs to 'fs_offset'\n"
		);
	exit (EXIT_FAILURE);
}

static void
print_header (prefix_image_header_t *hdr)
{

	uint32_t size;
    char *tmp;

	printf ("Data Size:    %d Bytes = %.2f kB = %.2f MB\n",
		size, (double)size / 1.024e3, (double)size / 1.048576e6 );

		int i, ptrs;
		uint32_t pos;
		unsigned long *len_ptr = (unsigned long *) (
					(unsigned long)hdr + sizeof(prefix_image_header_t)
				);

		/* determine number of images first (to calculate image offsets) */
		for (i=0; len_ptr[i]; ++i)	/* null pointer terminates list */
			;
		ptrs = i;		/* null pointer terminates list */

		pos = sizeof(prefix_image_header_t) + ptrs * sizeof(long);
		printf ("Contents:\n");
		for (i=0; len_ptr[i]; ++i) {
			size = ntohl(len_ptr[i]);

			printf ("   Image %d: %8d Bytes = %4d kB = %d MB\n",
				i, size, size>>10, size>>20);

			/* copy_file() will pad the first files to even word align */
			size += 3;
			size &= ~3;
			pos += size;
		}
	
}

