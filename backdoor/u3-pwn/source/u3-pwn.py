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
    import subprocess
    import time
    import os
    import sys
    import pexpect
    import banner
except ImportError:
    pass
definepath = os.getcwd()
sys.path.append('%s/src/' % definepath)
import banner

try:
    while 1 == 1:
        banner.print_banner()
        mainmenu = \
            raw_input('''
 U3-Pwn Main Menu:

 1.  Generate & Replace Iso Image.
 2.  Generate & Replace With Custom Exe.
 3.  Find Out U3 SanDisk Device Information.
 4.  Replace Iso Image With Original U3 Iso.
 5.  SanDisk Usb Compatibility List. 
 6.  About U3-Pwn & Disclaimer.
 7.  Exit U3-Pwn.

 Enter the number: ''').strip()

# Generate Single Iso & Replace Iso on SanDisk Device

        if mainmenu == '1':
            try:
                sys.path.append('%s/src/' % definepath)
                try:
                    reload(generator)
                except Exception:
                    pass
                import generator
            except KeyboardInterrupt:
                print '''[-]Returning to previous menu...'''
                time.sleep(2)

# Generate Single Iso & Replace Iso With Custom Exe on SanDisk Device

        if mainmenu == '2':
            try:
                sys.path.append('%s/src/' % definepath)
                try:
                    reload(customexe)
                except Exception:
                    pass
                import customexe
            except KeyboardInterrupt:
                print '''[-]Returning to previous menu...'''
                time.sleep(2)

# Get SanDisk Device Information

        if mainmenu == '3':
            try:
                sys.path.append('%s/src/' % definepath)
                try:
                    reload(deviceinfo)
                except Exception:
                    pass
                import deviceinfo
            except KeyboardInterrupt:
                print '''[-]Returning to previous menu...'''
                time.sleep(2)

 # Replace Backdoored SanDisk Device With Original Iso File

        if mainmenu == '4':
            try:
                sys.path.append('%s/src/' % definepath)
                try:
                    reload(backup)
                except Exception:
                    pass
                import backup
            except KeyboardInterrupt:
                print '''[-]Returning to previous menu...'''
                time.sleep(2)

# About & Disclaimer

        if mainmenu == '5':
            try:
		sys.path.append('%s/src/' % definepath)
                try:
                    reload(compat)
                except Exception:
                    import compat
            except KeyboardInterrupt:
                print '''[-]Returning to previous menu...'''
                time.sleep(2)

	if mainmenu == '6':
            try:
		sys.path.append('%s/src/' % definepath)
                try:
                    reload(about)
                except Exception:
                    import about
            except KeyboardInterrupt:
                print '''[-]Returning to previous menu...'''
                time.sleep(2)


        if mainmenu == '7':
            sys.exit()
except KeyboardInterrupt:

 # Error handling

    print '''[-]Exiting U3-Pwn...'''
