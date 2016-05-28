    .386p
    include e:\ddk\inc32\vmm.inc
    include e:\ddk\inc32\vdmad.inc
    include e:\ddk\inc32\debug.inc

Declare_Virtual_Device IK220VXD, 1, 0, IK220VXD_control,\
    Undefined_Device_ID, Undefined_Init_Order

Begin_Control_Dispatch IK220VxD
    Control_Dispatch Sys_Dynamic_Device_Init, _OnSysDynamicDeviceInit, cCall
    Control_Dispatch Sys_Dynamic_Device_Exit, _OnSysDynamicDeviceExit, cCall
    Control_Dispatch W32_DeviceIOControl,     _OnDeviceIoControl, cCall, <esi>
End_Control_Dispatch   IK220VxD

    end
