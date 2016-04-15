UNIT ik220_cnt_unit;

interface
(******************************************************************)
uses
  Windows;
(******************************************************************)
(******************************************************************)
PROCEDURE new_pointers;
PROCEDURE dispose_pointers;

PROCEDURE Default_Counters;
PROCEDURE Wr_Setup_Counters;
PROCEDURE Rd_Setup_Counters;
(******************************************************************)
(* DLL-functions                                                  *)
(******************************************************************)
implementation
uses
  ik220_def_unit;
(******************************************************************)
(*                                                                *)
(******************************************************************)
PROCEDURE new_pointers;
VAR
  axis : word;
BEGIN
  New(cnt);
  for axis:=0 to xnumber do
    begin
      New(countaxis[axis]);
      cnt^.count[axis]:=countaxis[axis];
    end;
  for axis:=xnumber+1 to maxaxis do
    begin
      cnt^.count[axis]:=nil;
    end;
  New(druck);
END;
(******************************************************************)
PROCEDURE dispose_pointers;
VAR
  axis : word;
BEGIN
  Dispose(druck);
  for axis:=0 to xnumber do
    begin
      Dispose(countaxis[axis]);
    end;
  Dispose(cnt);
END;
(******************************************************************)
(* Default values                                                 *)
(******************************************************************)
PROCEDURE Init_wert(axis:axis_pointr);
BEGIN

        axis^.display.resolut        := 1;
        axis^.display.digits         := 5;
        axis^.display.interpolation  := 12;
        axis^.display.system         := mm;
        axis^.display.direction      := normal;
        axis^.display.enline0        := true;
        axis^.display.enline1        := false;
        axis^.display.enstatus       := true;
        axis^.display.sourceline0    := reg0;
        axis^.display.sourceline1    := reg0;

        axis^.position.cnt_incr_reg0 := 0;
        axis^.position.cnt_incr_reg1 := 0;
        axis^.position.cnt_mm_reg0   := 0;
        axis^.position.cnt_mm_reg1   := 0;
        axis^.position.reg0_adc00    := 0;
        axis^.position.reg0_adc90    := 0;
        axis^.position.reg1_adc00    := 0;
        axis^.position.reg1_adc90    := 0;
        axis^.position.counter_disp0 := 0;
        axis^.position.sign_0        := '+';
        axis^.position.value_0       := '0.00000';
        axis^.position.sign_0        := '+';
        axis^.position.value_0       := '0';
        axis^.position.sign_1        := ' ';
        axis^.position.value_1       := '';
        
        axis^.position.dimension     := 0;
        axis^.position.dimoffset[0]  := 0;
        axis^.position.dimoffset[1]  := 0;
        axis^.position.dimoffset[2]  := 0;
        axis^.position.dimoffset[3]  := 0;
        axis^.position.preset        := 100;

        axis^.coder.inttype          := analog; 
        axis^.coder.sysin            := strom;
        axis^.coder.strichzahl       := 1000;
        axis^.coder.refmark          := no;
        axis^.coder.refdistance      := 1000;
        axis^.coder.periode          := 10;
        axis^.coder.speed            := false;

        axis^.parameter.encorrect    := true;
        axis^.parameter.encalc       := true;
        axis^.parameter.corrpoints   := 8;
        axis^.parameter.daempf       := 2; 
        axis^.parameter.enl0         := false;
        axis^.parameter.enl1         := false;
        axis^.parameter.entim        := false;
        axis^.parameter.timvalue     := 1000;
        axis^.parameter.ramvalue     := 100;

        axis^.latchmode.reg0_latch   := pctimer;
        axis^.latchmode.reg1_latch   := disable;

        axis^.cntmode.minmax         := typ;
        axis^.cntmode.minwert        := -100;
        axis^.cntmode.maxwert        := +100;
        axis^.cntmode.minbound       := -100;
        axis^.cntmode.maxbound       := +100;
        axis^.cntmode.min            := false;
        axis^.cntmode.max            := false;
        axis^.cntmode.typ            := false;

        axis^.status.ref             := false;
        axis^.status.amp             := typ;
        axis^.status.ramempty        := false;
        axis^.status.ramoverflow     := false;
        axis^.status.ssierror        := false;
        axis^.status.ndatalarm       := false;

        axis^.diagnose.ad00_org      := 0;
        axis^.diagnose.ad90_org      := 0;
        axis^.diagnose.ad00_corr     := 0;
        axis^.diagnose.ad90_corr     := 0;
        axis^.diagnose.ri_pos        := 0;
        axis^.diagnose.adcmode       := false;

        axis^.yde_e.off0            := 0;
        axis^.yde_e.off9            := 0;
        axis^.yde_e.pha0            := 0;
        axis^.yde_e.pha1            := 0;
        axis^.yde_e.sym0            := 0;
        axis^.yde_e.sym1            := 0;

        axis^.yde_c.off0            := 0;
        axis^.yde_c.off9            := 0;
        axis^.yde_c.pha0            := 0;
        axis^.yde_c.pha1            := 0;
        axis^.yde_c.sym0            := 0;
        axis^.yde_c.sym1            := 0;

        axis^.ssipara.code_bits      := 19;
        axis^.ssipara.prezero_bits   := 0;
        axis^.ssipara.postzero_bits  := 0;
        axis^.ssipara.parity         := even;
        axis^.ssipara.gray           := true;

        dbuffer.anzahl               := 0;
        dbuffer.daten[0]             := 0;

        adc0buffer.anzahl            := 0;
        adc0buffer.sig[0]            := 0;
        adc9buffer.anzahl            := 0;
        adc9buffer.sig[0]            := 0;
        intpbuffer.anzahl            := 0;
        intpbuffer.sig[0]            := 0;

