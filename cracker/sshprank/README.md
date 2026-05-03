# Description

A fast SSH mass-scanner, login cracker, banner grabber and password auth
checker tool using the python-masscan and shodan module.

# Usage

```
[ hacker@blackarch ~ ]$ sshprank -H
              __                           __
   __________/ /_  ____  _________ _____  / /__
  / ___/ ___/ __ \/ __ \/ ___/ __ `/ __ \/ //_/
 (__  |__  ) / / / /_/ / /  / /_/ / / / / ,<
/____/____/_/ /_/ .___/_/   \__,_/_/ /_/_/|_|
               /_/

      --== [ by nullsecurity.net ] ==--

usage

  sshprank <mode> [opts] | <misc>

mode options

  -h <hosts[:ports]>    - single host, cidr, ip range or host list file to
                          crack. multiple ports can be separated by comma,
                          e.g.: 127.0.0.1:22,222,2022 or 192.168.1.0/24:22
                          or 192.168.1.10-192.168.1.50:22 or 10.0.0.1-50
                          (default port: 22)

  -m <opts> [-r <num>]  - pass arbitrary masscan opts, portscan given hosts and
                          crack for logins. found sshd services will be saved to
                          'sshds.txt' in supported format for '-h' option and
                          even for '-b'. use '-r' for generating random ipv4
                          addresses rather than scanning given hosts. these
                          options are always on: '-sS -oX - --open'.
                          NOTE: if you intent to use the '--banner' option then
                          you need to specify '--source-ip <some_ipaddr>' which
                          is needed by masscan. better check masscan options!

  -s <str;page;lim>     - search ssh servers using shodan and crack logins.
                          see examples below. note: you need a better API key
                          than this one i offer in order to search more than 100
                          (= 1 page) ssh servers. so if you use this one use
                          '1' for 'page'.

  -b <hosts[:ports]>    - grab sshd banner from given target(s)
                          (default port: 22)
                          format: same as '-h' option

  -p <hosts[:ports]>    - check sshd(s) for password auth support
                          (default port: 22)
                          format: same as '-h' option

scan options

  -r <num>              - generate <num> random ipv4 addresses, check for open
                          sshd port and crack for login (only with -m option!)

credential options

  -U <user|file>        - single username or user list (default: root)
  -P <pass|file>        - single password or password list (default: root)
  -c <file>             - list of user:pass combination

brute options

  -e                    - exclude host after first login was found. continue
                          with other hosts instead
  -E                    - exit sshprank completely after first login was found
  -z                    - shuffle target list randomly before cracking
                          (only with -h <file>). saves to 'random_targets.txt'
  -Z <num>              - random brute: pick random target + creds each attempt.
                          <num> total attempts, 0 = infinite (use with -h, -U/-P)

exec options

  -C <cmd|file>         - read commands from file (line by line) or execute a
                          single command on host if login was cracked
  -N                    - do not output ssh command results

thread options

  -x <num>              - num threads for parallel host crack (default: 50)
  -S <num>              - num threads for parallel service crack (default: 20)
  -X <num>              - num threads for parallel login crack (default: 5)
  -B <num>              - num threads for parallel banner grabbing (default: 70)

timeout options

  -T <sec>              - num sec for auth and connect timeout (default: 5s)
  -R <sec>              - num sec for (banner) read timeout (default: 3s)

output options

  -o <file>             - write found logins to file. format:
                          <host>:<port>:<user>:<pass> (default: owned.txt)
  -v                    - verbose mode. show found logins, sshds, etc.
                          (default: off)

misc options

  -i <str>              - spoof ssh client version string sent to sshd
                          (default: paramiko's default version string)
  -w <file>             - session file: if it exists, restore progress from
                          it (skip already-tried creds). on ctrl+c / -E,
                          state is auto-saved to ./sshprank_session.json
                          (or to <file> if -w was given). pass it back via
                          -w to resume.
  -H                    - print help
  -V                    - print version information

examples

  # crack targets from a given list with user admin, pw-list and 20 host-threads
  $ sshprank -h sshds.txt -U admin -P /tmp/passlist.txt -x 20

  # first scan then crack from founds ssh services using 'root:admin'
  $ sudo sshprank -m '-p22,2022 --rate 5000 --source-ip 192.168.13.37 \
    --range 192.168.13.1/24' -P admin

  # generate 1k random ipv4 addresses, then port-scan (tcp/22 here) with 1k p/s
  # and crack logins using 'root:root' on found sshds
  $ sudo sshprank -m '-p22 --rate=1000' -r 1000 -v

  # search 50 ssh servers via shodan and crack logins using 'root:root' against
  # found sshds
  $ sshprank -s 'SSH;1;50'

  # grab banners and output to file with format supported for '-h' option
  $ sshprank -b hosts.txt > sshds2.txt

  # check if sshds support password auth
  $ sshprank -p sshds.txt -v

  # shuffle target list and crack
  $ sshprank -h sshds.txt -z -U root -P /tmp/passes.txt

  # random brute: 500 random attempts from ip/user/pass lists
  $ sshprank -h sshds.txt -U /tmp/users.txt -P /tmp/passes.txt -Z 500

  # random brute infinite (ctrl+c to stop)
  $ sshprank -h sshds.txt -U /tmp/users.txt -P /tmp/passes.txt -Z 0

  # spoof ssh client version and crack
  $ sshprank -h sshds.txt -i 'SSH-2.0-OpenSSH_8.9p1'

  # check pwauth with spoofed version
  $ sshprank -p sshds.txt -i 'SSH-2.0-OpenSSH_7.4' -v

  # session file: ctrl+c, then run again to resume
  $ sshprank -h sshds.txt -U /tmp/users.txt -P /tmp/passes.txt -w sess.json
```

# Author

noptrix

# Notes

- quick'n'dirty code
- sshprank is already packaged and available for [BlackArch
  Linux](https://www.blackarch.org/)
- My master-branches are always stable; dev-branches are created for current
  work.
- All of my public stuff you find are officially announced and published via
  [nullsecurity.net](https://www.nullsecurity.net).

# License

Check docs/LICENSE.

# Disclaimer
We hereby emphasize, that the hacking related stuff found on
[nullsecurity.net](http://nullsecurity.net/) are only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
