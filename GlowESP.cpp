#include  <Windows.h>
#include  "offsets.hpp"
#include <Thread>
struct RGBA { //player color struct
	float r = 1.f; // 
	float g = 1.f;
	float b = 1.f;
	float a = 1.f;
}RGBAA;
using namespace std; //lol uh
class ESP {
public:
	void Glow(DWORD clientDLL, HANDLE processHandle, DWORD localPlayer) { 
			DWORD glowManager; //get the glowobjectmanager 
			ReadProcessMemory(processHandle, (LPCVOID)(clientDLL + hazedumper::signatures::dwGlowObjectManager), &glowManager, sizeof(glowManager), NULL);
		    bool render = true; //should render ?
			bool occl = true;
			DWORD entity; //entity pointer
			DWORD glowIndex; //glow index of the entit
			int myTeam; //reading my team
			int enemyTeam; //reading enemy team
			ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + 0xF0), &myTeam, sizeof(myTeam), NULL);
			for (int i = 0; i <= 20; i++) { //20 to save cpu lol
				ReadProcessMemory(processHandle, (LPCVOID)(clientDLL + hazedumper::signatures::dwEntityList + (i * 0x10)), &entity, sizeof(entity), NULL); //read in the entity
				int enemyTeam; //read in enemy team->
				ReadProcessMemory(processHandle, (LPCVOID)(entity + 0xF0), &enemyTeam, sizeof(enemyTeam), NULL);
				ReadProcessMemory(processHandle, (LPCVOID)(entity + hazedumper::netvars::m_iGlowIndex), &glowIndex, sizeof(glowIndex), NULL);
				if (enemyTeam != myTeam) {		//if the enemy team is not my team then write memory to force glow		
						WriteProcessMemory(processHandle, (LPVOID)(glowManager + (glowIndex * 0x38) + 0x4), &RGBAA, sizeof(RGBA), NULL);		 //write the rgba struct	
						WriteProcessMemory(processHandle, (LPVOID)(glowManager + (glowIndex * 0x38) + 0x24), &render, sizeof(render), NULL); 
						WriteProcessMemory(processHandle, (LPVOID)(glowManager + (glowIndex * 0x38) + 0x25), &occl, sizeof(occl), NULL);			
					}
			}	
	}
};