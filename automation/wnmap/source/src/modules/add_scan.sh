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
# add_scan.sh                                                                  #
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

add_scan() {
    new_line=${*:2}
    wnmap_mode=${2:0:2}
    
    banner
    
    # dont duplicate wnmap_modes!
    find_arg=$(awk -F";" -v J=1 -v R="${wnmap_mode}" '{if (match($J, R)) print $0;}' "${MODE_FILE}")
    if [ -n "$find_arg" ]; then
        err "You fail. ${find_arg:0:2} opt already exist."
        exit "${FAILURE}"
    else
        printf "%s\n" "${new_line}" >> "${MODE_FILE}"
        printf "[+] New mode added!\n"
        exit "${SUCCESS}"
    fi

}

# EOF
