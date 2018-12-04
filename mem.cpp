

#include "offsets.hpp"

using namespace std;

class MemoryManagement {
	DWORD clientDLL;
	HANDLE processHandle;
public:
	MemoryManagement(DWORD clientDLL)  {
		this->clientDLL = clientDLL;
		
	}


	DWORD getLocalPlayer(DWORD clientDLL, HANDLE processHandle) {
		DWORD localPlayer;
		DWORD cliDLL = clientDLL;
		
		
		
		ReadProcessMemory(processHandle, (LPCVOID)(cliDLL + hazedumper::signatures::dwLocalPlayer), &localPlayer, sizeof(localPlayer), NULL);
		if (localPlayer == NULL) {
			return NULL;
		}
		
		return localPlayer;
	}
	DWORD getCrosshairID(DWORD localPlayer, HANDLE processHandle) {
		int crosshairID = 0;
		ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + hazedumper::netvars::m_iCrosshairId), &crosshairID, sizeof(crosshairID), NULL);
		return crosshairID;
	}

	int getFlags(DWORD localPlayer, HANDLE processHandle) {
		int onGround ;
		ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + hazedumper::netvars::m_fFlags), &onGround, sizeof(onGround), NULL);
		if (onGround == NULL) {
		
		}
		
		return  onGround;
	}

	short int getCurrentWeapon(DWORD localPlayer, DWORD clientDLL, HANDLE processHandle) {
		DWORD activeWeapon;
		ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + hazedumper::netvars::m_hActiveWeapon), &activeWeapon, sizeof(activeWeapon), NULL);

		DWORD weaponEntity = activeWeapon & 0xFFF;

		DWORD currentWeapon;
		ReadProcessMemory(processHandle, (LPCVOID)(clientDLL + hazedumper::signatures::dwEntityList + (weaponEntity - 1) * 0x10), &currentWeapon, sizeof(currentWeapon), NULL);

		short int weaponID;
		ReadProcessMemory(processHandle, (LPCVOID)(currentWeapon + hazedumper::netvars::m_iItemDefinitionIndex), &weaponID, sizeof(weaponID), NULL);
		return weaponID;

	}

	int getMyTeam(HANDLE processHandle, DWORD localPlayer) {
		int myTeam;
		ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + 0xF0), &myTeam, sizeof(myTeam), NULL);

		return myTeam;
	}

		int getEnemyTeam(HANDLE processHandle, DWORD entity) {
			int enemyTeam;
		ReadProcessMemory(processHandle, (LPCVOID)(entity + 0xF0), &enemyTeam, sizeof(enemyTeam), NULL);

		return enemyTeam;
	}
	
	 
	void main()

	 {
		DWORD localPlayer = getLocalPlayer(clientDLL, processHandle);
		//cout << localPlayer;
	}






};



