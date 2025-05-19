// Gracias KeyGen2009 por enseñar a realizar este proyecto
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>

using namespace std;

DWORD pid;
HANDLE hp;

// Importamos los modulos
#include "KGModulos.h"

uintptr_t CLIENT; //Para guardar el valor del client.dll, esto puede variar
uintptr_t FL_ONGROUD_OFFSET = 0x781C80; //Dirección de memoria del jugador en tierra
uintptr_t FL_CHAT_WINDOWS = 0x7579E8; //Direección de memoria de la ventana de chat
uintptr_t FL_ONGROUD[2]; //Para guardar la suma de direcciones entre el client.dll + FL_.._..
int ground[2]; //Para guardar el valor que se obtiene al leer el proceso de memoria

// Prototipo de Funciones
bool OpenChat();
void kgPulsa();


int main() {

	hp = kg_abrir_proceso("left4dead2.exe", PROCESS_ALL_ACCESS);
	CLIENT = kg_obtener_modulo(pid, L"client.dll");
	FL_ONGROUD[0] = CLIENT + FL_ONGROUD_OFFSET;

	while (1) {
		ReadProcessMemory(hp, (LPCVOID)FL_ONGROUD[0], &ground[0], sizeof(ground[0]), 0);
		bool chat = OpenChat();
		if (GetKeyState(VK_SPACE) &0X80000) {
			ReadProcessMemory(hp, (LPCVOID)FL_ONGROUD[0], &ground[0], sizeof(ground[0]), 0);
			if (((ground[0] > 1) || (ground[0] > 0)) && !(chat)) {
				kgPulsa();
			}
			else {
				ReadProcessMemory(hp, (LPCVOID)FL_ONGROUD[0], &ground[0], sizeof(ground[0]), 0);
			}
			
		}
	}
	
	return 0;
}
//Verificar si la ventana de chat esta aberta
bool OpenChat() {
	
	hp = kg_abrir_proceso("left4dead2.exe", PROCESS_ALL_ACCESS);
	CLIENT = kg_obtener_modulo(pid, L"client.dll");
	FL_ONGROUD[1] = CLIENT + FL_CHAT_WINDOWS;
	ReadProcessMemory(hp, (LPCVOID)FL_ONGROUD[1], &ground[1], sizeof(ground[1]), 0);
	if (ReadProcessMemory(hp, (LPCVOID)FL_ONGROUD[1], &ground[1], sizeof(ground[1]), 0))
	{
		ReadProcessMemory(hp, (LPCVOID)FL_ONGROUD[1], &ground[1], sizeof(ground[1]), 0);
		if (ground[1] > 0) {
			return true; //Si la ventana esta abirta retorna un True
		}
		else {
			return false; //Si la ventana esta cerrada retorna un False
		}
	}
	// return false; //Descomentar si es necesario
	
}
// simular la tecla SPACE
void kgPulsa() {
	INPUT kg[2];
	kg[0].type = INPUT_KEYBOARD;
	kg[0].ki.wScan = 0x25;
	kg[0].ki.dwFlags = KEYEVENTF_SCANCODE;

	kg[1].type = INPUT_KEYBOARD;
	kg[1].ki.wScan = 0x25;
	kg[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

	SendInput(1, kg, sizeof(INPUT));
	Sleep(2);
	SendInput(2, kg, sizeof(INPUT));
}
