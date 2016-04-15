unit ik220_heiden_unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Menus, DBCtrls,
  Buttons, MPlayer;

type
  THeidenhain = class(TForm)
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    control: TMenuItem;
    Timer1: TTimer;
    Exit0: TBitBtn;
    Adjust1: TMenuItem;
    Printer: TMenuItem;
    Exit1: TMenuItem;
    Visual1: TMenuItem;
    Parrameters1: TMenuItem;
    Display1: TMenuItem;
    Encoder1: TMenuItem;
    Options1: TMenuItem;
    Image2: TImage;

    procedure Timer1Timer(Sender: TObject);
    procedure Exit0Click(Sender: TObject);
    procedure controlClick(Sender: TObject);
    procedure Adjust1Click(Sender: TObject);
    procedure PrinterClick(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure Visual1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure InfoClick(Sender: TObject);
    procedure Display1Click(Sender: TObject);
    procedure Encoder1Click(Sender: TObject);
    procedure Options1Click(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Heidenhain: THeidenhain;

  blink     : boolean;
  tick      : word;

procedure rebuild_all;

implementation

uses
     ik220_cntrl_unit,
     display_unit1,
     adjust_unit,
     visual_unit,
     ik220_printer,
     ik220_def_unit,
     ik220_rdwr_unit,
     ik220_soft_unit,
     ik220_info_unit,
     ik220_prt_unit,
     ik220_para_general,
     ik220_para_displ,
     ik220_para_encoder,
     ik220_para_ssi,
     ik220_para_ndat;

{$R *.DFM}
(*************************************************)
(*                                               *)
(*************************************************)
procedure minmax_anzeige(axis:word);
  begin
          (* Min/Max status *)
          if cnt^.count[axis].cntmode.min then
            begin
              Counter[axis].x_disp_min.Font.Color:=clred;
            end
          else
            begin
              Counter[axis].x_disp_min.Font.Color:=clmaroon;
            end;

          if cnt^.count[axis].cntmode.max then
            begin
              Counter[axis].x_disp_max.Font.Color:=clred;
            end
          else
            begin
              Counter[axis].x_disp_max.Font.Color:=clmaroon;
            end;

          if cnt^.count[axis].cntmode.typ then
            begin
              Counter[axis].x_disp_typ.Font.Color:=clred;
            end
          else
            begin
              Counter[axis].x_disp_typ.Font.Color:=clmaroon;
            end;
  end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure ramstatus_anzeige(axis:word);
  begin
          (* RAM-Satus *)
          if cnt^.count[axis].status.ramempty then
            begin
              Counter[axis].Ram_Stat0.Font.Color:=clred;
            end
          else
            begin
              Counter[axis].Ram_Stat0.Font.Color:=clmaroon;
            end;

          if cnt^.count[axis].status.ramoverflow then
            begin
              Counter[axis].Ram_Stat1.Font.Color:=clred;
            end
          else
            begin
              Counter[axis].Ram_Stat1.Font.Color:=clmaroon;
            end;

         if cnt^.count[axis].status.ramrdy then
            begin
              Counter[axis].Ram_rdy.Font.Color:=clred;
            end
          else
            begin
              Counter[axis].Ram_rdy.Font.Color:=clmaroon;
            end;

  end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure ref_anzeige(axis:word);
  begin
          (* Ref. status *)
          if cnt^.count[axis]^.status.ref then
            begin
              Counter[axis].ref.Font.Color:=clred;
            end
          else
            begin
              Counter[axis].ref.Font.Color:=clmaroon;
            end;
  end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure amp_anzeige(axis:word);
  begin
          (* Amp. status *)
          case cnt^.count[axis]^.status.amp of
          min:  begin
                  if blink then
                    Counter[axis].minamp.Font.Color:=clyellow
                  else
                    Counter[axis].minamp.Font.Color:=clmaroon;
                  Counter[axis].maxamp.Font.Color:=clmaroon;
                end;
          max:  begin
                  if blink then
                    Counter[axis].maxamp.Font.Color:=clyellow
                  else
                    Counter[axis].maxamp.Font.Color:=clmaroon;
                  Counter[axis].minamp.Font.Color:=clmaroon;
                end;
          typ:  begin
                  Counter[axis].maxamp.Font.Color:=clmaroon;
                  Counter[axis].minamp.Font.Color:=clmaroon;
                end;
          end;
  end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure ssi_ndat_anzeige(axis:word);
  begin
    if cnt^.count[axis]^.status.ssierror or
       cnt^.count[axis]^.status.ndatalarm then
      begin
        Counter[axis].ssindat.Font.Color:=clred;
      end
    else
      begin
        Counter[axis].ssindat.Font.Color:=clmaroon;
      end;
  end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure line0_anzeige(axis:word);
  begin

          if cnt^.count[axis]^.display.system=mm then
            begin
              Build_reg0_mm(axis); (* ik220_soft_unit: incr->mm->value_0 *)
              Counter[axis].x_sign_0.caption:=cnt^.count[axis]^.position.sign_0;
              Counter[axis].x_display_0.caption:=cnt^.count[axis]^.position.value_0;
            end;

          if cnt^.count[axis]^.display.system=dgr then
            begin
              Build_reg0_degree(axis);
              Counter[axis].x_sign_0.caption:=cnt^.count[axis]^.position.sign_0;
              Counter[axis].x_display_0.caption:=cnt^.count[axis]^.position.value_0;
            end;

          if cnt^.count[axis]^.display.system=inc then
            begin
              Build_reg0_inc(axis);
              Counter[axis].x_display_0.caption:=cnt^.count[axis]^.position.value_0;
            end;

  end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure line1_anzeige(axis:word);
  begin

          if cnt^.count[axis]^.display.system=mm then
            begin
              Build_reg1_mm(axis);
              Counter[axis].x_sign_1.caption:=cnt^.count[axis]^.position.sign_1;
              Counter[axis].x_display_1.caption:=cnt^.count[axis]^.position.value_1;
            end;

          if cnt^.count[axis]^.display.system=dgr then
            begin
              if cnt^.count[axis]^.display.sourceline1=reg0turns then
                begin
                  Counter[axis].x_sign_1.caption:=cnt^.count[axis]^.position.turnsign_0;
                  Counter[axis].x_display_1.caption:=cnt^.count[axis]^.position.turnvalue_0;
                end
              else
                begin
                  Build_reg1_degree(axis);
                  Counter[axis].x_sign_1.caption:=cnt^.count[axis]^.position.sign_1;
                  Counter[axis].x_display_1.caption:=cnt^.count[axis]^.position.value_1;
                end
            end;

          if cnt^.count[axis]^.display.system=inc then
            begin
              Build_reg1_inc(axis);
              Counter[axis].x_display_1.caption:=cnt^.count[axis]^.position.value_1;
            end;

  end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure status_anzeige(axis:word);
begin

  (* Get AmpStatus *)
  cnt_ampstatus(axis);
  amp_anzeige(axis);

  (* Get RefStatus *)
  cnt_refstatus(axis);
  ref_anzeige(axis);

  (* Display Min/Max *)
  minmax_anzeige(axis);

  (* Get RamStatus *)
  cnt_ramstatus(axis);
  ramstatus_anzeige(axis);

  (* SSI/ENDAT-Status *)
  ssi_ndat_anzeige(axis);

end;
(*************************************************)
(*                                               *)
(*************************************************)
procedure action_splitter;
var
  axis : word;
begin
      (* Controller-Orders *)
      axis:=control_req.axis;
      case control_req.order of
      softstart : begin
                    control_req.order:=nop;
                    cnt_start(axis);
                  end;
      softstopp : begin
                    control_req.order:=nop;
                    cnt_stopp(axis);
                  end;
      softclear : begin
                    control_req.order:=nop;
                    cnt_stopp(axis);
                  end;
      softlatch0 : begin
                    control_req.order:=nop;
                    cnt_latch0(axis);
                  end;
      softlatch1 : begin
                    control_req.order:=nop;
                    cnt_latch1(axis);
                  end;

      refclear  : begin
                    control_req.order:=nop;
                    cnt_ref_clear(axis);
                  end;
      refrun    : begin
                    control_req.order:=nop;
                    cnt_ref_run(axis);
                  end;
      refpos    : begin
                    control_req.order:=nop;
                    Heidenhain.Timer1.Enabled:=false;
                    cnt_ref_pos(axis);
                    Heidenhain.Timer1.Enabled:=true;
                  end;
      clearstat : begin
                    control_req.order:=nop;
                    cnt_clr_stat(axis);
                    if cnt^.count[axis]^.coder.inttype=ndat then
                      begin
                        cnt_clr_ndat(axis);
                      end;
                  end;

      enRAM0    : begin
                    control_req.order:=nop;
                    cnt_RAM_start_l0(axis)
                  end;

      enRAM0s   : begin
                    control_req.order:=nop;
                    cnt_RAM_start_l0s(axis)
                  end;

      enRAM0adc : begin
                    control_req.order:=nop;
                    cnt_RAM_start_l0_adc(axis)
                  end;

      disRAM    : begin
                    control_req.order:=nop;
                    cnt_RAM_stopp(axis);
                  end;
      resRAM    : begin
                    control_req.order:=nop;
                    cnt_RAM_reset(axis);
                  end;

      entimer   : begin
                    control_req.order:=nop;
                    cnt_Timer_start(axis);
                  end;
      distimer  : begin
                    control_req.order:=nop;
                    cnt_Timer_stopp(axis);
                  end;
    ndat_config : begin
                    control_req.order:=nop;
                    cnt_config_ndat(axis);
                    rebuild_all;
                  end;
    ndat_reset  : begin
                    control_req.order:=nop;
                    cnt_reset_ndat(axis);
                  end;
      end;

      (* Parameter-Orders *)
      case parameter_req.order of
      parainit  : begin
                    parameter_req.order:=nop;
                    for axis:=0 to xnumber do
                      begin
                        cnt_init(axis);   (* ik220_rdwr_unit *)
                      end;
                  end;
      getcorr_e : begin
                    parameter_req.order:=nop;
                    cnt_get_corr_e(axis);
                    faktoren_e(axis);
                  end;
      getcorr_c : begin
                    parameter_req.order:=nop;
                    cnt_get_corr_c(axis);
                    faktoren_c(axis);
                  end;
      getcorr_ec: begin
                    parameter_req.order:=nop;
                    cnt_get_corr_e(axis);
                    cnt_get_corr_c(axis);
                    faktoren_e(axis);
                    faktoren_c(axis);
                  end;
      end;

end;
(*************************************************)
(*************************************************)
procedure read_axis0(axis:word);
begin
          case cnt^.count[axis]^.display.sourceline0 of
          reg0 :
            begin
              cnt_get_reg0(axis); (* ik220_rdwr_unit: if latched then Ik220->cnt_incr_reg0 *)
            end;
          blockram:
            begin
              cnt_get0_ram_block(axis);
            end;
          ssipos:
            begin
              cnt_getssi_reg0(axis);
            end;
          ndatpos:
            begin
              cnt_getndat_reg0(axis);
            end;
          end;
end;
(*************************************************)
(*************************************************)
procedure read_axis1(axis:word);
begin
          case cnt^.count[axis]^.display.sourceline1 of
          reg1:
            begin
              cnt_get_reg1(axis);
            end;
          blockram:
            begin
              cnt_get1_ram_block(axis);
            end;
          ssipos:
            begin
              cnt_getssi_reg1(axis);
            end;
          ndatpos:
            begin
              cnt_getndat_reg1(axis);
            end;
          end;
end;
(*************************************************)
(*************************************************)
procedure value_action(axis:word);
begin
   if cnt^.count[axis]^.latchmode.reg0_latch=pctimer then
      begin
        cnt_latch0(axis);
      end;
   read_axis0(axis);

   if cnt^.count[axis]^.latchmode.reg1_latch=pctimer then
      begin
        cnt_latch1(axis);
      end;
   read_axis1(axis);

   if en_drawer then
      begin
        cnt_get_corr_e(cnt^.axis);
        cnt_get_corr_c(cnt^.axis);
        faktoren_e(cnt^.axis);
        faktoren_c(cnt^.axis);
      end;

   action_splitter;

end;
(*************************************************)
(*************************************************)
procedure display_action(axis:word);
begin
         (* Show Display Line 0 *)
          if cnt^.count[axis]^.display.enline0 then
            begin
              line0_anzeige(axis);       (* incr->mm->value_0 *)
            end
          else
            begin
              Counter[axis].x_sign_0.caption:='';
              Counter[axis].x_display_0.caption:='';
            end;

          (* Show Display Line 1 *)
          if cnt^.count[axis]^.display.enline1 then
            begin
              line1_anzeige(axis);      (* incr->mm->value_1 *)
            end
          else
            begin
              Counter[axis].x_sign_1.caption:='';
              Counter[axis].x_display_1.caption:='';
            end;

          (* Show-Status-Lines *)
          if cnt^.count[axis]^.display.enstatus then
            begin
              status_anzeige(axis);
            end;

end;
(*************************************************)
(* Zentral-Timer                                 *)
(*     100 ms Cycle Timer                        *)
(*     -> Display X,Y                            *)
(*************************************************)
procedure THeidenhain.Timer1Timer(Sender: TObject);
var
  axen         : word;

begin

  tick:=tick+1;
  if tick>10 then
    begin
      tick:=0;
      blink:=not(blink);
    end;


  axen:=0;
  if start then
  begin
      for axen:=0 to xnumber do
        begin
          value_action(axen);
          display_action(axen);
        end;

      axen:=cnt^.axis;
      if en_visual then
        begin
          visual_action(axen);
        end;

      if en_drawer then
        begin
          draw_lisajout(axen);
        end;
    end;



end;
{*************************************************}
(* Option window                                 *)
(*************************************************)
procedure THeidenhain.Options1Click(Sender: TObject);
begin
  rebuild_all;
  para_general.show;
end;
{*************************************************}
(* Parameter window                              *)
(*************************************************)
procedure THeidenhain.Display1Click(Sender: TObject);
begin
  rebuild_all;
  para_display.Show;
end;
{*************************************************}
(* Parameter window                              *)
(*************************************************)
procedure THeidenhain.Encoder1Click(Sender: TObject);
begin
  rebuild_all;
  para_encoder.Show;
end;
(*************************************************)
(* Control window                                *)
(*************************************************)
procedure THeidenhain.controlClick(Sender: TObject);
begin
   rebuild_all;
   controller.Show;
end;
(*************************************************)
(*************************************************)
procedure THeidenhain.PrinterClick(Sender: TObject);
begin
  rebuild_all;
  rebuild_output(druck);
  output.Show;
end;
(*************************************************)
(*************************************************)
procedure THeidenhain.Adjust1Click(Sender: TObject);
begin
  rebuild_all;
  drawer.Show;
  en_drawer:=true;
end;
(*************************************************)
(*************************************************)
procedure THeidenhain.Visual1Click(Sender: TObject);
begin
  visual.show;
  rebuild_all;
  en_visual:=true;
end;
(*************************************************)
(*************************************************)
procedure THeidenhain.InfoClick(Sender: TObject);
begin
  rebuild_all;
  Info.show;
end;
(*************************************************)
(*************************************************)
procedure THeidenhain.Exit0Click(Sender: TObject);
begin
  Application.Terminate;
end;
(*************************************************)
(*************************************************)
procedure THeidenhain.Exit1Click(Sender: TObject);
begin
  Application.Terminate;
end;
(*************************************************)
(*************************************************)
procedure THeidenhain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Application.Terminate;
end;
(*************************************************)
(*************************************************)
procedure rebuild_all;
var
  axis : word;
begin
  for axis:=0 to xnumber do
    begin
      rebuild_display(axis);
    end;
  rebuild_visual;
  rebuild_cntrl;
  rebuild_draw;
  rebuild_para_general;
  rebuild_para_displ;
  rebuild_para_encoder;
  rebuild_para_ssi;
  rebuild_para_ndat;
end;
(*************************************************)
(*************************************************)
begin
  blink:=false;
  tick:=0;


end.


