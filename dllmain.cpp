#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "detours/detours.h"

typedef HMODULE(WINAPI* t_LoadLibraryW)(LPCWSTR);
t_LoadLibraryW o_LoadLibraryW = LoadLibraryW;

FARPROC fpAddr[17];

void HijackDll()
{
	WCHAR wcsDllPath[MAX_PATH];
	GetSystemDirectory(wcsDllPath, MAX_PATH);
	PathCombine(wcsDllPath, wcsDllPath, L"VERSION.dll");
	HMODULE hModule = LoadLibrary(wcsDllPath);
	fpAddr[0] = GetProcAddress(hModule, "GetFileVersionInfoA");
	fpAddr[1] = GetProcAddress(hModule, "GetFileVersionInfoByHandle");
	fpAddr[2] = GetProcAddress(hModule, "GetFileVersionInfoExA");
	fpAddr[3] = GetProcAddress(hModule, "GetFileVersionInfoExW");
	fpAddr[4] = GetProcAddress(hModule, "GetFileVersionInfoSizeA");
	fpAddr[5] = GetProcAddress(hModule, "GetFileVersionInfoSizeExA");
	fpAddr[6] = GetProcAddress(hModule, "GetFileVersionInfoSizeExW");
	fpAddr[7] = GetProcAddress(hModule, "GetFileVersionInfoSizeW");
	fpAddr[8] = GetProcAddress(hModule, "GetFileVersionInfoW");
	fpAddr[9] = GetProcAddress(hModule, "VerFindFileA");
	fpAddr[10] = GetProcAddress(hModule, "VerFindFileW");
	fpAddr[11] = GetProcAddress(hModule, "VerInstallFileA");
	fpAddr[12] = GetProcAddress(hModule, "VerInstallFileW");
	fpAddr[13] = GetProcAddress(hModule, "VerLanguageNameA");
	fpAddr[14] = GetProcAddress(hModule, "VerLanguageNameW");
	fpAddr[15] = GetProcAddress(hModule, "VerQueryValueA");
	fpAddr[16] = GetProcAddress(hModule, "VerQueryValueW");
}

HMODULE WINAPI h_LoadLibraryW(LPCWSTR lpLibFileName)
{
	auto pResult = o_LoadLibraryW(lpLibFileName);
	if (pResult != NULL) {

		MODULEINFO mInfo;
		HANDLE hProcess = GetCurrentProcess();
		GetModuleInformation(hProcess, pResult, &mInfo, sizeof(mInfo));

		auto pAddr = (PBYTE)pResult;
		for (size_t i = 0; i < mInfo.SizeOfImage - 20; i++) {
			if (memcmp(pAddr + i, L"bootStrap", 20) == 0) {
				WriteProcessMemory(hProcess, pAddr + 0xC3B8, "\x0", 1, NULL);
				WriteProcessMemory(hProcess, pAddr + 0xD90D, "\xB3\x01", 2, NULL);
				DetourTransactionBegin();
				DetourDetach(&o_LoadLibraryW, h_LoadLibraryW);
				DetourTransactionCommit();
				break;
			}
		}
	}
	return pResult;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		HijackDll();

		DetourTransactionBegin();
		DetourAttach(&o_LoadLibraryW, h_LoadLibraryW);
		DetourTransactionCommit();
	}
	return TRUE;
}

void ExpFunc001() { fpAddr[0](); }
void ExpFunc002() { fpAddr[1](); }
void ExpFunc003() { fpAddr[2](); }
void ExpFunc004() { fpAddr[3](); }
void ExpFunc005() { fpAddr[4](); }
void ExpFunc006() { fpAddr[5](); }
void ExpFunc007() { fpAddr[6](); }
void ExpFunc008() { fpAddr[7](); }
void ExpFunc009() { fpAddr[8](); }
void ExpFunc010() { fpAddr[9](); }
void ExpFunc011() { fpAddr[10](); }
void ExpFunc012() { fpAddr[11](); }
void ExpFunc013() { fpAddr[12](); }
void ExpFunc014() { fpAddr[13](); }
void ExpFunc015() { fpAddr[14](); }
void ExpFunc016() { fpAddr[15](); }
void ExpFunc017() { fpAddr[16](); }