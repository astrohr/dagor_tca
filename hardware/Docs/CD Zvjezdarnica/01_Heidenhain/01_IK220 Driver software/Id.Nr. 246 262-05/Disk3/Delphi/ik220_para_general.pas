unit ik220_para_general;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, Buttons,
  ik220_def_unit;

type
  Tpara_general = class(TForm)
    soft_l0: TPanel;
    Label21: TLabel;
    axis: TPanel;
    BitBtn1: TBitBtn;
    Label42: TLabel;
    Label43: TLabel;
    setup_ok: TBitBtn;
    para_def: TButton;
    para_save: TButton;
    para_load: TButton;
    Label1: TLabel;
    soft_l1: TPanel;
    Label5: TLabel;
    ext_en_l1: TPanel;
    int_en_tim: TPanel;
    Label10: TLabel;
    timer0: TEdit;
    Label15: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    Label19: TLabel;
    Label20: TLabel;
    Start_pwm: TButton;
    Stopp_pwm: TButton;
    ramval: TEdit;
    En_Ram0: TButton;
    Dis_Ram: TButton;
    res_Ram: TButton;
    SoftClick0: TButton;
    SoftClick1: TButton;
    Label16: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label40: TLabel;
    Label22: TLabel;
    auto_on_pan: TPanel;
    calc_on_pan: TPanel;
    corrpoints: TEdit;
    Label6: TLabel;
    Label9: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label24: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    extpad: TPanel;
    Label14: TLabel;
    ext_en_l0: TPanel;
    Label26: TLabel;
    Label27: TLabel;
    adcstartl0: TButton;
    Single: TButton;
    procedure BitBtn1Click(Sender: TObject);
    procedure setup_okClick(Sender: TObject);
    procedure para_defClick(Sender: TObject);
    procedure para_saveClick(Sender: TObject);
    procedure para_loadClick(Sender: TObject);
    procedure axisClick(Sender: TObject);
    procedure soft_l0Click(Sender: TObject);
    procedure ext_en_l0Click(Sender: TObject);
    procedure ext_en_l1Click(Sender: TObject);
    procedure int_en_timClick(Sender: TObject);
    procedure Softclick0Click(Sender: TObject);
    procedure Softclick1Click(Sender: TObject);
    procedure soft_l1Click(Sender: TObject);
    procedure dis_RAMClick(Sender: TObject);
    procedure timer0Change(Sender: TObject);
    procedure Start_pwmClick(Sender: TObject);
    procedure Stopp_pwmClick(Sender: TObject);
    procedure En_RamClick(Sender: TObject);
    procedure res_RamClick(Sender: TObject);
    procedure ramvalChange(Sender: TObject);
    procedure auto_on_panClick(Sender: TObject);
    procedure calc_on_panClick(Sender: TObject);
    procedure corrpointsChange(Sender: TObject);
    procedure extpadClick(Sender: TObject);
    procedure adcstartl0Click(Sender: TObject);
    procedure SingleClick(Sender: TObject);

  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  para_general: Tpara_general;

procedure rebuild_para_general;

implementation

uses
  ik220_cnt_unit,
  ik220_heiden_unit,
  ik220_cntrl_unit,
  ik220_rdwr_unit,
  visual_unit,
  adjust_unit;


{$R *.DFM}


