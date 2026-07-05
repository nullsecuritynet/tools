# Description

Python tool to fetch IP ranges of a given country in host, cidr or jsonl format,
and to reverse lookup which country an ip belongs to.

# Usage

```
$ ipcountry -H
    _                              __
   (_)___  _________  __  ______  / /________  __
  / / __ \/ ___/ __ \/ / / / __ \/ __/ ___/ / / /
 / / /_/ / /__/ /_/ / /_/ / / / / /_/ /  / /_/ /
/_/ .___/\___/\____/\__,_/_/ /_/\__/_/   \__, /
 /_/                                    /____/

      --== [ by nullsecurity.net ] ==--

usage

  ipcountry <mode> [options] | <misc>

mode

  -c <code>   - fetch ip ranges for country code(s), e.g.: am,gr,... ('all' = every one)
  -x <ip>     - reverse lookup ip(s) -> country code ('-' reads ips from stdin)

options

  -t <type>   - ip range type to fetch (default: 'host,cidr')
  -o <file>   - write ranges to <file> ('-' for stdout) instead of per-country files
  -j          - write ranges as jsonl (one {country,type,range} object per line)
  -i          - get ipv6 ranges
  -r          - remove downloaded tar.gz and extracted zones dir after processing

misc

  -l          - list all country codes and their full name
  -V          - print version information
  -H          - print this help

examples

  # fetch cidr + host ranges for multiple countries
  $ ipcountry -c am,gr,cy

  # fetch ranges for all countries at once
  $ ipcountry -c all

  # stream ranges to stdout for piping ('-' = stdout)
  $ ipcountry -c ru -t cidr -o -

  # reverse lookup: which country owns an ip ('-' reads ips from stdin)
  $ ipcountry -x 8.8.8.8
```

# Author

noptrix

# Notes

- quick'n'dirty code
- ipcountry is already packaged and available for [BlackArch Linux](https://www.blackarch.org/)
- My master-branches are always stable; dev-branches are created for current work.
- All of my public stuff you find are officially announced and published via [nullsecurity.net](https://www.nullsecurity.net).

# LICENSE

Check docs/LICENSE.

# Disclaimer

We hereby emphasize, that the hacking related stuff found on
[nullsecurity.net](http://nullsecurity.net) are only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
