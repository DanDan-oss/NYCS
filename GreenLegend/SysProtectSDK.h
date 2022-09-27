#ifndef __SPROTECT_SDK__
#define __SPROTECT_SDK__


//
// 代码加密水印 该水印默认为快速虚拟机
//

#define SP_Begin(flag) \
	__asm _emit 0xEB \
	__asm _emit 0x1C \
	__asm _emit 0x53 \
	__asm _emit 0x79 \
	__asm _emit 0x73 \
	__asm _emit 0x56 \
	__asm _emit 0x4d \
	__asm _emit 0x50 \
	__asm _emit 0x72 \
	__asm _emit 0x6f \
	__asm _emit 0x74 \
	__asm _emit 0x65 \
	__asm _emit 0x63 \
	__asm _emit 0x74 \
	__asm _emit 0x20 \
	__asm _emit 0x42 \
	__asm _emit 0x65 \
	__asm _emit 0x67 \
	__asm _emit 0x69 \
	__asm _emit 0x6e \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm jmp dword ptr ds : [flag] \
	__asm _emit 0x90


//
// 代码加密水印 该水印默认为变异虚拟机
//

#define SP_VMVariant(flag) \
	__asm _emit 0xEB \
	__asm _emit 0x1E \
	__asm _emit 0x53 \
	__asm _emit 0x79 \
	__asm _emit 0x73 \
	__asm _emit 0x56 \
	__asm _emit 0x4d \
	__asm _emit 0x50 \
	__asm _emit 0x72 \
	__asm _emit 0x6f \
	__asm _emit 0x74 \
	__asm _emit 0x65 \
	__asm _emit 0x63 \
	__asm _emit 0x74 \
	__asm _emit 0x20 \
	__asm _emit 'V' \
	__asm _emit 'a' \
	__asm _emit 'r' \
	__asm _emit 'i' \
	__asm _emit 'a' \
	__asm _emit 'n' \
	__asm _emit 't' \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm jmp dword ptr ds : [flag] \
	__asm _emit 0x90


//
// 代码加密水印 该水印默认为变异+乱序虚拟机
//

#define SP_VMVariantJcc(flag) \
	__asm _emit 0xEB \
	__asm _emit 0x21 \
	__asm _emit 0x53 \
	__asm _emit 0x79 \
	__asm _emit 0x73 \
	__asm _emit 0x56 \
	__asm _emit 0x4d \
	__asm _emit 0x50 \
	__asm _emit 0x72 \
	__asm _emit 0x6f \
	__asm _emit 0x74 \
	__asm _emit 0x65 \
	__asm _emit 0x63 \
	__asm _emit 0x74 \
	__asm _emit 0x20 \
	__asm _emit 'V' \
	__asm _emit 'a' \
	__asm _emit 'r' \
	__asm _emit 'i' \
	__asm _emit 'a' \
	__asm _emit 'n' \
	__asm _emit 't' \
	__asm _emit 'J' \
	__asm _emit 'c' \
	__asm _emit 'c' \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm jmp dword ptr ds : [flag] \
	__asm _emit 0x90


//
// 代码加密水印 该水印默认使用代码变异加密
//

#define SP_CodeVariant(flag) \
	__asm _emit 0xEB \
	__asm _emit 0x22 \
	__asm _emit 0x53 \
	__asm _emit 0x79 \
	__asm _emit 0x73 \
	__asm _emit 0x56 \
	__asm _emit 0x4d \
	__asm _emit 0x50 \
	__asm _emit 0x72 \
	__asm _emit 0x6f \
	__asm _emit 0x74 \
	__asm _emit 0x65 \
	__asm _emit 0x63 \
	__asm _emit 0x74 \
	__asm _emit 0x20 \
	__asm _emit 'V' \
	__asm _emit 'a' \
	__asm _emit 'r' \
	__asm _emit 'i' \
	__asm _emit 'a' \
	__asm _emit 'n' \
	__asm _emit 't' \
	__asm _emit 'C' \
	__asm _emit 'o' \
	__asm _emit 'd' \
	__asm _emit 'e' \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm jmp dword ptr ds : [flag] \
	__asm _emit 0x90


