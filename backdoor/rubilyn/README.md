# Description
64bit Mac OS-X kernel rootkit that uses no hardcoded address to hook the BSD
subsystem in all OS-X Lion & below. It uses a combination of syscall hooking and
DKOM to hide activity on a host. String resolution of symbols no longer works on
Mountain Lion as symtab is destroyed during load, this code is portable on all
Lion & below but requires re-working for hooking under Mountain Lion.

# Author
prdelka

# Disclaimer
We hereby emphasize, that the hacking related stuff on
[nullsecurity.net](http://nullsecurity.net) is only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
