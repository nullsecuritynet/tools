try:
    import banner
    import os
    import sys
except ImportError:
    print'\n[-]Failed To Import Module'

definepath = os.getcwd()
sys.path.append('%s/src/' % definepath)

banner.print_banner()

print 'U3-Pwn Change Log'
print'''
===================================================================================
Rebult most of the tool, added more user imput sanitization (cheers Shadow Master)
Recompiled shellcodeexec to bypass av agian.
===================================================================================







'''

raw_input('Press Enter To Return To Menu..')
