unit ik220_rdwr_unit;

interface
{$R+}
uses

 ik220_def_unit,
 ik220_conv_unit,
 ik220_base_unit,
 ik220_mz_unit,
 ik220_cnt_unit;
 
{*******************************************************************
PROCEDUREN
*******************************************************************}
procedure led_on(axis:word);
procedure led_off(axis:word);
procedure led_blink(axis:word);

procedure cnt_wr_para(axis:word;para_number,para_hi,para_lo:word);
procedure cnt_rd_para(axis:word;para_number:word;var para_hi,para_lo:word);

procedure cnt_start(axis:word);
procedure cnt_stopp(axis:word);
procedure cnt_clear(axis:word);
procedure cnt_latch0(axis:word);
procedure cnt_latch1(axis:word);
procedure poll_latch0(axis:word);
procedure poll_latch1(axis:word);

procedure cnt_clr_stat(axis:word);
procedure cnt_clr_ndat(axis:word);

procedure cnt_ref_clear(axis:word);
procedure cnt_ref_start(axis:word);
procedure cnt_ref_stopp(axis:word);

procedure cnt_ref_run(axis:word);
procedure cnt_ref_pos(axis:word);

procedure cnt_RAM_start_l0_adc(axis:word);
procedure cnt_RAM_start_l1_adc(axis:word);
procedure cnt_RAM_start_l0s_adc(axis:word);
procedure cnt_RAM_start_l1s_adc(axis:word);

procedure cnt_RAM_start_l0(axis:word);
procedure cnt_RAM_start_l1(axis:word);
procedure cnt_RAM_start_l0s(axis:word);
procedure cnt_RAM_start_l1s(axis:word);

procedure cnt_RAM_stopp(axis:word);
procedure cnt_RAM_reset(axis:word);

procedure cnt_Timer_start(axis:word);
procedure cnt_Timer_stopp(axis:word);
procedure cnt_Timer_set(axis:word;var wert0:longint);

procedure cnt_get0_ram_single(axis:word);
procedure cnt_get1_ram_single(axis:word);

procedure cnt_get0_ram_block(axis:word);
procedure cnt_get1_ram_block(axis:word);

procedure cnt_soft_reg0(axis:word);
procedure cnt_get_reg0(axis:word);
procedure cnt_getssi_reg0(axis:word);
procedure cnt_getndat_reg0(axis:word);

procedure cnt_soft_reg1(axis:word);
procedure cnt_get_reg1(axis:word);
procedure cnt_getssi_reg1(axis:word);
procedure cnt_getndat_reg1(axis:word);

procedure cnt_reset_ndat(axis:word);
procedure cnt_config_ndat(axis:word);

procedure cnt_get_corr_e(axis:word);
procedure cnt_get_corr_c(axis:word);

procedure cnt_refstatus(axis:word);
procedure cnt_ampstatus(axis:word);
procedure cnt_ramstatus(axis:word);

procedure Cnt_Init(axis:word);

procedure Start_Counters;
procedure Stop_Counters;

implementation


(******************************************************************)
(******************************************************************)
procedure led_off(axis:word);
begin
  Status:=IK220Led(axis,0);
end;
(******************************************************************)
procedure led_on(axis:word);
begin
  Status:=IK220Led(axis,1);
end;
(******************************************************************)
procedure led_blink(axis:word);
begin
  Status:=IK220Led(axis,2);
end;
(******************************************************************)
(*                                                                *)
(*                                                                *)
(******************************************************************)
procedure cnt_wr_para(axis:word;para_number,para_hi,para_lo:word);
var
  data : longint;
begin
  data:=para_hi;
  data:=data shl 16;
  data:=data + para_lo;
  Status:=IK220WritePar(axis,para_number,data);
end;
(******************************************************************)
(******************************************************************)
procedure cnt_rd_para(axis:word;para_number:word;var para_hi,para_lo:word);
var
  data : longint;
begin
  Status:=IK220ReadPar(axis,para_number,@data);
  para_lo:=data and $0000FFFF;
  para_lo:=data shr 16;;
