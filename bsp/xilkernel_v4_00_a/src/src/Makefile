############################################################################## 
#
# Copyright (c) 2004 Xilinx, Inc.  All rights reserved.
#
# Xilinx, Inc.
# XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
# COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
# ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
# STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
# IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
# FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.  
# XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
# THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
# ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
# FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
# AND FITNESS FOR A PARTICULAR PURPOSE.
# 
# File   : Makefile
# Company: Xilinx
#
# Makefile for lib_src direcotry
#
# $Id: Makefile,v 1.1.4.1 2007/08/20 06:08:21 tshui Exp $
#
##############################################################################
SUBDIRS = dir_arch dir_sys dir_ipc 

all:  $(SUBDIRS)


dir_arch:
	(cd arch/$(ARCH); $(MAKE) CFLAGS="$(CFLAGS)" CC="$(CC)" AR="$(AR)")

dir_sys:
	(cd sys; $(MAKE) CFLAGS="$(CFLAGS)" CC="$(CC)" AR="$(AR)")

dir_ipc:
	(cd ipc; $(MAKE) CFLAGS="$(CFLAGS)" CC="$(CC)" AR="$(AR)")

clean:
	(cd sys; $(MAKE) clean )
	(cd ipc; $(MAKE) clean )
	(cd arch/$(ARCH); $(MAKE) clean )