(*************************************************)
(*************************************************)
 procedure rebuild_para_general;
 var
  axis : word;
  s    : string;
  r    : real;
  i    : integer;
 begin

   axis:=cnt^.axis;

   (* Display axis *)
   str(axis,s);
   para_general.axis.caption:=s;

   (* Timer value *)
   r:=(cnt^.count[axis]^.parameter.timvalue/1000);
   str(r:4:3,s);
   para_general.timer0.text:=s;

   (* Ram value *)
   str(cnt^.count[axis]^.parameter.ramvalue,s);
   para_general.ramval.text:=s;

   (* Latch-Mode Reg.0 *)
   if cnt^.count[axis]^.latchmode.reg0_latch=pctimer then
       para_general.soft_l0.Caption:='X'
   else
       para_general.soft_l0.Caption:=' ';

   (* Latch-Mode Reg.1 *)
   if cnt^.count[axis]^.latchmode.reg1_latch=pctimer then
       para_general.soft_l1.Caption:='X'
   else
       para_general.soft_l1.Caption:=' ';

  (* ext-Latch *)
  if cnt^.count[axis]^.parameter.enl0 then
       para_general.ext_en_l0.Caption:='X'
   else
       para_general.ext_en_l0.Caption:='';

  if cnt^.count[axis]^.parameter.enl1 then
       para_general.ext_en_l1.Caption:='X'
   else
       para_general.ext_en_l1.Caption:='';

  if cnt^.count[axis]^.parameter.entim then
       para_general.int_en_tim.Caption:='X'
   else
       para_general.int_en_tim.Caption:='';

  (* Autocorrection *)
  if cnt^.count[axis]^.parameter.encorrect then
    para_general.auto_on_pan.caption:='X'
  else
    para_general.auto_on_pan.caption:='';
  if cnt^.count[axis]^.parameter.encalc then
    para_general.calc_on_pan.caption:='X'
  else
    para_general.calc_on_pan.caption:='';

  i:=cnt^.count[axis]^.parameter.corrpoints;
  str(i,s);
  para_general.corrpoints.text:=s;

  if cnt^.extload then
    para_general.extpad.Caption:='X'
  else
    para_general.extpad.Caption:=' ';

 end;
(*************************************************)
(*************************************************)
procedure Tpara_general.BitBtn1Click(Sender: TObject);
begin
  para_general.hide;
end;
{*************************************************)
(*************************************************}
procedure Tpara_general.setup_okClick(Sender: TObject);
begin
  Parameter_req.order:=parainit;
end;
{*************************************************)
(*************************************************}
procedure Tpara_general.para_defClick(Sender: TObject);
var
  axis : word;
begin
  for axis:=0 to xnumber do
    begin
      Default_Counters;
    end;
  rebuild_all;
end;
{*************************************************)
(*************************************************}
procedure Tpara_general.para_saveClick(Sender: TObject);
begin
  Wr_Setup_Counters;
end;
{*************************************************)
(*************************************************}
procedure Tpara_general.para_loadClick(Sender: TObject);
begin
  Rd_Setup_Counters;
  rebuild_all;
end;
{*************************************************)
(*************************************************}
procedure Tpara_general.axisClick(Sender: TObject);
begin
  cnt^.axis:=cnt^.axis+1;
  if cnt^.axis>xnumber then
    begin
      cnt^.axis:=0;
    end;
  rebuild_all;
end;
(*************************************************)
(* Latch-Mode                                    *)
(*************************************************)
procedure Tpara_general.soft_l0Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;

  control_req.order:=disRAM;
  control_req.axis:=axis;

  (* Ik220-Timer abschalten *)
  cnt^.count[axis]^.parameter.entim:=false;
  Parameter_req.order:=parainit;

  (* on->off   off->on *)
  if cnt^.count[axis]^.latchmode.reg0_latch=pctimer then
    begin
      cnt^.count[axis]^.latchmode.reg0_latch:=disable;
    end
  else
    begin
      cnt^.count[axis]^.latchmode.reg0_latch:=pctimer;
    end;  

  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_general.soft_l1Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;

  control_req.order:=disRAM;
  control_req.axis:=axis;

  if cnt^.count[axis]^.latchmode.reg1_latch=pctimer then
    begin
      cnt^.count[axis]^.latchmode.reg1_latch:=disable;
    end
  else
    begin
      cnt^.count[axis]^.latchmode.reg1_latch:=pctimer;
    end;

  rebuild_all;
