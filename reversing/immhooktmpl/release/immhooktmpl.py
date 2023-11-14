################################################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# immhooktmpl - Immunity template plugin for function hooking                  #
#                                                                              #
# FILE                                                                         #
# immhooktmpl.py                                                               #
#                                                                              #
# DATE                                                                         #
# 30-03-2014                                                                   #
#                                                                              #
# DESCRIPTION                                                                  #
# Due to the lack of documentation for Immnunity API this is an easy           #
# template for function hooking while RE.                                      #
#                                                                              #
# VERSION                                                                      #
# v0.3                                                                         #
#                                                                              #
# AUTHOR                                                                       #
# nrz@nullsecurity.net                                                         #
#                                                                              #
################################################################################


"""
    What:
        v0.3
        - hooking by signature
        - filter by thread number
        - output fncs for hex and string values

        v0.2
        - set API function names or addresses to hook
        - define routine to run when hook reached - Hook class

    Example:

    !immhooktmpl -H
0BADF00D   --==[ immhooktmpl by nrz@nullsecurity.net ]==--
0BADF00D
0BADF00D   Usage: !immhooktmpl [options] | <misc>
0BADF00D
0BADF00D   Dynamic Options:
0BADF00D      -n <function_name>: add API function name to hook [default callback]
0BADF00D      -a <name+0xADDR>: add address to hook [i.e. _crypt+0x0031337][default callback]
0BADF00D      -r <function_name>: remove hook
0BADF00D   Static Options [edit source code]:
0BADF00D      -N: hook all functions
0BADF00D      -A: hook all addresses
0BADF00D      -S: hook all signatures
0BADF00D      -R: remove all hooks
0BADF00D
0BADF00D   Misc:
0BADF00D      -v: verbose mode (default: quiet)
0BADF00D      -V: print version of immhooktmpl and exit
0BADF00D      -H: print this help and exit
        [+] Done.

    !immhooktmpl -N
0BADF00D   --==[ immhooktmpl by nrz@nullsecurity.net ]==--
0BADF00D   [+] hook WS2_32.recvfrom 0x71A32D0F
0BADF00D     -> Hook.add WS2_32.recvfrom returned 26465696
0BADF00D     -> Hook added
0BADF00D   [+] hook WS2_32.sendto 0x71A32C69
0BADF00D     -> Hook.add WS2_32.sendto returned 26465992
0BADF00D     -> Hook added
0BADF00D   [+] hook WS2_32.socket 0x71A33B91
0BADF00D     -> Hook.add WS2_32.socket returned 26466320
0BADF00D     -> Hook added
           [+] Done.

0BADF00D   [hook] WS2_32.socket
0BADF00D   [*] Thread: 0x744
0BADF00D   EIP on Stack: 0x0182f378 RETURN TO 0x016486fd
0BADF00D   1st arg - (int) addressfamily: 0x0182f37c -> 0x00000017
0BADF00D    -> AF_INET6
0BADF00D   2nd arg - (int) type: 0x0182f380 -> 0x00000001
0BADF00D    -> SOCK_STREAM
0BADF00D   3nd arg - (int) protocol: 0x0182f384 -> 0x00000006
0BADF00D    -> IPPROTO_TCP

0BADF00D   [hook] WS2_32.sendto
0BADF00D   [*] Thread: 0x744
0BADF00D   EIP on Stack: 0x0182f434 RETURN TO 0x0163bf65
0BADF00D   1st arg - (SOCKET) socket: 0x0182f438 -> 0x00000530
0BADF00D   2nd arg - (char *) buff: 0x0182f43c -> 0x01773428
0BADF00D   3rd arg - (int) len: 0x0182f440 -> 0x000000aa
0BADF00D   [+] ascii buffer addr: 01773428 - length: 000000AA
0BADF00D   }s'\\\\\\\\\"\\\\\x\\\\\\\\\\\\\\\\Yk\\C\`\\\\\\R\\
0BADF00D   \\X>3\X0\\\\y'\\.\=n\\\\\\\\=\1\f)\\\\w\\\\\\\r>\\
0BADF00D   2\\\\\o\\\\\\\)\\\r\\=\;<\k\\$Ph_v\W\E\.\\\\\\\\\7
0BADF00D   }w\\\\\u3\\\\\\:\\\
0BADF00D   [+] hex buffer addr: 01773428 - length: 170
0BADF00D      0x00: 7d73 27e4 f1cd 8a10 031d e6b0 229c dff0
0BADF00D      0x10: e3dc 78c8 f202 c5bf 97e0 c4e6 d7c2 b7b1
0BADF00D      0x20: e3ff be59 6bd8 0843 be60 e714 cf96 f8c8
0BADF00D      0x30: 52f5 0e84 9758 3e33 e758 30e7 cbac 7f79
0BADF00D      0x40: 271d 142e 163d 6ef0 c8b4 95ac ecff 1c3d
0BADF00D      0x50: a031 7f66 29af 05d7 d077 ce00 b38f dec7
0BADF00D      0x60: e972 3efd 8732 b1f0 00a6 b96f 1c93 cc9d
0BADF00D      0x70: 15cc 8829 a2c1 0c72 e8c1 3dee 3b3c a86b
0BADF00D      0x80: be8f 2450 685f 76b7 57bd 45a4 2ebf 00f0
0BADF00D      0x90: 0103 08f2 c794 377d 77f5 e6b6 c2ca 7533
0BADF00D      0xA0: c3c2 c891 ae02 3ade ef96
0BADF00D   4th arg - (int) flags: 0x0182f444 -> 0x00000000
0BADF00D   5th arg - (struct sockaddr *) to: 0x0182f448 -> 0x01772594
0BADF00D   6th arg - (int) tolen: 0x0182f44c -> 0x00000010

    !immhooktmpl -a _crypt+0x01001646
0BADF00D   [+] hook _crypt 0x01001646
0BADF00D     -> Hook.add _crypt returned 26466320
0BADF00D     -> Hook added

    !immhooktmpl -f VirtualAlloc
0BADF00D   --==[ immhooktmpl by nrz@nullsecurity.net ]==--
0BADF00D   [+] hook VirtualAlloc 0x7C809A81
0BADF00D     -> Hook.add VirtualAlloc returned 53109832
0BADF00D     -> Hook added

    !immhooktmpl -D
0BADF00D   --==[ immhooktmpl by nrz@nullsecurity.net ]==--
0BADF00D   [!] WARNING: Removed hook WS2_32.recvfrom
0BADF00D   [!] WARNING: Removed hook WS2_32.sendto
0BADF00D   [!] WARNING: Removed hook WS2_32.socket
0BADF00D   [!] WARNING: Removed Address hook fnc1
0BADF00D   [!] WARNING: Removed Address hook fnc2
0BADF00D   [!] WARNING: Removed Signature hook sgn1
0BADF00D   [!] WARNING: Removed Signature hook sgn2
           [+] Removed done
"""