#define SP_End() \
	__asm _emit 0xEB \
	__asm _emit 0x11 \
	__asm _emit 0x53 \
	__asm _emit 0x79 \
	__asm _emit 0x73 \
	__asm _emit 0x56 \
	__asm _emit 0x4d \
	__asm _emit 0x50 \
	__asm _emit 0x72 \
	__asm _emit 0x6f \
	__asm _emit 0x74 \
	__asm _emit 0x65 \
	__asm _emit 0x63 \
	__asm _emit 0x74 \
	__asm _emit 0x45 \
	__asm _emit 0x6e \
	__asm _emit 0x64 \
	__asm _emit 0x90 \
	__asm _emit 0x90

//
// 代码抽取 anti_dump 水印
// 大量使用该水印可非常有效的防止pe文件被脱壳
// 也可以不使用这个水印，而是直接在加壳器上的 代码抽取 设定
// 该水印只适合小片代码加密，最好是加密一行，隔开一行代码
//

#define SP_AntiDumpBegin() \
	__asm _emit 0xEB \
	__asm _emit 0x1d \
	__asm _emit 0x53 \
	__asm _emit 0x79 \
	__asm _emit 0x73 \
	__asm _emit 0x56 \
	__asm _emit 0x4d \
	__asm _emit 0x50 \
	__asm _emit 0x72 \
	__asm _emit 0x6f \
	__asm _emit 0x74 \
	__asm _emit 0x65 \
	__asm _emit 0x63 \
	__asm _emit 0x74 \
	__asm _emit 0x20 \
	__asm _emit 'D' \
	__asm _emit 'u' \
	__asm _emit 'm' \
	__asm _emit 'p' \
	__asm _emit 0x42 \
	__asm _emit 0x65 \
	__asm _emit 0x67 \
	__asm _emit 0x69 \
	__asm _emit 0x6e \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90

#define SP_AntiDumpEnd() \
	__asm _emit 0xEB \
	__asm _emit 0x1a \
	__asm _emit 0x53 \
	__asm _emit 0x79 \
	__asm _emit 0x73 \
	__asm _emit 0x56 \
	__asm _emit 0x4d \
	__asm _emit 0x50 \
	__asm _emit 0x72 \
	__asm _emit 0x6f \
	__asm _emit 0x74 \
	__asm _emit 0x65 \
	__asm _emit 0x63 \
	__asm _emit 0x74 \
	__asm _emit 'D' \
	__asm _emit 'u' \
	__asm _emit 'm' \
	__asm _emit 'p' \
	__asm _emit 0x45 \
	__asm _emit 0x6e \
	__asm _emit 0x64 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90 \
	__asm _emit 0x90


//
// 代码效验水印
// 快速模式
// 返回值
//

#define SP_Crc_Fast(flag) \
__asm _emit 0xEB \
__asm _emit 0x1b \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x66 \
__asm _emit 0x72 \
__asm _emit 0x63 \
__asm _emit 0x72 \
__asm _emit 0x63 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 1 \
__asm mov eax, 0 \
__asm mov dword ptr[flag], eax \
__asm _emit 0x90 \
__asm _emit 0x90

//
// 代码效验水印
// 虚拟化模式
// 返回值
//

#define SP_Crc_Vm(flag) \
__asm _emit 0xEB \
__asm _emit 0x1b \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x76 \
__asm _emit 0x72 \
__asm _emit 0x63 \
__asm _emit 0x72 \
__asm _emit 0x63 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 2 \
__asm mov eax, 0 \
__asm mov dword ptr[flag], eax \
__asm _emit 0x90 \
__asm _emit 0x90

//
// 代码效验水印
// JIT模式
// 返回值
//

#define SP_Crc_Jit(flag) \
__asm _emit 0xEB \
__asm _emit 0x1b \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x6a \
__asm _emit 0x72 \
__asm _emit 0x63 \
__asm _emit 0x72 \
__asm _emit 0x63 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 3 \
__asm mov eax, 0 \
__asm mov dword ptr[flag], eax \
__asm _emit 0x90 \
__asm _emit 0x90


