# Description

A fast async multithreaded bruteforcer of subdomains that leverages a wordlist
and/or character permutation.

# Usage

```
$ dnsspider -H
       __                      _     __
  ____/ /___  ______________  (_)___/ /__  _____
 / __  / __ \/ ___/ ___/ __ \/ / __  / _ \/ ___/
/ /_/ / / / (__  |__  ) /_/ / / /_/ /  __/ /
\__,_/_/ /_/____/____/ .___/_/\__,_/\___/_/
                    /_/

    --== [ by nullsecurity.net ] ==--

usage

  dnsspider.py -t <arg> -a <arg> [opts] | <misc>

target options

  -a <domain>     - target domain to bruteforce subdomains for

attack options

  -t <type>       - attack type: 0 = dictionary, 1 = bruteforce
  -l <wordlist>   - wordlist file, one hostname per line (default: built-in)
  -c <charset>    - charset: 0 = [a-z0-9], 1 = [a-z], 2 = [0-9] (default: 0)
  -m <maxchar>    - max chars for bruteforce (default: 2)
  -s <prefix>     - prefix for bruteforce, e.g. 'www'
  -g <postfix>    - postfix for bruteforce, e.g. 'www'
  -f              - force scan when wildcard detected (filters wildcard responses)
  -z              - attempt zone transfer (AXFR) before scanning
  -T <types>      - record type(s) to query, comma-separated (default: A)
                    supported: A, AAAA, CNAME, MX, TXT, NS

dns options

  -d <nameserver> - nameserver(s), comma-separated for round-robin rotation,
                    e.g. '8.8.8.8,8.8.4.4,9.9.9.9' (default: system resolver)
  -i <ipaddr>     - source ip address to use (default: system ip)
  -p <port>       - source port (default: 0 = random free port)
  -u <protocol>   - use udp or tcp (default: udp)
  -o <sec>        - timeout in seconds (default: 3)
  -w <sec>        - delay between requests in seconds (default: 0)
  -x <num>        - number of threads (default: 50)

output options

  -r <logfile>    - write found subdomains to file (default: stdout)
  -F <format>     - output format: default, host, json (default: default)
  -v              - verbose mode, print every attempt

misc options

  -V              - print version information
  -H              - print this help message

examples

  # dictionary scan against target.com using built-in wordlist
  $ dnsspider.py -t 0 -a target.com

  # dictionary scan with 100 threads and save results to file
  $ dnsspider.py -t 0 -a target.com -x 100 -r found.txt

  # bruteforce with alphanumeric charset, 3-char combos, www prefix
  $ dnsspider.py -t 1 -a target.com -c 0 -m 3 -s www

  # dictionary scan using google dns, tcp protocol, verbose output
  $ dnsspider.py -t 0 -a target.com -d 8.8.8.8 -u tcp -v

  # dictionary scan with custom wordlist and wildcard filtering forced
  $ dnsspider.py -t 0 -a target.com -l /path/to/wordlist.txt -f

  # bruteforce digits-only, 2-char combos, google dns, 30 threads
  $ dnsspider.py -t 1 -a target.com -c 2 -m 2 -d 8.8.8.8 -x 30

  # dictionary scan with 500ms delay between requests and 5s timeout
  $ dnsspider.py -t 0 -a target.com -w 0.5 -o 5

  # attempt zone transfer (AXFR) before dictionary scan
  $ dnsspider.py -t 0 -a target.com -z

  # query A and AAAA records
  $ dnsspider.py -t 0 -a target.com -T A,AAAA

  # output hostnames only for piping into other tools
  $ dnsspider.py -t 0 -a target.com -F host | nmap -iL -

  # json output to file
  $ dnsspider.py -t 0 -a target.com -F json -r found.json
```

# Author

noptrix

# Notes

- quick'n'dirty code
- dnsspider is already packaged and available for [BlackArch Linux](https://www.blackarch.org/)
- My master-branches are always stable; dev-branches are created for current work.
- All of my public stuff you find are officially announced and published via [nullsecurity.net](https://www.nullsecurity.net).

# License

Check docs/LICENSE.

# Disclaimer

We hereby emphasize, that the hacking related stuff found on
[nullsecurity.net](http://nullsecurity.net) are only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