# imports
import sys
import getopt

import immutils
from immlib import *

# filter by thread
#THREAD_ID = "0x8F4"
THREAD_ID = "" # dont filter anything

# !! FUNCTIONS TO HOOK - !immhooktmpl -F
RECVFROM = "WS2_32.recvfrom"
SENDTO = "WS2_32.sendto"
SOCKET = "WS2_32.socket"

FUNCTIONS = [ RECVFROM, # API fnc
              SENDTO,   # API fnc
              SOCKET ]  # API fnc

# !! ADDRESSES TO HOOK - !immhooktmpl -A
ADDRESSES = [
                [ "fnc1", "0x0BAD1337" ],  # non API routine
                [ "fnc2", "0x0BAD1337" ]   # address hook
            ]

# !! SIGNATURE TO HOOK - !immhooktmpl -S
SIGNATURES = [
                [ "sgn1", "51 57 B16B00B5" ],   # non API routine
                [ "sgn2", "50 DEADD00D" ]       # sign hook
             ]


# global
imm = Debugger()

# version of immhooktmpl
VERSION = "immhooktmpl v0.3"

# description for immunity
DESC = "Immunity template plugin for hooking"

# exit codes
SUCCESS = "[+] Done."
FAILURE = "[-] Failed. !immhooktmpl -H"


