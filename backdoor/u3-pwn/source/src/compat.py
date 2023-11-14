#!/usr/bin/env python
################################################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# U3-Pwn                                                                       #
#                                                                              #
# DATE                                                                         #
# 10/05/2013                                                                   #
#                                                                              #
# DESCRIPTION                                                                  #
# U3-Pwn is a tool designed to automate injecting executables to Sandisk       #
# smart usb devices with default U3 software install. This is performed by     #
# removing the original iso file from the device and creating a new iso        #
# with autorun features.                                                       #
#                                                                              #
# REQUREMENTS                                                                  #
# - Metasploit                                                                 #
# - U3-Tool                                                                    #
# - Python-2.7                                                                 #
#                                                                              #
# AUTHOR                                                                       #
# Zy0d0x - http://www.nullsecurity.net/                                        #
#                                                                              #
################################################################################
try:
    import os
    import sys
    import banner
except ImportError, error:
    print'\n[-]Failed To Import Module\n'
    print error

definepath = os.getcwd()
sys.path.append('%s/src/' % definepath)
banner.print_banner()


print ' Compatibility List Of SanDisk U3 Devices'

print '''

 VendorID  |  ProductID  |  Device Name				
       	   |		 |					
  0x0781   |   0x5406    |   Sandisk Cruzer Micro		
  0x0781   |   0x5408    |   Sandisk Cruzer Titanium		
  0x0781   |   0x550a    |   Sandisk Cruzer Pattern		
  0x0781   |   0x5151    |   Sandisk Cruzer Micro Skin 8GB	                 
  0x0781   |   0x540e    |   Sandisk Cruzer Contour		
  0x0781   |   0x5530    |   Sandisk Cruzer			
  0x0781   |   0x5535    |   Sandisk Ultra Backup		

'''
raw_input(' Press Enter To Return To Menu..')


