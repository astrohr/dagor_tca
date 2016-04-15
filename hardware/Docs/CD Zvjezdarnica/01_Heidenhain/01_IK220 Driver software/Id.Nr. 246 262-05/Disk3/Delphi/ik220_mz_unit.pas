unit ik220_mz_unit;

interface

{$R+}

const wait         = 0;    (* 0 wait *)
(* const wait         = 32;   (* 1 wait *)
(* const wait         = 64;   (* 2 wait *)

const bootmz        = wait+1;
const bootrd        = wait+1+4;
const bootwr        = wait+1+8;

const startmz       = wait+0;
const restartmz     = wait+16;

const debugmz       = wait+2;
const debugrd       = wait+2+4;
const debugwr       = wait+2+8;

const stepmz        = wait+1+2;
const stepwr        = wait+1+2+8;
const steprestart   = wait+1+2+16;

const befehlreg     = 12;
const mzstatflag    = 12;
const mzrdflag      = 13;
const mzwrflag      = 14;

{*******************************************************************
PROCEDUREN
*******************************************************************}
procedure wr_mz_port(axis:word;regadr:word;datum:word);
function  rd_mz_port(axis:word;regadr:word):word;
procedure wr_mz_ram(axis:word;adr,data:word);
function  rd_mz_ram(axis:word;adr:word):word;

procedure mzstart(axis:word);
procedure mzrestart(axis:word);
procedure mzhalt(axis:word);

function wr_fprogram(axis:word;name:string):boolean;
function cmp_program(axis:word;name:string):boolean;

procedure Download(var error:boolean);
{*******************************************************************
*******************************************************************}
implementation

uses
  ik220_def_unit,
  ik220_conv_unit,
  ik220_base_unit;
{*******************************************************************
*******************************************************************}
procedure wr_mz_port(axis:word;regadr:word;datum:word);
var
  ldata  : word; (*longint; *)
begin
  ldata:=datum;
  status:=Ik220Output(axis,regadr,ldata);
end;
{*******************************************************************
*******************************************************************}
function rd_mz_port(axis:word;regadr:word):word;
var
  data   : word;
begin
  status:=Ik220Inputw(axis,regadr,@data);
  rd_mz_port:=data;
end;
{*******************************************************************
   Poll bis RAM-Adresse und Datum aus Port gelesen
*******************************************************************}
procedure poll_rd_done(axis:word);
var
  datum : word;
begin
    repeat
      datum:=rd_mz_port(axis,mzrdflag);
      datum:=datum and $0003;
    until (datum=$0000);
end;
{*******************************************************************
   Poll bis Datum aus RAM in Port geschrieben
*******************************************************************}
procedure poll_wr_done(axis:word);
var
  datum : word;
begin
    repeat
      datum:=rd_mz_port(axis,mzwrflag);
      datum:=datum and $0002;
    until (datum=$0002);
end;
{*******************************************************************
   ein Wort in MZUMA RAM schreiben
*******************************************************************}
procedure wr_mz_ram(axis:word;adr,data:word);
begin
    wr_mz_port(axis,befehlreg,bootmz);       (* Bootmode *)
    wr_mz_port(axis,0,adr);                  (* Adr  : Port 0 *)
    wr_mz_port(axis,1,data);                 (* Data : Port 1 *)
    wr_mz_port(axis,befehlreg,bootwr);       (* Bootwr *)
    poll_rd_done(axis);                      (* Poll bis Adr & Daten gelesen *)
    wr_mz_port(axis,befehlreg,bootmz);       (* Bootmode *)
end;
{*******************************************************************
   ein Wort aus MZUMA-RAM lesen
*******************************************************************}
function rd_mz_ram(axis:word;adr:word):word;
begin
    wr_mz_port(axis,befehlreg,bootmz);       (* Bootmode *)
    wr_mz_port(axis,0,adr);                  (* Adr  : Port 0 *)
    wr_mz_port(axis,befehlreg,bootrd);       (* Bootrd *)
    poll_wr_done(axis);                      (* Poll bis Datum geschrieben *)
    rd_mz_ram:=rd_mz_port(axis,1);           (* Datum holen *)
    wr_mz_port(axis,befehlreg,bootmz);       (* Bootmode *)
end;
{*******************************************************************
*******************************************************************}
procedure mzstart(axis:word);
begin
  wr_mz_port(axis,befehlreg,bootmz);    (* Bootmode *)
  wr_mz_port(axis,befehlreg,startmz);   (* und start *)
end;
{*******************************************************************
*******************************************************************}
procedure mzrestart(axis:word);
begin
  wr_mz_port(axis,befehlreg,restartmz);   (* restart *)
  wr_mz_port(axis,befehlreg,startmz);     (* und start *)
end;
{*******************************************************************
*******************************************************************}
procedure mzhalt(axis:word);
begin
  wr_mz_port(axis,befehlreg,bootmz);
end;
{*******************************************************************
Transfer Program
*******************************************************************}
function wr_lprogram(axis:word;name:string):boolean;
var
    binfile  : text;
    a,line   : string;
    d        : string4;
    adr,data : word;
begin
  Assign(binfile,name);
  {$I-}
  Reset(binfile);
  {$I+}

  if ioresult=0 then
    begin
      wr_lprogram:=true;
      while not(eof(binfile))do
        begin
          readln(binfile,line);
          data:=bin_2_data(line);
          adr:=bin_2_adr(line);
          if line[1]<>'$' then
            begin
              str(adr,a);
              word2hex(data,d);
              wr_mz_ram(axis,adr,data);
            end
        end;
      close(binfile);
    end
  else
    begin
      wr_lprogram:=false;
    end;
end;
{*******************************************************************
Transfer Program
*******************************************************************}
procedure wr_block_ram(axis:word;
                       daten:loadram;
                       anzahl:longint);
var
  dzeiger  : wordptr;
begin
  dzeiger:=@daten[0];
  Status:=IK220DownLoad(axis,dzeiger,anzahl);
end;
{*******************************************************************

*******************************************************************}
function wr_fprogram(axis:word;name:string):boolean;
var
    binfile  : text;
    line     : string;
    index,
    adr,data : word;
    ende     : boolean;
    number   : longint;
    dbuffer  : loadram;

begin
  Assign(binfile,name);
  {$I-}
  Reset(binfile);
  {$I+}

  if ioresult=0 then
    begin
      wr_fprogram:=true;

      while not(eof(binfile))do
        begin
          readln(binfile,line);
          if line[1]<>'$' then
            begin
              data:=bin_2_data(line);
              adr:=bin_2_adr(line);

              dbuffer[0]:=axis;
              dbuffer[1]:=adr;
              dbuffer[2]:=data;

              index:=3;
              number:=6;
              ende:=false;
              while not(eof(binfile)) and not(ende) do
                begin
                  readln(binfile,line);
                  if line[1]='$' then
                    begin
                      ende:=true;
                    end
                  else
                    begin
                      data:=bin_2_data(line);
                      dbuffer[index]:=data;
                      index:=index+1;
                      number:=number+2;
                      if index=256 then ende:=true;
                    end;
                end;
              wr_block_ram(axis,dbuffer,number);
            end;

        end;
      close(binfile);
    end
  else
    begin
      wr_fprogram:=false;
    end;
end;
{*******************************************************************
   Programm vergleichen
*******************************************************************}
function cmp_program(axis:word;name:string):boolean;
var
    binfile        : text;
    a,line         : string;
    d              : string4;
    adr,data,tdata : word;
    error          : boolean;
begin
  Assign(binfile,name);
  {$I-}
  Reset(binfile);
  {$I+}
  if ioresult=0 then
    begin
      error:=false;
      while not(eof(binfile)) and not(error) do
        begin
          readln(binfile,line);
          data:=bin_2_data(line);
          adr:=bin_2_adr(line);
          if line[1]<>'$' then
            begin
              str(adr,a);
              word2hex(data,d);
              tdata:=rd_mz_ram(axis,adr);
              if tdata<>data then
                begin
                  error:=true;
                  str(adr,a);
                end;
            end;
        end;
      close(binfile);
    end
  else
    begin
      error:=true;
    end;
  cmp_program:=not(error);
end;
(******************************************************************)
(******************************************************************)
procedure Download(var error:boolean);
var
  axis  : word;
begin
  error:=false;
  for axis:=0 to xnumber do
    begin
      mzhalt(axis);
      error:=error or not(wr_fprogram(axis,'ik2.bin'));
      mzstart(axis);
    end;
end;
{******************************************************************)
(******************************************************************}
end.
