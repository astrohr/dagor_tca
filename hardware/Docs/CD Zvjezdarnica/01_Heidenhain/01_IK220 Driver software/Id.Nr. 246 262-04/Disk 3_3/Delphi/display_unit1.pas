unit display_unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls,Buttons, ExtCtrls;

type
  TCounter_X = class(TForm)
    xback0: TPanel;
    x_display_0: TLabel;
    x_sign_0: TLabel;
    x_display_1: TLabel;
    REF: TLabel;
    units: TLabel;
    x_disp_min: TLabel;
    x_disp_typ: TLabel;
    x_disp_max: TLabel;
    xlabel: TLabel;
    x_min: TLabel;
    x_max: TLabel;
    x_sign_1: TLabel;
    Label1: TLabel;
    x_dim: TLabel;
    minamp: TLabel;
    maxamp: TLabel;
    freqerror: TLabel;
    ram_stat0: TLabel;
    ram_stat1: TLabel;
    ram_rdy: TLabel;
    ssindat: TLabel;
    Label2: TLabel;
  end;

implementation
{$R *.DFM}
end.
