##############################################################################
#  						   CONFIDENTIAL								
#        Hangzhou GuoXin Science and Technology Co., Ltd.             
#                   (C)2007-2008, All right reserved
##############################################################################
# File Name : .gdbinit
# Author    : LINJJ
# Project   : API
# Type      : GDB Debug Script
##############################################################################
# Purpose   : Invoking gdb will then start the simulator and load ELF into it. 
# 			  You should start gdb using a command like `ckcore-elf-gdb *.elf'.
#             This Script file is used for Ckcore.
##############################################################################
# Release History:
# VERSION      Date          AUTHOR         Description
#   0.1      2008.04.08    	  Linjj          Creation
# 
##############################################################################
# Notation:
#       
##############################################################################

target remote 192.168.100.100:2331

################################################
# SET CHIP TYPE CONFIG
################################################
set $GX3110_MPW_DOT13_BOARD             =   0
set $GX3110_MPW_DOT12_BOARD             =   0
set $GX3110_NRE_DOT12_BOARD             =   1

################################################
# SET PLL CONFIG
################################################
# PLL MULTIPLE CONFIG
set $PLLDTO_CLOCK_FREQ_390MHZ           =   0
set $PLLDTO_CLOCK_FREQ_396MHZ           =   1
set $PLLDTO_CLOCK_FREQ_402MHZ           =   0
set $PLLDTO_CLOCK_FREQ_408MHZ           =   0

# XTAL CHOISE 
set $XTAL_CLOCK_FREQ_4MHZ               =   0
set $XTAL_CLOCK_FREQ_8MHZ               =   0
set $XTAL_CLOCK_FREQ_13DOT5MHZ          =   0
set $XTAL_CLOCK_FREQ_16MHZ              =   0
set $XTAL_CLOCK_FREQ_27MHZ              =   1

# CPU CLOCK CHOISE
set $CPU_CLOCK_FREQ_108MHZ              =   0
set $CPU_CLOCK_FREQ_120MHZ              =   0
set $CPU_CLOCK_FREQ_216MHZ              =   1
set $CPU_CLOCK_FREQ_264MHZ              =   0
set $CPU_CLOCK_FREQ_300MHZ              =   0

# DDR SDRAM CLOCK CHOISE
set $RAM_CLOCK_FREQ_108MHZ              =   0
set $RAM_CLOCK_FREQ_120MHZ              =   0
set $RAM_CLOCK_FREQ_141MHZ              =   0
set $RAM_CLOCK_FREQ_144MHZ              =   0
set $RAM_CLOCK_FREQ_147MHZ              =   0
set $RAM_CLOCK_FREQ_150MHZ              =   0
set $RAM_CLOCK_FREQ_153MHZ              =   0
set $RAM_CLOCK_FREQ_156MHZ              =   0
set $RAM_CLOCK_FREQ_159MHZ              =   0
set $RAM_CLOCK_FREQ_162MHZ              =   1
set $RAM_CLOCK_FREQ_165MHZ              =   0
set $RAM_CLOCK_FREQ_168MHZ              =   0
set $RAM_CLOCK_FREQ_171MHZ              =   0
set $RAM_CLOCK_FREQ_174MHZ              =   0
set $RAM_CLOCK_FREQ_177MHZ              =   0

# DVE CLOCK CHOISE 
set $VPU_CLOCK_FREQ_27MHZ               =   1
set $VPU_CLOCK_FREQ_108MHZ              =   0

################################################
# SDRAM CONFIG (INCLUDE SIZE / PHRASE.)
################################################
# TYPE CHOISE
set $CHOISE_CONFIG_SDR    		        = 	0
set $CHOISE_CONFIG_DDR    		        = 	1

# SDR / DDR HAD TWO CHIP
set $DRAM_HAD_TWO                       =   0

# SDR /DDR SIZE CONFIG 
set $DRAM_16M_BIT   				    = 	0
set $DRAM_64M_BIT   				    = 	0
set $DRAM_128M_BIT  				    = 	0
set $DRAM_256M_BIT  				    = 	0
set $DRAM_512M_BIT  				    = 	1
set $DRAM_1024M_BIT  				    = 	0

################################################
set $CLKEN_USB_OPEN                     =   1
set $CLKEN_NET_OPEN                     =   1

################################################

source gdbconfig

jtagstart


