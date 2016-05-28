set incsav=%include%
set libsav=%lib%

set include=e:\mssdk\include;e:\ddk\inc32
set lib=e:\mssdk\lib;e:\ddk\lib

ml -coff -DBLD_COFF -DIS_32 -W2 -c -Cx -DMASM6 -Zd -DDEBUG VxDDef.asm
cl -c -Gs -Zdpl -Od -D_X86_ -YX -W3 -DDEBLEVEL=1 -DBLD_COFF -DDEBUG -DIS_32 IK220VxD.c
link -machine:i386 -def:IK220VxD.def -out:IK220VxD.vxd -debug -debugtype:map -map:IK220VxD.map -vxd vxdwraps.clb -nodefaultlib VxdDef.obj IK220VxD.obj

set include=%incsav%
set lib=%libsav%
set incsav=
set libsav=
