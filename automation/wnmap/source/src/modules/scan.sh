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
# scan.sh                                                                      #
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

check_exist() {

    if [ -f "${d_scan}/${f_scan_xml}" ] || [ -f "${d_scan}/${f_scan_nmap}" ]; then
        [[ -n "${REDO}" ]] && return $(rm -rf "${d_scan}")
        #implement ndiff for changes
        printf "[*] Server already scanned, redo? [y/N]: "; read a
        if [ "$a" != "y" ]; then
            printf "\n"
            critical "Scan canceled!"
            return 1
        fi
    fi
    return 0

}
 
scan() {

    banner

    #export scan files
    f_nmap_args="-oX $d_scan/$f_scan_xml -oN $d_scan/$f_scan_nmap"
    if ! check_exist; then
        critical "Scan canceled!"
    fi
    
    printf "[+] Creating %s\n" "${d_scan}"
    mkdir -p "${d_scan}"
    printf "[+] Scanning %s\n" "${target}"
    args="${nmap_args} ${DEFAULT_NMAP_ARGS} ${f_nmap_args} ${target}"
    
    if ${sudo}; then
        printf "[*] cmd: sudo %s %s\n" "$(which nmap)" "${args}"
        sudo $(which nmap) ${args}
    else
        printf "[*] cmd: %s %s\n" "$(which nmap)" "${args}"
        $(which nmap) ${args}
    fi
    
    return 0

}


# EOF
