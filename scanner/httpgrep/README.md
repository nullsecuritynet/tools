# Description

A fast, asynchronous Python tool that scans HTTP(S) servers and greps for
strings or regex patterns in HTTP response bodies and headers.

It takes single hosts, URLs, CIDR ranges, IP ranges or files; scans multiple
ports per target (single, comma-lists or ranges, auto-detecting TLS vs plain per
port); can pull and scan name-based (v)hosts straight from TLS certificates;
streams matches live to the terminal; and can write results to text, CSV or
JSONL log files.

It is built for large scans: an async core drives thousands of concurrent
connections, a TCP preflight skips dead ports cheaply, per-host and global
timeouts keep it from hanging on slow/dead targets, and an interrupted run can
be resumed.

# Requirements

- Python 3.11+ on a POSIX system (Linux, \*BSD, macOS - uses `termios` and
  asyncio's Unix signal handling)
- [httpx](https://pypi.org/project/httpx/) - `pip install -r requirements.txt`
  (or `pip install httpx`)
- optional, auto-used if present: `uvloop` (faster event loop), `aiodns`
  (non-blocking dns for `-r`), `httpx[socks]` / socksio (SOCKS proxies)

httpgrep is a single self-contained script - just run `./httpgrep.py`.

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
                      NOTE: hosts can also contain ':<ports>' on cmdline or in
                      file, where <ports> is a single port, comma-list or
                      range, e.g.: foo.net:8080, foo.net:80,443, 10.0.0.1:1-1024
  -p <ports>        - port(s) to connect to: single port, comma-separated list
                      or range, e.g.: 80, 80,443,8080, 8000-8100
                      (default: 80, or 443 when -t is given)
  -t                - force TLS/SSL on all ports. by default the scheme is
                      auto-detected per port (plain http, switching to TLS if
                      the port speaks it)
  -u <URI|file>     - URI or comma-separated URIs or file with URIs (one per
                      line) to search given strings in, e.g.: /foobar/,
                      /foo.html, /admin,/login, /tmp/paths.txt (default: /)
  -r                - perform reverse dns lookup for given IPv4 addresses
                      (resolved concurrently before scanning)

http options

  -X <method>       - specify HTTP request method to use (default: get).
                      use '?' to list available methods.
  -a <user:pass>    - http auth credentials (format: 'user:pass')
  -U <UA>           - set custom User-Agent (default: latest ms edge, windows)
  -A                - use random user-agent per request
  -R <headers>      - set custom headers (format: 'foo=bar;lol=lulz;...')
  -C <cookies>      - set cookies (format: 'foo=bar;lol=lulz;...')
  -F                - don't follow HTTP redirects
  -L <num>          - max redirects to follow (default: 10; ignored with -F)
  -E                - verify TLS/SSL certificates (default: no verification)
  -P <proxy>        - use proxy (format: '[http|https|socks4|socks5]://host:port')
                      (socks needs the 'httpx[socks]' / socksio package)

search options

  -s <str|file>     - a single string/regex or multile strings/regex in a file
                      to find in given URIs and HTTP response headers,
                      e.g.: 'tomcat 8', '/tmp/igot0daysforthese.txt'
  -S <where>        - search strings in given places (default: headers,body)
  -b <bytes>        - num bytes of context to show from a body match
                      (default: 64). NOTE: only the first 256 KB of a body
                      is read and searched (for speed)
  -i                - use case-insensitive search
  -1                - stop scanning a host after its first match (skips its
                      remaining uris, ports and search strings)

scan options

  -x <num>          - max concurrent connections (async; default: 1000). raise
                      ulimit -n accordingly for very high values
  -c <seconds>      - per-host connect + read timeout in seconds, also caps
                      body read time (default: 3.0)
  -G <seconds>      - global timeout: hard-stop the whole scan after N seconds
                      (safety net against any hang; default: none)
  -f <codes>        - only report responses with given HTTP status codes,
                      e.g.: '200', '200,301,302'
  -z                - scan targets in random order (for cidr-/host-range or
                      target file; loads all targets into memory first)
  -W                - save/resume: on ctrl+c write progress to httpgrep.session;
                      rerun with -W to resume from it (else start fresh)
  -T                - pull (v)hosts from the TLS cert (CN + SAN) and scan them
                      too: against the same target via host header AND the
                      hostname directly. needs TLS (-t or a *443 port).

output options

  -l <file>         - log found matches to <file>.<fmt> per chosen -O format
                      (e.g. -l out -O csv,jsonl => out.csv, out.jsonl)
  -O <formats>      - log file format(s), comma-list of: txt, csv, jsonl
                      (default: txt; use '?' to list). terminal output always
                      stays human-readable.
  -v                - verbose: print each url as it gets scanned

misc options

  -H                - print help
  -V                - print version information

examples

  # grep for 'apache' in headers and body of a single host
  $ httpgrep -h foobar.net -s apache

  # scan a CIDR range on port 8080, search for 'tomcat' in body only
  $ httpgrep -h 192.168.0.0/24 -p 8080 -s tomcat -S body

  # scan a host across multiple ports and a port range for 'jenkins'
  $ httpgrep -h 192.168.0.10 -p 80,443,8080,8000-8100 -s jenkins -i

  # scan host list, search string file, log matches (-> /tmp/out.txt)
  $ httpgrep -h /tmp/hosts.txt -s /tmp/strings.txt -x 200 -l /tmp/out

  # grep for 'admin' case-insensitively across multiple URIs via TLS
  $ httpgrep -h foobar.net -t -u /admin,/login,/dashboard -s admin -i

  # scan IP range, reverse DNS, only report 200 responses
  $ httpgrep -h 10.0.0.1-10.0.0.254 -s 'powered by' -r -f 200

  # search headers only, don't follow redirects, verbose output
  $ httpgrep -h foobar.net -s 'X-Powered-By' -S headers -F -v

  # route through burp, custom UA, search for version strings
  $ httpgrep -h /tmp/hosts.txt -s 'nginx/1\.' -P http://127.0.0.1:8080 -U 'curl/8.0'

  # scan with http basic auth, 30s timeout, random user-agent
  $ httpgrep -h foobar.net -s secret -a admin:password -c 30 -A

  # extract vhosts from TLS certs across a /24 and scan them too
  $ httpgrep -h 192.168.0.0/24 -p 443 -T -s admin

  # log matches to out.txt, out.csv AND out.jsonl in one run
  $ httpgrep -h 192.168.0.0/24 -p 80,443 -s admin -l out -O txt,csv,jsonl

  # only scan hosts with a valid TLS cert, log jsonl (-> found.jsonl)
  $ httpgrep -h /tmp/hosts.txt -t -s login -E -l found -O jsonl

  # big resumable scan: ctrl+c saves state, rerun with -W to continue; also
  # cap the whole run at 1 hour as a hang safety net
  $ httpgrep -h 10.0.0.0/16 -p 80,443 -s admin -W -G 3600
```

# Output

Matches are printed live, one per line:

```
[*] <url> | [vhost] | <type> | <match>
```

- `<url>`   - the scanned URL (`scheme://host:port/uri`).
- `<vhost>` - only present with `-T`: the cert (v)host tried via the `Host`
              header (empty for direct scans).
- `<type>`  - `body` or `header`.
- `<match>` - body hit: a short repr'd window from the match (`-b` bytes);
              header hit: `name: value`.

The terminal always shows this human-readable form. With `-l <base>` the same
matches are mirrored to `<base>.<fmt>` for each `-O` format - `txt` (these
lines), `csv` (`url,vhost,type,match` rows with a header), `jsonl` (one JSON
object per match).

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
