unit ik220_para_ssi;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, StdCtrls, ExtCtrls,
  ik220_def_unit;

type
  Tpara_ssi = class(TForm)
    Label21: TLabel;
    axis: TPanel;
    para_def: TButton;
    para_save: TButton;
    para_load: TButton;
    BitBtn1: TBitBtn;
    setup_ok: TBitBtn;
    Label1: TLabel;
    Label5: TLabel;
    Label30: TLabel;
    Label35: TLabel;
    ssi_prezero: TEdit;
    ssi_codebits: TEdit;
    Label2: TLabel;
    ssi_postzero: TEdit;
    Label3: TLabel;
    Label4: TLabel;
    Label8: TLabel;
    ssi_no_pari: TPanel;
    Label6: TLabel;
    ssi_engray: TPanel;
    Label7: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label16: TLabel;
    ssi_odd_pari: TPanel;
    Label17: TLabel;
    ssi_even_pari: TPanel;
    rcn619_5gs7: TButton;
    Label12: TLabel;
    Button1: TButton;
    Label13: TLabel;
    Button2: TButton;
    procedure axisClick(Sender: TObject);
    procedure setup_okClick(Sender: TObject);
    procedure para_loadClick(Sender: TObject);
    procedure para_saveClick(Sender: TObject);
    procedure para_defClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure ssi_codebitsChange(Sender: TObject);
    procedure ssi_prezeroChange(Sender: TObject);
    procedure ssi_postzeroChange(Sender: TObject);
    procedure ssi_no_pariClick(Sender: TObject);
    procedure ssi_odd_pariClick(Sender: TObject);
    procedure ssi_even_pariClick(Sender: TObject);
    procedure ssi_engrayClick(Sender: TObject);
    procedure rcn619_5gs7Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  para_ssi: Tpara_ssi;

procedure rebuild_para_ssi;

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
procedure rebuild_para_ssi;
var
  s    : string;
  i    : integer;
  axis : word;
 begin

  axis:=cnt^.axis;

  (* Display axis *)
  str(axis,s);
  para_ssi.axis.caption:=s;

  (* Codebits *)
  i:=cnt^.count[axis]^.ssipara.code_bits;
  str(i,s);
  para_ssi.ssi_codebits.text:=s;

  (* Prezero *)
  i:=cnt^.count[axis]^.ssipara.prezero_bits;
  str(i,s);
  para_ssi.ssi_prezero.text:=s;

  (* Postzero *)
  i:=cnt^.count[axis]^.ssipara.postzero_bits;
  str(i,s);
  para_ssi.ssi_postzero.text:=s;

 (* Parity *)
  if cnt^.count[axis]^.ssipara.parity=non then
  para_ssi.ssi_no_pari.caption:='X' else para_ssi.ssi_no_pari.caption:=' ';

  if cnt^.count[axis]^.ssipara.parity=even then
  para_ssi.ssi_odd_pari.caption:='X' else para_ssi.ssi_odd_pari.caption:=' ';

  if cnt^.count[axis]^.ssipara.parity=evenzero then
  para_ssi.ssi_even_pari.caption:='X' else para_ssi.ssi_even_pari.caption:=' ';

  (* Gray *)
  if cnt^.count[axis]^.ssipara.gray then
  para_ssi.ssi_engray.caption:='X' else para_ssi.ssi_engray.caption:=' ';

  (* Encoders*)


end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.axisClick(Sender: TObject);
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
procedure Tpara_ssi.ssi_codebitsChange(Sender: TObject);
var
  s      : string;
  i,code : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_ssi.ssi_codebits.text;
  val(s,i,code);
  if i>48 then i:=48;
  if i<0 then i:=0;
  cnt^.count[axis]^.ssipara.code_bits:=i;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.ssi_prezeroChange(Sender: TObject);
var
  s      : string;
  i,code : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_ssi.ssi_prezero.text;
  val(s,i,code);
  if i>48 then i:=48;
  if i<0 then i:=0;
  cnt^.count[axis]^.ssipara.prezero_bits:=i;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.ssi_postzeroChange(Sender: TObject);
var
  s      : string;
  i,code : integer;
  axis   : word;
begin
  axis:=cnt^.axis;
  s:=para_ssi.ssi_postzero.text;
  val(s,i,code);
  if i>48 then i:=48;
  if i<0 then i:=0;
  cnt^.count[axis]^.ssipara.postzero_bits:=i;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.ssi_no_pariClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.ssipara.parity:=non;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.ssi_odd_pariClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.ssipara.parity:=even;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.ssi_even_pariClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.ssipara.parity:=evenzero;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.ssi_engrayClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.ssipara.gray:=not(cnt^.count[axis]^.ssipara.gray);
  rebuild_all;
end;
(*************************************************)
(* Default-Encoder                               *)
(*************************************************)
procedure Tpara_ssi.rcn619_5gs7Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.ssipara.code_bits:=19;
  cnt^.count[axis]^.ssipara.prezero_bits:=0;
  cnt^.count[axis]^.ssipara.postzero_bits:=0;
  cnt^.count[axis]^.ssipara.parity:=even;
  cnt^.count[axis]^.ssipara.gray:=true;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.setup_okClick(Sender: TObject);
begin
  Parameter_req.order:=parainit;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.para_loadClick(Sender: TObject);
begin
  Rd_Setup_Counters;
  rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.para_saveClick(Sender: TObject);
begin
  Wr_Setup_Counters;
end;
(*************************************************)
(*************************************************)
procedure Tpara_ssi.para_defClick(Sender: TObject);
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
procedure Tpara_ssi.BitBtn1Click(Sender: TObject);
begin
   para_ssi.hide;
end;
(*************************************************)
(*************************************************)


















end.
