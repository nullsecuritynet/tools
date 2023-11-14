#!/bin/bash
################################################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# wssh - ssh lazy wrapper                                                      #
#                                                                              #
# FILE                                                                         #
# wssh.sh                                                                      #
#                                                                              #
# DATE                                                                         #
# 2015-02-12                                                                   #
#                                                                              #
# AUTHOR                                                                       #
# nrz@nullsecurity.net                                                         #
#                                                                              #
################################################################################


MODE_FILE="/home/${USER}/.wssh/wssh.conf"

# dont check fingerprint
DONT_CHECK_HOST='-o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no'

# wssh version
VERSION="wssh v0.1"

# true / false
FALSE="0"
TRUE="1"

# return codes
SUCCESS="1337"
FAILURE="31337"

# verbose mode - default: quiet
VERBOSE="/dev/null"

# colors
WHITE="$(tput bold ; tput setaf 7)"
GREEN="$(tput setaf 2)"
RED="$(tput bold; tput setaf 1)"
YELLOW="$(tput bold ; tput setaf 3)"
NC="$(tput sgr0)" # No Color


rprintf() {
    fmt=$1
    shift
    printf "%s${fmt}%s\n" "${RED}" "$@" "${NC}"

    return "${SUCCESS}"
}

wprintf() {
    fmt=$1
    shift
    printf "%s${fmt}%s\n" "${WHITE}" "$@" "${NC}"

    return "${SUCCESS}"
}

# print warning
warn()
{
    fmt=${1}
    shift
    printf "%s[!] WARNING: ${fmt}%s\n" "${RED}" "${@}" "${NC}"

    return "${SUCCESS}"
}

# print error and exit
err()
{
    fmt=${1}
    shift
    printf "%s[-] ERROR: ${fmt}%s\n" "${RED}" "${@}" "${NC}"

    return "${FAILURE}"
}

# print error and exit
cri()
{
    fmt=${1}
    shift
    printf "%s[-] CRITICAL: ${fmt}%s\n" "${RED}" "${@}" "${NC}"

    exit "${FAILURE}"
}

# usage and help
usage()
{
cat <<EOF
Usage: wssh [mode|<opt>] [<overwrite>]
OPTIONS:
    -m <mode>: mode/id to machine
    -e: edit conf file [${MODE_FILE}]
    -l: list modes
    -S <file_upload/dir_upload>: scp to machine
    -s <file_download/dir_download: scp from machine
    -d <remote_dst>: change file where upload or download file [default:~/]
OVERWRITE:
    -u <user>: switch user
    -h <host>: switch host
    -p <port>: switch port
    -c: disable check host fingerprint [default:true]
    -x <cmd>: extra commands
MISC:
    -V: print version of wssh and exit
    -H: print this help and exit
EXAMPLE:
    wssh -m b0x -x'-D1234' # socks proxy mode
    wssh -m b0x -S file -d '/data/' # upload file to /data
    wssh -m b0x -s file # download file from home
    wssh -m b0x -p 1337 -S file # use diff port for b0x config uploading file to home
    wssh b0x # no shitty args anymore - just fucking show me the shell :)
EOF
    exit "${SUCCESS}"
}


# leet banner, very important
banner()
{
    echo "--==[ wssh by nrz@nullsecurity.net ]==--"

    return "${SUCCESS}"
}


# check argument count
check_argc()
{
    if [ "${#}" -lt 1 ]
    then
        usage
    fi

    return "${SUCCESS}"
}


# check if required arguments were selected
check_args()
{
    return "${SUCCESS}"
}


