# Description

A fast async multithreaded bruteforcer of subdomains that leverages a wordlist
and/or character permutation.

# Usage

```
$ dnsspider -H

--==[ dnsspider by nullsecurity.net ]==--

usage:
  dnsspider.py -t <arg> -a <arg> [options]

optional arguments:
  -t <type>        attack type (0 for dictionary 1 for bruteforce)
  -a <domain>      subdomain to bruteforce
  -l <wordlist>    wordlist, one hostname per line (default: built-in)
  -d <nameserver>  choose another nameserver (default: your system's)
  -i <ipaddr>      source ip address to use (default: your system's)
  -p <port>        source port to use (default: 0 -> first free random port)
  -u <protocol>    speak via udp or tcp (default: udp)
  -c <charset>     choose charset 0 [a-z0-9], 1 [a-z] or 2 [0-9] (default: 0)
  -m <maxchar>     max chars to bruteforce (default: 2)
  -s <prefix>      prefix for bruteforce, e.g. 'www'
  -g <postfix>     postfix for bruteforce, e.g. 'www'
  -o <sec>         timeout (default: 3)
  -v               verbose mode - prints every attempt (default: quiet)
  -w <sec>         seconds to wait for next request (default: 0)
  -x <num>         number of threads to use (default: 50)
  -f               force scanning if wildcard check was positive
  -r <logfile>     write found subdomains to file (default: stdout)
  -V               print version information
  -H               print this help
```

# Author

noptrix

# Notes

- quick'n'dirty code
- dnsspider is already packaged and available for [BlackArch Linux](https://www.blackarch.org/)
- My master-branches are always dev-branches; use releases for stable versions.
- All of my public stuff you find are officially announced and published via [nullsecurity.net](https://www.nullsecurity.net).

# License

Check docs/LICENSE.

# Disclaimer

We hereby emphasize, that the hacking related stuff found on
[nullsecurity.net](http://nullsecurity.net) are only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
