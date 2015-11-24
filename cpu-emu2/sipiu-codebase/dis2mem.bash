#!/bin/bash

## Address  Hexdump   Dissassembly
## -------------------------------
## $0600    a2 07     LDX #$07
## $0602    a5 fe     LDA $fe

awk -e 'begin { RS="[[:space:]][[:space:]][[:space:]]+"; pc= 0x600; }
{ m1 = $1; m2=$2; m3=$3; 

print $1;
print $2;
print $3;
}
' | sed '/^\s*$/d'  | awk -e 'BEGIN {pc= 0x600} { printf("%04x %s\n", pc++, $0);}'