//
// 反调试水印
// 32位模式
// 返回值
//

#define SP_AntiDebug32(flag) \
__asm _emit 0xEB \
__asm _emit 0x21 \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x33 \
__asm _emit 0x32 \
__asm _emit 0x61 \
__asm _emit 0x64 \
__asm _emit 0x62 \
__asm _emit 0x67 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 1 \
__asm mov eax, 0 \
__asm mov dword ptr[flag], eax \
__asm _emit 0x90 \
__asm _emit 0x90


//
// 反调试水印
// 64位模式
// 返回值
//

#define SP_AntiDebug64(flag) \
__asm _emit 0xEB \
__asm _emit 0x21 \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x36 \
__asm _emit 0x34 \
__asm _emit 0x61 \
__asm _emit 0x64 \
__asm _emit 0x62 \
__asm _emit 0x67 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 2 \
__asm mov eax, 0 \
__asm mov dword ptr[flag], eax \
__asm _emit 0x90 \
__asm _emit 0x90


//
// 反调试水印
// 32+64位模式
// 返回值
//

#define SP_AntiDebug32_64(flag) \
__asm _emit 0xEB \
__asm _emit 0x21 \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x39 \
__asm _emit 0x36 \
__asm _emit 0x61 \
__asm _emit 0x64 \
__asm _emit 0x62 \
__asm _emit 0x67 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 3 \
__asm mov eax, 0 \
__asm mov dword ptr[flag], eax \
__asm _emit 0x90 \
__asm _emit 0x90

//
// 反调试水印
// 32位模式
// 引发异常模式
//

#define SP_AntiDebug32_Auto() \
__asm _emit 0xEB \
__asm _emit 0x26 \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x33 \
__asm _emit 0x61 \
__asm _emit 0x61 \
__asm _emit 0x64 \
__asm _emit 0x62 \
__asm _emit 0x67 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 4 \
__asm mov eax, 0 

//
// 反调试水印
// 64位模式
// 引发异常模式
//

#define SP_AntiDebug64_Auto() \
__asm _emit 0xEB \
__asm _emit 0x26 \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x36 \
__asm _emit 0x61 \
__asm _emit 0x61 \
__asm _emit 0x64 \
__asm _emit 0x62 \
__asm _emit 0x67 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 5 \
__asm mov eax, 0 

//
// 反调试水印
// 64位模式
// 引发异常模式
//

#define SP_AntiDebug32_64_Auto() \
__asm _emit 0xEB \
__asm _emit 0x26 \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x39 \
__asm _emit 0x61 \
__asm _emit 0x61 \
__asm _emit 0x64 \
__asm _emit 0x62 \
__asm _emit 0x67 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 6 \
__asm mov eax, 0 

//
// 检测虚拟机运行环境
// 返回值
//

#define SP_CheckVMac(flag) \
__asm _emit 0xEB \
__asm _emit 0x1b \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x72 \
__asm _emit 0x63 \
__asm _emit 0x76 \
__asm _emit 0x6d \
__asm _emit 0x61 \
__asm _emit 0x63 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm mov eax, 0 \
__asm mov dword ptr[flag], eax \
__asm _emit 0x90 \
__asm _emit 0x90

#define SP_CheckVMac_Auto() \
__asm _emit 0xEB \
__asm _emit 0x1b \
__asm _emit 0x53 \
__asm _emit 0x79 \
__asm _emit 0x73 \
__asm _emit 0x56 \
__asm _emit 0x4d \
__asm _emit 0x50 \
__asm _emit 0x72 \
__asm _emit 0x6f \
__asm _emit 0x74 \
__asm _emit 0x65 \
__asm _emit 0x63 \
__asm _emit 0x74 \
__asm _emit 0x20 \
__asm _emit 0x73 \
__asm _emit 0x70 \
__asm _emit 0x62 \
__asm _emit 0x63 \
__asm _emit 0x76 \
__asm _emit 0x6d \
__asm _emit 0x61 \
__asm _emit 0x63 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x8b \
__asm _emit 0xc0 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 \
__asm _emit 0x90 

#endif	// __SPROTECT_SDK__