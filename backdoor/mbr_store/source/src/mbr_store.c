/*
 * mbr_store.c
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include <nettle/aes.h>
#include <nettle/sha.h>
#include <nettle/serpent.h>
#include <nettle/twofish.h>

#include "mbr_store.h"
#include "optget.h"


/* encrypt via hashed key */
void
enlarge_key(char *pw)
{
    struct sha256_ctx ctx;
    
    uint8_t digest[SHA256_DIGEST_SIZE];
       
    sha256_init(&ctx);
    sha256_update(&ctx, strlen(pw) , (uint8_t *)pw);
    sha256_digest(&ctx, SHA256_DIGEST_SIZE, digest);
    
    memcpy(key, digest, SHA256_DIGEST_SIZE);
    
    return;
}


/* crc32 checksum, for interal data integrity check */
uint32_t 
crc32(uint8_t *data, uint16_t len)
{
    uint32_t crc32  = 0;
    uint16_t i      = 0;
    
    for (i = 0; i < len; i++)
    {
        if (((crc32 & 0x80000000) ? 1 : 0) != data[i]) 
        {
             crc32 = (crc32 << 1) ^ CRC32POLY;
        }
        else 
        {
             crc32 <<= 1;
        }
    }
    
    return( crc32 );
}
    

void
encrypt(uint8_t *data, uint16_t len)
{
    uint8_t *aes1   = calloc(1, len);
    uint8_t *serp2  = calloc(1, len);
    uint8_t *two3   = calloc(1, len);
    
    struct aes_ctx *ctx_aes    = calloc(1, sizeof(struct aes_ctx    ));
    struct serpent_ctx *ctx_sp = calloc(1, sizeof(struct serpent_ctx));
    struct twofish_ctx *ctx_tf = calloc(1, sizeof(struct twofish_ctx));
    
    aes_set_encrypt_key(ctx_aes, sizeof(key), key);
    serpent_set_key    (ctx_sp,  sizeof(key), key);
    twofish_set_key    (ctx_tf,  sizeof(key), key);
    
    aes_encrypt    (ctx_aes, len, aes1,  data);
    serpent_encrypt(ctx_sp,  len, serp2, aes1);
    twofish_encrypt(ctx_tf,  len, two3, serp2);
    
    memcpy(data, two3, len);
    
    free(aes1);
    free(serp2);
    free(two3);
    
    free(ctx_tf);
    free(ctx_sp);
    free(ctx_aes);
    
    return;
}


void
decrypt(uint8_t *data, uint16_t len)
{
    uint8_t *two1   = calloc(1, len);
    uint8_t *serp2  = calloc(1, len);
    uint8_t *aes3   = calloc(1, len);
    
    struct twofish_ctx *ctx_tf = calloc(1, sizeof(struct twofish_ctx));
    struct serpent_ctx *ctx_sp = calloc(1, sizeof(struct serpent_ctx));
    struct aes_ctx *ctx_aes = calloc(1, sizeof(struct aes_ctx));
    
    twofish_set_key    (ctx_tf, sizeof(key), key);
    serpent_set_key    (ctx_sp, sizeof(key), key);
    aes_set_decrypt_key(ctx_aes, sizeof(key), key);
    
    twofish_decrypt(ctx_tf,  len , two1 ,  data);
    serpent_decrypt(ctx_sp,  len , serp2 , two1);
    aes_decrypt    (ctx_aes, len , aes3 , serp2);
    
    memcpy(data, aes3, len);
    
    free(two1);
    free(serp2);
    free(aes3);
    
    free(ctx_tf);
    free(ctx_sp);
    free(ctx_aes);
    
    return;
}


void
data_print(uint8_t *data)
{
    data_header *dh  = (data_header *) data;
    uint16_t     len = ntohs(dh->len);
    uint16_t     i   = 0;
    
    printf("  len:        %d bytes\n",len);
    
    if( len > DATA_SIZE) 
    {
        fprintf(stderr, "error: data len too big\n");
        __EXIT_FAILURE;
    }
        
    if( ntohl(dh->crc) != crc32(dh->data, DATA_SIZE) )
    {
        fprintf(stderr, "warning: CRC32 invalid\n");
    }
    else 
    {
        printf("  crc32:      valid 0x%x\n", ntohl(dh->crc) );
    }
    
    
    printf("\n -- DATA ----------------------------------------\n");
    
    for(i=0; i< len; i++) {
        printf("%c",dh->data[i]);
    }

    printf("\n ------------------------------------------------\n");
    
    return;
}


