#
# Program Name
#

PROG = sssetchannel


#SSSETCHANNEL_PATH=$(LITEON_APP_PATH)/misc/sssetchannel


#include $(LITEON_APP_PATH)/rules.gcc


#
# Xtra Options Required ( Compiler / Linker )
#

ifeq ($(DEBUG),y)
CFLAGS += -g
else
CFLAGS += -O2
endif

#CFLAGS += -DEBUG

#CFLAGS += -DLINUX_SUPPORT -DHTTP_SUPPORT -DREADLINE_LIBRARY

#CFLAGS += -DIPC_UDP_SUPPORT


#
# Xtra Source Diretories
#


#
# Xtra Include Paths
#


#CFLAGS += -I $(sssetchannel)
#CFLAGS += -I /usr/local/bin


#LDFLAGS += -lcrypto -L$(LIBRARY_OPENSSL)
#LDFLAGS += -lcrypto
OBJDIR=obj

ALLOBJS	=	sssetchannel.o 
		

OBJS = $(ALLOBJS:%=$(OBJDIR)/%)			


$(PROG): objdir $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROG) -lm -static
	

clean:
	$(RM) $(PROG) *.o $(OBJS) *~
	$(RM) -rf $(OBJDIR)
	
install:
	$(call install_program,$(SSSETCHANNEL_PATH)/$(PROG),$(APP_INSTALL_ROOT))
	


$(OBJDIR)/%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: objdir
objdir:
	mkdir -p $(OBJDIR)