end;
{******************************************************************)
(*                                                                *)
(*                                                                *)
(******************************************************************}
procedure cnt_start(axis:word);
begin
  Status:=IK220Start(axis);
end;
{******************************************************************)
(******************************************************************}
procedure cnt_stopp(axis:word);
begin
  Status:=IK220Stop(axis);
end;
(******************************************************************)
procedure cnt_clear(axis:word);
begin
  Status:=IK220Reset(axis);
end;
(******************************************************************)
(* REG 0                                                          *)
(******************************************************************)
procedure cnt_latch0(axis:word);
begin
  Status:=IK220Latch(axis,0);
end;
(******************************************************************)
procedure poll_latch0(axis:word);
var
  latched     : bool32;
begin
  latched:=0;
  repeat
    Status:=IK220Latched(axis,0,@latched);
  until latched<>0;
end;
(******************************************************************)
(*  Hardware -> cnt_incr_reg0                                     *)
(******************************************************************)
procedure cnt_get_reg0(axis:word);
var
  latched     : bool32;
  cntval      : double;
  adc         : rdiagnose;
  refsta,samp : word;
begin
  Status:=IK220Latched(axis,0,@latched);
  if latched<>0 then
    begin
      Status:=IK220Get48(axis,0,@cntval);
      cnt^.count[axis]^.position.cnt_incr_reg0:=cntval;
      Status:=IK220CntStatus(axis,0,@refsta,
                             @adc.ad00_corr,@adc.ad90_corr,
                             @adc.ad00_org,@adc.ad90_org,
                             @samp);
      adc.ri_pos:=cnt^.count[axis].diagnose.ri_pos;   (* important!!! *)
      cnt^.count[axis].diagnose:=adc;
    end
end;
(******************************************************************)
procedure cnt_getssi_reg0(axis:word);
var
  stat        : word;
  cntval      : double;
begin
  Status:=IK220ReadSSI(axis,@stat,@cntval);
  cnt^.count[axis]^.position.cnt_incr_reg0:=cntval;
  (*
  if stat<>0 then
    count^.count[axis]^.status.ssierror:=true
  else
    count^.count[axis]^.status.ssierror:=false;
  *)
end;
(******************************************************************)
procedure cnt_getndat_reg0(axis:word);
var
  stat,alarm  : word;
  cntval      : double;
begin
  Status:=IK220ReadEn(axis,@stat,@cntval,@alarm);
  cnt^.count[axis]^.position.cnt_incr_reg0:=cntval;
  if alarm<>0 then
    cnt^.count[axis]^.status.ndatalarm:=true
  else
    cnt^.count[axis]^.status.ndatalarm:=false;
end;
(******************************************************************)
(* REG 1                                                          *)
(******************************************************************)
procedure cnt_latch1(axis:word);
begin
  Status:=IK220Latch(axis,1);
end;
(******************************************************************)
procedure poll_latch1(axis:word);
var
  latched     : bool32;
begin
  latched:=0;
  repeat
    Status:=IK220Latched(axis,1,@latched);
  until latched<>0;
end;
(******************************************************************)
procedure cnt_get_reg1(axis:word);
var
  latched     : bool32;
  cntval      : double;
  adc         : rdiagnose;
  refsta,samp : word;
begin
  Status:=IK220Latched(axis,1,@latched);
  if latched<>0 then
    begin
      Status:=IK220Get48(axis,1,@cntval);
      cnt^.count[axis]^.position.cnt_incr_reg1:=cntval;
      Status:=IK220CntStatus(axis,1,@refsta,
                          @adc.ad00_corr,@adc.ad90_corr,
                          @adc.ad00_org,@adc.ad90_org,
                          @samp);
      cnt^.count[axis].diagnose:=adc;
    end
end;
(******************************************************************)
procedure cnt_getssi_reg1(axis:word);
var
  stat        : word;
  cntval      : double;
begin
  Status:=IK220ReadSSI(axis,@stat,@cntval);
  cnt^.count[axis]^.position.cnt_incr_reg1:=cntval;
  (*
  if stat<>0 then
    count^.count[axis]^.status.ssierror:=true
  else
    count^.count[axis]^.status.ssierror:=false;
  *)
