#include <math.h>
#include <Windows.h>
#include "offsets.hpp"
#include "enums.h"

using namespace std; //lol
class RCS {
  public:
    void controlRecoil(HANDLE processHandle, DWORD clientDLL, DWORD engineDLL, DWORD localPlayer) {
      DWORD clientState; //get client offset
      ReadProcessMemory(processHandle, (LPCVOID)(engineDLL + hazedumper::signatures::dwClientState), & clientState, sizeof(clientState), NULL);
      static vectors::Vector oldAimPunch; //old aimpunch
      vectors::Vector fixedAngle; //fixed angle

      int inAttack; //check if we are shooting
      ReadProcessMemory(processHandle, (LPCVOID)(clientDLL + hazedumper::signatures::dwForceAttack), & inAttack, sizeof(int), NULL);

      if (inAttack == 5) { //if shooting
        vectors::Vector aimPunch; //current aimpunch
        ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + hazedumper::netvars::m_aimPunchAngle), & aimPunch, sizeof(aimPunch), NULL); //read it
        aimPunch.x = aimPunch.x * 2. f; //fix values since it only gives us half the angle
        aimPunch.y = aimPunch.y * 2. f; //fix values

        vectors::Vector viewAngle; //current view angle
        ReadProcessMemory(processHandle, (LPCVOID)(clientState + hazedumper::signatures::dwClientState_ViewAngles), & viewAngle, sizeof(viewAngle), NULL);

        fixedAngle.x = viewAngle.x - (aimPunch.x - oldAimPunch.x); //fix new angle
        fixedAngle.y = viewAngle.y - (aimPunch.y - oldAimPunch.y);
        fixedAngle.z = 0. f; //reset Z axis(idk why it sets it to a 45 degree tilt to be honest)

        int shotsFired = 0; //dont RCS on tapping
        ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + hazedumper::netvars::m_iShotsFired), & shotsFired, sizeof(shotsFired), NULL);
        if (shotsFired > 1) { //if we are spraying
          WriteProcessMemory(processHandle, (LPVOID)(clientState + hazedumper::signatures::dwClientState_ViewAngles), & fixedAngle, sizeof(fixedAngle), NULL);
        }
        oldAimPunch = aimPunch; //set old to current for the while loopy thing to work properly

      } else {
        oldAimPunch.x = 0; //reset angle
        oldAimPunch.y = 0;
        fixedAngle.x = 0;
        fixedAngle.y = 0;
      }

    }

  vectors::Vector ClampAngles(vectors::Vector AngleToNormalize) //clamp so we dont get Untrusted ban for impossible view angles
  {
    vectors::Vector vec = AngleToNormalize;
    if (vec.x > 89.0 f && vec.x <= 180.0 f) {
      vec.x = 89.0 f;
    }
    while (vec.x > 180. f) {
      vec.x -= 360. f;
    }
    while (vec.x < -89.0 f) {
      vec.x = -89.0 f;
    }
    while (vec.y > 180. f) {
      vec.y -= 360. f;
    }
    while (vec.y < -180. f) {
      vec.y += 360. f;
    }
    return vec;
  }

};
