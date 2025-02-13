#include <windows.h>
#include <stdio.h>


int main() {
	HANDLE File = CreateFile("File.txt", GENERIC_WRITE | GENERIC_READ, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (File == INVALID_HANDLE_VALUE) {
		printf("Error.");
		return 1;
	}
	const char Text[] = "Hello World!";
	HANDLE FileMapping = CreateFileMapping(File, NULL, PAGE_READWRITE, 0, sizeof(Text), "FileMemory");
	if (FileMapping == INVALID_HANDLE_VALUE || FileMapping == NULL) {
		printf("Error.");
		return 1;
	}
	char* MappedMemory = (char*)MapViewOfFile(FileMapping, FILE_MAP_WRITE, 0, 0, 0);
	if (MappedMemory == INVALID_HANDLE_VALUE || MappedMemory == NULL) {
		printf("Error.");
		return 1;
	}
	memcpy(MappedMemory, Text, sizeof(Text));

	int Size = MultiByteToWideChar(CP_UTF8, 0, MappedMemory, -1, NULL, 0);
	wchar_t* WcharText = new wchar_t[Size];
	MultiByteToWideChar(CP_UTF8, 0, MappedMemory, -1, WcharText, Size);

	HANDLE WFile = CreateFile("FileCopy.txt", GENERIC_WRITE | GENERIC_READ, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (WFile == INVALID_HANDLE_VALUE) {
		printf("Error.");
		return 1;
	}
	HANDLE WFileMapping = CreateFileMapping(WFile, NULL, PAGE_READWRITE, 0, strlen(MappedMemory) * sizeof(wchar_t), "WCharFileMemory");
	if (WFileMapping == INVALID_HANDLE_VALUE || WFileMapping == NULL) {
		printf("Error.");
		return 1;
	}
	wchar_t* WMappedMemory = (wchar_t*)MapViewOfFile(WFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	if (WMappedMemory == INVALID_HANDLE_VALUE || WMappedMemory == NULL) {
		printf("Error.");
		return 1;
	}
	memcpy(WMappedMemory, WcharText, wcslen(WcharText) * sizeof(wchar_t));

	UnmapViewOfFile(MappedMemory);
	UnmapViewOfFile(WMappedMemory);
	CloseHandle(FileMapping);
	CloseHandle(WFileMapping);
	CloseHandle(File);
	CloseHandle(WFile);
	delete WcharText;
	return 0;
}