end;
(******************************************************************)
procedure cnt_getndat_reg1(axis:word);
var
  stat,alarm  : word;
  cntval      : double;
begin
  Status:=IK220ReadEn(axis,@stat,@cntval,@alarm);
  cnt^.count[axis]^.position.cnt_incr_reg1:=cntval;
  if alarm<>0 then
    cnt^.count[axis]^.status.ndatalarm:=true
  else
    cnt^.count[axis]^.status.ndatalarm:=false;
end;
(******************************************************************)
(* NDAT                                                           *)
(******************************************************************)
procedure cnt_reset_ndat(axis:word);
var
  stat   : word;
begin
  Status:=IK220ResetEn(axis,@stat);
end;
(******************************************************************)
procedure cnt_config_ndat(axis:word);
var
  stat,typ,turns,ref,dir    : word;
  period,step               : longint;
begin
  Status:=IK220ConfigEn(axis,@stat,@typ,@period,@step,@turns,@ref,@dir);
  if period<=0 then period:=1;
  if period>1000000 then period:=1000000;
  cnt^.count[axis]^.coder.strichzahl:=period;
end;
(******************************************************************)
(* STATUS                                                         *)
(******************************************************************)
procedure cnt_clr_stat(axis:word);
begin
  Status:=IK220ClearErr(axis);
end;
(******************************************************************)
(* EnDat STATUS                                                         *)
(******************************************************************)
procedure cnt_clr_ndat(axis:word);
var status,alarm : word;
begin
  Status:=IK220AlarmEn(axis,@status,@alarm);
end;
(******************************************************************)
(* REF                                                            *)
(******************************************************************)
procedure cnt_ref_clear(axis:word);
begin
  Status:=IK220ResetRef(axis);
end;
(******************************************************************)
procedure cnt_ref_start(axis:word);
begin
  Status:=IK220Startref(axis);
end;
(******************************************************************)
procedure cnt_ref_stopp(axis:word);
begin
  Status:=IK220Stopref(axis);
end;
(******************************************************************)
procedure cnt_ref_run(axis:word);
begin
  Status:=IK220DoRef(axis);
end;
(******************************************************************)
procedure cnt_ref_pos(axis:word);
var
  data        : double;
  period      : longint;
  w,
  intp        : word;
  zw          : real;
begin
  intp:=0;
  period:=0;
  data:=0;
  Status:=IK220positionref(axis,@data,@period,@intp);

  zw:=(intp*360)/4096;
  
  w:=round(zw);
  cnt^.count[axis]^.diagnose.ri_pos:=w;
end;
(******************************************************************)
(*  RAM                                                           *)
(******************************************************************)
(* ADC-Werte *)
(******************************************************************)
procedure cnt_RAM_start_l0_adc(axis:word);
begin
  Status:=IK220ModeRam(axis,17);
end;
(******************************************************************)
procedure cnt_RAM_start_l1_adc(axis:word);
begin
  Status:=IK220ModeRam(axis,18);
end;
(******************************************************************)
procedure cnt_RAM_start_l0s_adc(axis:word);
begin
  Status:=IK220ModeRam(axis,19);
end;
(******************************************************************)
procedure cnt_RAM_start_l1s_adc(axis:word);
begin
  Status:=IK220ModeRam(axis,20);
end;
(******************************************************************)
(* Positionen *)
(******************************************************************)
procedure cnt_RAM_start_l0(axis:word);
begin
  Status:=IK220ModeRam(axis,1);
end;
(******************************************************************)
procedure cnt_RAM_start_l1(axis:word);
begin
  Status:=IK220ModeRam(axis,2);
end;
(******************************************************************)
(* Singel shot *)
(******************************************************************)
procedure cnt_RAM_start_l0s(axis:word);
begin
  Status:=IK220ModeRam(axis,3);
end;
(******************************************************************)
procedure cnt_RAM_start_l1s(axis:word);
begin
  Status:=IK220ModeRam(axis,4);
end;
(******************************************************************)

(******************************************************************)
procedure cnt_RAM_stopp(axis:word);
begin
  Status:=IK220ModeRam(axis,0);
