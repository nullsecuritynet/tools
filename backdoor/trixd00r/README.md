# Description
trixd00r is an advanced and invisible userland backdoor based on TCP/IP for UNIX
systems. It consists of a server and a client. The server sits and waits for
magic packets using a sniffer. If a magic packet arrives, it will bind a shell
over TCP or UDP on the given port or connecting back to the client again over
TCP or UDP. The client is used to send magic packets to trigger the server and
get a shell.

# Author
noptrix

# Disclaimer
We hereby emphasize, that the hacking related stuff on
[nullsecurity.net](http://nullsecurity.net) is only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
