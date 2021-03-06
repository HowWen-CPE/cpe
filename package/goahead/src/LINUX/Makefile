#
# Makefile for the GoAhead web server reference source base
#  for the Linux OS
#
# Copyright (c) GoAhead Software, Inc. 1995-2000
#
# $Id: Makefile,v 1.2 2002/11/01 16:20:19 mmitchell Exp $
#

all:	compile 

ARCH	= libwebs.a
NAME	= goahead

# User Management switch
UMSW	= -DUSER_MANAGEMENT_SUPPORT

# Digest Access switch
DASW	= -DDIGEST_ACCESS_SUPPORT

#
#	SSL switches
#
# You must uncomment this line to enable SSL
# SSLPATCHFILE = ../websSSL.o
#
# Then uncomment these three lines if you are using the RSAREF library
# and set them to the correct path.
#
# SSLINC = /RSASSL/library/include
# SSLLIB = /RSASSL/library/lib/libsslc.a
# SSLSW	= -DWEBS_SSL_SUPPORT -I$(SSLINC)
#
# Or alternatively,
#
# Uncomment these three lines if you are using the OpenSSL library.
# Change the value of SSLINC to the path where your OpenSSL header
# files reside, and change the value of SSLLIB to the path to your
# OpenSSL libssl.a and libcrypto.a.
#
# SSLINC = /usr/include/openssl
# SSLLIB = /usr/lib/libssl.a /usr/lib/libcrypto.a
# SSLSW	= -DWEBS_SSL_SUPPORT -I$(SSLINC) -DOPENSSL


# If-Modified-Support switches (requires math library, libm.a)
# IFMODSW = -DWEBS_IF_MODIFIED_SUPPORT
# IFMODLIB = /usr/lib/libm.a

#Alvin add
SSLPATCHFILE =../websSSL.o
SSLINC =../../openssl-1.0.1e/include #openssl header files
#openssl static library files
SSLLIB =../../openssl-1.0.1e/libssl.a ../../openssl-1.0.1e/libcrypto.a
#add openssl macro compile and designate openssl header files path
CFLAGS = -DWEBS -DUEMF -DOS="LINUX" -DLINUX -DWEBS_SSL_SUPPORT -DOPENSSL -I$(SSLINC)
#add end

# Dependencies
DEPEND_FILES	= ../md5_dgst.o ../asp.o ../balloc.o ../base64.o ../cgi.o ../default.o  \
				  ../ejlex.o ../ejparse.o ../form.o \
				  ../h.o ../handler.o ../mime.o ../misc.o ../page.o  \
				  ../ringq.o ../rom.o \
				  ../sock.o ../sockGen.o $(SSLPATCHFILE) \
				  ../security.o ../sym.o ../uemf.o ../url.o ../value.o \
				  ../md5c.o ../um.o ../umui.o ../websda.o ../emfdb.o \
				  ../webrom.o ../webs.o ../websuemf.o ../wireless.o ../wps.o\
				  ../management.o ../utils.o ../qos.o ../firewall.o ../internet.o main.o
#CFLAGS	= -DWEBS -DUEMF -DOS="LINUX" -DLINUX $(UMSW) $(DASW) $(SSLSW) $(IFMODSW)
OTHERS	= -DB_STATS -DB_FILL -DDEBUG
DEBUG	= -g -Wall
IFLAGS 	= -I.. 
LDFLAGS	= $(SSLLIB) $(IFMODLIB)
LDFLAGS	+= -L../../ethread-1.0 -lethread 
LDFLAGS	+= -L../../nvram -lnvram 
LDFLAGS += -L../../wlan-middleware -lwlan-middleware
#LDFLAGS	+= -L../ezp-nvram

compile: $(NAME)

#
#	Build archive of objects
#
$(ARCH): $(DEPEND_FILES)
	$(AR) $(ARFLAGS) $(ARCH) $?

#
#	Primary link
#
$(NAME): Makefile $(ARCH)
	$(CC) -o $(NAME) $(DEBUG) \
		$(CFLAGS) $(IFLAGS) \
		main.o $(ARCH) $(LDFLAGS) -ldl

clean:
	rm -f $(NAME) $(ARCH) $(DEPEND_FILES)

#
#	Dependencies
#
../md5_dgst.o: ../md5_md5.h ../md5_locl.h

../asp.o:  ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../balloc.o: ../balloc.c ../uemf.h

../base64.o:  ../base64.c ../webs.h ../wsIntrn.h  ../ej.h ../ejIntrn.h ../uemf.h

../cgi.o:  ../webs.h ../wsIntrn.h ../uemf.h

../default.o:  ../default.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../ejlex.o:  ../ejlex.c ../ej.h ../ejIntrn.h ../uemf.h

../ejparse.o:  ../ejparse.c ../ej.h ../ejIntrn.h ../uemf.h

../emfdb.o:  ../emfdb.h ../wsIntrn.h ../uemf.h

../form.o:  ../form.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../h.o:  ../h.c ../uemf.h

../handler.o:  ../handler.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../md5c.o:  ../md5.h ../wsIntrn.h ../uemf.h

../mime.o:  ../mime.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../misc.o:  ../misc.c ../uemf.h

../page.o:  ../page.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../ringq.o:  ../ringq.c ../uemf.h

../rom.o:  ../rom.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../security.o:  ../security.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../sock.o:  ../sock.c ../uemf.h

../sockGen.o:  ../sockGen.c ../uemf.h

../sym.o:  ../sym.c ../uemf.h

../uemf.o:  ../uemf.c ../uemf.h

../um.o:  ../webs.h ../wsIntrn.h ../um.h ../uemf.h

../umui.o:  ../webs.h ../wsIntrn.h ../um.h ../uemf.h

../url.o:  ../url.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../value.o:  ../value.c ../uemf.h

../webrom.o:  ../webrom.c ../webs.h ../wsIntrn.h ../uemf.h

../webs.o:  ../webs.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../websda.o:  ../webs.h ../wsIntrn.h ../websda.h ../uemf.h

../websuemf.o:  ../websuemf.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../websSSL.o:  ../websSSL.c ../websSSL.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../management.o:  ../management.c ../management.h ../webs.h

../utils.o: ../utils.c ../utils.h ../webs.h

../qos.o: ../qos.h ../utils.h

../firewall.o: ../firewall.c ../webs.h ../firewall.h

../wireless.o: ../wireless.c ../wireless.h ../internet.h ../utils.h ../webs.h

../wps.o: ../wps.c ../wps.h ../utils.h ../webs.h ../internet.h ../wireless.h

../internet.o: ../internet.c ../internet.h ../utils.h ../webs.h

main.o:  main.c ../wsIntrn.h ../webs.h ../ej.h ../ejIntrn.h ../uemf.h


#
#	Transition rules (add -o to put object in right directory)
#
.c.o:
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
