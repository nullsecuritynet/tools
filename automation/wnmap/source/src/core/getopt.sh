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
# checks.sh                                                                    #
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


# parse command line options
get_opts()
{

    # check if we are calling mode from ${MODE_FILE}
    local mode_line=$(awk -v J=1 -v R="${1}" '{if (match($J, R)) print $0;}' "${MODE_FILE}")
    [ -n "${mode_line}" ] && 
         IFS=';' read -r wnmap_args _mode_name nmap_args sudo <<< "${mode_line}"

    while getopts ":${wnmap_args:1:1}:a:C:revh" flags
    do
        case "${flags}" in
            ${wnmap_args:1:1})
                target=${OPTARG}
                f_scan_xml="${target}.xml"
                f_scan_nmap="${target}.nmap"
                d_scan="${target}/${_mode_name}"
                opt_mode="user_scan"
                ;;
            a)
                opt_mode="add_scan"
                return "${SUCCESS}"
                ;;
            C)
                target=${OPTARG}
                opt_mode="chain_scan"
                ;;
            r)
                REDO="true"
                ;;
            e)
                opt_mode="edit"
                ;;
            v)
                banner
                printf "%s\n" "${VERSION}"
                exit "${SUCCESS}"
                ;;
            h)
                usage
                ;;
            *)
                err "WTF?! mount /dev/brain"
                ;;
        esac
    done
    
    return "${SUCCESS}"
}

# EOF