END;
(******************************************************************)
(* Initialize axis with default values                                  *)
(******************************************************************)
PROCEDURE Default_Counters;
VAR
  axis : word;
BEGIN
    cnt^.axis:=xnumber;
    cnt^.extload:=false;
    for axis:=0 to xnumber do
      begin
        Init_wert(cnt^.count[axis]);
      end;
END;
(******************************************************************)
(* Save setup in file:  IK220.CNT                                 *)
(******************************************************************)
PROCEDURE Wr_Setup_Counters;
VAR
  initfile        : file of countstore_neu;
  buffer          : countstore_neu;
  axis            : word;
BEGIN
  AssignFile(initfile,'ik220_v01.cnt');
  {$I-}
  Rewrite(initfile);
  {$I+}
  if ioresult=0 then
    begin
      for axis:=0 to xnumber do
        begin
          buffer.count[axis]:=cnt^.count[axis]^;
        end;
      buffer.extload:=cnt^.extload;
      buffer.countnr:=xnumber;
      write(initfile,buffer);
      CloseFile(initfile);
    end;
END;
(******************************************************************)
(* Load setup                                                     *)
(******************************************************************)
PROCEDURE Rd_Setup_Counters;
VAR
  axis              : word;
  anzahl            : byte;
  
  initfile          : file of countstore_neu;
  buffer            : countstore_neu;

  oldfile           : file of countstore;
  buffer_old        : countstore;

  nofile            : boolean;
