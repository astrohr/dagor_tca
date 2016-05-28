unit ik220_printer;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, Grids, Outline, DirOutln, ExtCtrls, FileCtrl,
  ik220_def_unit;

type
  TOutput = class(TForm)
    dir_box: TDirectoryListBox;
    file_box: TFileListBox;
    Label2: TLabel;
    res_button: TButton;
    save: TButton;
    print_single: TButton;
    default: TButton;
    name_box: TEdit;
    drive_box: TDriveComboBox;
    stop_printer: TBitBtn;
    printer_error: TLabel;
    load: TButton;
    print_block: TButton;

    procedure res_buttonClick(Sender: TObject);
    procedure saveClick(Sender: TObject);
    procedure print_singleClick(Sender: TObject);
    procedure print_blockClick(Sender: TObject);
    procedure defaultClick(Sender: TObject);
    procedure stop_printerClick(Sender: TObject);
    procedure loadClick(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Output: TOutput;

procedure rebuild_output(cprint:print_pointr);

implementation

uses
  ik220_prt_unit;

{$R *.DFM}
(*************************************************)
(*************************************************)
procedure update_output(cprint:print_pointr);
begin
   output.dir_box.Update;
   output.file_box.Update;
   output.name_box.text:=cprint^.filename;
   output.name_box.Update;
end;
(*************************************************)
(*************************************************)
procedure rebuild_output(cprint:print_pointr);
begin
  (* Drive *)
  output.drive_box.Drive:=cprint^.drvname;

  (* Directory *)
  output.Dir_box.Drive:=cprint^.drvname;
  output.Dir_box.Directory:=cprint^.dirname;

  (* List files *)
  output.file_box.Drive:=cprint^.drvname;
  output.file_box.Directory:=cprint^.dirname;
  output.file_box.mask:='*.prt';

  (* Enter Name *)
  output.name_box.text:=cprint^.filename;

  (* Error-Output *)
  output.printer_error.caption:='';

  update_output(cprint);
end;
(*************************************************)
(*************************************************)
procedure get_settings(cprint:print_pointr);
begin
  cprint^.drvname:=output.drive_box.Drive;
  cprint^.dirname:=output.dir_box.Directory;
  cprint^.filename:=output.name_box.text;
end;
(*************************************************)
(* Write X,Y to file                             *)
(*************************************************)
procedure TOutput.print_singleClick(Sender: TObject);
begin
  output.printer_error.caption:='';
  get_settings(druck);
  print_wert(druck);
  update_output(druck);
end;
(*************************************************)
(* Write X,Y to file                             *)
(*************************************************)
procedure TOutput.print_blockClick(Sender: TObject);
begin
  output.printer_error.caption:='';
  get_settings(druck);
  print_ram(druck);
  update_output(druck);
end;
(*************************************************)
(*************************************************)
procedure TOutput.res_buttonClick(Sender: TObject);
begin
  get_settings(druck);
  output.printer_error.caption:='';
  reset_printer(druck);
  update_output(druck);
end;
(*************************************************)
(*************************************************)
procedure TOutput.defaultClick(Sender: TObject);
begin
  output.printer_error.caption:='';
  init_printer(druck);
  update_output(druck);
end;
(*************************************************)
(*************************************************)
procedure TOutput.saveClick(Sender: TObject);
begin
  get_settings(druck);
  Wr_Setup_Printer;
  update_output(druck);
end;
(*************************************************)
(*************************************************)
procedure TOutput.stop_printerClick(Sender: TObject);
begin
  get_settings(druck);
  output.hide;
end;
(*************************************************)
(*************************************************)
procedure TOutput.loadClick(Sender: TObject);
begin
  update_output(druck);
  get_settings(druck);
  Rd_Setup_Printer;
  rebuild_output(druck);
end;
(*************************************************)
(*************************************************)

end.
