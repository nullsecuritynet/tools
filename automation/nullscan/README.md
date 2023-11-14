# Description
A modular framework designed to chain and automate security tests. It parses
target definitions from the command line and runs corresponding modules and
their nullscan-tools afterwards. It can also take hosts and start nmap first in
order to perform a basic portscan and run the modules afterwards. Also, nullscan
can parse a given nmap logfile for open tcp and udp ports and again run the
modules afterwards. All results will be logged in specified directories with a
clean structure and a HTML report can subsequently be generated.

This code is dedicated to my friend Zeljko (R.I.P.), who passed away,
2nd Dec 2012.

# Usage

```
[ hacker@blackarch ~ ]$ nullscan -H
                ____
   ____  __  __/ / /_____________ _____
  / __ \/ / / / / / ___/ ___/ __ `/ __ \
 / / / / /_/ / / (__  ) /__/ /_/ / / / /
/_/ /_/\__,_/_/_/____/\___/\__,_/_/ /_/

   --==[ by nullsecurity.net ]==--

usage

  nullscan <modes> [options] | <misc>

modes

  -t <targets> - hosts to scan via nmap and then attack - ? for info
  -u <uris>    - targets to attack directly via URIs - ? for info
  -l <file>    - parse nmap xml logfile and attack hosts on open ports

options

  -o <opts>    - extra options for modes - ? for info
  -i <mods>    - include modules (default: all) - ? for info
  -I <tools>   - include tools (default: all) - ? for info
  -x <mods>    - exclude modules (default: see nullscan.cfg) - ? for info
  -X <tools>   - exclude tools (default: see nullscan.cfg) - ? for info
  -T <num>     - num workers for parallel target checks (default: 15)
  -M <num>     - num workers to run parallel modules (default: 10)
  -P <num>     - num workers to run parallel tools (default: 15)
  -k <sec>     - num seconds for tool (global) timeout (default: 0.0)
  -r           - generate an html report
  -R <dir>     - work, log and report dir (default: pwd + date)
  -c <file>    - config file (default: /etc/nullscan.conf)
  -v           - verbose mode (default: false)
  -d           - debug mode (default: false)

misc

  -C           - check for missing tools (recommended)
  -p <args>    - print tools and exit - ? for info
  -m <args>    - create and add a new module - ? for info
  -a <args>    - add tool to existing module - ? for info
  -V           - print version of nullscan and exit
  -H           - print this help and exit

examples

  -t 192.168.0.0/24 -i tcp=ssh,http -r -I hydra_ssh,crack_http_auth

  -u 'tcp://nsa.gov:80=http,22=ssh;udp://foo.bar:1337;
      http://fbi.gov,https://cia.gov;mail://foo@bar.baz;
      person://justin bieber,noptrix;lan://eth0,tap0;wifi://wlan0'
      -o 'user=root;plists=/tmp/pwds.txt;rhost=192.168.0.1;
      sport=1337;dirsearch_web=-o my -p "own opts" -c 1 -f 4;'

  -n /tmp/scanned.xml -i 'host=icmp;tcp=default' -r

  -l hosts.txt -X sqlmap,wpscan -v -o 'httping_web=-p cia.gov;
     rpcdump_udp=-f foo -b bar;nmap=-sT,-n,-p-;'

  -p 'tcp=ssh,http;host=zonetransfer;udp'

  -m 'icmp/ping ping_flood ping -f -s 9999'

  -a 'tcp/ssh crack_ssh sshcracker -c arg -f arg'
```

# Example

[![asciicast](https://asciinema.org/a/kUNVbUEIde0e6vtsKiFi5neXb.png)](https://asciinema.org/a/kUNVbUEIde0e6vtsKiFi5neXb)

# Installation

Run `setup.sh`. Install needed python modules afterwards using `pip install -r docs/requirements.txt`.

# Author

noptrix

# Notes

- Please check the manpage from docs/nullscan.1
- Use '?' option-value for any cmdline options. It gives you information for usage and examples.
- clean code; real project
- nullscan is already packaged and available for [BlackArch Linux](https://www.blackarch.org/)
- My master-branches are always dev-branches; use releases for stable versions.
- All of my public stuff you find are officially announced and published via [nullsecurity.net](https://www.nullsecurity.net).

# License

Check docs/LICENSE.

# Disclaimer
We hereby emphasize, that the hacking related stuff found on
[nullsecurity.net](http://nullsecurity.net/) are only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
