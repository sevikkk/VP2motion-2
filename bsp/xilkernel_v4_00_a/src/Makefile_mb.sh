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
# Top level Makefile
#
# $Id: Makefile_mb.sh,v 1.1.4.2 2007/10/15 20:12:06 vasanth Exp $
#
##############################################################################

#
# Processor architecture
# microblaze
#
ARCH = microblaze

SYSTEMDIR = ../../..

TOPDIR = .

ARCH_PREFIX = mb

#
# gnu tools for Makefile
#
CC = $(ARCH_PREFIX)-gcc
AR = $(ARCH_PREFIX)-ar
CP = cp

# export ARCH ARCH_PREFIX ARCH_DEFINE CC AR CP

#
# Compiler, linker and other options.
#
CFLAGS = ${COMPILER_FLAGS} -D__XMK__ ${EXTRA_COMPILER_FLAGS}

#
# System project directories.
#

LIBDIR = $(SYSTEMDIR)/lib
INCLUDEDIR = $(SYSTEMDIR)/include

# ELF process's system call wrapper library
LIBSYSCALL = ${TOPDIR}/libsyscall.a

# Kernel library. Contains system call wrappers and the entire kernel
LIBXILKERNEL = ${LIBDIR}/libxil.a
LIBXILKERNEL_OLD = ${TOPDIR}/libxilkernel.a

INCLUDES = -I${TOPDIR}/include -I$(INCLUDEDIR)
LIBS = -L${TOPDIR} -L$(LIBDIR)

# These objects go into libxil.a for linking with kernel bundled threads
KERNEL_AR_OBJS = ${TOPDIR}/src/sys/*.o ${TOPDIR}/src/ipc/*.o ${TOPDIR}/syscall/arch/${ARCH}/*.o ${TOPDIR}/syscall/*.o ${TOPDIR}/syscall/arch/${ARCH}/*.o ${TOPDIR}/syscall/*.o ${TOPDIR}/src/arch/${ARCH}/debugmon.o
KERNEL_AR_OBJS_2 = ${TOPDIR}/src/arch/${ARCH}/*.o 

# These objects go into the ELF process system call wrapper library - libsyscall.a
LIBSYSCALL_OBJS = ${TOPDIR}/syscall/arch/${ARCH}/*.o ${TOPDIR}/syscall/*.o

INCLUDEFILES = ${TOPDIR}/include/* 

STANDALONE_STDIN_SRC = inbyte.c 
STANDALONE_STDOUT_SRC = outbyte.c
STANDALONE_STDIN_OBJ = inbyte.o
STANDALONE_STDOUT_OBJ = outbyte.o 

LIBXIL = libxil.a

#SUBDIRS = dir_src 
SUBDIRS = 

libs: real_libs
	echo "Compiling Xilkernel"

real_libs: dir_syscall dir_src standalone rellibs

dir_src:
	(cd src; $(MAKE) CFLAGS="$(CFLAGS)" ARCH="$(ARCH)" CC="$(CC)" AR="$(AR)" all)

dir_syscall: 
	(cd syscall; $(MAKE) CFLAGS="$(CFLAGS)" ARCH="$(ARCH)" CC="$(CC)" AR="$(AR)" libs)


#
# This target is required because of the way inbyte.c and outbyte.c get generated under xilkernel instead
# of standalone. This is due to the behavior of standard tcl library routines xhandle_stdin and xhandle_stdout
#

standalone:
	(if test -f $(STANDALONE_STDIN_SRC); then \
          $(CC) $(CFLAGS) -c $(INCLUDES) $(STANDALONE_STDIN_SRC); \
	  $(AR) -r $(LIBDIR)/$(LIBXIL) $(STANDALONE_STDIN_OBJ); \
	fi)
	(if test -f $(STANDALONE_STDOUT_SRC); then \
          $(CC) $(CFLAGS) -c $(INCLUDES) $(STANDALONE_STDOUT_SRC); \
	  $(AR) -r $(LIBDIR)/$(LIBXIL) $(STANDALONE_STDOUT_OBJ); \
	fi)

# Does not work on SOL !!
#.PHONY: include

include:  dummy
	$(CP) -rf $(INCLUDEFILES) $(INCLUDEDIR)

dummy:

rellibs: 
	$(AR) -r ${LIBSYSCALL} $(LIBSYSCALL_OBJS)
	$(CP)  $(LIBSYSCALL) $(LIBDIR)
	$(AR) -r ${LIBXILKERNEL} $(KERNEL_AR_OBJS)
	$(AR) -r ${LIBXILKERNEL_OLD} ${KERNEL_AR_OBJS_2}
	$(CP)  $(LIBXILKERNEL_OLD) $(LIBDIR)

clean:
	(cd src; $(MAKE) ARCH="$(ARCH)" clean)
	(cd syscall; $(MAKE) ARCH="$(ARCH)" clean)
	rm -f ${LIBSYSCALL}
