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

# check software required
check_soft()
{
    command -v nmap >/dev/null 2>&1 || 
        critical "Nmap required. Install it!"

    command -v "${EDITOR}" >/dev/null 2>&1 || 
        critical "${EDITOR} required. Install it or change EDITOR variable in src/core/globals.sh!"
}

# check argument count
check_argc()
{
    [ "${#}" -eq "0" ] && usage && exit "${SUCCESS}"
}


# check if required arguments were selected
check_args()
{
    return "${SUCCESS}"
}


# EOF
