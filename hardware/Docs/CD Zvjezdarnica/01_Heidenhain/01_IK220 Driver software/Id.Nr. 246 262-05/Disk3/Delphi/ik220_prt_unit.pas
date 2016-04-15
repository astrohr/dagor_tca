UNIT ik220_prt_unit;

interface
(******************************************************************)
uses
  Windows,
  display_unit1,
  ik220_cnt_unit,
  ik220_def_unit;
(******************************************************************)
(******************************************************************)
PROCEDURE Init_Printer(cprint:print_pointr);
PROCEDURE Wr_Setup_Printer;
PROCEDURE Rd_Setup_Printer;
procedure print_wert(cprint:print_pointr);
procedure print_ram(cprint:print_pointr);
procedure reset_printer(cprint:print_pointr);
(******************************************************************)
(* DLL-functions                                                  *)
(******************************************************************)
implementation

uses
    ik220_printer,
    ik220_rdwr_unit,
    ik220_soft_unit,
    ik220_base_unit;
(******************************************************************)
(******************************************************************)
PROCEDURE Init_Printer(cprint:print_pointr);
BEGIN
    cprint^.drvname:='c';
    cprint^.dirname:='';
    cprint^.filename:='ik220.prt';
END;
(******************************************************************)
(* Save setup in file:  IK220.PNT                                 *)
(******************************************************************)
PROCEDURE Wr_Setup_Printer;
VAR
  initfile        : file of printstore;
  buffer          : printstore;
BEGIN
  AssignFile(initfile,'ik220.pnt');
  {$I-}
  Rewrite(initfile);
  {$I+}
  if ioresult=0 then
    begin
      buffer.drvname  := druck^.drvname;
      buffer.dirname  := druck^.dirname;
      buffer.filename := druck^.filename;
      write(initfile,buffer);
      CloseFile(initfile);
    end;
END;
(******************************************************************)
(* Load setup                                                     *)
(******************************************************************)
PROCEDURE Rd_Setup_Printer;
VAR
  initfile          : file of printstore;
  buffer            : printstore;
  nofile            : boolean;
BEGIN
  init_printer(druck);
  nofile:=true;
  AssignFile(initfile,'ik220.pnt');
  {$I-}
  Reset(initfile);
  {$I+}
  if ioresult=0 then
    begin
      {$I-}
      read(initfile,buffer);
      {$I+}
      if ioresult=0 then
        begin
          druck^.drvname  := buffer.drvname;
          druck^.dirname  := buffer.dirname;
          druck^.filename := buffer.filename;
          nofile:=false;
        end;
      CloseFile(initfile);
    end;
  if nofile then
    begin
      Wr_Setup_Printer;
    end;
END;
(******************************************************************)
(******************************************************************)
procedure reset_printer(cprint:print_pointr);
begin
  AssignFile(printfile,cprint^.filename);
  {$I-}
  Rewrite(printfile);
  {$I+}
  if ioresult=0 then
    begin
      CloseFile(printfile);
    end
  else
    begin
      output.printer_error.caption:='ERROR';
    end;
end;
(******************************************************************)
(******************************************************************)
procedure print_xy(count:count_pointr);
var
  axis : word;
  s    : string;
begin
  for axis:=0 to xnumber do
    begin
     str(axis,s);
     writeln(printfile,'Axis'+s+' : ',count^.count[axis]^.position.sign_0,count^.count[axis]^.position.value_0);
    end;
  writeln(printfile);
end;
(******************************************************************)
(******************************************************************)
procedure print_block;
var
  axis,i           : word;
  s                : string;
  ende             : boolean;
  cntval           : double;
  rzeig,wzeig,stat : word;
begin
   axis:=cnt^.axis;
   ende:=false;
   i:=0;
   while not(ende) do
     begin
       Status:=IK220GetRam(axis,@cntval,@rzeig,@wzeig,@stat);
       if stat=2 then
         begin
           ende:=true;
         end  
       else
         begin
           cnt^.count[axis]^.position.cnt_incr_reg0:=cntval;
           Build_reg0_mm(axis);
           str(i,s);
           writeln(printfile,'Ram ',i,' : ',cnt^.count[axis]^.position.sign_0,
                                            cnt^.count[axis]^.position.value_0);
           i:=i+1;
         end;
     end;
end;
(******************************************************************)
(******************************************************************)
procedure print_wert(cprint:print_pointr);
begin
  AssignFile(printfile,cprint^.filename);
  {$I-}
  Append(printfile);             (* when file exists *)
  {$I+}
  if ioresult=0 then
    begin
      print_xy(cnt);
      CloseFile(printfile);
    end
  else
    begin
      {$I-}
      Rewrite(printfile);        (* if new file *)
      {$I+}
      if ioresult=0 then
        begin
          print_xy(cnt);
          CloseFile(printfile);
        end
      else
        begin
          output.printer_error.caption:='ERROR';
        end;
    end;
end;
(******************************************************************)
(******************************************************************)
procedure print_ram(cprint:print_pointr);
begin
  AssignFile(printfile,cprint^.filename);
  {$I-}
  Append(printfile);             (* when file exists *)
  {$I+}
  if ioresult=0 then
    begin
      print_block;
      CloseFile(printfile);
    end
  else
    begin
      {$I-}
      Rewrite(printfile);        (* if new file *)
      {$I+}
      if ioresult=0 then
        begin
          print_block;
          CloseFile(printfile);
        end
      else
        begin
          output.printer_error.caption:='ERROR';
        end;
    end;
end;
(******************************************************************)
(******************************************************************)
end.
