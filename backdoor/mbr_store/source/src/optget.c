/*
 * optget.c
 * 
 * Copyright 2012 atzeton <atzeton@lavabit.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "optget.h"
#include "mbr_store.h"


void
optget(int argc, char** argv) 
{
    int32_t index      = -1;
    int32_t result     =  0;
    struct option *opt = NULL;
    
    static const struct option long_options[] = {
        { "help",     	no_argument,           0, 0 },
        { "read",     	no_argument,           0, 0 },
        { "store",     	no_argument,           0, 0 },   
        { "password",  	required_argument,     0, 0 },
        { "device",  	required_argument,     0, 0 },
        {0,0,0,0} 
    };
    
    while (optind < argc) {
        result = getopt_long(argc, argv, "", long_options, &index);
        if (result != 0) {
            fprintf(stderr,"getopt failed\n");
            __EXIT_FAILURE;
        }
        else if( result == 0) {
            opt = (struct option *)&(long_options[index]);
                
            if( strcmp(opt->name,"help") == 0) {
                usage();
                __EXIT_SUCCESS;
            }
            if( strcmp(opt->name,"read") == 0) {
                mode = MODE_READ;
            }
            if( strcmp(opt->name,"store") == 0) {
                mode = MODE_STORE;
            }
            else if( strcmp(opt->name,"password") == 0 ) {
                if( opt->has_arg == required_argument ) {
                    encrypted = 1;
                    password = optarg;
                }
            }
            else if( strcmp(opt->name,"device") == 0 ) {
                if( opt->has_arg == required_argument ) {
                    device = optarg;
                }
            }
            
        }
    }
    
    return;
}


void
optcheck()
{
    if (!device)
    {
        fprintf(stderr,"error: no device or file given\n");
        __EXIT_FAILURE;
    }
    
    if( !password) 
    {
        encrypted = 0;
    }
    
    return;
}


void
usage()
{
    printf("usage:\n"
    "./mbr_store [--store|--read|--help] [--password=<password>] [--device=<device/file>]\n\n"
    "--help\n"
    "\tshow help\n"
    "--store\n"
    "\tsave data in the MBR's bootloader code section\n"
    "--read\n"
    "\tread data, watch out to to give the correct password if required\n"
    "--password=<pw>\n"
    "\tenable aes(serpent(twofish())) encryption using <pw>'s sha256 hash\n"
    "--device=<device/file>\n"
    "\tspecify the device (e.g. /dev/sdd) or file (e.g. mbr_image)\n\n"
    "> for more detailed information take a look at the man file\n");

    return;
}
