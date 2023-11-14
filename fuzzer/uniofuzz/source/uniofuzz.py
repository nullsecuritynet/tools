#!/usr/bin/env python
# -*- coding: latin-1 -*- # ###################################################
#                                                                             #
#         ~    .__ °.__   0       o                    ^   .__ °__  `´        #
#  °____) __ __|  | | °|   ______°____ 0 ____  __ _________|__|/  |_ ___.__.  #
#  /    \|  | °\  |°|  | °/  ___// __ \_/ ___\|  | °\_  __ \ o\   __<   |  |  #
# | o°|  \  |  /  |_|  |__\___ \\  ___/\ °\___| o|  /|  | \/  ||  |° \___ O|  #
# |___|  /____/|____/____/____ °>\___  >\___  >____/ |__|° |__||__|  / ____|  #
# `´´`´\/´`nullsecurity team`´\/`´´`´\/`´``´\/  ``´```´```´´´´`´``0_o\/´´`´´  #
#                                                                             #
# uniofuzz.py - UniOFuzz                                                      #
#                                                                             #
# DATE                                                                        #
# 01/16/2012                                                                  #
#                                                                             #
# DESCRIPTION                                                                 #
# UniOFuzz - the universal fuzzing tool for browsers, web services, files,    #
# programs and network services/ports                                         #
#                                                                             #
# FOUND BY UNIOFUZZ                                                           #
# http://www.exploit-db.com/exploits/18019                                    #
# http://www.exploit-db.com/exploits/18008                                    #
# http://www.exploit-db.com/exploits/18006                                    #
#                                                                             #
# DEMONSTRATION                                                               #
# http://www.youtube.com/watch?v=oYzc_vEidBQ                                  #
#                                                                             #
# AUTHOR                                                                      #
# pigtail23 - http://www.nullsecurity.net/                                    #
#                                                                             #
###############################################################################


import argparse
import sys
import random
import socket
import time
import os

NAME    = 'UniOFuzz - the universal fuzzing tool for browsers, web services, files , programs and network services/ports.'
VERSION = '0.1.2-beta'
AUTHOR  = ("pigtail23 | http://www.nullsecurity.net/ | pigtail23-(at)-nullsecurity.net")

def parh():
	
	global args

	parser = argparse.ArgumentParser(
	usage='%(prog)s -h/--help',
	formatter_class=argparse.RawDescriptionHelpFormatter,
	description='%s #v%s\nby: %s\n' % (NAME, VERSION, AUTHOR),
	epilog='''
	Examples:
	Browser Fuzzing:
	usage: ./%(prog)s -b template.html -m 1000 -e html/php/aspx -o /var/www [-c A] [-s 10000]

	Web Service Fuzzing:
	usage: ./%(prog)s -w -i 0.01 -ip 127.0.0.1 -port 80 [-c A] [-s 10000]

	File Fuzzing:
	usage: ./%(prog)s -f -m 1000 -e m3u/ini/pdf -o ~/files [-c A] [-s 10000]
	usage: ./%(prog)s -ft template.m3u -m 1000 -e m3u/ini/pdf -o ~/files [-c A] [-s 10000]

	Piping String to Program:
	usage: ./%(prog)s -p -m 1000 -i 1.5 -d '/bin/ls -n' [-c A] [-s 10000]

	Network Service/Port Fuzzing:
	usage: ./%(prog)s -n -i 0.01 -ip 127.0.0.1 -port 25 [-c A] [-s 10000]
	
	''')

	if len(sys.argv)==1:   	
		parser.print_help()
	   	sys.exit(1)

	parser.add_argument('-b', default=True, metavar='templatefile.*', help='browser fuzz')
	parser.add_argument('-f', metavar='', help='filefuzz', const=True , action='store_const')
	parser.add_argument('-ft', default=True, metavar='templatefile.*', help='fuzz files with template file')
	parser.add_argument('-p', metavar='', help='pipes strings to a program', const=True , action='store_const')
	parser.add_argument('-d', default=True, metavar='\'/bin/ls -n\'', help='program to execute')
	parser.add_argument('-w', metavar='', help='web daemon fuzz', const=True , action='store_const')
	parser.add_argument('-n',metavar='', help='network service fuzz', const=True , action='store_const')
	parser.add_argument('-i', default=True, metavar='0.01', help='intervall')
	parser.add_argument('-o', default=True, metavar='/var/www', help='outputfolder')
	parser.add_argument('-s', default=True, metavar='10000', help='max string length (default: 30000)')
	parser.add_argument('-m', default=True, metavar='10000', help='how many files/pipes')
	parser.add_argument('-c', default=True, metavar='A', help='const character (default: random)')
	parser.add_argument('-ip', default=True, metavar='127.0.0.1', help='host ip')
	parser.add_argument('-port', default=True, metavar='80', help='host port')
	parser.add_argument('-e', default=True, metavar='html,m3u,pdf,...', help='file extension')

	args = parser.parse_args()

