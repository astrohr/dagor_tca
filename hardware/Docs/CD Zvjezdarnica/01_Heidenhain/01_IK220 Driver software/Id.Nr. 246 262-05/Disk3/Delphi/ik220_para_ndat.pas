unit ik220_para_ndat;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, Buttons,
  ik220_def_unit;

type
  Tpara_ndat = class(TForm)
    Label21: TLabel;
    axis: TPanel;
    BitBtn1: TBitBtn;
    setup_ok: TBitBtn;
    para_def: TButton;
    para_save: TButton;
    para_load: TButton;
    config_ndat: TButton;
    reset_ndat: TButton;
    procedure setup_okClick(Sender: TObject);
    procedure para_loadClick(Sender: TObject);
    procedure para_saveClick(Sender: TObject);
    procedure para_defClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure axisClick(Sender: TObject);
    procedure config_ndatClick(Sender: TObject);
    procedure reset_ndatClick(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  para_ndat: Tpara_ndat;

procedure rebuild_para_ndat;

implementation

{$R *.DFM}

uses
 ik220_cnt_unit,
 ik220_soft_unit,
 ik220_heiden_unit,
 ik220_cntrl_unit,
 ik220_rdwr_unit,
 ik220_para_general, ik220_para_encoder;

 (*************************************************)
(*************************************************)
 procedure rebuild_para_ndat;
 var
  s    : string;
  axis : word;
 begin

  axis:=cnt^.axis;

  (* Display axis *)
  str(axis,s);
  para_ndat.axis.caption:=s;

 end;
(*************************************************)
(*************************************************)
procedure Tpara_ndat.setup_okClick(Sender: TObject);
begin
  Parameter_req.order:=parainit;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ndat.para_loadClick(Sender: TObject);
begin
  Rd_Setup_Counters;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ndat.para_saveClick(Sender: TObject);
begin
  Wr_Setup_Counters;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ndat.para_defClick(Sender: TObject);
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
procedure Tpara_ndat.BitBtn1Click(Sender: TObject);
begin
  para_ndat.hide;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ndat.axisClick(Sender: TObject);
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
procedure Tpara_ndat.config_ndatClick(Sender: TObject);
var
  axis : word;
begin
   axis:=cnt^.axis;
   control_req.order:=ndat_config;
   control_req.axis:=axis;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ndat.reset_ndatClick(Sender: TObject);
var
  axis : word;
begin
   axis:=cnt^.axis;
   control_req.order:=ndat_reset;
   control_req.axis:=axis;
end;
(*************************************************)
(*************************************************)
end.
