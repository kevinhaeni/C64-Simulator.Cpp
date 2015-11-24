#!/bin/bash
# write a something

awk -F'@' -e '{
     {
	#printf("{ 0x%s, \"%s\", \"%s\", \"%s\" },\n", $4, $2, $7, $6);
	 printf("0x%s @ , \"%s\", \"%s\", \"%s\" },\n", $4, $2, $7, $6);
    }

}' < codebase-c64.txt 
