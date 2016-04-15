unit ik220_info_unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TInfo = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label5: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    asm_version: TLabel;
    drv_version: TLabel;
    dll_version: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    procedure FormActivate(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  Info: TInfo;

implementation

{$R *.DFM}

uses
  ik220_def_unit,
  ik220_base_unit;

(*************************************************)
(*                                               *)
(*************************************************)
procedure TInfo.FormActivate(Sender: TObject);
type
  (* ver_string = array[0..255] of char; *)
  ver_string = TChar;
var
  asm_string,drv_string,dll_string : TChar;
  p_asm,p_drv,p_dll : CharPtr;
  s : string;

begin
  p_asm:=@asm_string;
  p_drv:=@drv_string;
  p_dll:=@dll_string;

  Status:=IK220Version(0,p_asm,p_drv,p_dll);

  s:=asm_string;
  info.asm_version.caption:=s;
  s:=drv_string;
  info.drv_version.caption:=s;
  s:=dll_string;
  info.dll_version.caption:=s;
end;
(*************************************************)
(*                                               *)
(*************************************************)

end.
