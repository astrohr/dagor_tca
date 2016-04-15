unit ik220_cntrl_unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls,Buttons,
  ik220_def_unit,
  ik220_cnt_unit;

type
  TController = class(TForm)
    Label1: TLabel;
    axis: TPanel;
    cntrl_ok: TBitBtn;
    cntrl_start: TButton;
    cntrl_stopp: TButton;
    cntrl_clr: TButton;
    cntrl_preset: TButton;
    cntrl_set: TEdit;
    cntrl_dim0_pan: TPanel;
    cntrl_dim1_pan: TPanel;
    cntrl_dim2_pan: TPanel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    cntrl_dim3_pan: TPanel;
    cntrl_ref: TButton;
    BitBtn1: TBitBtn;
    cntrl_clrdim: TButton;
    Label2: TLabel;
    Label4: TLabel;
    Label9: TLabel;
    Label3: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    cntrl_clrstat: TButton;

    procedure axisClick(Sender: TObject);
    procedure cntrl_okClick(Sender: TObject);
    procedure cntrl_startClick(Sender: TObject);
    procedure cntrl_stoppClick(Sender: TObject);
    procedure cntrl_clrClick(Sender: TObject);
    procedure cntrl_dim0_panClick(Sender: TObject);
    procedure cntrl_dim1_panClick(Sender: TObject);
    procedure cntrl_dim2_panClick(Sender: TObject);
    procedure cntrl_dim3_panClick(Sender: TObject);
    procedure cntrl_setChange(Sender: TObject);
    procedure cntrl_presetClick(Sender: TObject);
    procedure cntrl_refClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure cntrl_clrdimClick(Sender: TObject);
    procedure cntrl_clrstatClick(Sender: TObject);

    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

procedure rebuild_cntrl;

var
  Controller: TController;
implementation

uses
  ik220_heiden_unit,
  ik220_rdwr_unit,
  adjust_unit,
  ik220_para_general,
  visual_unit;

{$R *.DFM}

(*************************************************)
(*************************************************)
 procedure rebuild_cntrl;
 var
  axis  : word;
  s     : string;
  dig   : byte;
  r     : real;

 begin

   axis:=cnt^.axis;
   (* Axis-Display *)
   str(axis,s);
   controller.axis.caption:=s;

   if cnt^.count[axis]^.position.dimension=0 then
     begin
       controller.cntrl_dim0_pan.caption:='X';
       controller.cntrl_dim1_pan.caption:='';
       controller.cntrl_dim2_pan.caption:='';
       controller.cntrl_dim3_pan.caption:='';
     end;
   if cnt^.count[axis]^.position.dimension=1 then
     begin
       controller.cntrl_dim0_pan.caption:='';
       controller.cntrl_dim1_pan.caption:='X';
       controller.cntrl_dim2_pan.caption:='';
       controller.cntrl_dim3_pan.caption:='';
     end;
   if cnt^.count[axis]^.position.dimension=2 then
     begin
       controller.cntrl_dim0_pan.caption:='';
       controller.cntrl_dim1_pan.caption:='';
       controller.cntrl_dim2_pan.caption:='X';
       controller.cntrl_dim3_pan.caption:='';
     end;
    if cnt^.count[axis]^.position.dimension=3 then
     begin
       controller.cntrl_dim0_pan.caption:='';
       controller.cntrl_dim1_pan.caption:='';
       controller.cntrl_dim2_pan.caption:='';
       controller.cntrl_dim3_pan.caption:='X';
     end;

    r:=cnt^.count[axis]^.position.preset;
    dig:=cnt^.count[axis]^.display.digits;
    str(r:5:dig,s);
    controller.cntrl_set.text:=s;

 end;
(*************************************************)
(*************************************************)
procedure TController.axisClick(Sender: TObject);
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
procedure TController.cntrl_okClick(Sender: TObject);
begin
    controller.Hide;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_startClick(Sender: TObject);
var
  axis : word;
begin
   axis:=cnt^.axis;
   control_req.order:=softstart;
   control_req.axis:=axis;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_stoppClick(Sender: TObject);
var
  axis : word;
begin
   axis:=cnt^.axis;
   control_req.order:=softstopp;
   control_req.axis:=axis;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_clrClick(Sender: TObject);
var
  axis : word;
  dim  : byte;
begin
   axis:=cnt^.axis;
   dim:=cnt^.count[axis]^.position.dimension;
   cnt^.count[axis]^.position.dimoffset[dim]:=-cnt^.count[axis]^.position.cnt_mm_reg0;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_dim0_panClick(Sender: TObject);
var
  axis : word;
begin
    axis:=cnt^.axis;
    cnt^.count[axis]^.position.dimension:=0;
    rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_dim1_panClick(Sender: TObject);
var
  axis : word;
begin
    axis:=cnt^.axis;
    cnt^.count[axis]^.position.dimension:=1;
    rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_dim2_panClick(Sender: TObject);
var
  axis : word;
begin
    axis:=cnt^.axis;
    cnt^.count[axis]^.position.dimension:=2;
    rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_dim3_panClick(Sender: TObject);
var
  axis : word;
begin
    axis:=cnt^.axis;
    cnt^.count[axis]^.position.dimension:=3;
    rebuild_all;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_setChange(Sender: TObject);
var
  s      : string;
  code   : integer;
  axis   : word;
  r      : real;
begin
    axis:=cnt^.axis;
    s:=controller.cntrl_set.text;
    val(s,r,code);
    cnt^.count[axis]^.position.preset:=r;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_presetClick(Sender: TObject);
var
  axis  : word;
  dim   : byte;
  r,p,e : real;
begin
   axis:=cnt^.axis;
   dim:=cnt^.count[axis]^.position.dimension;
   r:=cnt^.count[axis]^.position.cnt_mm_reg0;
   p:=cnt^.count[axis]^.position.preset;
   if cnt^.count[axis]^.display.system=dgr then
      begin
        if p>360 then p:=360;
        cnt^.count[axis]^.position.preset:=360;
        rebuild_all;
      end;
   e:=-r+p;
   cnt^.count[axis]^.position.dimoffset[dim]:=e;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_refClick(Sender: TObject);
var
  axis : word;
begin
   axis:=cnt^.axis;
   if (cnt^.count[axis]^.coder.refmark=one) then
     begin
       control_req.order:=refrun;
     end;
   if cnt^.count[axis]^.coder.refmark=coded then
     begin
       control_req.order:=refrun;
     end;
   control_req.axis:=axis;
end;
(*************************************************)
(*************************************************)
procedure TController.BitBtn1Click(Sender: TObject);
begin
  Controller.hide;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_clrdimClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  cnt^.count[axis]^.position.dimoffset[0]:=0;
  cnt^.count[axis]^.position.dimoffset[1]:=0;
  cnt^.count[axis]^.position.dimoffset[2]:=0;
  cnt^.count[axis]^.position.dimoffset[3]:=0;
end;
(*************************************************)
(*************************************************)
procedure TController.cntrl_clrstatClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  control_req.order:=clearstat;
  control_req.axis:=axis;

  cnt^.count[axis]^.status.ssierror:=false;
  cnt^.count[axis]^.status.ndatalarm:=false;
  
end;
(*************************************************)
(*************************************************)
end.


