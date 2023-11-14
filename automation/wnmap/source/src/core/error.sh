################################################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# wnmap - modular nmap wrapper and automation script                           #
#                                                                              #
# FILE                                                                         #
# error.sh                                                                     #
#                                                                              #
# DATE                                                                         #
# 2013-06-19                                                                   #
#                                                                              #
# DESCRIPTION                                                                  #
# A shell script written with the purpose to automate and chain scans via nmap.#
# You can run nmap with a custom mode written by user and create directories   #
# for every mode with the xml/nmap files inside.                               #
#                                                                              #
# AUTHOR                                                                       #
# nrz@nullsecurity.net                                                         #
#                                                                              #
################################################################################


# print warning
warn()
{
    printf "[!] WARNING: %s\n" "${*}"
    return "${SUCCESS}"
}

# print error
err()
{
    printf "\e[1;31m[-] ERROR: %s\e[m\n" "${*}"
    return "${SUCCESS}"
}

# print critical and exit
critical()
{
    banner
    printf "\e[1;31m[-] %-6s\e[m\n" "${*}" 
    exit "${FAILURE}"
}

# EOF