end;
(*************************************************)
(* Extern Latch                                  *)
(*************************************************)
procedure Tpara_general.ext_en_l0Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if not(cnt^.count[axis]^.parameter.enl0) then
    begin
      cnt^.count[axis]^.parameter.enl0:=true;
    end
  else
    begin
      cnt^.count[axis]^.parameter.enl0:=false;
    end;
  rebuild_all;
  Parameter_req.order:=parainit;
end;
(*************************************************)
procedure Tpara_general.ext_en_l1Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if not(cnt^.count[axis]^.parameter.enl1) then
    begin
      cnt^.count[axis]^.parameter.enl1:=true;
    end
  else
    begin
      cnt^.count[axis]^.parameter.enl1:=false;
    end;
  rebuild_all;
  Parameter_req.order:=parainit;
end;
(*************************************************)
procedure Tpara_general.int_en_TimClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  (* PC-Timer abschalten *)
  cnt^.count[axis]^.latchmode.reg0_latch:=disable;

  if not(cnt^.count[axis]^.parameter.entim) then
    begin
      cnt^.count[axis]^.parameter.entim:=true;
    end
  else
    begin
      cnt^.count[axis]^.parameter.entim:=false;
    end;
    
  rebuild_all;
  Parameter_req.order:=parainit;
end;
{*************************************************}
procedure Tpara_general.timer0Change(Sender: TObject);
var
  s      : string;
  r      : real;
  i      : longint;
  code   : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_general.timer0.text;
  val(s,r,code);
  r:=r*1000;
  i:=round(r);
  if i>1000000 then i:=1000000;
  cnt^.count[axis]^.parameter.timvalue:=i;
end;
{*************************************************}
procedure Tpara_general.Softclick0Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  control_req.order:=softlatch0;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.Softclick1Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  control_req.order:=softlatch1;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.Start_pwmClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  control_req.order:=entimer;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.Stopp_pwmClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  control_req.order:=distimer;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.Dis_RamClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.diagnose.adcMode:=false;
  control_req.order:=disRAM;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.res_RamClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  control_req.order:=resRAM;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.En_RamClick(Sender: TObject);
var
  axis   : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.diagnose.adcMode:=false;
  control_req.order:=enRAM0;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.SingleClick(Sender: TObject);
var
  axis   : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.diagnose.adcMode:=false;
  control_req.order:=enRAM0s;
  control_req.axis:=axis;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.ramvalChange(Sender: TObject);
var
  s      : string;
  i,code : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_general.ramval.text;
  val(s,i,code);
  if i>$1FFF then i:=$1FFF;
  if i<0 then i:=0;
  cnt^.count[axis]^.parameter.ramvalue:=i;
  rebuild_all;
end;
(*************************************************)
(* Auto-Correct                                  *)
(*************************************************)
procedure Tpara_general.auto_on_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.parameter.encorrect then
    cnt^.count[axis]^.parameter.encorrect:=false
  else
    cnt^.count[axis]^.parameter.encorrect:=true;
  rebuild_all;
end;
(*************************************************)
procedure Tpara_general.calc_on_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  if cnt^.count[axis]^.parameter.encalc then
    cnt^.count[axis]^.parameter.encalc:=false
  else
    cnt^.count[axis]^.parameter.encalc:=true;
  rebuild_all;
end;
{*************************************************}
procedure Tpara_general.corrpointsChange(Sender: TObject);
var
  s      : string;
  i,code : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_general.corrpoints.text;
  val(s,i,code);
  if i>8 then i:=8;
  if i<1 then i:=1;
  cnt^.count[axis]^.parameter.corrpoints:=i;
  rebuild_all;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.extpadClick(Sender: TObject);
begin
  if cnt^.extload then
    cnt^.extload:=false
  else
    cnt^.extload:=true;
  rebuild_all;
end;
{*************************************************}
{*************************************************}
procedure Tpara_general.adcstartl0Click(Sender: TObject);
var
  axis   : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.diagnose.adcMode:=true;
  control_req.order:=enRAM0adc;
  control_req.axis:=axis;
end;
{***********************************************}
{*************************************************}
begin

end.
