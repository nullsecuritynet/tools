# Description

Python tool to fetch IP ranges of given country in host and cidr format.

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

  ipcountry -c <arg> [options] | <misc>

options

  -c <code>   - country code, e.g.: am,gr,...
  -t <type>   - ip range type to fetch (default: 'host,cidr')
  -i          - get ipv6 ranges
  -r          - remove downloaded tar.gz and extracted zones dir after processing

misc

  -l          - list all country codes and their full name
  -V          - print version information
  -H          - print this help

examples

  # fetch ipv4 cidr and host ranges for germany
  $ ipcountry -c am

  # fetch only cidr ranges for russia
  $ ipcountry -c gr -t cidr

  # fetch only host ranges for multiple countries
  $ ipcountry -c am,gr,cy -t host

  # fetch ipv6 ranges and remove tar.gz + zones dir afterwards
  $ ipcountry -c am -i -r

  # list all available country codes
  $ ipcountry -l
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