end;
(******************************************************************)
procedure cnt_RAM_reset(axis:word);
begin
  Status:=IK220ResetRam(axis);
end;
(******************************************************************)
(* Timer                                                          *)
(******************************************************************)
procedure cnt_Timer_start(axis:word);
begin
  Status:=IK220ModeTimer(axis,$0001);
end;
(******************************************************************)
procedure cnt_Timer_stopp(axis:word);
begin
  Status:=IK220ModeTimer(axis,$0000);
end;
(******************************************************************)
procedure cnt_Timer_set(axis:word;var wert0:longint);
var
  wert1: longint;
begin
  Status:=IK220SetTimer(axis,wert0,@wert1);
  wert0:=wert1;
end;
(******************************************************************)
(* Zähler-Wert                                                    *)
(******************************************************************)
procedure cnt_soft_reg0(axis:word);
var
  cntval      : double;
  adc         : rdiagnose;
  refsta,samp : word;
begin
  Status:=IK220Read48(axis,0,@cntval);
  cnt^.count[axis]^.position.cnt_incr_reg0:=cntval;
  Status:=IK220CntStatus(axis,0,@refsta,
                      @adc.ad00_corr,@adc.ad90_corr,
                      @adc.ad00_org,@adc.ad90_org,
                      @samp);
  cnt^.count[axis].diagnose:=adc;
end;
(******************************************************************)
procedure cnt_soft_reg1(axis:word);
var
  cntval      : double;
  adc         : rdiagnose;
  refsta,samp : word;
begin
  Status:=IK220Read48(axis,1,@cntval);
  cnt^.count[axis]^.position.cnt_incr_reg1:=cntval;
  Status:=IK220CntStatus(axis,1,@refsta,
                      @adc.ad00_corr,@adc.ad90_corr,
                      @adc.ad00_org,@adc.ad90_org,
                      @samp);
  cnt^.count[axis].diagnose:=adc;
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure cnt_get0_ram_single(axis:word);
var
  cntval             : double;
  rzeig,wzeig,stat   : word;
  period,adc00,adc90 : word;
begin

  if not(cnt^.count[axis]^.diagnose.adcmode) then
    begin
      Status:=IK220GetRam(axis,@cntval,@rzeig,@wzeig,@stat);
    end
  else
    begin
      Status:=IK220GetSig(axis,@period,@adc00,@adc90,@rzeig,@wzeig,@stat);
    end;

  stat:=stat and $0003;
  if (stat=0) then
       begin
         if not(cnt^.count[axis]^.diagnose.adcmode) then
           begin
             cnt^.count[axis]^.position.reg0_adc00:=0;
             cnt^.count[axis]^.position.reg0_adc90:=0;
             cnt^.count[axis]^.position.cnt_incr_reg0:=cntval;
           end
         else
           begin
             cnt^.count[axis]^.position.reg0_adc00:=adc00;
             cnt^.count[axis]^.position.reg0_adc90:=adc90;
             cnt^.count[axis]^.position.cnt_incr_reg0:=0;
           end;
         cnt^.count[axis]^.status.ramoverflow:=false;
         cnt^.count[axis]^.status.ramempty:=false;
       end;
   (* overflow *)
   if (stat and $0001)=$0001 then
       begin
         if not(cnt^.count[axis]^.diagnose.adcmode) then
           begin
             cnt^.count[axis]^.position.reg0_adc00:=0;
             cnt^.count[axis]^.position.reg0_adc90:=0;
             cnt^.count[axis]^.position.cnt_incr_reg0:=cntval;
           end
         else
           begin
             cnt^.count[axis]^.position.reg0_adc00:=adc00;
             cnt^.count[axis]^.position.reg0_adc90:=adc90;
             cnt^.count[axis]^.position.cnt_incr_reg0:=0;
           end;
         cnt^.count[axis]^.status.ramoverflow:=true;
         cnt^.count[axis]^.status.ramempty:=false;
       end;
   (* empty *)
   if (stat and $0002)=$0002 then
       begin
         cnt^.count[axis]^.status.ramoverflow:=false;
         cnt^.count[axis]^.status.ramempty:=true;
       end;
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure cnt_get1_ram_single(axis:word);
var
  cntval             : double;
  rzeig,wzeig,stat   : word;
  period,adc00,adc90 : word;
