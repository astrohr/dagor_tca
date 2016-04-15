unit ik220_conv_unit;

interface

{$R+}

type
   string2 = array[1..2] of char;
   string4 = array[1..4] of char;

{*******************************************************************
PROCEDUREN
*******************************************************************}

function  nib2hex(wert:byte):char;
function  hex2nib(wert:char):byte;

procedure word2hex(wert:word;var e:string4);
procedure hex2word(wert:string4;var e:word);

procedure word_2_char(wert:word;var a,b,c,d:byte);
function  char_2_word(a,b,c,d:char):word;

function bin_2_adr(line:string):word;
function bin_2_data(line:string):word;

implementation
{*******************************************************************
*******************************************************************}
function nib2hex(wert:byte):char;
var
   x : char;
begin
  x:='0';
  case wert of
    0 : x:='0';
    1 : x:='1';
    2 : x:='2';
    3 : x:='3';
    4 : x:='4';
    5 : x:='5';
    6 : x:='6';
    7 : x:='7';
    8 : x:='8';
    9 : x:='9';
    10: x:='A';
    11: x:='B';
    12: x:='C';
    13: x:='D';
    14: x:='E';
    15: x:='F';
  end;

  nib2hex:=x;

end;
{*******************************************************************
*******************************************************************}
function hex2nib(wert:char):byte;
var
  x : byte;
begin
  x:=0;
  case wert of
    '0' : x:=0;
    '1' : x:=1;
    '2' : x:=2;
    '3' : x:=3;
    '4' : x:=4;
    '5' : x:=5;
    '6' : x:=6;
    '7' : x:=7;
    '8' : x:=8;
    '9' : x:=9;
    'A' : x:=$A;
    'B' : x:=$B;
    'C' : x:=$C;
    'D' : x:=$D;
    'E' : x:=$E;
    'F' : x:=$F;
  end;

  hex2nib:=x;

end;
{*******************************************************************
*******************************************************************}
procedure word2hex(wert:word;var e:string4);
var
  a        : byte;
  x,y      : word;

begin

  x:=wert;
  y:=x and $000F;
  a:=y;
  e[1]:=nib2hex(a);

  x:=x shr 4;
  y:=x and $000F;
  a:=y;
  e[2]:=nib2hex(a);

  x:=x shr 4;
  y:=x and $000F;
  a:=y;
  e[3]:=nib2hex(a);

  x:=x shr 4;
  y:=x and $000F;
  a:=y;
  e[4]:=nib2hex(a);

end;
{*******************************************************************
*******************************************************************}
procedure hex2word(wert:string4;var e:word);
var
  x         : byte;
  y         : char;
  a,b,c,d   : word;
begin
  y:=wert[4];
  x:=hex2nib(y);
  a:=x;
  y:=wert[3];
  x:=hex2nib(y);
  b:=x;
  y:=wert[2];
  x:=hex2nib(y);
  c:=x;
  y:=wert[1];
  x:=hex2nib(y);
  d:=x;
  a:=a shl 12;
  b:=b shl 8;
  c:=c shl 4;
  e:=a or b or c or d;
end;
{*******************************************************************
*******************************************************************}
procedure word_2_char(wert:word;var a,b,c,d:byte);
var
  value : string4;
begin
  word2hex(wert,value);
  a:=ord(value[4]);
  b:=ord(value[3]);
  c:=ord(value[2]);
  d:=ord(value[1]);
end;
{*******************************************************************
*******************************************************************}
function char_2_word(a,b,c,d:char):word;
var
  wert : word;
