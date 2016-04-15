UNIT ik220_soft_unit;

interface
(******************************************************************)
uses
  Windows,
  Math,
  display_unit1,
  ik220_def_unit;
(******************************************************************)
(******************************************************************)
procedure Build_reg0_inc(axis:word);
procedure Build_reg1_inc(axis:word);

procedure Build_reg0_mm(axis:word);
procedure Build_reg1_mm(axis:word);

procedure Build_reg0_degree(axis:word);
procedure Build_reg1_degree(axis:word);

(******************************************************************)
(******************************************************************)
implementation

uses
    ik220_rdwr_unit,
    adjust_unit,
    ik220_printer;

(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Calc_reg_mm(periode:word;
                      incr:double;
                      var wert:double);
begin
  (* Calculate mm*)
  wert:=incr*periode/10000;
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Calc_reg_degree(periode:word;
                          incr:double;
                          strichzahl:longint;
                          var wert:double);
begin
  (* Calculate Degree *)
  wert:=incr*360/strichzahl;
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Round_and_Dim(resolut:double;
                        dim:byte;
                        dimoffset:offset;
                        wert:double;
                        var ausgang:double);
var
  resolution : double;
begin
  (* Display Resolution *)
  resolution:=resolut/10000000;
  if resolution<0.000000001 then resolution:=0.000000001;
  wert:=wert/resolution;
  wert:=int(wert);
  wert:=wert*resolution;

  (* Add dimension offset *)
  ausgang:=wert+dimoffset[dim];
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure sorting(wert:double;
                  minbound,maxbound:double;
                  var min,max,typ:boolean);
begin
  if wert>maxbound then
    begin
      max:=true;
    end
  else
    begin
      max:=false;
    end;

  if wert<minbound then
    begin
      min:=true;
    end
  else
    begin
      min:=false;
    end;

   if (wert>=minbound) and
      (wert<=maxbound) then
    begin
      typ:=true;
    end
  else
    begin
      typ:=false;
    end;
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure minmax(wert:double;
                 minmax:tristatus;
                 var minwert,maxwert,ausgabe:double);
begin
   if minmax=min then
    begin
      if wert<minwert then
        begin
          minwert:=wert;
          ausgabe:=wert;
        end
      else
        begin
          ausgabe:=minwert;
        end;
    end;

  (* Max Mode *)
  if minmax=max then
    begin
      if wert>maxwert then
        begin
          maxwert:=wert;
          ausgabe:=wert;
        end
      else
        begin
          ausgabe:=maxwert;
        end;
    end;

  if minmax=typ then
    begin
      ausgabe:=wert;
    end;
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure make_string_mm(wert:double;
                         digits:byte;
                         var sign:char;
                         var value:string255);
var
   a,b   : byte;
begin

  (* Get algebraic sign *)
  if wert<0 then
    begin
      wert:=abs(wert);
      sign:='-';
    end
  else
    begin
      sign:='+';
    end;

  (* Integers *)
  a:=14-digits;
  (* Dec. places *)
  b:=digits;

  (* convert string *)
  str(wert:a:b,value);

end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure make_string_degree(wert:double;
                             digits:byte;
                             var arcsign:char;
                             var arcval:string255;
                             var turnsign:char;
                             var turnval:string255);
var
   a,b            : byte;
   arc,turns      : double;
begin

 (* Modulo 360 *)
  arc:=wert;
  arc:=arc-int(arc/360)*360;
  turns:=(wert-arc)/360;

  (* Get algebraic sign *)
  if turns<0 then
    begin
      turns:=abs(turns);
      turnsign:='-';
    end
  else
    begin
      turnsign:='+';
    end;

  if arc<0 then
    begin
      arc:=abs(arc);
      arcsign:='+';
    end;

  (* Integers *)
  a:=14-digits;
  (* Dec. places *)
  b:=digits;

  (* convert string *)
  str(arc:a:b,arcval);
  str(turns:6:0,turnval);

end;
(******************************************************************)
(*                                                                *)
(*                                                                *)
(*                                                                *)
(*                                                                *)
(*                                                                *)
(*                                                                *)
(*                                                                *)
(*                                                                *)
(******************************************************************)
procedure Build_reg0_inc(axis:word);
var
   dig    : byte;
   wert   : double;
BEGIN
  (* Get Value *)
  wert:=cnt^.count[axis]^.position.cnt_incr_reg0;
  (* No Dec. Places *)
  for dig:=1 to cnt^.count[axis]^.display.interpolation do
    begin
      wert:=wert*2;
    end;
  cnt^.count[axis]^.position.cnt_mm_reg0:=wert;
  cnt^.count[axis]^.position.counter_disp0:=wert;

  (* Build Output String-Value *)
  make_string_mm(wert,
                 cnt^.count[axis]^.display.digits,
                 cnt^.count[axis]^.position.sign_0,
                 cnt^.count[axis]^.position.value_0);
END;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Build_reg1_inc(axis:word);
var
   dig    : byte;
   wert   : double;
BEGIN
  (* Get Value *)
  wert:=cnt^.count[axis]^.position.cnt_incr_reg1;
  (* No Dec. Places *)
  for dig:=1 to cnt^.count[axis]^.display.interpolation do
    begin
      wert:=wert*2;
    end;
  cnt^.count[axis]^.position.cnt_mm_reg1:=wert;
  cnt^.count[axis]^.position.counter_disp1:=wert;

  (* Build Output String-Value *)
  make_string_mm(wert,
                 cnt^.count[axis]^.display.digits,
                 cnt^.count[axis]^.position.sign_1,
                 cnt^.count[axis]^.position.value_1);
END;
(***********************************************************************)
(*                                                                     *)
(* cnt_incr_reg0 -> cnt_mm_reg0 -> counter_disp_0 -> position.value_0  *)
(*                                                                     *)
(***********************************************************************)
procedure Build_reg0_mm(axis:word);
BEGIN
  (* Increments to mm *)
  Calc_reg_mm(cnt^.count[axis]^.coder.periode,
              cnt^.count[axis]^.position.cnt_incr_reg0,
              cnt^.count[axis]^.position.cnt_mm_reg0);

  (* Resolution and dimension *)
  Round_and_dim(cnt^.count[axis]^.display.resolut,
                cnt^.count[axis]^.position.dimension,
                cnt^.count[axis]^.position.dimoffset,
                cnt^.count[axis]^.position.cnt_mm_reg0,
                cnt^.count[axis]^.position.counter_disp0);

  (* Sorting *)
  sorting(cnt^.count[axis]^.position.counter_disp0,
          cnt^.count[axis]^.cntmode.minbound,
          cnt^.count[axis]^.cntmode.maxbound,
          cnt^.count[axis]^.cntmode.min,
          cnt^.count[axis]^.cntmode.max,
          cnt^.count[axis]^.cntmode.typ);

  (* Build Min/Max *)
  minmax(cnt^.count[axis]^.position.counter_disp0,
         cnt^.count[axis]^.cntmode.minmax,
         cnt^.count[axis]^.cntmode.minwert,
         cnt^.count[axis]^.cntmode.maxwert,
         cnt^.count[axis]^.position.counter_disp0);

  (* Build Output String-Value *)
  make_string_mm(cnt^.count[axis]^.position.counter_disp0,
                 cnt^.count[axis]^.display.digits,
                 cnt^.count[axis]^.position.sign_0,
                 cnt^.count[axis]^.position.value_0);
END;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Build_reg1_mm(axis:word);
BEGIN
  (* Increments to mm *)
  Calc_reg_mm(cnt^.count[axis]^.coder.periode,
              cnt^.count[axis]^.position.cnt_incr_reg1,
              cnt^.count[axis]^.position.cnt_mm_reg1);

  (* Resolution and dimension *)
  Round_and_dim(cnt^.count[axis]^.display.resolut,
                cnt^.count[axis]^.position.dimension,
                cnt^.count[axis]^.position.dimoffset,
                cnt^.count[axis]^.position.cnt_mm_reg1,
                cnt^.count[axis]^.position.counter_disp1);

  (* Build Output String-Value *)
  make_string_mm(cnt^.count[axis]^.position.counter_disp1,
                 cnt^.count[axis]^.display.digits,
                 cnt^.count[axis]^.position.sign_1,
                 cnt^.count[axis]^.position.value_1);
END;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Build_reg0_degree(axis:word);
var
   strichzahl : longint;
BEGIN

  case cnt^.count[axis]^.coder.inttype of
  ssi:
    begin
      strichzahl:=round(power(2,cnt^.count[axis]^.ssipara.code_bits));
    end;
  ndat:
    begin
      strichzahl:=cnt^.count[axis]^.coder.strichzahl;
    end
  else
    begin
      strichzahl:=cnt^.count[axis]^.coder.strichzahl;
    end;
  end;

  (* Increments to degree *)
  Calc_reg_degree(cnt^.count[axis]^.coder.periode,
                  cnt^.count[axis]^.position.cnt_incr_reg0,
                  strichzahl,
                  cnt^.count[axis]^.position.cnt_mm_reg0);

  (* Resolution and dimension *)
  Round_and_dim(cnt^.count[axis]^.display.resolut,
                cnt^.count[axis]^.position.dimension,
                cnt^.count[axis]^.position.dimoffset,
                cnt^.count[axis]^.position.cnt_mm_reg0,
                cnt^.count[axis]^.position.counter_disp0);

  (* Sorting *)
  sorting(cnt^.count[axis]^.position.counter_disp0,
          cnt^.count[axis]^.cntmode.minbound,
          cnt^.count[axis]^.cntmode.maxbound,
          cnt^.count[axis]^.cntmode.min,
          cnt^.count[axis]^.cntmode.max,
          cnt^.count[axis]^.cntmode.typ);

  (* Min Mode *)
  minmax(cnt^.count[axis]^.position.counter_disp0,
         cnt^.count[axis]^.cntmode.minmax,
         cnt^.count[axis]^.cntmode.minwert,
         cnt^.count[axis]^.cntmode.maxwert,
         cnt^.count[axis]^.position.counter_disp0);

  (* Build Output String-Value *)
  make_string_degree(cnt^.count[axis]^.position.counter_disp0,
                     cnt^.count[axis]^.display.digits,
                     cnt^.count[axis]^.position.sign_0,
                     cnt^.count[axis]^.position.value_0,
                     cnt^.count[axis]^.position.turnsign_0,
                     cnt^.count[axis]^.position.turnvalue_0);
END;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Build_reg1_degree(axis:word);
var
   strichzahl : longint;
BEGIN

  case cnt^.count[axis]^.coder.inttype of
  ssi:
    begin
      strichzahl:=round(power(2,cnt^.count[axis]^.ssipara.code_bits));
    end;
  ndat:
    begin
      strichzahl:=cnt^.count[axis]^.coder.strichzahl;
    end
  else
    begin
      strichzahl:=cnt^.count[axis]^.coder.strichzahl;
    end;
  end;

  (* Increments to degree *)
  Calc_reg_degree(cnt^.count[axis]^.coder.periode,
                  cnt^.count[axis]^.position.cnt_incr_reg1,
                  strichzahl,
                  cnt^.count[axis]^.position.cnt_mm_reg1);

  (* Resolution and dimension *)
  Round_and_dim(cnt^.count[axis]^.display.resolut,
                cnt^.count[axis]^.position.dimension,
                cnt^.count[axis]^.position.dimoffset,
                cnt^.count[axis]^.position.cnt_mm_reg1,
                cnt^.count[axis]^.position.counter_disp1);

  (* Build Output String-Value *)
  make_string_degree(cnt^.count[axis]^.position.counter_disp1,
                     cnt^.count[axis]^.display.digits,
                     cnt^.count[axis]^.position.sign_1,
                     cnt^.count[axis]^.position.value_1,
                     cnt^.count[axis]^.position.turnsign_0,
                     cnt^.count[axis]^.position.turnvalue_0);
END;
(******************************************************************)
(******************************************************************)
end.
