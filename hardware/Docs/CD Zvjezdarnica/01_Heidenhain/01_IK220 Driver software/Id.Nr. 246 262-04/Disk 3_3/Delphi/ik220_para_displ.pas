unit ik220_para_displ;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls,Buttons, ExtCtrls,
  ik220_def_unit;


type
  Tpara_display = class(TForm)
    Label1: TLabel;
    Label5: TLabel;
    digits1: TEdit;
    resolution1: TEdit;
    x_sys_1_pan: TPanel;
    x_sys_2_pan: TPanel;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    x_inv_1_pan: TPanel;
    Label7: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    minbound: TEdit;
    maxbound: TEdit;
    minmax_1_pan: TPanel;
    minmax_2_pan: TPanel;
    Label18: TLabel;
    Label21: TLabel;
    para_def: TButton;
    para_save: TButton;
    para_load: TButton;
    BitBtn1: TBitBtn;
    axis: TPanel;
    Panel1: TPanel;
    Panel2: TPanel;
    Label27: TLabel;
    intp: TEdit;
    Label28: TLabel;
    Label30: TLabel;
    Label31: TLabel;
    x_sys_3_pan: TPanel;
    Label34: TLabel;
    Label35: TLabel;
    Label39: TLabel;
    setup_ok: TBitBtn;
    Label3: TLabel;
    Label4: TLabel;
    Label8: TLabel;
    Label6: TLabel;
    no_l1: TPanel;
    no_l0: TPanel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label19: TLabel;
    Label20: TLabel;
    line0_reg0: TPanel;
    line0_ssi: TPanel;
    line1_reg1: TPanel;
    line0_blockram: TPanel;
    line1_ssi: TPanel;
    Label25: TLabel;
    Label26: TLabel;
    Label29: TLabel;
    en_status: TPanel;
    Label32: TLabel;
    Label33: TLabel;
    Label36: TLabel;
    line1_ndat: TPanel;
    line0_ndat: TPanel;
    Label37: TLabel;
    Label38: TLabel;
    Label40: TLabel;
    line1_turns: TPanel;

    procedure resolution1Change(Sender: TObject);
    procedure digits1Change(Sender: TObject);
    procedure x_sys_2_panClick(Sender: TObject);
    procedure x_sys_1_panClick(Sender: TObject);
    procedure x_inv_1_panClick(Sender: TObject);
    procedure setup_okClick(Sender: TObject);
    procedure para_defClick(Sender: TObject);
    procedure para_saveClick(Sender: TObject);
    procedure para_loadClick(Sender: TObject);
    procedure minmax_1_panClick(Sender: TObject);
    procedure minmax_2_panClick(Sender: TObject);
    procedure minboundChange(Sender: TObject);
    procedure maxboundChange(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure axisClick(Sender: TObject);
    procedure intpChange(Sender: TObject);
    procedure x_sys_3_panClick(Sender: TObject);
    procedure no_l0Click(Sender: TObject);
    procedure no_l1Click(Sender: TObject);
    procedure line0_reg0Click(Sender: TObject);
    procedure line1_reg1Click(Sender: TObject);
    procedure line0_blockramClick(Sender: TObject);
    procedure line1_blockramClick(Sender: TObject);
    procedure en_statusClick(Sender: TObject);
    procedure line0_ssiClick(Sender: TObject);
    procedure line1_ssiClick(Sender: TObject);
    procedure line0_ndatClick(Sender: TObject);
    procedure line1_ndatClick(Sender: TObject);
    procedure line1_turnsClick(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
  end;

procedure rebuild_display(axis:word);
procedure rebuild_para_displ;

var
  para_display: Tpara_display;

implementation

uses
 ik220_heiden_unit,
 ik220_cnt_unit,
 ik220_soft_unit,
 ik220_cntrl_unit,
 ik220_rdwr_unit,
 ik220_para_general,
 adjust_unit,
 visual_unit;

{$R *.DFM}
(*************************************************)
(*************************************************)
 procedure rebuild_display(axis:word);
 var
   s : string;
 begin

   str(axis,s);
   Counter[axis].Caption:='Axis '+s;
   Counter[axis].xlabel.Caption:='X'+s+':';

   (* Linear, rotary *)

   if cnt^.count[axis]^.display.system=mm then
     begin
      Counter[axis].units.caption:='[mm]';
     end;

   if cnt^.count[axis]^.display.system=dgr then
     begin
      Counter[axis].units.caption:='[dgr]';
    end;

   if cnt^.count[axis]^.display.system=inc then
     begin
      Counter[axis].units.caption:='[inc]';
     end;

  (* Min/Max *)

  if cnt^.count[axis]^.cntmode.minmax=max then
    begin
      Counter[axis].x_max.Font.Color:=clred;
    end
  else
    begin
      Counter[axis].x_max.Font.Color:=clmaroon;
    end;
  if cnt^.count[axis]^.cntmode.minmax=min then
    begin
      Counter[axis].x_min.Font.Color:=clred;
    end
  else
    begin
      Counter[axis].x_min.Font.Color:=clmaroon;
    end;

  (* Dimension *)

  if cnt^.count[axis]^.position.dimension=0 then
     begin
       Counter[axis].x_dim.Caption:='0';
     end;
  if cnt^.count[axis]^.position.dimension=1 then
     begin
       Counter[axis].x_dim.Caption:='1';
     end;
  if cnt^.count[axis]^.position.dimension=2 then
     begin
       Counter[axis].x_dim.Caption:='2';
     end;
  if cnt^.count[axis]^.position.dimension=3 then
     begin
       Counter[axis].x_dim.Caption:='3';
     end;

 end;
(*************************************************)
(*************************************************)
 procedure rebuild_para_displ;
 var
  s    : string;
  i    : integer;
  r    : real;
  axis : word;
  data : byte;
 begin

  axis:=cnt^.axis;

  (* Display axis *)
  str(axis,s);
  para_display.axis.caption:=s;

  (* Resolution *)
  i:=cnt^.count[axis]^.display.resolut;
  str(i,s);
  para_display.resolution1.text:=s;

  (* Digits *)
  i:=cnt^.count[axis]^.display.digits;
  str(i,s);
  para_display.digits1.text:=s;

  (* Interpolation *)
  i:=cnt^.count[axis]^.display.interpolation;
  str(i,s);
  para_display.intp.text:=s;

  (* Direction *)
  if cnt^.count[axis]^.display.direction=normal then
      para_display.x_inv_1_pan.caption:=' '
  else
      para_display.x_inv_1_pan.caption:='X';

  (* Values mm,degree,incr *)
  if cnt^.count[axis]^.display.system=mm then
      para_display.x_sys_1_pan.caption:='X'
  else
      para_display.x_sys_1_pan.caption:='';

  if cnt^.count[axis]^.display.system=dgr then
      para_display.x_sys_2_pan.caption:='X'
  else
      para_display.x_sys_2_pan.caption:='';

  if cnt^.count[axis]^.display.system=inc then
      para_display.x_sys_3_pan.caption:='X'
  else
      para_display.x_sys_3_pan.caption:='';

  (* Min/Max *)
  if cnt^.count[axis]^.cntmode.minmax=min then
      para_display.minmax_1_pan.caption:='X'
  else
      para_display.minmax_1_pan.caption:=' ';

  if cnt^.count[axis]^.cntmode.minmax=max then
      para_display.minmax_2_pan.caption:='X'
  else
      para_display.minmax_2_pan.caption:=' ';

  (* Sorting *)
  data:=cnt^.count[axis]^.display.digits;

  r:=cnt^.count[axis]^.cntmode.minbound;
  str(r:5:data,s);
  para_display.minbound.text:=s;

  r:=cnt^.count[axis]^.cntmode.maxbound;
  str(r:5:data,s);
  para_display.maxbound.text:=s;

 (* Enable Display Lines *)
  if cnt^.count[axis]^.display.enline0 then
      para_display.no_l0.caption:='X'
  else
      para_display.no_l0.caption:=' ';

  if cnt^.count[axis]^.display.enline1 then
      para_display.no_l1.caption:='X'
  else
      para_display.no_l1.caption:=' ';

  if cnt^.count[axis]^.display.enstatus then
      para_display.en_status.caption:='X'
  else
      para_display.en_status.caption:=' ';


  (* Source *)
  if cnt^.count[axis]^.display.sourceline0=reg0 then
      para_display.line0_reg0.caption:='X'
  else
      para_display.line0_reg0.caption:=' ';


  if cnt^.count[axis]^.display.sourceline0=blockram then
      para_display.line0_blockram.caption:='X'
  else
      para_display.line0_blockram.caption:=' ';

  if cnt^.count[axis]^.display.sourceline0=ssipos then
      para_display.line0_ssi.caption:='X'
  else
      para_display.line0_ssi.caption:=' ';

  if cnt^.count[axis]^.display.sourceline0=ndatpos then
      para_display.line0_ndat.caption:='X'
  else
      para_display.line0_ndat.caption:=' ';

  if cnt^.count[axis]^.display.sourceline1=reg1 then
      para_display.line1_reg1.caption:='X'
  else
      para_display.line1_reg1.caption:=' ';

  if cnt^.count[axis]^.display.sourceline1=reg0turns then
      para_display.line1_turns.caption:='X'
  else
      para_display.line1_turns.caption:=' ';

  if cnt^.count[axis]^.display.sourceline1=ssipos then
      para_display.line1_ssi.caption:='X'
  else
      para_display.line1_ssi.caption:=' ';

  if cnt^.count[axis]^.display.sourceline1=ndatpos then
      para_display.line1_ndat.caption:='X'
  else
      para_display.line1_ndat.caption:=' ';

 end;
(*************************************************)
(* Axis 0,1,..                                   *)
(*************************************************)
procedure Tpara_display.axisClick(Sender: TObject);
begin
  cnt^.axis:=cnt^.axis+1;
  if cnt^.axis>xnumber then
    begin
      cnt^.axis:=0;
    end;
  rebuild_all;
end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure Tpara_display.line0_reg0Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline0=reg0 then
    cnt^.count[axis]^.display.sourceline0:=off
  else
    cnt^.count[axis]^.display.sourceline0:=reg0;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line0_blockramClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline0=blockram then
    cnt^.count[axis]^.display.sourceline0:=off
  else
    cnt^.count[axis]^.display.sourceline0:=blockram;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line0_ssiClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline0=ssipos then
    cnt^.count[axis]^.display.sourceline0:=off
  else
    cnt^.count[axis]^.display.sourceline0:=ssipos;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line0_ndatClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline0=ndatpos then
    cnt^.count[axis]^.display.sourceline0:=off
  else
    cnt^.count[axis]^.display.sourceline0:=ndatpos;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line1_reg1Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline1=reg1 then
    cnt^.count[axis]^.display.sourceline1:=off
  else
    cnt^.count[axis]^.display.sourceline1:=reg1;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line1_turnsClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline1=reg0turns then
    cnt^.count[axis]^.display.sourceline1:=off
  else
    cnt^.count[axis]^.display.sourceline1:=reg0turns;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line1_blockramClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline1=blockram then
    cnt^.count[axis]^.display.sourceline1:=off
  else
    cnt^.count[axis]^.display.sourceline1:=blockram;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line1_ssiClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline1=ssipos then
    cnt^.count[axis]^.display.sourceline1:=off
  else
    cnt^.count[axis]^.display.sourceline1:=ssipos;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.line1_ndatClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.sourceline1=ndatpos then
    cnt^.count[axis]^.display.sourceline1:=off
  else
    cnt^.count[axis]^.display.sourceline1:=ndatpos;
 rebuild_all;
end;
(*************************************************)
(* Resolution                                    *)
(*************************************************)
procedure Tpara_display.resolution1Change(Sender: TObject);
var
  s      : string;
  i      : integer;
  code   : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_display.resolution1.text;
  val(s,i,code);
  if i<=1 then i:=1;
  cnt^.count[axis]^.display.resolut:=i;
  rebuild_all;
end;
(*************************************************)
(* Digits                                        *)
(*************************************************)
procedure Tpara_display.digits1Change(Sender: TObject);
var
  s      : string;
  i,code : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_display.digits1.text;
  val(s,i,code);
  if i>9 then i:=9;
  if i<0 then i:=0;
  cnt^.count[axis]^.display.digits:=i;
  rebuild_all;
end;
(*************************************************)
(* Interpolation                                 *)
(*************************************************)
procedure Tpara_display.intpChange(Sender: TObject);
var
  s      : string;
  i,code : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_display.intp.text;
  val(s,i,code);
  if i<0 then i:=1;
  if i>16 then i:=16;
  cnt^.count[axis]^.display.interpolation:=i;
  rebuild_all;
end;
(*************************************************)
(* Encoder (linear,rotary                        *)
(*************************************************)
procedure Tpara_display.x_sys_1_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.display.system:=mm;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.x_sys_2_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.display.system:=dgr;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_display.x_sys_3_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.display.system:=inc;
  rebuild_all;
end;
(*************************************************)
(* Direction                                     *)
(*************************************************)
procedure Tpara_display.x_inv_1_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.direction=normal then
    begin
      cnt^.count[axis]^.display.direction:=inverted;
    end
  else
    begin
      cnt^.count[axis]^.display.direction:=normal;
    end;
  rebuild_all;
end;
(*************************************************)
(* Min-Mode                                      *)
(*************************************************)
procedure Tpara_display.minmax_1_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if not(cnt^.count[axis]^.cntmode.minmax=min) then
    begin
      cnt^.count[axis]^.cntmode.minmax:=min;
      cnt^.count[axis]^.cntmode.minwert:=cnt^.count[axis]^.position.counter_disp0;
    end
  else
    begin
      cnt^.count[axis]^.cntmode.minmax:=typ;
    end;
  rebuild_all;
end;
(*************************************************)
(* Max-Mode                                      *)
(*************************************************)
procedure Tpara_display.minmax_2_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if not(cnt^.count[axis]^.cntmode.minmax=max) then
    begin
      cnt^.count[axis]^.cntmode.minmax:=max;
      cnt^.count[axis]^.cntmode.maxwert:=cnt^.count[axis]^.position.counter_disp0;
    end
  else
    begin
      cnt^.count[axis]^.cntmode.minmax:=typ;
    end;
  rebuild_all;
end;
(*************************************************)
(* Min-Value                                     *)
(*************************************************)
procedure Tpara_display.minboundChange(Sender: TObject);
var
  s      : string;
  code   : integer;
  axis   : word;
  r      : real;
begin
  axis:=cnt^.axis;
  s:=para_display.minbound.text;
  val(s,r,code);
  cnt^.count[axis]^.cntmode.minbound:=r;
  rebuild_all;
end;
(*************************************************)
(* Max-Value                                     *)
(*************************************************)
procedure Tpara_display.maxboundChange(Sender: TObject);
var
  s      : string;
  code   : integer;
  axis   : word;
  r      : real;
begin
  axis:=cnt^.axis;
  s:=para_display.maxbound.text;
  val(s,r,code);
  cnt^.count[axis]^.cntmode.maxbound:=r;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_display.no_l0Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.enline0 then
    cnt^.count[axis]^.display.enline0:=false
  else
    cnt^.count[axis]^.display.enline0:=true;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_display.no_l1Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.enline1 then
    cnt^.count[axis]^.display.enline1:=false
  else
    cnt^.count[axis]^.display.enline1:=true;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_display.en_statusClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.display.enstatus then
    cnt^.count[axis]^.display.enstatus:=false
  else
    cnt^.count[axis]^.display.enstatus:=true;
  rebuild_all;
end;
(*************************************************)
(*  C O N T R O L                                *)
(*  B U T T O N S                                *)
(*************************************************)
procedure Tpara_display.setup_okClick(Sender: TObject);
begin
  Parameter_req.order:=parainit;
end;
(*************************************************)
(* Default                                       *)
(*************************************************)
procedure Tpara_display.para_defClick(Sender: TObject);
var
  axis : word;
begin
  for axis:=0 to xnumber do
    begin
      Default_Counters;
    end;
  rebuild_all;
end;
(*************************************************)
(* Ik220.cnt write                               *)
(*************************************************)
procedure Tpara_display.para_saveClick(Sender: TObject);
begin
  Wr_Setup_Counters;
end;
(*************************************************)
(* IK220.cnt read                                *)
(*************************************************)
procedure Tpara_display.para_loadClick(Sender: TObject);
begin
  Rd_Setup_Counters;
  rebuild_all;
end;
(*************************************************)
(* Exit                                          *)
(*************************************************)
procedure Tpara_display.BitBtn1Click(Sender: TObject);
begin
  para_display.hide;
end;
(*************************************************)
(*************************************************)

end.
