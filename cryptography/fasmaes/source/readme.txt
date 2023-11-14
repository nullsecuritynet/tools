###############################################################################
#                                                                             #
#         ~    .__ °.__   0       o                    ^   .__ °__  `´        #
#  °____) __ __|  | | °|   ______°____ 0 ____  __ _________|__|/  |_ ___.__.  #
#  /    \|  | °\  |°|  | °/  ___// __ \_/ ___\|  | °\_  __ \ o\   __<   |  |  #
# | o°|  \  |  /  |_|  |__\___ \\  ___/\ °\___| o|  /|  | \/  ||  |° \___ O|  #
# |___|  /____/|____/____/____ °>\___  >\___  >____/ |__|° |__||__|  / ____|  #
# `´´`´\/´`nullsecurity team`´\/`´´`´\/`´``´\/  ``´```´```´´´´`´``0_o\/´´`´´  #
#                                                                             #
# AES Implementation for Flat Assembler                                       #
#                                                                             #
# VERSION                                                                     #
# 1.4                                                                         #
#                                                                             #
# DATE                                                                        #
# 12.12.2019                                                                  #
#                                                                             #
# AUTHOR                                                                      #
# belial - http://www.nullsecurity.net/                                       #
#                                                                             #
# LICENSE                                                                     #
# BSD-License                                                                 #
#                                                                             #
# DESCRIPTION                                                                 #
# An AES-128, AES-192 and AES-256 implementation for FASM. Uses the the x86   #
# 32-bit or 64-bit instruction set and operates completely on the stack. No   #
# data segments are necessary which makes it easy to integrate the AES        #
# functions in any existing project. The implementation is not optimized      #
# for speed but for easy maintainability.                                     #
#                                                                             #
# Consists of two functions:                                                  #
#                                                                             #
# - encAES(size, source, destination, key)                                    #
# - decAES(size, source, destination, key)                                    #
#                                                                             #
# "Size" has to be an integer multiple of 16 (the AES block size). The        #
# function encAES() encrypts the data in the "source" buffer with the         #
# corresponding "key" and stores the result in "destination". The function    #
# decAES() decrypts the data in "source" and stores the result in the         #
# "destination" buffer. They key size (128, 192 and 256) can be set by        #
# a switch in the "aes.inc" file. Comes with two examples:                    #
#                                                                             #
# aesexample1.asm:      Simple exe which encrypts and decrypts a text         #
# aesexample2.asm:      Produces a dll which exports the encryption and       #
#                       decryption functions                                  #
# aesexampel2test.asm:  calls the generated dll                               #
#                                                                             #
# Examples use windows APIs but the implementation itself can be used on any  #
# x86 plattform.                                                              #
#                                                                             #
# TODO                                                                        #
# - Not yet tested against the complete set of AES test vectors               #
# - different block cypher modes are missing                                  #
#                                                                             #
# CHANGELOG:                                                                  #
#                                                                             #
# v1.4:                                                                       #
# - removed macros to decrease size by 7kb                                    #
# - changed sbox, galois lookup table and rcon generation                     #
#                                                                             #
# v1.3:                                                                       #
# - fixed bug: little/big endian byte order                                   #
# - fixed bug: wrong implementation of shiftRows                              #
#                                                                             #
# v1.2:                                                                       #
# - added a 64 bit port                                                       #
#                                                                             #
# v1.1:                                                                       #
# - set .reloc sections to readable, otherwise dll can not be loaded in       #
#   in Windows 10                                                             #
#                                                                             #
# v1.0:                                                                       #
# - initial release                                                           #
#                                                                             #
###############################################################################
