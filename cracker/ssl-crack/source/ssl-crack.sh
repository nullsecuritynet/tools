#!/bin/bash
################################################################################
#                ____                     _ __                                 #
#     ___  __ __/ / /__ ___ ______ ______(_) /___ __                           #
#    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           #
#   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            #
#                                            /___/ team                        #
#                                                                              #
# ssl-crack.sh - wordlist-based encrypted SSL and SSH Private Key Passphase    #
# Cracker                                                                      #
#                                                                              #
# FILE                                                                         #
# ssl-crack.sh                                                                 #
#                                                                              #
# DATE                                                                         #
# 2013-08-31                                                                   #
#                                                                              #
# DESCRIPTION                                                                  #
# Loads a wordlist file into memory and reveals the password for the RSA       #
# encrypted private SSL / SSH key                                              #
#                                                                              #
# AUTHOR                                                                       #
# TheXero                                                                      #
#                                                                              #
################################################################################


WORDLIST='/home/thexero/lists/passwords.txt'


echo """
                            ____                     _ __                                 
                 ___  __ __/ / /__ ___ ______ ______(_) /___ __                           
                / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                           
               /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                            
                                                        /___/ team                        

     *********************************************************************
                 SSL / SSH Private Key Passphase Cracker v1.0                
     *********************************************************************

"""

if [ -z "$1" ]
then 
    echo "Enter path to encrypted SSL / SSH private key"
    read KEYFILE
else
    KEYFILE=$1
fi

for PASSWORD in $(cat $WORDLIST);

do
    
    if [[ -n $(echo -n $PASSWORD | openssl rsa -in $KEYFILE -passin stdin 2>&1 |
        grep -v unable |grep -v error) ]]  
        then
        echo -e 'Password for SSL / SSH Key is:' $PASSWORD 
        echo -n $PASSWORD | openssl rsa -in $KEYFILE -passin stdin 2>&1
        
        exit
    fi

done