begin

  if not(cnt^.count[axis]^.diagnose.adcmode) then
    begin
      Status:=IK220GetRam(axis,@cntval,@rzeig,@wzeig,@stat);
    end
  else
    begin
      Status:=IK220GetSig(axis,@period,@adc00,@adc90,@rzeig,@wzeig,@stat);
    end;

  stat:=stat and $0003;
  if (stat=0) then
       begin
         if not(cnt^.count[axis]^.diagnose.adcmode) then
           begin
             cnt^.count[axis]^.position.reg1_adc00:=0;
             cnt^.count[axis]^.position.reg1_adc90:=0;
             cnt^.count[axis]^.position.cnt_incr_reg1:=cntval;
           end
         else
           begin
             cnt^.count[axis]^.position.reg1_adc00:=adc00;
             cnt^.count[axis]^.position.reg1_adc90:=adc90;
             cnt^.count[axis]^.position.cnt_incr_reg1:=0;
           end;
         cnt^.count[axis]^.status.ramoverflow:=false;
         cnt^.count[axis]^.status.ramempty:=false;
       end;
   (* overflow *)
   if (stat and $0001)=$0001 then
       begin
         if not(cnt^.count[axis]^.diagnose.adcmode) then
           begin
             cnt^.count[axis]^.position.reg1_adc00:=0;
             cnt^.count[axis]^.position.reg1_adc90:=0;
             cnt^.count[axis]^.position.cnt_incr_reg1:=cntval;
           end
         else
           begin
             cnt^.count[axis]^.position.reg1_adc00:=adc00;
             cnt^.count[axis]^.position.reg1_adc90:=adc90;
             cnt^.count[axis]^.position.cnt_incr_reg1:=0;
           end;
         cnt^.count[axis]^.status.ramoverflow:=true;
         cnt^.count[axis]^.status.ramempty:=false;
       end;
   (* empty *)
   if (stat and $0002)=$0002 then
       begin
         cnt^.count[axis]^.status.ramoverflow:=false;
         cnt^.count[axis]^.status.ramempty:=true;
       end;
end;
(******************************************************************)
(* Liest aus IK bis Daten-RAM leer in ein dyn. Datenfeld          *)
(* max 256 Werte                                                  *)
(******************************************************************)
procedure cnt_get0_ram_block(axis:word);
var
  stat : word;
begin

  if not(cnt^.count[axis]^.diagnose.adcmode) then
    begin
      Status:=IK220BurstRam(axis,255,@dbuffer.daten[0],@dbuffer.anzahl,@stat);
      if dbuffer.anzahl>0 then
        begin
          cnt^.count[axis]^.position.reg0_adc00:=0;
          cnt^.count[axis]^.position.reg0_adc90:=0;
          cnt^.count[axis]^.position.cnt_incr_reg0:=dbuffer.daten[0];
        end;
    end
  else
    begin
      Status:=IK220BurstSig(axis,255,@intpbuffer.sig[0],@adc0buffer.sig[0],@adc9buffer.sig[0],@intpbuffer.anzahl,@stat);
      if intpbuffer.anzahl>0 then
        begin
          cnt^.count[axis]^.position.reg0_adc00:=adc0buffer.sig[0];
          cnt^.count[axis]^.position.reg0_adc90:=adc9buffer.sig[0];
          cnt^.count[axis]^.position.cnt_incr_reg0:=0;
        end;
    end;

  stat:=stat and $0003;
  if stat=0 then
    begin
      cnt^.count[axis]^.status.ramoverflow:=false;
      cnt^.count[axis]^.status.ramempty:=false;
    end;
  (* overflow *)
  if (stat and $0001)=$0001 then
    begin
      cnt^.count[axis]^.status.ramoverflow:=true;
      cnt^.count[axis]^.status.ramempty:=false;
    end;
  (* empty *)
  if (stat and $0002)=$0002 then
    begin
      cnt^.count[axis]^.status.ramoverflow:=false;
      cnt^.count[axis]^.status.ramempty:=true;
    end;