void
user_readin(uint8_t *data)
{
    data_header *dh = (data_header *) data;
    char *f         = calloc(1, DATA_SIZE);
    uint16_t len    = 0;
    int i           = 0;
        
    printf("please enter your data [426 bytes]: ");
    
    if( fgets(f, DATA_SIZE, stdin) == NULL)
    {
		fprintf(stderr, "error: fgets failed\n");
        __EXIT_FAILURE;
	}
    
    len = strlen(f);
    len--;
    dh->len = htons(len);
    
    printf("> saved len:  %d\n",len);
    
    memcpy(dh->data, f, len);
    
    for( i=len; i< DATA_SIZE; i++) {
        memset(&dh->data[i], __RAND , 1);
    }
    
    dh->crc = htonl( crc32(dh->data, DATA_SIZE) );
    
    for( i=0; i< 8; i++) {
        memset(&dh->payload[i], __RAND , 1);
    }

    return;
}


void 
mbr_store(int fd, uint8_t *mbr)
{
    if( lseek(fd, 0, SEEK_SET) != 0 )
    {
        fprintf(stderr, "error: lseek failed\n");
        __EXIT_FAILURE;
    }
    
    if( write(fd, mbr, MBR_SIZE) != MBR_SIZE)
    {
        fprintf(stderr, "error: writing to device file failed\n");
        __EXIT_FAILURE;
    }
    
    return;
}


void
mbr_read(int fd, uint8_t *buf)
{
    if( read(fd, buf, MBR_SIZE) != MBR_SIZE) 
    {
        fprintf(stderr, "error: reading from device file failed\n");
        __EXIT_FAILURE;
    }
    
    return;
}


/* check the MBR's 0x55aa signature */
void
mbr_check_signature(mbr_struct *mbr)
{
    if( !( ntohs(mbr->signature) == 0x55aa) ) 
    {
        fprintf(stderr, "error: MBR structure not valid (reading signature 0x55aa failed)\n");
        __EXIT_FAILURE;
    }
    
    printf("  signature:  valid\n");
    
    return;
}


/* check the nullz */
void
mbr_check_nulls(mbr_struct *mbr)
{
    if( !(mbr->nulls == 0x0000) ) 
    {
        fprintf(stderr, "error: MBR structure not valid (reading nulls failed)\n");
        __EXIT_FAILURE;
    }
    
    return;
}


/* search GRUB pattern in bootloader code */
void
mbr_check_grub(uint8_t *data)
{
    uint16_t i = 0;
    
    for(i=0; i< 436; i++) {
        if( data[i] == 'G' && data[i+1] == 'R' && data[i+2] == 'U' && data[i+3] == 'B') 
        {
            fprintf(stderr, "warning: mbr data contains GRUB\n");
        }
    }
    
    return;
}


int
device_open(const char *dev) 
{
    int fd = open(dev , O_RDWR);
    
    if( fd < 0 )
    {
        fprintf(stderr, "error: opening device file failed\n");
        __EXIT_FAILURE;
    }
    
    return(fd);
}


void
print_banner()
{
    printf(
    "        _              _               \n"
    "  _____| |_ ___    ___| |_ ___ ___ ___ \n"
    " |     | . |  _|  |_ -|  _| . |  _| -_|\n"
    " |_|_|_|___|_|    |___|_| |___|_| |___|\n"
    "                              atzeton |\n"
    "                     nullsecurity.net |\n"
    "              use it at your own risk |\n"
    "                          version %3s |\n"
    "\n", VERSION);
    
    return;
}

int main(int argc, char **argv)
{
    uint8_t     *buf = calloc(1, MBR_SIZE);
    mbr_struct  *mbr = (mbr_struct *) buf;
    int          fd  = 0;
    
    srand ( time(NULL) );
    
    optget(argc, argv);
    optcheck();
    print_banner();

    fd = device_open(device);
    
    if( __ENCRYPTED ) 
    {
        printf("> encryption enabled:    aes(serpent(twofish()))\n");
        printf("> using hashed password: sha256(password)\n");
        enlarge_key(password);
    }
    else 
    {   
        printf("> encryption disabled. Data is stored in plaintext!\n");
    }

    printf("> reading device\n");
    
    mbr_read(fd, buf);
    
    printf("> checking for present GRUB on device\n");
    mbr_check_grub(mbr->data);
    
    mbr_check_signature(mbr);
    mbr_check_nulls(mbr);
    
    printf("  nulls:      valid\n");
    printf("  ms_disk_id: %04x\n",mbr->disk_sig);


    if( __READ ) 
    {
        if( __ENCRYPTED ) 
        {
            decrypt(mbr->data, 432);
        }
    
        data_print(mbr->data);
    }
    else if ( __STORE )
    {   
        user_readin(mbr->data);
    
        if( __ENCRYPTED ) 
        {
            encrypt(mbr->data, 432);
        }

        mbr_store(fd, buf);
        
    }
    else {
        fprintf(stderr, "error: nothing to do xD\n");
    }
    
    mbr = NULL;
    free(buf);
    close(fd);
    
    
    return 0;
}

