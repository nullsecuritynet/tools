# Description

A python tool which scans for HTTP servers and finds given strings in HTTP body
and HTTP response headers.

# Usage

```
$ httpgrep -H
    __    __  __
   / /_  / /_/ /_____  ____ _________  ____
  / __ \/ __/ __/ __ \/ __ `/ ___/ _ \/ __ \
 / / / / /_/ /_/ /_/ / /_/ / /  /  __/ /_/ /
/_/ /_/\__/\__/ .___/\__, /_/   \___/ .___/
             /_/    /____/         /_/

     --== [ by nullsecurity.net ] ==--

usage

  httpgrep -h <args> -s <arg> [opts] | <misc>

target options

  -h <hosts|file>   - single host/url or host-/cidr-range or file containing
                      hosts or file containing URLs, e.g.: foobar.net,
                      192.168.0.1-192.168.0.254, 192.168.0.0/24, /tmp/hosts.txt
                      NOTE: hosts can also contain ':<port>' on cmdline or in
                      file.
  -p <port>         - port to connect to (default: 80 if hosts were given)
  -t                - use TLS/SSL to connect to service
  -u <URI|file>     - URI or comma-separated URIs or file with URIs (one per
                      line) to search given strings in, e.g.: /foobar/,
                      /foo.html, /admin,/login, /tmp/paths.txt (default: /)
  -r                - perform reverse dns lookup for given IPv4 addresses
                      NOTE: this will slow down the scanz

http options

  -X <method>       - specify HTTP request method to use (default: get).
                      use '?' to list available methods.
  -a <user:pass>    - http auth credentials (format: 'user:pass')
  -U <UA>           - set custom User-Agent (default: firefox, rv84, windows)
  -A                - use random user-agent per request
  -R <headers>      - set custom headers (format: 'foo=bar;lol=lulz;...')
  -C <cookies>      - set cookies (format: 'foo=bar;lol=lulz;...')
  -F                - don't follow HTTP redirects
  -P <proxy>        - use proxy (format: '[http|https|socks4|socks5]://host:port')

search options

  -s <str|file>     - a single string/regex or multile strings/regex in a file
                      to find in given URIs and HTTP response headers,
                      e.g.: 'tomcat 8', '/tmp/igot0daysforthese.txt'
  -S <where>        - search strings in given places (default: headers,body)
  -b <bytes>        - num bytes to read from response. offset == response[0].
                      (default: 64)
  -i                - use case-insensitive search

scan options

  -x <threads>      - num threads for concurrent scans and checks (default: 80)
  -c <seconds>      - num seconds for socket timeout (default: 3.0)
  -f <codes>        - only report responses with given HTTP status codes,
                      e.g.: '200', '200,301,302'

output options

  -l <file>         - log found matches to file
  -v                - verbose mode (default: quiet)

misc options

  -H                - print help
  -V                - print version information
```

# Author

noptrix

# Notes

- quick'n'dirty code
- httpgrep is already packaged and available for [BlackArch Linux](https://www.blackarch.org/)
- My master-branches are always stable; dev-branches are created for current work.
- All of my public stuff you find are officially announced and published via [nullsecurity.net](https://www.nullsecurity.net).

# License

Check docs/LICENSE.

# Disclaimer

We hereby emphasize, that the hacking related stuff found on
[nullsecurity.net](http://nullsecurity.net) are only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
