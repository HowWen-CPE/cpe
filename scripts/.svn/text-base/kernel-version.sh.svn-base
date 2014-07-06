#!/bin/sh
#
# Copyright (C), 2014, Sean@Liteon.com 
#
# Prints the kernel version of `kernel-command' in a canonical 4-digit form
# such as `020631' for linux-2.6.31, `030303' for linux-3.3.3, etc.
#
#

if [ "$1" = "-p" ] ; then
	with_patchlevel=1;
	shift;
fi

if [ $# -gt 3 ]; then
  echo "Error: Wrong number of parameters."
  printf "Usage:\n\t$0 <LINUX_VERSION> <LINUX_PATCHLEVEL> <LINUX_SUBLEVEL>"
  exit 1
fi

if [ "x$with_patchlevel" != "x" ] ; then
	printf "%02d%02d%02d\\n" $1 $2 $3
else
	printf "%02d%02d%02d\\n" $1 $2 $3
fi
