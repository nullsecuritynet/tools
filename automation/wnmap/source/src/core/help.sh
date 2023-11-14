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
# help.sh                                                                      #
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


# leet banner, very important
banner()
{
    printf "%s\n" "--==[ wnmap by nrz@nullsecurity.net ]==--"
    return ${SUCCESS}
}


# usage and help
usage()
{
    banner
    cat <<EOF
Usage: ${APP_NAME} [options] <target specification> | <misc>

TARGET SPECIFICATION:
    Can pass hostname or IP address.

OPTIONS:
    SCANNING - USER MODE:
EOF
    while IFS=';' read -r wnmap_arg mode_name nmap_args _;do
        [[ ${wnmap_arg} = \#* ]] || [[ -z ${wnmap_arg} ]] && continue
        printf "    %s: '%s' scan - nmap args: %s\n" "$wnmap_arg" "$mode_name" "$nmap_args"
    done < "${MODE_FILE}"
    cat <<EOF
    SCANNING - STANDARD MODE:
    -C: chain all modes
    LOAD NEW SCAN:
    -a {wnmap_arg;mode_name;nmap_args;need_sudo?[true,false]}: add new scan to user mode

MISC:
    -r: rescan host by default
    -e: edit ${MODE_FILE##*/}
    -v: print version of wnmap and exit
    -h: print this help and exit

EXAMPLES:
    ${APP_NAME} -F 8.8.8.8 -r # scan again
    ${APP_NAME} -C scanme.nmap.org # give it all you got
    ${APP_NAME} -a "-I;iddle-scan;-P0 -p- -sI sweet.host.com;true"
    ${APP_NAME} -e # edit modes
EOF
    exit "${SUCCESS}"
}

# EOF
