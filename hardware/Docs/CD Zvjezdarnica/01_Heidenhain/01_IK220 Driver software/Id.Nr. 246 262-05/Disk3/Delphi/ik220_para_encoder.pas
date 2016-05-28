unit ik220_para_encoder;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls,
  ik220_def_unit, Buttons;

type
  Tpara_encoder = class(TForm)
    Label2: TLabel;
    Label3: TLabel;
    Label6: TLabel;
    Label19: TLabel;
    Label20: TLabel;
    Label22: TLabel;
    Label23: TLabel;
    Label24: TLabel;
    Label29: TLabel;
    Label32: TLabel;
    Label36: TLabel;
    strichzahl1: TEdit;
    enc_ana_pan: TPanel;
    enc_ndat_pan: TPanel;
    enc_ssi_pan: TPanel;
    enc_strom_pan: TPanel;
    enc_spann_pan: TPanel;
    Label4: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label8: TLabel;
    Label33: TLabel;
    Label38: TLabel;
    x_ref_1_pan: TPanel;
    x_ref_2_pan: TPanel;
    x_ref_3_pan: TPanel;
    distance: TEdit;
    period1: TEdit;
    Label21: TLabel;
    axis: TPanel;
    BitBtn1: TBitBtn;
    setup_ok: TBitBtn;
    para_def: TButton;
    para_save: TButton;
    para_load: TButton;
    Speed: TPanel;
    Label1: TLabel;
    procedure period1Change(Sender: TObject);
    procedure strichzahl1Change(Sender: TObject);
    procedure enc_strom_panClick(Sender: TObject);
    procedure enc_spann_panClick(Sender: TObject);
    procedure enc_ana_panClick(Sender: TObject);
    procedure enc_ndat_panClick(Sender: TObject);
    procedure enc_ssi_panClick(Sender: TObject);
    procedure x_ref_1_panClick(Sender: TObject);
    procedure x_ref_2_panClick(Sender: TObject);
    procedure x_ref_3_panClick(Sender: TObject);
    procedure distanceClick(Sender: TObject);
    procedure axisClick(Sender: TObject);
    procedure setup_okClick(Sender: TObject);
    procedure para_loadClick(Sender: TObject);
    procedure para_saveClick(Sender: TObject);
    procedure para_defClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure SpeedClick(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  para_encoder: Tpara_encoder;

procedure rebuild_para_encoder;


implementation

uses
 ik220_cnt_unit,
 ik220_soft_unit,
 ik220_heiden_unit,
 ik220_cntrl_unit,
 ik220_rdwr_unit,
 ik220_para_general,
 adjust_unit,
 visual_unit,
 ik220_para_ssi,
 ik220_para_ndat;

{$R *.DFM}

(*************************************************)
(*************************************************)
 procedure rebuild_para_encoder;
 var
  s    : string;
  l    : longint;
  w    : real;
  axis : word;
 begin

  axis:=cnt^.axis;

  (* Display axis *)
  str(axis,s);
  para_encoder.axis.caption:=s;

  (* Period *)
  w:=cnt^.count[axis]^.coder.periode;
  str(w:5:6,s);
  para_encoder.period1.text:=s;

  (* Ref. marks *)
  if cnt^.count[axis]^.coder.refmark=no then
      para_encoder.x_ref_1_pan.caption:='X'
  else
      para_encoder.x_ref_1_pan.caption:='';

  if cnt^.count[axis]^.coder.refmark=one then
      para_encoder.x_ref_2_pan.caption:='X'
  else
      para_encoder.x_ref_2_pan.caption:='';

  if cnt^.count[axis]^.coder.refmark=coded then
      para_encoder.x_ref_3_pan.caption:='X'
  else
      para_encoder.x_ref_3_pan.caption:='';

  w:=cnt^.count[axis]^.coder.refdistance;
  str(w,s);
  para_encoder.distance.text:=s;

  (* Encoder *)
  if cnt^.count[axis]^.coder.inttype=analog then
      para_encoder.enc_ana_pan.caption:='X'
  else
      para_encoder.enc_ana_pan.caption:=' ';

  if cnt^.count[axis]^.coder.inttype=ndat then
      para_encoder.enc_ndat_pan.caption:='X'
  else
      para_encoder.enc_ndat_pan.caption:=' ';

  if cnt^.count[axis]^.coder.inttype=ssi then
      para_encoder.enc_ssi_pan.caption:='X'
  else
      para_encoder.enc_ssi_pan.caption:=' ';

  (* Current/Voltage *)
  if cnt^.count[axis]^.coder.sysin=strom then
      para_encoder.enc_strom_pan.caption:='X'
  else
      para_encoder.enc_strom_pan.caption:=' ';

  if cnt^.count[axis]^.coder.sysin=spannung then
      para_encoder.enc_spann_pan.caption:='X'
  else
      para_encoder.enc_spann_pan.caption:=' ';

  (* Input Bandwidth *)
  if cnt^.count[axis]^.coder.speed then
      para_encoder.speed.caption:='X'
  else
      para_encoder.speed.caption:=' ';


 (* Rotary: periods/rotation axis X *)
  l:=cnt^.count[axis]^.coder.strichzahl;
  str(l,s);
  para_encoder.strichzahl1.text:=s;

 end;
(*************************************************)
(*************************************************)

(*************************************************)
(*************************************************)
procedure Tpara_encoder.period1Change(Sender: TObject);
var
  s      : string;
  i      : real;
  code   : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_encoder.period1.text;
  val(s,i,code);
  if i<=0 then i:=0.001;
  if i>60000 then i:=60000;
  cnt^.count[axis]^.coder.periode:=i;
  rebuild_para_encoder;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.strichzahl1Change(Sender: TObject);
var
  s      : string;
  i      : longint;
  code   : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_encoder.strichzahl1.text;
  val(s,i,code);
  if i<=0 then i:=1;
  cnt^.count[axis]^.coder.strichzahl:=i;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.enc_strom_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.sysin:=strom;
  rebuild_para_encoder;
  Parameter_req.order:=parainit;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.enc_spann_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.sysin:=spannung;
  rebuild_para_encoder;
  Parameter_req.order:=parainit;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.SpeedClick(Sender: TObject);
var
  axis : word;
  sp   : boolean;
begin
  axis:=cnt^.axis;
  sp:=cnt^.count[axis]^.coder.speed;
  if sp then
    cnt^.count[axis]^.coder.speed:=false
  else
    cnt^.count[axis]^.coder.speed:=true;

  rebuild_para_encoder;
  Parameter_req.order:=parainit;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.enc_ana_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.inttype:=analog;
  rebuild_para_encoder;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.enc_ndat_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.inttype:=ndat;
  rebuild_all;
  Parameter_req.order:=parainit;
  para_ndat.show;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.enc_ssi_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.inttype:=ssi;
  rebuild_all;
  Parameter_req.order:=parainit; 
  para_ssi.show;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.x_ref_1_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.refmark:=no;
  rebuild_para_encoder;
  Parameter_req.order:=parainit;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.x_ref_2_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.refmark:=one;
  rebuild_para_encoder;
  Parameter_req.order:=parainit;  
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.x_ref_3_panClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.coder.refmark:=coded;
  rebuild_para_encoder;
  Parameter_req.order:=parainit;  
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.distanceClick(Sender: TObject);
var
  s      : string;
  axis   : word;
  flag   : boolean;
begin

  axis:=cnt^.axis;

  flag:=false;
  if (cnt^.count[axis]^.coder.refdistance=5000) and not(flag) then
    begin
      flag:=true;
      cnt^.count[axis]^.coder.refdistance:=0;
    end;
  if (cnt^.count[axis]^.coder.refdistance=2000) and not(flag) then
    begin
      flag:=true;
      cnt^.count[axis]^.coder.refdistance:=5000;
    end;
  if (cnt^.count[axis]^.coder.refdistance=1000) and not(flag) then
    begin
      flag:=true;
      cnt^.count[axis]^.coder.refdistance:=2000;
    end;
  if (cnt^.count[axis]^.coder.refdistance=500)  and not(flag)  then
    begin
      flag:=true;
      cnt^.count[axis]^.coder.refdistance:=1000;
    end;
  if (cnt^.count[axis]^.coder.refdistance=0)    and not(flag)    then
    begin
      cnt^.count[axis]^.coder.refdistance:=500;
    end;

  str(cnt^.count[axis]^.coder.refdistance,s);
  para_encoder.distance.text:=s;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.axisClick(Sender: TObject);
begin
  cnt^.axis:=cnt^.axis+1;
  if cnt^.axis>xnumber then
    begin
      cnt^.axis:=0;
    end;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.setup_okClick(Sender: TObject);
begin
  Parameter_req.order:=parainit;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.para_loadClick(Sender: TObject);
begin
  Rd_Setup_Counters;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.para_saveClick(Sender: TObject);
begin
  Wr_Setup_Counters;
end;
(*************************************************)
(*************************************************)
procedure Tpara_encoder.para_defClick(Sender: TObject);
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
(*************************************************)
procedure Tpara_encoder.BitBtn1Click(Sender: TObject);
begin
  para_encoder.hide;
end;
(*************************************************)
(*************************************************)

end.
