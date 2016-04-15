

#ifndef _EXPORTS_H_
#define _EXPORTS_H_


BOOL InputW		  (USHORT Axis, USHORT Adr, USHORT* pBuffer);
BOOL InputL		  (USHORT Axis, USHORT Adr, ULONG*  pBuffer);
BOOL OutputW	  (USHORT Axis, USHORT Adr, USHORT Data);

BOOL LatchInt	  (USHORT Card);
BOOL LatchExt	  (USHORT Card);

BOOL IKFind		  (ULONG* pBuffer256);


BOOL OutCmd		  (USHORT Axis, USHORT IK220Cmd);
BOOL OutCmd1	  (USHORT Axis, USHORT IK220Cmd);

BOOL GetVers	  (USHORT Axis, char* pVersCard, char* pVersDrv);

BOOL RamRead	  (USHORT Axis, USHORT Adr, USHORT* pData);
BOOL RamWrite     (USHORT Axis, USHORT Adr, USHORT Data);

BOOL DownLoad	  (USHORT Axis, USHORT* PgmData, DWORD PgmSize);

BOOL WritePar	  (USHORT Axis, USHORT ParNum, ULONG ParVal);
BOOL ReadPar	  (USHORT Axis, USHORT ParNum, ULONG* pParVal);

BOOL SysLed		  (USHORT Card, USHORT Mode);

BOOL FindEnRange  (USHORT Axis, USHORT* pMemAdr, USHORT* pMrsCodeEn);

BOOL SetLatInt	  (USHORT Card, USHORT Mode);
BOOL SetLatExt	  (USHORT Card, USHORT Mode);


#endif