end;
(******************************************************************)
(* Liest aus IK bis Daten-RAM leer in ein dyn. Datenfeld          *)
(* max 256 Werte                                                  *)
(******************************************************************)
procedure cnt_get1_ram_block(axis:word);
var
  stat : word;
begin

 if not(cnt^.count[axis]^.diagnose.adcmode) then
    begin
      Status:=IK220BurstRam(axis,255,@dbuffer.daten[0],@dbuffer.anzahl,@stat);
      if dbuffer.anzahl>0 then
        begin
          cnt^.count[axis]^.position.reg1_adc00:=0;
          cnt^.count[axis]^.position.reg1_adc90:=0;
          cnt^.count[axis]^.position.cnt_incr_reg1:=dbuffer.daten[0];
        end;
    end
  else
    begin
      Status:=IK220BurstSig(axis,255,@intpbuffer.sig[0],@adc0buffer.sig[0],@adc9buffer.sig[0],@intpbuffer.anzahl,@stat);
      if intpbuffer.anzahl>0 then
        begin
          cnt^.count[axis]^.position.reg1_adc00:=adc0buffer.sig[0];
          cnt^.count[axis]^.position.reg1_adc90:=adc9buffer.sig[0];
          cnt^.count[axis]^.position.cnt_incr_reg1:=0;
        end;
    end;

  stat:=stat and $0003;
  if stat=0 then
    begin
      cnt^.count[axis]^.status.ramoverflow:=false;
      cnt^.count[axis]^.status.ramempty:=false;
    end;
  (* overflow *)
  if (stat and $0001)=$0001 then
    begin
      cnt^.count[axis]^.status.ramoverflow:=true;
      cnt^.count[axis]^.status.ramempty:=false;
    end;
  (* empty *)
  if (stat and $0002)=$0002 then
    begin
      cnt^.count[axis]^.status.ramoverflow:=false;
      cnt^.count[axis]^.status.ramempty:=true;
    end;
end;
{******************************************************************)
(* Korrektur-Werte                                                *)
(******************************************************************}
procedure cnt_get_corr_c(axis:word);
var
  off0,off9,
  pha0,pha1,sym0,sym1  : smallint;
  flag1,flag2          : word;
begin
  Status:=IK220GetCorrB(axis,@off0,@off9,@Pha0,@Pha1,@Sym0,@Sym1,@Flag1,@Flag2);
  cnt^.count[axis]^.hyde_c.off0:=off0;
  cnt^.count[axis]^.hyde_c.off9:=off9;
  cnt^.count[axis]^.hyde_c.pha0:=pha0;
  cnt^.count[axis]^.hyde_c.pha1:=pha1;
  cnt^.count[axis]^.hyde_c.sym0:=sym0;
  cnt^.count[axis]^.hyde_c.sym1:=sym1;
end;
(******************************************************************)
procedure cnt_get_corr_e(axis:word);
var
  off0,off9,
  pha0,pha1,sym0,sym1  : smallint;
  flag1,flag2          : word;
begin
  Status:=IK220GetCorrA(axis,@off0,@off9,@Pha0,@Pha1,@Sym0,@Sym1,@Flag1,@Flag2);
  cnt^.count[axis]^.hyde_e.off0:=off0;
  cnt^.count[axis]^.hyde_e.off9:=off9;
  cnt^.count[axis]^.hyde_e.pha0:=pha0;
  cnt^.count[axis]^.hyde_e.pha1:=pha1;
  cnt^.count[axis]^.hyde_e.sym0:=sym0;
  cnt^.count[axis]^.hyde_e.sym1:=sym1;
end;
{******************************************************************)
(* Ref-Status                                                     *)
(******************************************************************}
procedure cnt_refstatus(axis:word);
var
  refstatus : bool32;
begin
  Status:=IK220RefActive(axis,@refstatus);
  if refstatus=0 then cnt^.count[axis]^.status.ref:=false
  else cnt^.count[axis]^.status.ref:=true;
end;
{******************************************************************)
(* Amplituden-Status                                              *)
{******************************************************************}
procedure cnt_ampstatus(axis:word);
var
  ampstatus : tristatus;
  freqerr,actamp,minamp,maxamp : word;
