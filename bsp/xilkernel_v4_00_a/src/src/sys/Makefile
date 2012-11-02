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
# Makefile for sys directory
#
# $Id: Makefile,v 1.1.4.2 2007/09/27 02:34:40 vasanth Exp $
#
##############################################################################

OBJS = init.o alloc.o queue.o process.o sched.o main.o mem.o timer.o pthread.o intr.o elf.o bufmalloc.o syscall.o xtrace.o
INCLUDEDIR = ../../../../../
INCLUDES = -I$(INCLUDEDIR)/include

all: $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)	

%.o:%.S
	$(CC) $(CFLAGS) -c $< -D__ASM__ -o $@ $(INCLUDES)

clean:
	rm -f $(OBJS) *~
