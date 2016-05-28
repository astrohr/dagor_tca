unit adjust_unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons,
  ik220_def_unit,
  ik220_cnt_unit;

type
  Tdrawer = class(TForm)
    stop_adjust: TBitBtn;
    clr_button: TButton;
    axis: TPanel;
    Label2: TLabel;
    eoff9: TPanel;
    essym: TPanel;
    esphas: TPanel;
    eoff0: TPanel;
    esym: TPanel;
    ephas: TPanel;
    Label1: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    coff0: TPanel;
    coff9: TPanel;
    csym: TPanel;
    cphas: TPanel;
    Label8: TLabel;
    Label9: TLabel;
    refposbut: TButton;
    Label10: TLabel;
    ref_anzeige: TPanel;
    daempf: TPanel;
    Label11: TLabel;
    Timer1: TTimer;

    procedure FormActivate(Sender: TObject);
    procedure stop_adjustClick(Sender: TObject);
    procedure clr_buttonClick(Sender: TObject);
    procedure axisClick(Sender: TObject);
    procedure getval0Click(Sender: TObject);
    procedure FormHide(Sender: TObject);
    procedure refposbutClick(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  drawer: Tdrawer;

procedure rebuild_draw;
procedure faktoren_e(axis:word);
procedure faktoren_c(axis:word);
procedure draw_lisajout(axis:word);

implementation

uses
     ik220_heiden_unit,
     ik220_cntrl_unit,
     ik220_para_general,
     visual_unit;

{$R *.DFM}

(******************************************************************)
(******************************************************************)
procedure rebuild_draw;
var
  axis      : word;
  s         : string;
begin
  axis:=cnt^.axis;
  str(axis,s);
  drawer.axis.caption:=s;

  drawer.Canvas.Rectangle(1,1,256,256);

  drawer.Canvas.Pen.Color:=clblue;
  drawer.Canvas.moveto(1,1);
  drawer.Canvas.lineto(1,256);
  drawer.Canvas.lineto(256,256);
  drawer.Canvas.lineto(256,1);
  drawer.Canvas.lineto(1,1);

  drawer.Canvas.Pen.Color:=clyellow;
  drawer.Canvas.moveto(128,1);
  drawer.Canvas.lineto(128,256);
  drawer.Canvas.moveto(1,128);
  drawer.Canvas.lineto(256,128);

  drawer.Canvas.TextOut(230,20,'0°');
  drawer.Canvas.TextOut(230,230,'90°');
  drawer.Canvas.TextOut(20,230,'180°');
  drawer.Canvas.TextOut(20,20,'270°');

end;
(******************************************************************)
(*              Calculated Test ONLY                              *)
(******************************************************************)
procedure faktoren_e(axis:word);
var
  s   : string;
  a,b : smallint;
  r   : double;
  i   : integer;
begin

  a:=cnt^.count[axis]^.hyde_e.off0;
  r:=a;
  str(r:5:0,s);
  drawer.eoff0.Caption:=s;

  a:=cnt^.count[axis]^.hyde_e.off9;
  r:=a;
  str(r:5:0,s);
  drawer.eoff9.Caption:=s;

  b:=cnt^.count[axis]^.hyde_e.pha1;
  r:=b;
  str(r:5:0,s);
  drawer.ephas.Caption:=s;

  b:=cnt^.count[axis]^.hyde_e.sym1;
  r:=b;
  str(r:5:0,s);
  drawer.esym.Caption:=s;

  i:=cnt^.count[axis]^.parameter.daempf;
  str(i,s);
  drawer.daempf.Caption:=s;

end;
(******************************************************************)
(*             Effektive                                          *)
(******************************************************************)
procedure faktoren_c(axis:word);
var
  s   : string;
  a,b : smallint;
  r   : double;
begin

  (* 12Bit adc +- 2048 *)

  a:=cnt^.count[axis]^.hyde_c.off0;
  r:=a;
  r:=0.001525902*r; (* % *)

  str(r:3:3,s);
  drawer.coff0.Caption:=s;

  a:=cnt^.count[axis]^.hyde_c.off9;
  r:=a;
  r:=0.001525902*r; (* % *)

  str(r:3:3,s);
  drawer.coff9.Caption:=s;

  b:=cnt^.count[axis]^.hyde_c.pha0;
   if b=0 then
    drawer.esphas.Caption:='+'
  else
    drawer.esphas.Caption:='-';

  b:=cnt^.count[axis]^.hyde_c.sym0;
  if b=0 then
    drawer.essym.Caption:='+'
  else
    drawer.essym.Caption:='-';

  b:=cnt^.count[axis]^.hyde_c.pha1;
  r:=b;
  r:=0.0005493247*r; (*°*)
  str(r:5:3,s); 
  drawer.cphas.Caption:=s;

  b:=cnt^.count[axis]^.hyde_c.sym1;

  r:=65535-b;
  r:=0.001525902*r; (* % *)
  str(r:3:3,s);
  drawer.csym.Caption:=s;

end;
(******************************************************************)
(* Draw:                                                          *)
(******************************************************************)
procedure draw_ad_0(x,y:smallint);
var
  z : integer;
begin
  z:=x;
  z:=(z div 256);
  z:=z+128;
  x:=round(z);

  z:=y;
  z:=(z div 256);
  z:=z+128;
  y:=round(z);

  if x<0 then x:=0;
  if x>256 then x:=256;
  if y<0 then y:=0;
  if y>256 then y:=256;
  drawer.Canvas.moveto(x,y);
  drawer.Canvas.lineto(x+1,y+1);
end;
(******************************************************************)
(* Draw: groesser!                                                *)
(******************************************************************)
procedure draw_ad_1(x,y:smallint);
var
  z : integer;
begin
  z:=x;
  z:=(z div 240);
  z:=z+128;
  if z>0 then x:=round(z) else x:=0;

  z:=y;
  z:=(z div 240);
  z:=z+128;
  if z>0 then y:=round(z) else y:=0;

  if x<0 then x:=0;
  if x>256 then x:=256;
  if y<0 then y:=0;
  if y>256 then y:=256;
  drawer.Canvas.moveto(x,y);
  drawer.Canvas.lineto(x+1,y+1);
end;
(******************************************************************)
procedure draw_lisajout(axis:word);
var
  x,y  : smallint;
  w    : word;
  s    : string;
begin

  drawer.Canvas.Pen.Color:=clred;
  x:=cnt^.count[axis]^.diagnose.ad00_org;
  y:=cnt^.count[axis]^.diagnose.ad90_org;
  draw_ad_0(x,y); (* normal *)

  drawer.Canvas.Pen.Color:=clblue;
  x:=cnt^.count[axis]^.diagnose.ad00_corr;
  y:=cnt^.count[axis]^.diagnose.ad90_corr;
  draw_ad_1(x,y); (* vergroessert *)

  w:=cnt^.count[axis]^.diagnose.ri_pos;

  str(w,s);
  drawer.ref_anzeige.Caption:=s;

  drawer.Canvas.Pen.Color:=clpurple;
  drawer.Canvas.moveto(128,128);

  x:=round(128+(20*sin(2*pi*w/360)));
  y:=round(128+(20*cos(2*pi*w/360)));
  drawer.Canvas.lineto(x,y);

end;
(******************************************************************)
(******************************************************************)
procedure Tdrawer.getval0Click(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  parameter_req.order:=getcorr_ec;
  control_req.axis:=axis;
end;
(******************************************************************)
(******************************************************************)
procedure Tdrawer.FormActivate(Sender: TObject);
begin
  rebuild_draw;
end;
(******************************************************************)
(******************************************************************)
procedure Tdrawer.axisClick(Sender: TObject);
begin
  cnt^.axis:=cnt^.axis+1;
  if cnt^.axis>xnumber then
    begin
      cnt^.axis:=0;
    end;
  rebuild_all;
end;
(******************************************************************)
(******************************************************************)
procedure Tdrawer.stop_adjustClick(Sender: TObject);
begin
  drawer.hide;
end;
(******************************************************************)
(******************************************************************)
procedure Tdrawer.clr_buttonClick(Sender: TObject);
begin
  rebuild_draw;
end;
(******************************************************************)
(******************************************************************)
procedure Tdrawer.FormHide(Sender: TObject);
begin
  en_drawer:=false;
end;
(******************************************************************)
(******************************************************************)
procedure Tdrawer.refposbutClick(Sender: TObject);
var
  axis : word;
begin
  axis:=cnt^.axis;
  control_req.order:=refpos;
  control_req.axis:=axis;
end;
(******************************************************************)
(******************************************************************)
end.