# parse command line options
get_opts()
{
    while getopts m:elS:s:d:u:h:p:cx:vVH flags
    do
        case "${flags}" in
            m)
                MODE=${OPTARG}
                ;;
            e)
                vim "${MODE_FILE}"
                exit "${SUCCESS}"
                ;;
            l)
                cat "${MODE_FILE}"
                exit "${SUCCESS}"
                ;;
            S)
                SCP=true
                UPLOAD="${OPTARG}"
                ;;
            s)
                SCP=true
                if [ "${OPTARG:0:1}" == "/" ]; then
                    DOWNLOAD=${OPTARG}
                else
                    DOWNLOAD="~/${OPTARG}"
                fi
                ;;
            d)
                DOWN_UP_DIR=${OPTARG}
                ;;
            u)
                OPT_USER=${OPTARG}
                ;;
            h)
                OPT_HOST=${OPTARG}
                ;;
            p)
                OPT_PORT=${OPTARG}
                ;;
            c)
                OPT_CHECK=true
                ;;
            x)
                OPT_EXTRA=${OPTARG}
                ;;
            v)
                VERBOSE="/dev/stdout"
                ;;
            V)
                echo "${VERSION}"
                exit "${SUCCESS}"
                ;;
            H)
                usage
                ;;
            *)
                err "WTF?! mount /dev/brain"
                ;;
        esac
    done

    return "${SUCCESS}"
}

set_vars()
{
    if [ -n "${MODE}" ]; then
        local mode_line=$(awk -v J=1 -v R="${MODE}" '{if (match($J, R)) print $0;}' "${MODE_FILE}")
        if [ -n "${mode_line}" ]; then
            IFS=';' read -r MODE HOST PORT USER CHECK EXTRA <<< "${mode_line}"
        else
            cri "mode not found"
        fi
    fi
    # set vars - use config from file if not arguments or just defaults
    [ -n "${OPT_USER}" ] && USER=${OPT_USER}
    [ -n "${OPT_HOST}" ] && HOST=${OPT_HOST}
    [ -n "${OPT_PORT}" ] && PORT=${OPT_PORT}
    [ -z "${PORT}" ] && PORT=22
    [ -n "${OPT_CHECK}" ] && CHECK=false
    [ -n "${OPT_EXTRA}" ] && EXTRA=${OPT_EXTRA}

    # scp
    DOWNLOAD_DIR="."
    UPLOAD_DIR="~/"
    [ -n "${DOWN_UP_DIR}" ] && DOWNLOAD_DIR="${DOWN_UP_DIR}/"
    [ -n "${DOWN_UP_DIR}" ] && UPLOAD_DIR="${DOWN_UP_DIR}/"

    return
}

create_mode_file()
{
    mkdir -p /home/${USER}/.wssh
    printf "# id;ip;port[def:22];user;check_host_fingerprint;extra_opt\n" >> ${MODE_FILE}
    printf "sample;192.168.1.1;4564;nrz;false;-L 8080:localhost:80\n" >> "${MODE_FILE}"

    return
}

# controller and program flow
main()
{
    # defaults
    CHECK=true
    SCP=false

    #gogogo
    banner

    #check mode file
    ! [ -f "${MODE_FILE}" ] && create_mode_file

    # ssh(lazy(lazy())) -> $ wssh <mode>
    if [ "$#" == "1" ] && ! [ "${1:0:1}" == "-" ]; then
        MODE=${1}
    else
        check_argc ${*}
        get_opts ${*}
        check_args ${*}
    fi

    set_vars

    if ${SCP}; then
        printf "[+] scp(%s:%s) %s\n" "${HOST}" "${PORT}" "${EXTRA}"
    else
        printf "[+] ssh(%s:%s) %s\n" "${HOST}" "${PORT}" "${EXTRA}"
    fi

    if ${CHECK}; then
        if ${SCP}; then
            if [ -n "${UPLOAD}" ]; then
                scp -r -P "${PORT}" ${UPLOAD} "${USER}@${HOST}:${UPLOAD_DIR}"
            else
                scp -r -P "${PORT}" "${USER}@${HOST}:${DOWNLOAD}" "${DOWNLOAD_DIR}"
            fi
        else
            ssh -p "${PORT}" "${USER}@${HOST}" ${EXTRA}
        fi
    else
        if ${SCP}; then
            if [ -n "${UPLOAD}" ]; then
                scp -r ${DONT_CHECK_HOST} -P "${PORT}" ${UPLOAD} "${USER}@${HOST}:${UPLOAD_DIR}"
            else
                scp -r ${DONT_CHECK_HOST} -P "${PORT}" "${USER}@${HOST}:${DOWNLOAD}" "${DOWNLOAD_DIR}"
            fi
        else
            ssh ${DONT_CHECK_HOST} -p "${PORT}" "${USER}@${HOST}" ${EXTRA}
        fi
    fi

    return "${SUCCESS}"
}

main ${*}

# EOF