begin
  Status:=IK220SignalStatus(axis,@freqerr,@actamp,@minamp,@maxamp);
  ampstatus:=typ;
  if (minamp and $0003)=$0003 then
    begin
      ampstatus:=min;
    end;
  if (maxamp and $0003)=$0002 then
    begin
      ampstatus:=max;
    end;
  cnt^.count[axis]^.status.amp:=ampstatus;
end;
{******************************************************************)
(* IK-RAM-Status                                                      *)
{******************************************************************}
procedure cnt_ramstatus(axis:word);
var
  istatus : longint;
begin
  Status:=IK220Status(axis,@istatus);
  if (istatus and $0400 = $0400) then
    begin
      cnt^.count[axis]^.status.ramrdy:=true;
    end
  else
    begin
      cnt^.count[axis]^.status.ramrdy:=false;
    end;
end;
{******************************************************************)
(* Download aller Parameter                                       *)
(******************************************************************}
procedure Cnt_Init(axis:word);

 const
   geber_typ_para             = 1;
   geber_u_a_para             = 2;
   geber_lin_rot_para         = 3;
   geber_dir_para             = 4;
   geber_period_para          = 5;
   geber_refdis_para          = 6;
   count_intp_para            = 7;
   comp_calc_para             = 8;
   comp_corr_para             = 9;
   comp_cnt_para              = 10;
   timer_val0_para            = 11;
   timer_val1_para            = 12;
   ram_val_para               = 13;
   latch_en_para              = 14;
   ssi_bitcnt_para            = 15;
   ssi_parity_para            = 16;
   ssi_bin_gray_para          = 17;
   ssi_prenull_para           = 18;
   ssi_postnull_para          = 19;
   comp_daempf_para           = $1000;
   corr_freigabe              = $1001;

var
  longbuffer    : longint;
  buffer,
  loword,hiword : word;
