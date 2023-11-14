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
# ftp-fuzzer.py - Master FTP fuzzer                                           #
#                                                                             #
# DATE                                                                        #
# 01/27/2012                                                                  #
#                                                                             #
# DESCRIPTION                                                                 #
# The master of all master fuzzing scripts specifically targeted towards      #
# FTP server sofware                                                          #
#                                                                             #
# AUTHOR                                                                      #
# TheXero - http://www.nullsecurity.net/                                      #
#                                                                             #
###############################################################################

import sys, socket, argparse

## Lists the extra chars
extra = ["\x01", "\x02", "\x03", "\x04", "\x05", "\x06", "\x07", "\x08",
"\x09", "\x0a", "\x0b", "\x0c", "\x0d", "\x0e", "\x0f", "\x10", "\x11",
"\x12", "\x13", "\x14", "\x15", "\x16", "\x17", "\x18", "\x19", "\x1a",
"\x1b", "\x1c", "\x1d", "\x1e", "\x1f", "\x80", "\x81", "\x82", "\x83",
"\x84", "\x85", "\x86", "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c",
"\x8d", "\x8e", "\x8f", "\x90", "\x91", "\x92", "\x93", "\x94", "\x95",
"\x96", "\x97", "\x98", "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e",
"\x9f", "\xa0", "\xa1", "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7",
"\xa8", "\xa9", "\xaa", "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0",
"\xb1", "\xb2", "\xb3", "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9",
"\xba", "\xbb", "\xbc", "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2",
"\xc3", "\xc4", "\xc5", "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb",
"\xcc", "\xcd", "\xce", "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4",
"\xd5", "\xd6", "\xd7", "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd",
"\xde", "\xdf", "\xe0", "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6",
"\xe7", "\xe8", "\xe9", "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef",
"\xf0", "\xf1", "\xf2", "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8",
"\xf9", "\xfa", "\xfb", "\xfc", "\xfd", "\xfe", "\xff"]     

## Lists the normal ascii chars
normal = ["\x41", "\x42", "\x43", "\x44", "\x45", "\x46", "\x47", "\x48",
"\x49", "\x4a", "\x4b", "\x4c", "\x4d", "\x4e", "\x4f", "\x50", "\x51",
"\x52", "\x53", "\x54", "\x55", "\x56", "\x57", "\x58", "\x59", "\x5a",
"\x5b", "\x5c", "\x5d", "\x5e", "\x5f", "\x60", "\x61", "\x62", "\x63",
"\x64", "\x65", "\x66", "\x67", "\x68", "\x69", "\x6a", "\x6b", "\x6c",
"\x6d", "\x6e", "\x6f", "\x70", "\x71", "\x72", "\x73", "\x74", "\x75",
"\x76", "\x77", "\x78", "\x79", "\x7a", "\x7b", "\x7c", "\x7d", "\x7e",
"\x7f", "\x20", "\x21", "\x22", "\x23", "\x24", "\x25", "\x26", "\x27",
"\x28", "\x29", "\x2a", "\x2b", "\x2c", "\x2d", "\x2e", "\x2f", "\x30",
"\x31", "\x32", "\x33", "\x34", "\x35", "\x36", "\x37", "\x38", "\x39",
"\x3a", "\x3b", "\x3c", "\x3d", "\x3e", "\x3f", "\x40"]
	
## Lists all of the available commands to fuzz
commands =["ABOR", "ACCT", "ADAT", "ALLO", "APPE", "AUTH", "CCC",
"CDUP", "CONF", "CWD", "DELE", "ENC", "EPRT", "EPSV", "FEAT", "HELP",
"LANG", "LIST", "LPRT", "LPSV", "MDTM", "MIC", "MKD", "MLSD", "MLST",
"MODE", "NLST", "NOOP", "PASS", "OPTS", "PASS", "PASV", "PORT", "PROT",
"PWD", "QUIT", "REIN", "REST", "RETR", "RMD", "RNFR", "RNTO", "SITE",
"SIZE", "STAT", "STOR", "STOU", "STRU", "SYST", "TYPE", "USER", "XCUP",
"XMKD", "XPWD", "XRSQ", "XSEM", "XSEN"] 

def main():
	parser.print_help()

parser = argparse.ArgumentParser(
	add_help=False,
	formatter_class=argparse.RawDescriptionHelpFormatter, # This will fix description and epilog format
	#description="Usage: %(prog)s -h",
	epilog="Examples: %(prog)s -t 192.168.0.1 -p 21 -U username -P password -m 500 -e")

## Adds arguments to help menu
parser.add_argument("-h", action="help",  help="Print this help message then exit")
parser.add_argument("-t", dest="target", required="yes") ## Required argument
parser.add_argument("-p", dest="port", help="Optional (default 21)") 
parser.add_argument("-U", dest="username", help="Optional") 
parser.add_argument("-P", dest="password", help="Optional") 
parser.add_argument("-m", dest="size", help="Optional maximum buffer size (default 10000)")
parser.add_argument("-e", action="store_true", help="Optional (enable extensive fuzzing)")

## Assigns the arguments to various variables
args = parser.parse_args()
target = args.target

## Checks if a port was given
if args.port:
	port = int(args.port)
else: 
	port = 21

## checks if a username is present
if args.username:
	## Makes sure the password is also specified
	if args.password:
		username = args.username
		password = args.password
		credentials = 1
	else: ## No password specified so shows help and quits
		parser.print_help()
		quit()
else:
	credentials = 0

## Checks if a maximum buffer size was specified
if args.size:
	buffmax = int(args.size)

else:
	buffmax = 10000

## Checks if extended fuzzing is enabled
if args.e:
	chars = normal + extra
else:
	chars = normal
 
## Sets the counter
counter = 1	

## Loops through each of the chars
for char in chars:
	
	buffer = char * 100

	## Loops through each of the commands
	for command in commands:
		
		## Sets up the while loop
		while len(buffer) <= buffmax:
			try:

				## Sets up the socket
				s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				socket.setdefaulttimeout(10)
				
				## Connects and receives the banner
				s.connect((target, port)) 			
				s.recv(1024)	
		
				## Check if credentials are to be used
				if credentials == 1:

					## Logins with credentials
					s.send("USER " + username + "\r\n")
					s.recv(1024)
					s.send("PASS " + password + "\r\n")
					s.recv(1024)
				
				## Sends the command and buffer
				print "Fuzzing " + command + " with " + str(len(buffer)) + " bytes of " + char 
				s.send(command + " " + buffer + "\r\n")
				s.recv(1024)
				s.send("quit\r\n")
				s.close()
				buffer += char * 100
			except:
				print "Socket timeout"
				quit()

		## Resets the variables
		buffer = char * 100
quit()
if __name__ == "__main__":
    main()

