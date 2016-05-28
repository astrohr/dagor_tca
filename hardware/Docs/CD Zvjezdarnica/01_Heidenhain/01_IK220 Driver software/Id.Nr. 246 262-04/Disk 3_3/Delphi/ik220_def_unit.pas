UNIT ik220_def_unit;

interface
(******************************************************************)
uses
  Windows,
  display_unit1;
(******************************************************************)
const
  maxaxis        = 15; (* 0..15 axis *)
(******************************************************************)
type

(******************************************************************)
  tristatus      = (min,max,typ);
  latchmodus     = (disable,pctimer);
  sourcemodus    = (off,
                    reg0,reg1,
                    reg0turns,
                    blockram,
                    ssipos,ndatpos);
(******************************************************************)
  string255      = string[255];
  offset         = array[0..3] of double;
  loadram        = array [0..255] of word;
(******************************************************************)

  rdisplay       = record
                     resolut       : word;                  (* resolution [0.1um] *)
                     digits        : byte;                  (* digits *)
                     interpolation : word;                  (* bits of interpolation *)
                     system        : (mm,dgr,inc); (* display value *)
                     direction     : (normal,inverted);     (* count direction *)
                     enline0       : boolean;               (* display-line enable *)
                     enline1       : boolean;
                     enstatus      : boolean;
                     sourceline0   : sourcemodus;
                     sourceline1   : sourcemodus;
                   end;
  rposition     = record
                     counter_disp0 : double;                (* display value *)
                     counter_disp1 : double;

                     preset        : double;
                     dimension     : byte;
                     dimoffset     : offset;

                     cnt_incr_reg0 : double;
                     cnt_incr_reg1 : double;

                     cnt_mm_reg0   : double;
                     cnt_mm_reg1   : double;

                     reg0_adc00    : word;
                     reg0_adc90    : word;
                     reg1_adc00    : word;
                     reg1_adc90    : word;

                     sign_0        : char;
                     value_0       : string255;
                     turnsign_0    : char;
                     turnvalue_0   : string255;
                     
                     sign_1        : char;
                     value_1       : string255;
                  end;
  rencoder      = record
                     inttype       : (analog,ndat,ssi); 
                     sysin         : (strom,spannung);
                     periode       : word;                (* period [0.1um] *)
                     strichzahl    : longint;
                     refmark       : (no,one,coded);
                     refdistance   : word;                (* refmark distance *)
                     speed         : boolean;
                  end;
  rparameter    = record
                     encorrect     : boolean;
                     encalc        : boolean;
                     daempf        : word;
                     corrpoints    : byte;
                     enl0          : boolean;
                     enl1          : boolean;
                     entim         : boolean;
                     timvalue      : longint;
                     ramvalue      : word;
                  end;
  rlatchmode    = record
                     reg0_latch    : latchmodus;
                     reg1_latch    : latchmodus;
                  end;
  rcntmode      = record
                     minmax        : tristatus;
                     minwert       : double;
                     maxwert       : double;
                     minbound      : double;
                     maxbound      : double;
                     max           : boolean;
                     min           : boolean;
                     typ           : boolean;
                  end;
  rstatus       = record
                     ref           : boolean;
                     amp           : tristatus;
                     ramempty      : boolean;
                     ramoverflow   : boolean;
                     ramrdy        : boolean;
                     ssierror      : boolean;
                     ndatalarm     : boolean;
                  end;
  rdiagnose     = record
                     ad00_org      : smallint;
                     ad90_org      : smallint;
                     ad00_corr     : smallint;
                     ad90_corr     : smallint;
                     ri_pos        : word;
                     adcmode       : boolean;
                   end;
  rhyde         = record
                     off0          : smallint;
                     off9          : smallint;
                     pha0          : smallint;
                     pha1          : smallint;
                     sym0          : smallint;
                     sym1          : smallint;
                  end;
  rssipara     = record
                     code_bits     : word;
                     prezero_bits  : word;
                     postzero_bits : word;
                     parity        : (non,even,evenzero);
                     gray          : boolean;
                 end;
 (******************************************************************)
 request = record
             order     : (nop,
                          softstart,softstopp,softclear,
                          softlatch0,softlatch1,
                          refstart,refstopp,refclear,refrun,
                          refpos,
                          clearstat,
                          enRAM0,
                          enRAM0s,
                          enRAM0adc,
                          disRAM,resRAM,
                          enTimer,disTimer,
                          getcorr_e,getcorr_c,getcorr_ec,
                          parainit,
                          ndat_config,ndat_reset);
             axis      : word;
           end;
(******************************************************************)
  axis_record   = record
                     display       : rdisplay;
                     position      : rposition;
                     coder         : rencoder;
                     parameter     : rparameter;
                     cntmode       : rcntmode;
                     latchmode     : rlatchmode;
                     status        : rstatus;
                     diagnose      : rdiagnose;
                     hyde_e        : rhyde;
                     hyde_c        : rhyde;
                     ssipara       : rssipara;
                end;
(******************************************************************)
(******************************************************************)
 axis_pointr    = ^axis_record;
 count_pointr   = ^count_record;
 print_pointr   = ^print_record;
(******************************************************************)
(******************************************************************)
 count_record = record
                    count      : array[0..maxaxis] of axis_pointr;
                    axis       : word;       (* selected axis number *)
                    extload    : boolean;
                end;
(******************************************************************)
 countstore   = record
                    count      : array[0..maxaxis] of axis_record;
                    countnr    : byte;
                    extload    : boolean;
                end;
(******************************************************************)
 print_record = record
                    drvname    : char;       (* printer output drive *)
                    dirname    : string[64]; (* printer output directory *)
                    filename   : string[64]; (* printer output file *)
                end;
(******************************************************************)
 printstore   = record
                    drvname    : char;
                    dirname    : string[64];
                    filename   : string[64];
                end;
(******************************************************************)
datenram = record
             daten   : array [0..255] of double;
             anzahl  : word;
           end;
sigram = record
             sig     : array [0..255] of word;
             anzahl  : word;
           end;
(******************************************************************)
(******************************************************************)
var
  xnumber       : word;  (* actuell amount of axis *)
  start         : boolean;
(******************************************************************)
  counter       : array[0..maxaxis] of TCounter_X;
(******************************************************************)
  countaxis     : array[0..maxaxis] of axis_pointr;
  cnt           : count_pointr;
  druck         : print_pointr;
  printfile     : text;
(******************************************************************)
  control_req   : request;
  parameter_req : request;
(******************************************************************)
  dbuffer       : datenram;
  adc0buffer    : sigram;     (* adc00 *)
  adc9buffer    : sigram;     (* adc90 *)
  intpbuffer    : sigram;     (* interp *)
(******************************************************************)
  en_drawer     : boolean;
  en_visual     : boolean;
(******************************************************************)
implementation
(******************************************************************)
end.

