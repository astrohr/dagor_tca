unit visual_unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, Buttons, StdCtrls;

type
  TVisual = class(TForm)
    Clear: TButton;
    BitBtn1: TBitBtn;
    zoom_in: TButton;
    zoom_out: TButton;
    Label1: TLabel;
    axis: TPanel;
    procedure ClearClick(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure zoom_inClick(Sender: TObject);
    procedure zoom_outClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure axisClick(Sender: TObject);
    procedure FormHide(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

procedure rebuild_visual;
procedure draw_select;
procedure visual_action(axis:word);

var
  Visual: TVisual;

  zoomfaktor : real;
  px,py      : word;

const breite = 600;
const hohe   = 256;

implementation

uses
  ik220_heiden_unit,
  ik220_def_unit,
  ik220_cntrl_unit,
  ik220_para_general,
  ik220_soft_unit,
  adjust_unit;

{$R *.DFM}
(******************************************************************)
(******************************************************************)
procedure rebuild_visual;
var
  axis : word;
  s    : string;
begin
  axis:=cnt^.axis;
  (* Axis-Display *)
  str(axis,s);
  visual.axis.caption:=s;

  visual.Canvas.Rectangle(1,1,breite,hohe);

  visual.Canvas.Pen.Color:=clblue;
  visual.Canvas.moveto(1,1);
  visual.Canvas.lineto(1,hohe);
  visual.Canvas.lineto(breite,hohe);
  visual.Canvas.lineto(breite,1);
  visual.Canvas.lineto(1,1);

end;
(******************************************************************)
(******************************************************************)
procedure draw_select;
begin
  visual.Canvas.Pen.Color:=clyellow;
  visual.Canvas.moveto(px,0);
  visual.Canvas.lineto(px,256);
end;
(******************************************************************)
(******************************************************************)
procedure draw_pos(axis:word;wert:double);
var
  iw   : double;
begin

  wert:=wert*zoomfaktor;

  iw:=round(wert/256);
  wert:=wert-(iw*256);
  wert:=wert+128;

  if wert<1 then wert:=1;
  if wert>hohe then wert:=hohe;

  py:=round(wert);

  case axis of
  0 : visual.Canvas.Pen.Color:=clblue;
  1 : visual.Canvas.Pen.Color:=clred;
  2 : visual.Canvas.Pen.Color:=clgreen;
  3 : visual.Canvas.Pen.Color:=clyellow;
  else
      visual.Canvas.Pen.Color:=clblack;
  end;

  visual.Canvas.moveto(px,py);
  visual.Canvas.lineto(px+1,py+1);

  px:=px+1;
  if px>breite then
    begin
      px:=1;
      rebuild_visual;
    end;

end;
(******************************************************************)
(******************************************************************)
procedure draw_adc(adc0,adc9:word);
var
  wert,iw : double;

begin

  (* dg00 *)

  wert:=adc0;

  if wert<$8000 then
    begin
      wert:=wert+$8000;
    end
  else
    begin
      wert:=wert-$8000;
    end;

  wert:=wert*(zoomfaktor/4);

  iw:=round(wert/256);
  wert:=wert-(iw*256);
  wert:=wert+128;

  if wert<1 then wert:=1;
  if wert>hohe then wert:=hohe;

  py:=round(wert);

  visual.Canvas.Pen.Color:=clred;
  visual.Canvas.moveto(px,py);
  visual.Canvas.lineto(px+1,py+1);


  (* dg90 *)

  wert:=adc9;

  if wert<$8000 then
    begin
      wert:=wert+$8000;
    end
  else
    begin
      wert:=wert-$8000;
    end;

  wert:=wert*(zoomfaktor/4);

  iw:=round(wert/256);
  wert:=wert-(iw*256);
  wert:=wert+128;


  if wert<1 then wert:=1;
  if wert>hohe then wert:=hohe;

  py:=round(wert);

  visual.Canvas.Pen.Color:=clblue;

  visual.Canvas.moveto(px,py);
  visual.Canvas.lineto(px+1,py+1);

  px:=px+1;
  if px>breite then
    begin
      px:=1;
      rebuild_visual;
    end;

end;
(******************************************************************)
(******************************************************************)
procedure TVisual.zoom_inClick(Sender: TObject);
begin
  zoomfaktor:=zoomfaktor*8;
  rebuild_visual;
end;
(******************************************************************)
(******************************************************************)
procedure TVisual.zoom_outClick(Sender: TObject);
begin
   zoomfaktor:=zoomfaktor/8;
   rebuild_visual;
end;
(******************************************************************)
(******************************************************************)
procedure TVisual.ClearClick(Sender: TObject);
begin
  px:=1;
  rebuild_visual;
end;
(******************************************************************)
(******************************************************************)
procedure TVisual.BitBtn1Click(Sender: TObject);
begin
  visual.hide;
end;
(******************************************************************)
(******************************************************************)
procedure TVisual.FormActivate(Sender: TObject);
begin
  px:=1;
  rebuild_visual;
end;
(******************************************************************)
(******************************************************************)
procedure TVisual.FormShow(Sender: TObject);
begin
  px:=1;
  rebuild_visual;
end;
(******************************************************************)
(******************************************************************)
procedure TVisual.axisClick(Sender: TObject);
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
procedure TVisual.FormHide(Sender: TObject);
begin
  en_visual:=false;
end;
(******************************************************************)
(******************************************************************)
procedure ram_blockstep_display(axis:word);
var
  wert    : double;
  adc0,adc9,
  werte,i : word;
begin

      if not(cnt^.count[axis]^.diagnose.adcmode) then
        begin
          werte:=dbuffer.anzahl;
          if werte>0 then
            begin
              for i:=0 to (werte-1) do
                begin
                  wert:=dbuffer.daten[i];
                  draw_pos(axis,wert);
                  px:=px+1;
                  if px>breite then
                    begin
                      px:=1;
                      rebuild_visual;
                    end;
                end;
            end;
        end
      else
        begin
          werte:=intpbuffer.anzahl;
          if werte>0 then
            begin
              for i:=0 to (werte-1) do
                begin
                  adc0:=adc0buffer.sig[i];
                  adc9:=adc9buffer.sig[i];
                  draw_adc(adc0,adc9);
                  px:=px+1;
                  if px>breite then
                    begin
                      px:=1;
                      rebuild_visual;
                    end;
                end;
            end;
          end;

      draw_select;

end;
(******************************************************************)
(******************************************************************)
procedure visual_action(axis:word);
var
  wert : double;
  sel  : sourcemodus;
begin

  sel:= cnt^.count[axis]^.display.sourceline0;
  
  if sel=blockram then
      begin
        ram_blockstep_display(axis);
      end;

  if sel=reg0 then
      begin
        wert:=cnt^.count[axis]^.position.cnt_incr_reg0;
        draw_pos(axis,wert);
      end;

end;
(******************************************************************)
(******************************************************************)
begin
  px:=1;
  py:=1;
  zoomfaktor:=1;
end.

