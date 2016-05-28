program ik220;
(**************************************************************)
(*   Dr. Johannes Heidenhain GmbH                             *)
(*                                                            *)
(*                                                            *)
(*   IK 220  Version 1.00                                     *)
(*                                                            *)
(*   Mai 1998                                                 *)
(*                                                            *)
(**************************************************************)
uses
  Windows,
  Forms,
  ik220_heiden_unit in 'ik220_heiden_unit.pas' {Heidenhain},
  ik220_start in 'ik220_start.pas' {Welcome},
  ik220_cntrl_unit in 'ik220_cntrl_unit.pas' {Controller},
  ik220_para_general in 'ik220_para_general.pas' {para_general},
  ik220_para_displ in 'ik220_para_displ.pas' {para_display},
  ik220_para_ssi in 'ik220_para_ssi.pas' {para_ssi},
  ik220_para_ndat in 'ik220_para_ndat.pas' {para_ndat},
  ik220_para_encoder in 'ik220_para_encoder.pas' {para_encoder},
  ik220_printer in 'ik220_printer.pas' {Output},
  ik220_info_unit in 'ik220_info_unit.pas' {Info},
  visual_unit in 'visual_unit.pas' {Visual},
  display_unit1 in 'display_unit1.pas' {Counter_X},
  adjust_unit in 'adjust_unit.pas' {Drawer},
  ik220_def_unit,
  ik220_base_unit,
  ik220_rdwr_unit,
  ik220_cnt_unit;

var
  load : bool32;

(**************************************************************)
{$R *.RES}
(**************************************************************)
begin

  load:=loadik220dll('IK220DLL.dll');

  if load<>0 then
    begin

      adrzeiger:=@boardadressen;
      Status:=IK220Find(adrzeiger);

      Application.Initialize;
      Application.CreateForm(TWelcome, Welcome);
      Welcome.Left:=5;
      Welcome.Top:=30;

      Application.Run;

    end;

    if load=0 then
      begin
        load:=freeik220dll;
      end;

(**************************************************************)
end.

