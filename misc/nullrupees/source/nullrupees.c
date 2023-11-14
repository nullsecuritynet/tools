/*******************************************************************************
 *                ____                     _ __                                *
 *     ___  __ __/ / /__ ___ ______ ______(_) /___ __                          *
 *    / _ \/ // / / (_-</ -_) __/ // / __/ / __/ // /                          *
 *   /_//_/\_,_/_/_/___/\__/\__/\_,_/_/ /_/\__/\_, /                           *
 *                                            /___/ nullsecurity team          *
 *                                                                             *
 *                                                                             *
 * Simple trainer which patches "The Legend of Zelda" for Famicom/NES          *
 * After usage, each item in the NPC shops is for free.                        *
 *                                                                             *
 * DATE                                                                        *
 * 02/10/2012                                                                  *
 *                                                                             *
 * TECHNICAL DETAILS:                                                          *
 *                                                                             *
 * According to                                                                *
 * http://datacrystal.romhacking.net/wiki/The_Legend_of_Zelda:RAM_map          *
 * the players rupees are stored in Famicoms RAM at address 0x066D.            *
 * I disassembled                                                              *
 * my Zelda ROM file and found the following code at 0x000048DD and            *
 * 0x000048EE:                                                                 *
 *                                                                             *
 * lda $066D                                                                   *
 * cmp $0430,x                                                                 *
 * bcc $FFFFC952                                                               *
 * lda $0430,x                                                                 *
 * jsr $89DE                                                                   *
 *                                                                             *
 * It loads the current amount of rupees into A and compares it with the item  *
 * prize. If the player has enough rupees, the game calls $89DE which adds the *
 * item to the players backpack.                                               *
 *                                                                             *
 * The trainer simply overwrites both "bcc" instructions with "nops".          *
 *                                                                             *
 * Enjoy :-)                                                                   *
 *                                                                             *
 * AUTHOR                                                                      *
 * belial - http://www.nullsecurity.net/                                       *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define ROM_SIZE 131088
#define FIRST_BCC 0x48E3
#define SECOND_BCC 0x48FD

int main(int argc, char *argv[]){
  int input_size = 0;
  unsigned char* input = 0;
  FILE* input_handle = 0;
  FILE* output_handle = 0;

  if(argc!=3){
    printf("Legend of Zelda (NES) NullRupees Trainer\n");
    printf("Written by Belial/Nullsecurity\n\n");
    printf("Usage: ./nullrupees <infile> <outfile>\n");
    return 0;
  }

  /* open input file */
  input_handle = fopen(argv[1],"r");
  if(input_handle == NULL){
    printf("Error: Could not open %s\n", argv[1]);
    return -1;
  }

  /* obtain input file size: */
  fseek (input_handle , 0 , SEEK_END);
  input_size = ftell (input_handle);
  rewind (input_handle);
  if(input_size < ROM_SIZE){
    printf("Error: Input file has wrong file size\n");
    return -1;
  }

  /* copy input file to memory */
  input = (unsigned char*) malloc(input_size);
  fread((void*) input, 1, input_size, input_handle);
  fclose(input_handle);

  /* check for bcc instruction */
  if(input[FIRST_BCC] != 0x90){
    printf("Error: Couldn't find first bcc opcode\n");
    free((void*) input);
    return -1;
  }
  else if(input[SECOND_BCC] != 0x90){
    printf("Error: Couldn't find second bcc opcode\n");
    free((void*) input);
    return -1;
  }

  /* patch them and write to output file */
  input[FIRST_BCC] = 0xea;
  input[FIRST_BCC+1] = 0xea;
  input[SECOND_BCC] = 0xea;
  input[SECOND_BCC+1] = 0xea;
  output_handle = fopen(argv[2],"w");
  if(output_handle == NULL){
    printf("Error: Could not open %s\n", argv[2]);
    free((void*) input);
    return -1;
  }
  fwrite((void*) input, 1, input_size, output_handle);
  fclose(output_handle);

  /* Ill be back! */
  printf("The Legend of Zelda was successfully patched :-)\n");
  free((void*) input);
  return 0;
}