#random number
def rand():

	if args.s == True:	
		ra = random.randrange(1,30000)
	else:
		ra = random.randrange(1,int(args.s))
	
	return ra

#generates simple random strings
def ran():		

	rstr = ''
	for v in range(0,rand()):
		ra = random.randrange(1,256)
		rstr += "%c"%(ra)
	
	return rstr

#Generate Random Strings
def genstr(i):	

	if args.c != True:
		rd = 4
	else:
		rd = random.randrange(0,i)
	
	#String with a char from ASCII 1-255
	if rd == 0:	
		randomstr = ''
		ra = random.randrange(1,256)		
		for v in range(0,rand()):			
			randomstr += "%c"%(ra)
	
	#String with chars from ASCII 1-255
	elif rd == 1:
		randomstr = ran()

	#String like %%%AAAAAAAAA... or !"Â§!"Â§!"Â§!"Â§...
	elif rd == 2:
		randomstr = ''
		newstr = ''
		ra = random.randrange(0,6)
		for y in range(0,ra):

			rb = random.randrange(0,4)

			if rb == 0:
				newchar = random.randrange(33,48)
			elif rb == 1:
				newchar = random.randrange(58,65)
			elif rb == 2:
				newchar = random.randrange(91,97)
			elif rb == 3:
				newchar = random.randrange(123,127)

			newstr += "%c"%(newchar)
		
		rc = random.randrange(0,2)
		if rc == 0:
			for y in range(0,rand()/2):
				randomstr += newstr
		else:
			randomstr += newstr + 'A' * rand()
	
	#String with chars from ASCII 1-255	(HTML encoded) like &#65;&#122;&#23;...
	elif rd == 3:
		newstr = ''
		randomstr = ''
		ra = random.randrange(0,20)
		for y in range(0,ra):
			rb = random.randrange(1,256)
			newstr += '&#' + str(rb) + ';'

		for y in range(0,rand()/2):
			randomstr += newstr

	#String with const char -c
	elif rd == 4:
		ra = rand()		
		randomstr = args.c * ra

	return randomstr

#File Fuzz -f , -ft *
def filefuzz():

	for y in range(0,int(args.m)):

		filename = 'fuzz' + str(y) + '.' + args.e
		if args.ft != True:
			try:
				content = open(args.ft, 'r').read()
			except:			
				print "Found no " + args.ft
				sys.exit(1)

			ra = random.randrange(0,len(content))
			content2 = content[:ra] + genstr(3) + content[ra:]	
		else:
			content2 = genstr(3)
		
		FILE = open(args.o + '/' + filename,"w")
		FILE.write(content2)
		FILE.close()

		sys.stdout.flush()
		sys.stdout.write(("\rfuzz{0}." + args.e + " generated!").format(y))

#Browser Fuzzing -b *
def brow():

	for y in range(0,int(args.m)):		
		try:
			content = open(args.b, 'r').read()
		except:			
			print "Found no " + args.b
			sys.exit(1)		

		#First File is fuzz0.html	
		filename = 'fuzz' + str(y) + '.' + args.e	
		fix = '<meta http-equiv="refresh" content="0.2; URL=fuzz' + str(y+1) + '.html">\n\n' 
	
		ra = random.randrange(0,len(content))
		content2 = content[:ra] + genstr(4) + content[ra:]
	
		FILE = open(args.o + '/' + filename,"w")
		FILE.write(fix + content2)
		FILE.close()


		sys.stdout.flush()		
		sys.stdout.write(("\rfuzz{0}." + args.e + " generated!").format(y))

#Connect to Target 
def conn(sbuf, ind):

	vuln = (range(30))	
	try:
		sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		connect=sock.connect((args.ip, int(args.port)))
		sock.send(sbuf)
		#print sock.recv(10000)
		sock.close()
		time.sleep(float(args.i))
		vuln[ind] = sbuf
		
	except socket.error:						
		for i in range(0,30):
			FILE = open('crashlogs/crashlog' + str(i) +'.txt',"w") #Create Crashlogs
			FILE.write(str(vuln[i]))
			FILE.close()
		
		print '\n\nHas it crashed? If not, your intervall may be to low! 30 last request saved to crashlogs/\n'
		sys.exit(1)