class Wrapper:
    """ class for wrapper methods """

    def __init__(self, options):
        """ init """
        self.options = options

    def verbose(self, msg):
        """ print verbose lines """
        if "verbose" in self.options:
            imm.log(msg)
        return

class Error:
    """ class for error handling """

    def __init__(self):
        """ init """
        pass

    @staticmethod
    def warn(msg):
        """ print warning message """
        imm.log("[!] WARNING: " + msg \
                , focus=1, highlight=1)
        return SUCCESS

    @staticmethod
    def error(msg):
        """ print error message and exit """
        imm.log("[-] ERROR: " + msg \
                , focus=1, highlight=1)
        return FAILURE

class Help:
    """ class for printing help and usage messages """

    def __init__(self):
        """ init """
        pass

    @staticmethod
    def banner():
        """ print banner """
        imm.log("--==[ immhooktmpl by nrz@nullsecurity.net ]==--" \
                , focus=1, highlight=1)
        return SUCCESS

    @staticmethod
    def usage():
        """ print usage and help """
        imm.log("")
        imm.log("Usage: !immhooktmpl [options] | <misc>")
        imm.log("")
        imm.log("Dynamic Options:")
        imm.log("   -f <function_name>: add API function name to hook [default callback]")
        imm.log("   -a <name+0xADDR>: add address to hook [i.e. _crypt+0x0031337][default callback]")
        imm.log("   -r <function_name>: remove hook")
        imm.log("Static Options [edit source code]:")
        imm.log("   -F: hook all functions")
        imm.log("   -A: hook all addresses")
        imm.log("   -S: hook all signatures")
        imm.log("   -R: remove all hooks")
        imm.log("")
        imm.log("Misc:")
        imm.log("   -v: verbose mode (default: quiet)")
        imm.log("   -V: print version of immhooktmpl and exit")
        imm.log("   -H: print this help and exit")

        return SUCCESS

class OptsParser:
    """ class for command line parsing """

    def __init__(self, options):
        """ init """
        self.options = options

    def parse_opts(self, args):
        """ PARSE command line options """
        try:
            self.opts, self.args = getopt.getopt(args, \
                                         "f:a:r:FASRvVH")
        except getopt.GetoptError as err:
            return (str(err))
        for self.o, self.a in self.opts:
            if self.o == "-f":
                self.options["add_function"] = self.a
            elif self.o == "-a":
                self.options["add_address"] = self.a
            elif self.o == "-r":
                self.options["remove"] = self.a
            elif self.o == "-F":
                self.options["run_fnc_hook"] = True
            elif self.o == "-A":
                self.options["run_addr_hook"] = True
            elif self.o == "-S":
                self.options["run_sign_hook"] = True
            elif self.o == "-R":
                self.options["cleanup"] = True
            elif self.o == "-v":
                self.options["verbose"] = True
            elif self.o == "-V":
                imm.log(VERSION)
                return SUCCESS
            elif self.o == "-H":
                Help.usage()
                return SUCCESS
        if self.options:
            return self.options
        else:
            return FAILURE

class Print():
    @staticmethod
    def ascii_buffer(imm, buff, length, name = None):
        """
            print ascii buffer
        """
        imm.log("[+] ascii buffer addr: %08X - length: %08X" \
                 % (buff, length))
        if name:
            imm.log("   * %s" % name)

        string = ''
        for i in range(0, length):
            char = imm.readMemory(buff+i, 1)
            string += str(repr(char)[1])
            if i and not (i % 50):
                imm.log(string)
                string = ''
        imm.log(string)

    @staticmethod
    def hex_buffer(imm, buff, length, name = None):
        """
            print hex values from buffer
        """
        imm.log("[+] hex buffer addr: %08x - length: %s " \
                 % (buff, length))
        if name:
            imm.log("   * %s" % name)

        for i in range(0, length, 16):
            mem = imm.readMemory(buff+i, 16)
            string = ''
            for x in range(0, 16):
                val = hex(ord(mem[x])).replace("0x","")
                # dirty fix for null char auto remove
                if len(val) == 1:
                    string += "0" + str(val)
                else:
                    string += str(val)
                if x % 2:
                    string += " "
                if i+x >= length-1:
                    break

            imm.log("   0x%02X: %s" % (i, string))

