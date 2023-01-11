// bhopandtrigger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "offsets.hpp"
#include "mem.cpp"
#include "trigger.cpp"
#include "bunnyhop.cpp"
#include "GlowESP.cpp"
#include "Radar.cpp"
#include "RCS.cpp"
#include "aimbot.cpp"
#include "enums.h"
#include <string>
#include <Thread>
#include <TlHelp32.h>

using namespace std;
#define SPACEBAR GetKeyState(VK_SPACE) 
#define PRESSED 0x8000
#define triggerBind GetKeyState('Q') 
#define F1 GetKeyState(VK_F1)
#define F2 GetKeyState(VK_F2)
#define F3 GetKeyState(VK_F3)
#define F4 GetKeyState(VK_F4)
#define F5 GetKeyState(VK_F5)
#define F6 GetKeyState(VK_F6)
//bools for menu
bool triggerActive = false;
bool bhopActive = false;;
bool glowActive = false;
bool radarActive = false;
bool RCSActive = false;
bool aimActive = false;
//
int toggle = 0;
void checkToggles();
void printStatus(bool active, string name);

DWORD GetModuleBaseExtern(const DWORD dwProcessId, const char *szModuleName) //get modules (panorama.dll and engine.dll) bytheir names, pretty epic
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (!hSnap) return 0;

	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);

	DWORD dwReturn = 0;

	if (Module32First(hSnap, &me)) {
		while (Module32Next(hSnap, &me)) {
			if (lstrcmpi(me.szModule, szModuleName) == 0) {
				dwReturn = (DWORD)me.modBaseAddr;
				break;
			}
		}
	}

	CloseHandle(hSnap);

	return dwReturn;
}


int main()
{
	HWND hwnd = FindWindowA(0, "Counter-Strike: Global Offensive"); //get hwnd to csgo
	DWORD procID;
	GetWindowThreadProcessId(hwnd, &procID); //get procid of csgo
	//cout << procID;
	DWORD clientDLL = GetModuleBaseExtern(procID, "client_panorama.dll"); //find client panorama 
	DWORD engineDLL = GetModuleBaseExtern(procID, "engine.dll"); //find engine dll
	//cout << clientDLL << endl;
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, procID); //open handle to csgo 
	

	MemoryManagement mem = MemoryManagement(clientDLL);
	mem.main();
	Trigger trig = Trigger();
	BunnyHop bhop = BunnyHop();
	ESP esp = ESP();
	Radar radar = Radar();
	RCS rcs = RCS();
	Aimbot aimbot = Aimbot();
	int read;

	while (1) {
		
		checkToggles();

		DWORD localPlayer = mem.getLocalPlayer(clientDLL, processHandle); //get local player 
	

		if (SPACEBAR &  PRESSED && bhopActive) { //bhop check
			int flags = mem.getFlags(localPlayer, processHandle);
			bhop.jump(flags, clientDLL, processHandle);
			
		}
		if (triggerBind & PRESSED && triggerActive) { //trigger check
			trig.shoot(mem.getCrosshairID(localPlayer, processHandle), clientDLL, processHandle);
		}

		if (glowActive) { //glow check
			esp.Glow(clientDLL, processHandle, localPlayer);
		}

		if (radarActive) { //radar check
			radar.showRadar(processHandle, clientDLL);
		} 
		if (RCSActive) { //rcs check and uh no rcs on awp but was fixed with the shotsFired > 1 thingy
		
			if (mem.getCurrentWeapon(localPlayer, clientDLL, processHandle) != enums::WEAPON_AWP && mem.getCurrentWeapon(localPlayer, clientDLL, processHandle) != enums::WEAPON_GLOCK &&mem.getCurrentWeapon(localPlayer, clientDLL, processHandle) != enums::WEAPON_USP_SILENCER ){
				rcs.controlRecoil(processHandle, clientDLL, engineDLL, localPlayer);
			}
		}

		
		if (aimActive) { //aim check, DO NOT USE, aimbot doesnt have smooth or anything so you will get OW banned 100%
			aimbot.aim(processHandle, clientDLL, engineDLL, localPlayer);
		}
		

		Sleep(5); //sleep to save cpu
	}


	
}

void checkToggles() {
	//had to optimize menu so it only updates on a key press 
	if (toggle == 0) {
		printStatus(triggerActive, "Trigger");
		printStatus(bhopActive, "Bhop");
		printStatus(glowActive, "Glow");
		printStatus(radarActive, "Radar");
		printStatus(RCSActive, "RCS");
		printStatus(aimActive, "Aimbot");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		toggle++;
	}
	if (F1 & PRESSED) {
		triggerActive = !triggerActive;
		system("CLS");
		
		printStatus(triggerActive, "Trigger");
		printStatus(bhopActive, "Bhop");
		printStatus(glowActive, "Glow");
		printStatus(radarActive, "Radar");
		printStatus(RCSActive, "RCS");
		printStatus(aimActive, "Aimbot");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	
	if (F2 & PRESSED) {
		bhopActive = !bhopActive;
		system("CLS");
		printStatus(triggerActive, "Trigger");
		printStatus(bhopActive, "Bhop");
		printStatus(glowActive, "Glow");
		printStatus(radarActive, "Radar");
		printStatus(RCSActive, "RCS");
		printStatus(aimActive, "Aimbot");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
	}
	if (F3 & PRESSED) {
		glowActive = !glowActive;
		system("CLS");
		printStatus(triggerActive, "Trigger");
		printStatus(bhopActive, "Bhop");
		printStatus(glowActive, "Glow");
		printStatus(radarActive, "Radar");
		printStatus(RCSActive, "RCS");
		printStatus(aimActive, "Aimbot");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (F4 & PRESSED) {
		radarActive = !radarActive;
		system("CLS");
		
		printStatus(triggerActive, "Trigger");
		printStatus(bhopActive, "Bhop");
		printStatus(glowActive, "Glow");
		printStatus(radarActive, "Radar");
		printStatus(RCSActive, "RCS");
		printStatus(aimActive, "Aimbot");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (F5 & PRESSED) {
		RCSActive = !RCSActive;
		system("CLS");
		printStatus(triggerActive, "Trigger");
		printStatus(bhopActive, "Bhop");
		printStatus(glowActive, "Glow");
		printStatus(radarActive, "Radar");
		printStatus(RCSActive, "RCS");
		printStatus(aimActive, "Aimbot");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}	if (F6 & PRESSED) {
		aimActive= !aimActive;
		system("CLS");
		printStatus(triggerActive, "Trigger");
		printStatus(bhopActive, "Bhop");
		printStatus(glowActive, "Glow");
		printStatus(radarActive, "Radar");
		printStatus(RCSActive, "RCS");
		printStatus(aimActive, "Aimbot");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}
	

	

}

void printStatus(bool active, string name) {
	if (active) {
		cout << name << " is ACTIVE" << endl;	
	}
	else {	
		cout << name << " is NOT ACTIVE" << endl;
	}
}
