unit ik220_start;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls;

type
  TWelcome = class(TForm)
    Status: TPanel;
    Timer1: TTimer;
    Timer2: TTimer;
    procedure Timer1Timer(Sender: TObject);
    procedure Timer2Timer(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;


var
  Welcome   : TWelcome;
  bootstate : word;
  error     : boolean;
 
  
implementation

{$R *.DFM}

uses
  ik220_printer,
  adjust_unit,
  visual_unit,
  display_unit1,
  ik220_heiden_unit,
  ik220_cntrl_unit,
  ik220_def_unit,
  ik220_base_unit,
  ik220_cnt_unit,
  ik220_rdwr_unit,
  ik220_prt_unit,
  ik220_info_unit,
  ik220_para_general,
  ik220_para_displ,
  ik220_para_encoder,
  ik220_para_ssi,
  ik220_para_ndat,
  ik220_mz_unit;

{******************************************************************)
(******************************************************************}
procedure Setup_Window;
var
  axis,
  a,b : word;
begin

   Application.CreateForm(TInfo, Info);
   Info.top:=25;
   Info.left:=55;
   Info.FormStyle:=fsStayonTop;

   Application.CreateForm(TController, Controller);
   Controller.Top:=25;
   Controller.Left:=30;
   Controller.FormStyle:=fsStayonTop;

   Application.CreateForm(Tdrawer, drawer);
   drawer.top:=25;
   drawer.left:=35;
   drawer.FormStyle:=fsStayonTop;

   Application.CreateForm(TOutput, Output);
   Output.top:=25;
   Output.left:=40;
   Output.FormStyle:=fsStayonTop;

   Application.CreateForm(TVisual, Visual);
   Visual.top:=25;
   Visual.left:=45;
   Visual.FormStyle:=fsStayonTop;

   Application.CreateForm(Tpara_general, para_general);
   para_general.top:=25;
   para_general.left:=50;
   para_general.FormStyle:=fsStayonTop;

   Application.CreateForm(Tpara_display, para_display);
   para_display.top:=25;
   para_display.left:=50;
   para_display.FormStyle:=fsStayonTop;

   Application.CreateForm(Tpara_encoder, para_encoder);
   para_encoder.top:=25;
   para_encoder.left:=50;
   para_encoder.FormStyle:=fsStayonTop;

   Application.CreateForm(Tpara_ssi, para_ssi);
   para_ssi.Top:=150;
   para_ssi.Left:=25;
   para_ssi.FormStyle:=fsStayonTop;

   Application.CreateForm(Tpara_ndat, para_ndat);
   para_ndat.Top:=150;
   para_ndat.Left:=25;
   para_ndat.FormStyle:=fsStayonTop;

   (* Display windows *)

   a:=50;
   b:=16;
   for axis:=0 to xnumber do
     begin
       Application.CreateForm(TCounter_X, Counter[axis]);
       Counter[axis].Top:=a;
       Counter[axis].Left:=b;
       if axis<1 then
         begin
           a:=a+160;
         end
       else
         begin
           a:=a+20;
         end;
     end;

   for axis:=0 to xnumber do
     begin
       Counter[axis].Visible:=true;
       Counter[axis].autoscroll:=false;
       Counter[axis].borderstyle:=bsSingle;
       Counter[axis].formstyle:=fsStayonTop;
       rebuild_display(axis);
     end;

   Application.CreateForm(THeidenhain, Heidenhain);
   Heidenhain.Top:=3;
   Heidenhain.Left:=3;
   Heidenhain.visible:=true;

end;
{******************************************************************)
(* Boot-Action                                                    *)
(******************************************************************}
procedure TWelcome.Timer1Timer(Sender: TObject);
var
  axis : word;
begin
  case bootstate of
 (********************************)
  1 :
    begin
      bootstate:=100;
      Welcome.show;
      bootstate:=2;
    end;
(********************************)
  3 :
    begin
      bootstate:=100;
      xnumber:=0;
      for axis:=0 to maxaxis do
        begin
          if boardadressen[axis]<>0 then
            begin
              xnumber:=axis;
            end;
        end;
      bootstate:=4;
    end;
 (********************************)
  5 :
    begin
      bootstate:=100;
      new_pointers;
      bootstate:=6;
    end;
  (********************************)
  7 :
    begin
      bootstate:=100;
      Default_Counters;
      Rd_Setup_Counters;
      Rd_Setup_Printer;
      bootstate:=8;
    end;
  (********************************)
  9 :
    begin
      bootstate:=100;

      (* Download  Default-file *)

      if cnt^.extload then
        begin
          Download(error);
        end
      else
        begin
          for axis:=0 to xnumber do
            begin
              IK220Init(axis);
            end;
        end;

      bootstate:=10;
    end;
  (********************************)
  11 :
    begin
      bootstate:=100;
      Start_Counters;
      for axis:=0 to xnumber do
        begin
          led_blink(axis);
        end;
      control_req.order:=entimer;
      bootstate:=12;
    end;
  (********************************)
  13 :
   begin
      bootstate:=100;
      Welcome.hide;
      Setup_Window;
      start:=true;
      ik220settimeout(10000);
      bootstate:=14;
   end;
  (********************************)
  15 :
  begin
    Welcome.Timer1.Enabled:=false;
    bootstate:=16;
  end;
  (********************************)
  end;
end;
{******************************************************************)
(* Boot Status Display                                            *)
(******************************************************************}
procedure TWelcome.Timer2Timer(Sender: TObject);
begin

  case bootstate of
  0 :
    begin
      Welcome.status.caption:='Start...............';
      Welcome.color:=clblue;
      bootstate:=1;
    end;
  2 :
    begin
      Welcome.status.caption:='Find IK220 Hardware.';
      Welcome.color:=clgreen;
      bootstate:=3;
    end;

  4 :
    begin
      if (xnumber<>255) then
        begin
          Welcome.status.caption:='Allocate Memory.....';
          Welcome.color:=clblue;
          bootstate:=5;
        end;
      if (xnumber=255) then
        begin
          Welcome.status.caption:='No Hardware found...';
          Welcome.color:=clred;
          bootstate:=200;
        end;
    end;

  6 :
    begin
      Welcome.status.caption:='Init Memory.........';
      Welcome.color:=clgreen;
      bootstate:=7;
    end;

  8 :
    begin
      Welcome.status.caption:='Download Program....';
      Welcome.color:=clblue;
      bootstate:=9;
    end;

  10 :
    begin
      if error then
        begin
          Welcome.status.caption:='No IK2.bin found!!!...';
          Welcome.color:=clred;
          bootstate:=11;
        end;

      if not(error) then
        begin
          Welcome.status.caption:='Start Counters......';
          Welcome.color:=clyellow;
          bootstate:=11;
        end;
    end;
  12 :
   begin
     Welcome.status.caption:='Run Application ....';
     Welcome.color:=clgreen;
     bootstate:=13;
   end;
  14 :
   begin
     Welcome.Timer2.Enabled:=false;
     bootstate:=15;
   end;
  (********************************)
  
  (********************************)


   end;

end;
{******************************************************************)
(******************************************************************}
begin
  error:=false;
  start:=false;
  bootstate:=0;
  en_drawer:=false;
  en_visual:=false;
end.