class Hook(LogBpHook):
    def __init__(self):
        LogBpHook.__init__(self)
        return

    def set_hook_by_name(self, name):
        """
            hook functions by API function name
        """
        addr = imm.getAddress(name)

        imm.log("[+] hook %s 0x%X" % (name, addr))

        imm.setBreakpoint(addr)
        imm.setComment(addr, name)
        imm.setLabel(addr, name)

        if addr == -1:
            return Error.error("Could not locate function %s" % name)

        ret = self.add(name, addr)

        imm.log("  -> Hook.add %s returned %s" % (name, ret))
        if ret == -1:
            return Error.error("Failed to add hook %s" % name)
        else:
            imm.log("  -> Hook added")

        imm.addKnowledge("%08x" % addr, name)

        return

    def set_hook_by_address(self, name, addr):
        """
            hook functions by Address function name
        """
        try:
            addr = int(addr, 16)
        except:
            pass

        imm.log("[+] hook %s 0x%X" % (name, addr))

        imm.setBreakpoint(addr)
        imm.setComment(addr, name)
        imm.setLabel(addr, name)

        if addr == -1:
            return Error.error("Could not locate function %s" % name)

        ret = self.add(name, addr)

        imm.log("  -> Hook.add %s returned %s" % (name, ret))
        if ret == -1:
            return Error.error("Failed to add hook %s" % name)
        else:
            imm.log("  -> Hook added")

        imm.addKnowledge("%08x" % addr, name)

        return


    """
    !! DEFINE ROUTINE FOR EACH HOOK HERE
    """
    def recvfrom_hook(self, imm, function_name, regs):
        """
            Example recvfrom routine

        int recvfrom(_In_ SOCKET s,_Out_ char *buf,_In_ int len,
                     _In_ int flags,_Out_ struct sockaddr *from,
                     _Inout_opt_  int *fromlen);
        """
        esp = regs['ESP']

        imm.log("EIP on Stack: 0x%08x RETURN TO 0x%08x" % \
                ( esp, imm.readLong(esp) ) )
        imm.log("1st arg - (SOCKET) socket: 0x%08x -> 0x%08x" % \
                ( (esp+4), imm.readLong(esp+4) ) )
        imm.log("2nd arg - (char *) buff: 0x%08x -> 0x%08x" % \
                ( (esp+8), imm.readLong(esp+8) ) )

        Print.ascii_buffer(imm, imm.readLong(esp+8), 10)
        Print.hex_buffer(imm, imm.readLong(esp+8), 10)

        imm.log("3rd arg - (int) len: 0x%08x -> 0x%08x" % \
                ( (esp+12), imm.readLong(esp+12) ) )
        imm.log("4th arg - (int) flags: 0x%08x -> 0x%08x" % \
                ( (esp+16), imm.readLong(esp+16) ) )
        imm.log("5th arg - (struct sockaddr *)from: 0x%08x -> 0x%08x" % \
                ( (esp+20), imm.readLong(esp+20) ) )
        imm.log("6th arg - (int *) fromlen: 0x%08x -> 0x%08x" % \
                ( (esp+24), imm.readLong(esp+24) ) )

        return

    def socket_hook(self, imm, function_name, regs):
        """
            Example socket routine

        SOCKET WSAAPI socket(_In_  int af, _In_  int type,
                            _In_  int protocol);
        """
        esp = regs['ESP']

        imm.log("EIP on Stack: 0x%08x RETURN TO 0x%08x" % \
                ( esp, imm.readLong(esp) ) )

        imm.log("1st arg - (int) addressfamily: 0x%08x -> 0x%08x" % \
                ( (esp+4), imm.readLong(esp+4) ) )
        addrfam = int(str(imm.readLong(esp+4)).replace("0x",""))
        if addrfam == 0:
            imm.log(" -> AF_UNSPEC")
        elif addrfam == 2:
            imm.log(" -> AF_INET")
        elif addrfam == 6:
            imm.log(" -> AF_IPX")
        elif addrfam == 16:
            imm.log(" -> AF_APPLETALK")
        elif addrfam == 17:
            imm.log(" -> AF_NETBIOS")
        elif addrfam == 23:
            imm.log(" -> AF_INET6")
        elif addrfam == 26:
            imm.log(" -> AF_IRDA")
        elif addrfam == 32:
            imm.log(" -> AF_BTH")

        imm.log("2nd arg - (int) type: 0x%08x -> 0x%08x" % \
                ( (esp+8), imm.readLong(esp+8) ) )
        stype = int(str(imm.readLong(esp+8)).replace("0x",""))
        if stype == 1:
            imm.log(" -> SOCK_STREAM")
        elif stype == 2:
            imm.log(" -> SOCK_DGRAM")
        elif stype == 3:
            imm.log(" -> SOCK_RAW")
        elif stype == 4:
            imm.log(" -> SOCK_RDM")
        elif stype == 5:
            imm.log(" -> SOCK_SEQPACKET")

        imm.log("3nd arg - (int) protocol: 0x%08x -> 0x%08x" % \
                ( (esp+12), imm.readLong(esp+12) ) )

        proto = int(str(imm.readLong(esp+12)).replace("0x",""))
        if proto == 1:
            imm.log(" -> IPPROTO_ICMP")
        elif proto == 2:
            imm.log(" -> IPPROTO_IGMP")
        elif proto == 3:
            imm.log(" -> BTHPROTO_RFCOMM")
        elif proto == 6:
            imm.log(" -> IPPROTO_TCP")
        elif proto == 17:
            imm.log(" -> IPPROTO_UDP")
        elif proto == 58:
            imm.log(" -> IPPROTO_ICMPV6")
        elif proto == 113:
            imm.log(" -> IPPROTO_RM")

        return

    def sendto_hook(self, imm, function_name, regs):
        """
            Example sendto routine

        int sendto(_In_  SOCKET s, _In_  const char *buf,
                   _In_  int len, _In_  int flags,
                   _In_  const struct sockaddr *to, _In_  int tolen);
        """
        esp = regs['ESP']

        imm.log("EIP on Stack: 0x%08x RETURN TO 0x%08x" % \
                ( esp, imm.readLong(esp) ) )
        imm.log("1st arg - (SOCKET) socket: 0x%08x -> 0x%08x" % \
                ( (esp+4), imm.readLong(esp+4) ) )
        imm.log("2nd arg - (char *) buff: 0x%08x -> 0x%08x" % \
                ( (esp+8), imm.readLong(esp+8) ) )
        imm.log("3rd arg - (int) len: 0x%08x -> 0x%08x" % \
                ( (esp+12), imm.readLong(esp+12) ) )

        Print.ascii_buffer(imm, imm.readLong(esp+8), imm.readLong(esp+12))
        Print.hex_buffer(imm, imm.readLong(esp+8), imm.readLong(esp+12))

        imm.log("4th arg - (int) flags: 0x%08x -> 0x%08x" % \
                ( (esp+16), imm.readLong(esp+16) ) )
        imm.log("5th arg - (struct sockaddr *) to: 0x%08x -> 0x%08x" % \
                ( (esp+20), imm.readLong(esp+20) ) )
        imm.log("6th arg - (int) tolen: 0x%08x -> 0x%08x" % \
                ( (esp+24), imm.readLong(esp+24) ) )

        return

    def default_hook(self, imm, function_name, regs):
        """
            Catch all calls from not defined hooks
        """
        imm.log("[hook][default] %s" % function_name)

        esp = regs['ESP']

        # read stack and pointer ret address
        imm.log("ESP:      0x%08x RETURN TO 0x%08x" % \
                ( esp, imm.readLong(esp) ) )
        imm.log("ESP +  4: 0x%08x -> 0x%08x" % \
                ( (esp+4), imm.readLong(esp+4) ) )
        imm.log("ESP +  8: 0x%08x -> 0x%08x" % \
                ( (esp+8), imm.readLong(esp+8) ) )
        imm.log("ESP + 12: 0x%08x -> 0x%08x" % \
                ( (esp+12), imm.readLong(esp+12) ) )

        return

    def run(self, regs):
        '''
            Hook controller
                It will run up when function hook reached
        '''
        imm = Debugger()

        # filter by thread
        if THREAD_ID and int(hex(imm.getThreadId()),16) != THREAD_ID:
            return

        # call the routine for the hook reached
        function_name = imm.getKnowledge("%08x" % regs['EIP'])
        imm.log("[hook] %s" % function_name)
        imm.log("[*] Thread: %s" % hex(imm.getThreadId()))

        function_name = imm.getKnowledge("%08x" % regs['EIP'])
        try:
            if function_name == RECVFROM:
                self.recvfrom_hook(imm, function_name, regs)
            elif function_name == SENDTO:
                self.sendto_hook(imm, function_name, regs)
            elif function_name == SOCKET:
                self.socket_hook(imm, function_name, regs)
            #elif function_name == "sgn1":
            #   self.socket_sgn1(imm, function_name, regs)
            else:
                self.default_hook(imm, function_name, regs)
        except:
            imm.log("HOOK ERROR")

        imm.log("")

        return

