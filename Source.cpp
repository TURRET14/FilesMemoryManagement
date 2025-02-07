#include <windows.h>
#include <stdio.h>


void GetArr(char (*Arr)[10]) {
	printf("\nGot Character Array!");
}
int main() {
	HANDLE File = CreateFile("File.txt", GENERIC_WRITE | GENERIC_READ, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (File == INVALID_HANDLE_VALUE) {
		printf("Error.");
		return 1;
	}
	HANDLE FileMapping = CreateFileMapping(File, NULL, PAGE_READWRITE, 0, 256, "FileMemory");
	if (FileMapping == INVALID_HANDLE_VALUE || FileMapping == NULL) {
		printf("Error.");
		return 1;
	}
	char* MappedMemory = (char*)MapViewOfFile(FileMapping, FILE_MAP_WRITE, 0, 0, 0);
	if (MappedMemory == INVALID_HANDLE_VALUE || MappedMemory == NULL) {
		printf("Error.");
		return 1;
	}
	const char Text[] = "Hello World!";
	memcpy(MappedMemory, Text, sizeof(Text));

	int Size = MultiByteToWideChar(CP_UTF8, 0, MappedMemory, -1, NULL, 0);
	wchar_t* WcharText = new wchar_t[Size];
	MultiByteToWideChar(CP_UTF8, 0, MappedMemory, -1, WcharText, Size);

	HANDLE WFile = CreateFile("FileCopy.txt", GENERIC_WRITE | GENERIC_READ, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (WFile == INVALID_HANDLE_VALUE) {
		printf("Error.");
		return 1;
	}
	HANDLE WFileMapping = CreateFileMapping(WFile, NULL, PAGE_READWRITE, 0, 256, "WCharFileMemory");
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


	// Работа С Памятью Массивов
	char Arr1[] = "Hello";
	char Arr2[] = "World";
	char Arr3[sizeof(Arr1) + sizeof(Arr2)] = "\0";
	strcat_s(Arr3, sizeof(Arr1) + sizeof(Arr2), Arr1);
	strcat_s(Arr3, sizeof(Arr1) + sizeof(Arr2), Arr2);
	char* Arr3StrLen = new char[(strlen(Arr1) + strlen(Arr2) + 1)];
	strcpy_s(Arr3StrLen, (strlen(Arr1) + strlen(Arr2) + 1), "\0");
	strcat_s(Arr3StrLen, strlen(Arr1) + strlen(Arr2) + 1, Arr1);
	strcat_s(Arr3StrLen, strlen(Arr1) + strlen(Arr2) + 1, Arr2);
	printf(Arr3);
	printf("\n");
	printf(Arr3StrLen);
	printf("\n");

	wchar_t Str[] = L"Hello";
	printf("%d", sizeof(Str));
	printf("\n");
	wchar_t* StrPtr = new wchar_t[10] {L"Hello"};
	printf("%d", sizeof(StrPtr));
	printf("\n");
	char TwoDimensional[][10] = { "Hello", "World" };
	char (*Ptr)[10] = TwoDimensional;
	char* TwoDimensionalPtr[] = { new char[10] {"Hello"}, new char[10] {"World"} };
	printf(TwoDimensionalPtr[0]);
	printf(TwoDimensionalPtr[1]);
	GetArr(TwoDimensional);
	return 0;
}