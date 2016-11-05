# libraries
arduino library elements rounded up over time

These are some libraries that I've got shields for, but the source of their support
libraries doesn't have a good mechanism to get or maintain their source (like on GitHub!)

One of the libraries that *doesn't* fit this bill is the CAN_BUS_Shield, which has good
support found at https://github.com/Seeed-Studio/CAN_BUS_Shield

# library list

## CAN_BUS_Shield:
Forked from https://github.com/Seeed-Studio/CAN_BUS_Shield and has accepted at least one documentation
update I submitted.  It's not actually in this repository, as my fork lives at https://github.com/wdross/CAN_BUS_Shield.
I put it on my .gitignore list so it doesn't keep showing up in my list with a 'git status'.

## ITDB02_Graph16
ITDB02_Graph16 library to support SainSmart 3.2" LCD shield.
Source found at https://s3-ap-northeast-1.amazonaws.com/sain-amzn/20/20-011-846+/ITDB02_Graph16.rar, which
was the link given on the Amazon listing I purchased it through.  It was an old library, as the compiler
I was using at the time (1.6.12) complained about DefaultFonts.c's bitmaps should be defined as const and it wouldn't
actually compile due to #include "WProgram.h", which needed to be "arduino.h".  Oh, and the extensions of
the examples needed to be changed to .ino from the earlier arduino source .pde extension.  I also used
translate.google.com to understand and update the comments.
