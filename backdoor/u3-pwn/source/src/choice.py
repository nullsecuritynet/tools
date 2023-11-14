try:
	import subprocess
	import pexpect
	import time
	import os
	import sys
	import shutil
	import banner
except ImportError:
	pass

definepath = os.getcwd()
sys.path.append('%s/src/' % definepath)


devlist=[]
devices=[1]

banner.print_banner()

for num in devices:
	dev = raw_input("\nEnter the device to change iso image on (example sdb1):")
	devlist.append(str(dev))
	
try:
	banner.print_banner()
	#format and create a new partition on virtual cd partition using u3-tool
	print '\nCreating partition of 8058880 bytes on /dev/%s ' % devlist[0]
	child1 = pexpect.spawn('u3-tool -v  -p 8058880 /dev/' + devlist[0])
	#Accepts warning that the device will be formatted
	#WARNING: Loading a new cd image causes the whole device to be whiped. This INCLUDES the data partition.
	#I repeat: ANY EXCISTING DATA WILL BE LOST!
	#Are you sure you want to continue? [yn]
	child1.sendline('y')
	time.sleep(2)

	#inserts new iso file to virtual rom devices using u3-tool
	banner.print_banner()
	print '\nInserting new iso file to /dev/%s\n' % devlist[0]
	subprocess.Popen('u3-tool -l "/home/zy0d0x/programming/u3-pwn/src/system.iso"  /dev/%s' % (devlist[0]), shell=True).wait()
	banner.print_banner()
	print '\nSuccessfully backed up /dev/%s' % devlist[0]
	time.sleep(2)




except Exception, error:
	print '\n[-]Something went wrong, printing error message..'
	print error
	time.sleep(2)
	sys.exit(0)

