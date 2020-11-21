from capstone import *
import sys

###
# Minimal ARM Cortex-M disassembler.
# Input: a filename pointing to a file containing a
# hexadecimal string containing sparse Thumb-2 machine code.
#
# This script depends on the 'SKIPDATA' option in Capstone,
# which is currently only available in the 'next' branch.
# So check that branch out and build it from source, don't
# use a "pip install capstone" version.
#
# To save the output to a file, run:
# "python3 hex_str_disas.py <input_filename> | tee <output_filename>"
#
# Just because the disassembler prints out an instruction doesn't
# mean that the memory address contains valid code. Much of the
# Game and Watch SPI Flash probably contains sprites, sounds, etc.
###

with open( sys.argv[ 1 ], 'r' ) as f:
  flash_log = f.read()
  flash_log_bytes = bytes.fromhex( flash_log.strip() )

md = Cs( CS_ARCH_ARM, CS_MODE_THUMB )
md.skipdata = True
for ( addr, size, mnemonic, op_str ) in md.disasm_lite( flash_log_bytes, 0x90000000 ):
  print( "0x%x:\t%s\t%s"%( addr, mnemonic, op_str ) )