class Controller:
    """ control program flow routines """

    def __init__(self, args):
        """ init """
        # functions to hook
        self.ret = SUCCESS
        self.args = args
        self.options = {} # options from command line, config, etc.
        self.wrapper = Wrapper(self.options)
        self.hook = ''
        self.parser = OptsParser(self.options)

        return

    def start(self):
        """ main """
        Help.banner()
        self.ret = self.parser.parse_opts(self.args)

        # check args
        if not self.options:
            return self.ret

        self.wrapper.verbose("verbose")

        if self.options.get("add_function"):
            self.hook = Hook()
            self.hook.set_hook_by_name(self.options.get("add_function"))

        elif self.options.get("add_address"):
            self.hook = Hook()
            self.hook.set_hook_by_address( \
                    self.options.get("add_address").split("+")[0], \
                    self.options.get("add_address").split("+")[1].replace('0x',''))

        elif self.options.get("run_fnc_hook"):
            self.hook = Hook()
            imm.addKnowledge("immhooktmpl", self.hook)

            for name in FUNCTIONS:
                self.hook.set_hook_by_name(name)

        elif self.options.get("run_addr_hook"):
            self.hook = Hook()
            imm.addKnowledge("immhooktmpl", self.hook)

            for addr in ADDRESSES:
                self.hook.set_hook_by_address(addr[0], \
                                              addr[1])

        elif self.options.get("run_sign_hook"):
            self.hook = Hook()
            imm.addKnowledge("hooktmpl", self.hook)

            for sign in SIGNATURES:
                addrs = imm.search(str(sign[1]).replace(" ","").decode('hex'))
                for addr in addrs:
                    self.hook.set_hook_by_address(sign[0], \
                                                  addr)

        elif self.options.get("remove"):
            imm.removeHook(self.options.get("remove"))
            Error.warn("Removed hook %s " % self.options.get("remove"))

        elif self.options.get("cleanup"):
            self.hook = imm.getKnowledge("immhooktmpl")

            for name in FUNCTIONS:
                imm.removeHook(name)
                Error.warn("Removed hook %s " % name)
            for addr in ADDRESSES:
                imm.removeHook(addr[0])
                Error.warn("Removed Address hook %s" % addr[0])
            for sign in SIGNATURES:
                imm.removeHook(sign[0])
                Error.warn("Removed Signature hook %s" % sign[0])

            imm.forgetKnowledge("immhooktmpl")

            return "[+] Removed done"

        return SUCCESS

    def end(self):
        """ do last needed things """
        pass


def main(args):
    ret = SUCCESS
    cntrl = Controller(args)
    ret = cntrl.start()
    cntrl.end()
    return ret

# EOF
