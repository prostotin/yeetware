
#include <Windows.h>
#include "offsets.hpp"
using namespace std;

class Radar { //good old in game radar i love it so much haha m_bSpotted uwu
   public:
      void showRadar(HANDLE processHandle, DWORD clientDLL) {

         int playerSpotted; //check if player is spotted
         int forceSpot = 1; //1 if spotted 0 if not so we force 1
         DWORD entity;

         for (int i = 0; i < 20; i++) { //hmmm

            ReadProcessMemory(processHandle, (LPCVOID)(clientDLL + hazedumper::signatures::dwEntityList + (i * 0x10)), & entity, sizeof(entity), NULL); //iterate through every entity 
            ReadProcessMemory(processHandle, (LPCVOID)(entity + hazedumper::netvars::m_bSpotted), & playerSpotted, sizeof(playerSpotted), NULL); //check if spotted

            if (playerSpotted == 0) { //if not spotted, make it lol
               WriteProcessMemory(processHandle, (LPVOID)(entity + hazedumper::netvars::m_bSpotted), & forceSpot, sizeof(playerSpotted), NULL);
            }
         }
      }
};