#TCP Network Service/Port Fuzzing -n
def netf():

	j = 0
	l = 1
	o = 0
	while True:
		if j == 10:
			j = 0
		buffer = genstr(3)

		u = (['\\','|','/','-'])
		sys.stdout.flush()		
		sys.stdout.write("\r[{1}] Sending request: {0}".format(l,u[o]))
		o += 1
		if o == 4:
			o = 0
		l += 1		

		conn(buffer, j)

#Web Daemon Fuzzing -w
def httpf():

	#Config
	getfile = 'index.html'
	#####
	
	l = 1
	j = 0
	o = 0
	while True:
	
		if j == 10:
			j = 0

		#HTTP Methods
		method = (["GET /","POST /","TRACE /","PUT /","OPTION /","HEAD /"])
		
		#random string
		randomstr = genstr(4)	
	
		#HTTP Version
		ver = ([" HTTP/0.9\r\n"," HTTP/1.0\r\n"," HTTP/1.1\r\n"," HTTP/2.0\r\n"])
		
		#Header
		header = (['Host: ' + str(args.ip) + ':' + str(args.port),'User-Agent: Mozilla','Accept: */*'])
		
		ra = random.randrange(0,6)
		rb = random.randrange(0,4)
		rc = random.randrange(0,4)
		
		if rc == 3:
			buffer = method[ra] + str(randomstr) + ver[rb]
		else:
			buffer = method[ra] + getfile + ver[rb]
		
		for v in range(0,3):
			if v != rc:
				buffer += header[v] + '\n'
			else:
				buffer += header[rc] + randomstr + '\n'
		
		buffer += '\r\n\r\n'
		
		u = (['\\','|','/','-'])
		sys.stdout.flush()		
		sys.stdout.write("\r[{1}] Sending request: {0}".format(l,u[o]))
		o += 1
		if o == 4:
			o = 0
		l += 1
		
		conn(buffer, j)

#Piping to a Program -p
def printc():

	PROG = args.d
	for i in range(0,int(args.m)):
		randomstr = genstr(3)
		
		try:
			FILE = open('pipes/pipe' + str(i) + '.txt',"w")
			FILE.write(randomstr)
			FILE.close()
		except IOError:
			print "\nPlease create the folder pipes/\n"
			sys.exit(1)
		
		PFILE = "pipes/pipe" + str(i) + ".txt"
		cmd = "%s < %s" % (PROG , PFILE)
		print '\n' + cmd + '\n'
		os.system(cmd)
		time.sleep(float(args.i))

def main():
	
	parh()
	if(args.o != True):
		try:
			FILE = open(args.o + '/test.txt',"w")
			FILE.write("1")
			FILE.close()
		except IOError:
			print "\nPlease create the folder " + args.o + "\n"
			sys.exit(1)
	
	if(args.n == True or args.w == True):
		try:
			FILE = open('crashlogs/test.txt',"w") 
			FILE.write("1")
			FILE.close()
		except IOError:
			print "\nPlease create the folder crashlogs/\n"
			sys.exit(1)

	try:
		if (args.m != True) and (args.o != True) and (args.b != True) and (args.e != True):
			print '\nBrowser fuzz!\n'
			brow()
			print '\n\nReady to fuzz!\n'
		
		elif (args.n == True) and (args.ip != True) and (args.port != True) and (args.i != True):
			print '\nNetwork fuzz running!\n\nFuzzing: ' + args.ip + ':' + args.port + '\n'
			netf()
			print '\n'
		
		elif ((args.f == True) or (args.ft != True)) and (args.m != True) and (args.e != True) and (args.o != True):
			print 'File fuzz!\n'
			filefuzz()
			print '\n\nReady to fuzz!\n'
		
		elif (args.ip != True) and (args.port != True) and (args.w == True) and (args.i != True):
			print '\nWeb daemon fuzz running!\n\nFuzzing: http://' + args.ip + ':' + args.port + '/\n'
			httpf()
			print '\n'
		
		elif (args.p == True) and (args.i != True) and (args.m != True) and (args.d != True):
			print "Piping it baby!\n"
			printc()
			print '\n'
		
		else:
			print 'Fail!'
	
	except KeyboardInterrupt:
		print '\n\nWTF? Why did you stop me?\n'

if __name__ == "__main__":
	main()