begin

  wert:=0;

  case d of
  '1' : wert:=wert + $0001;
  '2' : wert:=wert + $0002;
  '3' : wert:=wert + $0003;
  '4' : wert:=wert + $0004;
  '5' : wert:=wert + $0005;
  '6' : wert:=wert + $0006;
  '7' : wert:=wert + $0007;
  '8' : wert:=wert + $0008;
  '9' : wert:=wert + $0009;
  'A' : wert:=wert + $000A;
  'B' : wert:=wert + $000B;
  'C' : wert:=wert + $000C;
  'D' : wert:=wert + $000D;
  'E' : wert:=wert + $000E;
  'F' : wert:=wert + $000F;
  end;

  case c of
  '1' : wert:=wert + $0010;
  '2' : wert:=wert + $0020;
  '3' : wert:=wert + $0030;
  '4' : wert:=wert + $0040;
  '5' : wert:=wert + $0050;
  '6' : wert:=wert + $0060;
  '7' : wert:=wert + $0070;
  '8' : wert:=wert + $0080;
  '9' : wert:=wert + $0090;
  'A' : wert:=wert + $00A0;
  'B' : wert:=wert + $00B0;
  'C' : wert:=wert + $00C0;
  'D' : wert:=wert + $00D0;
  'E' : wert:=wert + $00E0;
  'F' : wert:=wert + $00F0;
  end;

  case b of
  '1' : wert:=wert + $0100;
  '2' : wert:=wert + $0200;
  '3' : wert:=wert + $0300;
  '4' : wert:=wert + $0400;
  '5' : wert:=wert + $0500;
  '6' : wert:=wert + $0600;
  '7' : wert:=wert + $0700;
  '8' : wert:=wert + $0800;
  '9' : wert:=wert + $0900;
  'A' : wert:=wert + $0A00;
  'B' : wert:=wert + $0B00;
  'C' : wert:=wert + $0C00;
  'D' : wert:=wert + $0D00;
  'E' : wert:=wert + $0E00;
  'F' : wert:=wert + $0F00;
  end;

  case a of
  '1' : wert:=wert + $1000;
  '2' : wert:=wert + $2000;
  '3' : wert:=wert + $3000;
  '4' : wert:=wert + $4000;
  '5' : wert:=wert + $5000;
  '6' : wert:=wert + $6000;
  '7' : wert:=wert + $7000;
  '8' : wert:=wert + $8000;
  '9' : wert:=wert + $9000;
  'A' : wert:=wert + $A000;
  'B' : wert:=wert + $B000;
  'C' : wert:=wert + $C000;
  'D' : wert:=wert + $D000;
  'E' : wert:=wert + $E000;
  'F' : wert:=wert + $F000;
  end;

  char_2_word:=wert;

end;
{*******************************************************************
*******************************************************************}
function lst_2_code(line:string):word;
var
  code : word;
begin

  code:=char_2_word(line[7],line[8],line[9],line[10]);
  lst_2_code:=code;

end;
{*******************************************************************
*******************************************************************}
function lst_2_adr(line:string):word;
var
  adr : word;
begin

  adr:=char_2_word(line[1],line[2],line[3],line[4]);
  lst_2_adr:=adr;

end;
{*******************************************************************
*******************************************************************}
function bin_2_adr(line:string):word;
var
  adr : word;
begin

  adr:=char_2_word(line[20],line[21],line[22],line[23]);
  bin_2_adr:=adr;

end;
{*******************************************************************
*******************************************************************}
function bin_2_data(line:string):word;
var
  data : word;
begin
  data:=0;
  if line[16]='1'  then data:=data or $0001;
  if line[15]='1'  then data:=data or $0002;
  if line[14]='1'  then data:=data or $0004;
  if line[13]='1'  then data:=data or $0008;
  if line[12]='1'  then data:=data or $0010;
  if line[11]='1'  then data:=data or $0020;
  if line[10]='1'  then data:=data or $0040;
  if line[9]='1'   then data:=data or $0080;
  if line[8]='1'   then data:=data or $0100;
  if line[7]='1'   then data:=data or $0200;
  if line[6]='1'   then data:=data or $0400;
  if line[5]='1'   then data:=data or $0800;
  if line[4]='1'   then data:=data or $1000;
  if line[3]='1'   then data:=data or $2000;
  if line[2]='1'   then data:=data or $4000;
  if line[1]='1'   then data:=data or $8000;

  bin_2_data:=data;

 end;
{*******************************************************************
                                ENDE
*******************************************************************}
end.