begin
      (* Protokoll *)
      case cnt^.count[axis]^.coder.inttype of
      analog : begin
                 cnt_wr_para(axis,geber_typ_para,$0000,$0000);
               end;
      ndat   : begin
                 cnt_wr_para(axis,geber_typ_para,$0000,$0001);
                 cnt_config_ndat(axis);
               end;
      ssi    : begin
                 cnt_wr_para(axis,geber_typ_para,$0000,$0002);
               end;
      end;

      (* Interface *)
      case cnt^.count[axis]^.coder.sysin of
      strom    : begin
                   cnt_wr_para(axis,geber_u_a_para,$0000,$0000);
                 end;
      spannung : begin
                   cnt_wr_para(axis,geber_u_a_para,$0000,$0001);
                 end;
      end;

      (* Bandwidth *)

      if cnt^.count[axis]^.coder.speed then
        begin
             wr_mz_port(axis,$0001,$0000);                (* fast *)
             wr_mz_port(axis,$0000,$1006);
        end
      else
        begin
             wr_mz_port(axis,$0001,$0001);
             wr_mz_port(axis,$0000,$1006);
        end;

      (* Encoder *)
      case cnt^.count[axis]^.display.system of
      mm        : begin
                    cnt_wr_para(axis,geber_lin_rot_para,$0000,$0001);
                  end;
      dgr       : begin
                    cnt_wr_para(axis,geber_lin_rot_para,$0000,$0002);
                    longbuffer:=cnt^.count[axis]^.coder.strichzahl and $0000FFFF;
                    loword:=longbuffer;
                    longbuffer:=cnt^.count[axis]^.coder.strichzahl and $FFFF0000;
                    longbuffer:=longbuffer shr 16;
                    hiword:=longbuffer;
                    cnt_wr_para(axis,geber_lin_rot_para,hiword,loword);
                  end;
      end;

      (* Zählrichtung *)
      if cnt^.count[axis]^.display.direction=normal then
        begin
          cnt_wr_para(axis,geber_dir_para,$0000,$0000);
        end
      else
        begin
          cnt_wr_para(axis,geber_dir_para,$0000,$0001);
        end;

      (* Ref-Distance *)
      if cnt^.count[axis]^.coder.refmark=coded then
        begin
          cnt_wr_para(axis,geber_refdis_para,$0000,cnt^.count[axis]^.coder.refdistance);
        end
      else
        begin
          cnt_wr_para(axis,geber_refdis_para,$0000,$0000);
        end;

      (* Interpolation *)
      cnt_wr_para(axis,count_intp_para,$0000,cnt^.count[axis].display.interpolation);

      (* Kompensation Calculate *)
      if cnt^.count[axis]^.parameter.encalc then
        begin
          cnt_wr_para(axis,comp_calc_para,$0000,$0001);
        end
      else
        begin
          cnt_wr_para(axis,comp_calc_para,$0000,$0000);
        end;

      (* Kompensation Correct *)
      if cnt^.count[axis]^.parameter.encorrect then
        begin
          cnt_wr_para(axis,comp_corr_para,$0000,$0001);
        end
      else
        begin
          cnt_wr_para(axis,comp_corr_para,$0000,$0000);
        end;

      (* Daempfung *)
      cnt_wr_para(axis,comp_daempf_para,$0000,cnt^.count[axis].parameter.daempf);

      (* Einzel-Freigabe der Korrektur *)
      cnt_wr_para(axis,corr_freigabe,$0000,$000F);

      (* Correct Values *)
      cnt_wr_para(axis,comp_cnt_para,$0000,cnt^.count[axis].parameter.corrpoints);

      (* Timer *)
      cnt_Timer_set(axis,cnt^.count[axis]^.parameter.timvalue);

      (* RAM-Values *)
      cnt_wr_para(axis,ram_val_para,$0000,cnt^.count[axis]^.parameter.ramvalue);

      (* Hardware-Latch Enable *)
      buffer:=0;
      if cnt^.count[axis]^.parameter.enl0 then
        begin
          buffer:=buffer or 1;
        end;
      if cnt^.count[axis]^.parameter.entim then
        begin
          buffer:=buffer or 2;
        end;
      if cnt^.count[axis]^.parameter.enl1 then
        begin
          buffer:=buffer or 4;
        end;
      cnt_wr_para(axis,latch_en_para,$0000,buffer);

      (* SSI-Parameters *)
      cnt_wr_para(axis,ssi_bitcnt_para,$0000,cnt^.count[axis]^.ssipara.code_bits);
      cnt_wr_para(axis,ssi_prenull_para,$0000,cnt^.count[axis]^.ssipara.prezero_bits);
      cnt_wr_para(axis,ssi_postnull_para,$0000,cnt^.count[axis]^.ssipara.postzero_bits);
      buffer:=0;
      if cnt^.count[axis]^.ssipara.parity=even then buffer:=1;
      if cnt^.count[axis]^.ssipara.parity=evenzero then buffer:=2;
      cnt_wr_para(axis,ssi_parity_para,$0000,buffer);
      buffer:=0;
      if cnt^.count[axis]^.ssipara.gray then buffer:=1;
      cnt_wr_para(axis,ssi_bin_gray_para,$0000,buffer);

     (* NDAT-Parameters *)


end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Start_Counters;
var
  axis : word;
begin
  for axis:=0 to xnumber do
    begin
      (* LED-blink *)
      Status:=IK220Led(axis,0);
      (* Parameter initialisieren *)
      cnt_init(axis);
      (* Clear Counter *)
      Status:=IK220Reset(axis);
      (* Start Counter *)
      Status:=IK220Start(axis);
      (* Referenz *)
      if cnt^.count[axis]^.coder.refmark=one then
        begin
          Status:=IK220DoRef(axis);
        end;
      if cnt^.count[axis]^.coder.refmark=coded then
        begin
          Status:=IK220DoRef(axis);
        end;
    end;
end;
(******************************************************************)
(*                                                                *)
(******************************************************************)
procedure Stop_Counters;
var
  axis : word;
begin
  for axis:=0 to xnumber do
    begin
      (* Blink off *)
      Status:=IK220Led(axis,1);
      (* Counter stop *)
      Status:=IK220Stop(axis);
    end;
  dispose_pointers;
end;
{*******************************************************************
                             ENDE
*******************************************************************}
end.