BEGIN
  nofile:=true;
  AssignFile(initfile,'ik220_v01.cnt');
  {$I-}
  Reset(initfile);
  {$I+}
  if ioresult=0 then
    begin
      {$I-}
      read(initfile,buffer);
      {$I+}
      if ioresult=0 then
        begin

          if xnumber<=buffer.countnr then
            begin
              anzahl:=xnumber;
            end
          else
            begin
              anzahl:=buffer.countnr;
            end;

          for axis:=0 to anzahl do
            begin
              (* not all parameters are stored !! *)

              cnt^.count[axis]^.display.resolut      :=buffer.count[axis].display.resolut;
              cnt^.count[axis]^.display.digits       :=buffer.count[axis].display.digits;
              cnt^.count[axis]^.display.interpolation:=buffer.count[axis].display.interpolation;
              cnt^.count[axis]^.display.system       :=buffer.count[axis].display.system;
              cnt^.count[axis]^.display.direction    :=buffer.count[axis].display.direction;
              cnt^.count[axis]^.display.enline0      :=buffer.count[axis].display.enline0;
              cnt^.count[axis]^.display.enline1      :=buffer.count[axis].display.enline1;
              cnt^.count[axis]^.display.enstatus     :=buffer.count[axis].display.enstatus;
              cnt^.count[axis]^.display.sourceline0  :=buffer.count[axis].display.sourceline0;
              cnt^.count[axis]^.display.sourceline1  :=buffer.count[axis].display.sourceline1;

              cnt^.count[axis]^.position.dimension   :=buffer.count[axis].position.dimension;
              cnt^.count[axis]^.position.dimoffset[0]:=buffer.count[axis].position.dimoffset[0];
              cnt^.count[axis]^.position.dimoffset[1]:=buffer.count[axis].position.dimoffset[1];
              cnt^.count[axis]^.position.dimoffset[2]:=buffer.count[axis].position.dimoffset[2];
              cnt^.count[axis]^.position.dimoffset[3]:=buffer.count[axis].position.dimoffset[3];

              cnt^.count[axis]^.coder.inttype        :=buffer.count[axis].coder.inttype;
              cnt^.count[axis]^.coder.sysin          :=buffer.count[axis].coder.sysin;
              cnt^.count[axis]^.coder.strichzahl     :=buffer.count[axis].coder.strichzahl;
              cnt^.count[axis]^.coder.refmark        :=buffer.count[axis].coder.refmark;
              cnt^.count[axis]^.coder.refdistance    :=buffer.count[axis].coder.refdistance;
              cnt^.count[axis]^.coder.periode        :=buffer.count[axis].coder.periode;
              cnt^.count[axis]^.coder.speed          :=buffer.count[axis].coder.speed;

              cnt^.count[axis]^.parameter.encorrect  :=buffer.count[axis].parameter.encorrect;
              cnt^.count[axis]^.parameter.encalc     :=buffer.count[axis].parameter.encalc;
              cnt^.count[axis]^.parameter.corrpoints :=buffer.count[axis].parameter.corrpoints;
              cnt^.count[axis]^.parameter.enl0       :=buffer.count[axis].parameter.enl0;
              cnt^.count[axis]^.parameter.enl1       :=buffer.count[axis].parameter.enl1;
              cnt^.count[axis]^.parameter.entim      :=buffer.count[axis].parameter.entim;
              cnt^.count[axis]^.parameter.timvalue   :=buffer.count[axis].parameter.timvalue;
              cnt^.count[axis]^.parameter.ramvalue   :=buffer.count[axis].parameter.ramvalue;

              cnt^.count[axis]^.latchmode.reg0_latch :=buffer.count[axis].latchmode.reg0_latch;
              cnt^.count[axis]^.latchmode.reg1_latch :=buffer.count[axis].latchmode.reg1_latch;

              cnt^.count[axis]^.cntmode.minmax       :=buffer.count[axis].cntmode.minmax;
              cnt^.count[axis]^.cntmode.minwert      :=buffer.count[axis].cntmode.minwert;
              cnt^.count[axis]^.cntmode.maxwert      :=buffer.count[axis].cntmode.maxwert;
              cnt^.count[axis]^.cntmode.minbound     :=buffer.count[axis].cntmode.minbound;
              cnt^.count[axis]^.cntmode.maxbound     :=buffer.count[axis].cntmode.maxbound;

              cnt^.count[axis]^.ssipara.code_bits    :=buffer.count[axis].ssipara.code_bits;
              cnt^.count[axis]^.ssipara.prezero_bits :=buffer.count[axis].ssipara.prezero_bits;
              cnt^.count[axis]^.ssipara.postzero_bits:=buffer.count[axis].ssipara.postzero_bits;
              cnt^.count[axis]^.ssipara.parity       :=buffer.count[axis].ssipara.parity;
              cnt^.count[axis]^.ssipara.gray         :=buffer.count[axis].ssipara.gray;

            end;
          cnt^.extload:=buffer.extload;
          nofile:=false;
        end;
      CloseFile(initfile);
    end; (* ioresult *)

  if nofile then
    begin
      nofile:=true;
      AssignFile(oldfile,'ik220.cnt');
      {$I-}
      Reset(oldfile);
      {$I+}
      if ioresult=0 then
        begin
          {$I-}
          read(oldfile,buffer_old);
          {$I+}
          if ioresult=0 then
            begin
              if xnumber<=buffer_old.countnr then
                begin
                  anzahl:=xnumber;
                end
              else
                begin
                  anzahl:=buffer_old.countnr;
                end;

              for axis:=0 to anzahl do
                begin
                  cnt^.count[axis]^.display.resolut      :=buffer_old.count[axis].display.resolut/10;
                  cnt^.count[axis]^.display.digits       :=buffer_old.count[axis].display.digits;
                  cnt^.count[axis]^.display.interpolation:=buffer_old.count[axis].display.interpolation;
                  cnt^.count[axis]^.display.system       :=buffer_old.count[axis].display.system;
                  cnt^.count[axis]^.display.direction    :=buffer_old.count[axis].display.direction; 
                  cnt^.count[axis]^.display.enline0      :=buffer_old.count[axis].display.enline0;
                  cnt^.count[axis]^.display.enline1      :=buffer_old.count[axis].display.enline1;
                  cnt^.count[axis]^.display.enstatus     :=buffer_old.count[axis].display.enstatus;
                  cnt^.count[axis]^.display.sourceline0  :=buffer_old.count[axis].display.sourceline0;
                  cnt^.count[axis]^.display.sourceline1  :=buffer_old.count[axis].display.sourceline1;

                  cnt^.count[axis]^.position.dimension   :=buffer_old.count[axis].position.dimension;
                  cnt^.count[axis]^.position.dimoffset[0]:=buffer_old.count[axis].position.dimoffset[0];
                  cnt^.count[axis]^.position.dimoffset[1]:=buffer_old.count[axis].position.dimoffset[1];
                  cnt^.count[axis]^.position.dimoffset[2]:=buffer_old.count[axis].position.dimoffset[2];
                  cnt^.count[axis]^.position.dimoffset[3]:=buffer_old.count[axis].position.dimoffset[3];

                  cnt^.count[axis]^.coder.inttype        :=buffer_old.count[axis].coder.inttype;
                  cnt^.count[axis]^.coder.sysin          :=buffer_old.count[axis].coder.sysin;
                  cnt^.count[axis]^.coder.strichzahl     :=buffer_old.count[axis].coder.strichzahl;
                  cnt^.count[axis]^.coder.refmark        :=buffer_old.count[axis].coder.refmark;
                  cnt^.count[axis]^.coder.refdistance    :=buffer_old.count[axis].coder.refdistance;
                  cnt^.count[axis]^.coder.periode        :=buffer_old.count[axis].coder.periode/10;
                  cnt^.count[axis]^.coder.speed          :=buffer_old.count[axis].coder.speed;

                  cnt^.count[axis]^.parameter.encorrect  :=buffer_old.count[axis].parameter.encorrect;
                  cnt^.count[axis]^.parameter.encalc     :=buffer_old.count[axis].parameter.encalc;
                  cnt^.count[axis]^.parameter.corrpoints :=buffer_old.count[axis].parameter.corrpoints;
                  cnt^.count[axis]^.parameter.enl0       :=buffer_old.count[axis].parameter.enl0;
                  cnt^.count[axis]^.parameter.enl1       :=buffer_old.count[axis].parameter.enl1;
                  cnt^.count[axis]^.parameter.entim      :=buffer_old.count[axis].parameter.entim;
                  cnt^.count[axis]^.parameter.timvalue   :=buffer_old.count[axis].parameter.timvalue;
                  cnt^.count[axis]^.parameter.ramvalue   :=buffer_old.count[axis].parameter.ramvalue;

                  cnt^.count[axis]^.latchmode.reg0_latch :=buffer_old.count[axis].latchmode.reg0_latch;
                  cnt^.count[axis]^.latchmode.reg1_latch :=buffer_old.count[axis].latchmode.reg1_latch;

                  cnt^.count[axis]^.cntmode.minmax       :=buffer_old.count[axis].cntmode.minmax;
                  cnt^.count[axis]^.cntmode.minwert      :=buffer_old.count[axis].cntmode.minwert;
                  cnt^.count[axis]^.cntmode.maxwert      :=buffer_old.count[axis].cntmode.maxwert;
                  cnt^.count[axis]^.cntmode.minbound     :=buffer_old.count[axis].cntmode.minbound;
                  cnt^.count[axis]^.cntmode.maxbound     :=buffer_old.count[axis].cntmode.maxbound;

                  cnt^.count[axis]^.ssipara.code_bits    :=buffer_old.count[axis].ssipara.code_bits;
                  cnt^.count[axis]^.ssipara.prezero_bits :=buffer_old.count[axis].ssipara.prezero_bits;
                  cnt^.count[axis]^.ssipara.postzero_bits:=buffer_old.count[axis].ssipara.postzero_bits;
                  cnt^.count[axis]^.ssipara.parity       :=buffer_old.count[axis].ssipara.parity;
                  cnt^.count[axis]^.ssipara.gray         :=buffer_old.count[axis].ssipara.gray;

                end;
              cnt^.extload:=buffer_old.extload;
              nofile:=false;
            end;
          CloseFile(oldfile);
        end;
    end;

  if nofile then
    begin
      Wr_Setup_Counters;
    end;
    
END;
(******************************************************************)
(******************************************************************)
end.
