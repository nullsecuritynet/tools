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
    import pexpect
    import time
    import os
    import sys
    import shutil
    import banner
except ImportError, error:
    print'\n[-]Failed To Import Module\n'
    print error

definepath = os.getcwd()
sys.path.append('%s/src/' % definepath)

try:
    banner.print_banner()
    print ' Detecting Device...\n'
    u3 = ['5406', '5408', '5151', '5530', '5535', '550a', '540e']
    vendorid = '0781:'
    for id in u3:
        try:
             device = subprocess.check_output("lsusb -d %s%s" %(vendorid,id),  shell=True)
             print ' Device Found:', device
        except Exception, error:
            pass


    time.sleep(2)
    banner.print_banner()
    print ' Detecting Partitions...'
    time.sleep(2)
    partitionsFile = open("/proc/partitions")
    lines = partitionsFile.readlines()[2:]

    for line in lines:
        words = [x.strip() for x in line.split()]
        minorNumber = int(words[1])
        deviceName = words[3]
        if minorNumber % 16 == 0:
                path = "/sys/class/block/" + deviceName
                if os.path.islink(path):
                        if os.path.realpath(path).find("/usb") > 0:
                                if deviceName.startswith("sd"):
                                        print "\n Partition Found: /dev/%s1" % deviceName
                                        time.sleep(2)



except KeyboardInterrupt:
    print'\n[-]Keyboard Interrupted Exiting'
    time.sleep(2)
    sys.exit(0)
except Exception, error:
    print '[-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)



try:
    dev = deviceName
    banner.print_banner()

    if dev.startswith('sda'):
	print 'No Devices Found'
	time.sleep(2)
	
    else:
	  print '\n=================================\n'
          print '\nDevice information for /dev/%s\n' % deviceName
          print '\n=================================\n'
          p = subprocess.Popen('u3-tool -D /dev/%s'  % (deviceName),shell=True).wait()

    raw_input('\nPress Enter To Continue..')

except Exception, error:
    print '[-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)

        







