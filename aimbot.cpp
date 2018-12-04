#include <Windows.h>
#include "offsets.hpp"
#include "enums.h"

class Aimbot {
public:
	void aim(HANDLE processHandle, DWORD clientDLL, DWORD engineDLL, DWORD localPlayer) {
		int bone = 8; //head

		DWORD clientState; //get Client
		ReadProcessMemory(processHandle, (LPCVOID)(engineDLL + hazedumper::signatures::dwClientState), &clientState, sizeof(clientState), NULL);

	
		int myTeam; //no shooting at teammates pls :(
		ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + 0xF0), &myTeam, sizeof(myTeam), NULL);

		for (int i = 0; i <= 20; i++) { //20 because we are trying to save cpu
			DWORD entity;
			ReadProcessMemory(processHandle, (LPCVOID)(clientDLL + hazedumper::signatures::dwEntityList + (i * 0x10)), &entity, sizeof(entity), NULL); //read entities

			if (entity == NULL) { //if null we dont aim ok
				continue;
			}
			int health; //get enemy health
			ReadProcessMemory(processHandle, (LPCVOID)(entity + hazedumper::netvars::m_iHealth), &health, sizeof(health), NULL);
			if (health < 1) { //if dead also dont aim
				continue;
			}
			
			int enemyTeam; //get team
			ReadProcessMemory(processHandle, (LPCVOID)(entity + 0xF0), &enemyTeam, sizeof(enemyTeam), NULL);
			if (enemyTeam == myTeam) { //no shooting at teammates!!!
				continue;
			}
			vectors::Vector myHead = getMyHead(processHandle, localPlayer); //get my head pos

			vectors::Vector enemyHead = getEnemyHead(processHandle, clientDLL, entity, bone); //get enemy head

			if (enemyHead.x == NULL) { //well no shooting at non existent angles
				continue;
			}
			static vectors::Vector angle; //vector hmm
			setAngle(myHead, enemyHead, angle); //find angle needed
			vectors::Vector clampedAngle = clampAngles(angle); //clamp the found angle
			vectors::Vector currentViewAngle = currentAngles(processHandle, clientState); //find my view angle

			float fov = FOV(clampedAngle, currentViewAngle); //find field of view from player to my view angle

			//cout << "trying to write" << endl;
			if (fov < 5  && GetAsyncKeyState(VK_LBUTTON) & 0x8000) { //check ok
				WriteProcessMemory(processHandle, (LPVOID)(clientState + hazedumper::signatures::dwClientState_ViewAngles), &clampedAngle, sizeof(clampedAngle), NULL); //write 
				//write angle and shoot hmmm
			}
			//m_iHealth

		}


	}

	vectors::Vector getMyHead(HANDLE processHandle, DWORD localPlayer) {
		//set them vectors hmmmm
		vectors::Vector origin;
		vectors::Vector viewVector;
		vectors::Vector myHead;

		ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + 0x134), &origin, sizeof(origin), NULL); //find my origin of view
		ReadProcessMemory(processHandle, (LPCVOID)(localPlayer + 0x104), &viewVector, sizeof(viewVector), NULL); 
		myHead.x = origin.x + viewVector.x; //get my head hmmm
		myHead.y = origin.y + viewVector.y;
		myHead.z = origin.z + viewVector.z;
		//cout << "MY HEAD : " << myHead.x << endl;
		return myHead;
	}

	vectors::Vector getEnemyHead(HANDLE processHandle, DWORD clientDLL, DWORD entity, int bone) {
		DWORD bones; //SESH
		ReadProcessMemory(processHandle, (LPCVOID)(entity + hazedumper::netvars::m_dwBoneMatrix), &bones, sizeof(bones), NULL);

		if (bones == NULL) {
			return{ 0,0,0 };
		}

		vectors::Vector enemyHead;
			
		//get the position of the enemy head basically
		ReadProcessMemory(processHandle, (LPCVOID)(bones + 0x30 * bone + 0x0C), &enemyHead.x, sizeof(enemyHead.x), NULL);
		ReadProcessMemory(processHandle, (LPCVOID)(bones + 0x30 * bone + 0x1C), &enemyHead.y, sizeof(enemyHead.y), NULL);
		ReadProcessMemory(processHandle, (LPCVOID)(bones + 0x30 * bone + 0x2C), &enemyHead.z, sizeof(enemyHead.z), NULL);
	
		return enemyHead;
	}

	void setAngle(vectors::Vector& myHead, vectors::Vector& enemyHead, vectors::Vector& angle) { //find the angle needed to aim
		double delta[3] = { (myHead.x - enemyHead.x), (myHead.y - enemyHead.y), (myHead.z - enemyHead.z) };
		double hyp = sqrt(pow(delta[0], 2) + pow(delta[1], 2));
		float angles[3];
		angles[0] = (float)(atan(delta[2] / hyp) * 57.295779513082); 
		angles[1] = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f); 
		angles[2] = 0.0f;

		if (delta[0] >= 0.0)
		{
			angles[1] += 180.0f;
		}

		if (angles[0] != angles[0]) {
			angles[0] = 0.f;
		}

		if (angles[1] != angles[1]) {
			angles[1] = 0.f;
		}
		if (angles[2] != angles[2]) {
			angles[2] = 0.f;
		}
		angle.x = angles[0]; //Verticaal (Pitch)
		angle.y = angles[1]; //Horizontaal (Yaw)
		angle.z = angles[2];



	}

	vectors::Vector clampAngles(vectors::Vector angle) { //again same as rcs clamp em angles not to get untrusted mid game
		vectors::Vector clampedVector = angle;
		if (clampedVector.x > 89.0f && clampedVector.x <= 180.0f)
		{
			clampedVector.x = 89.0f;
		}
		while (clampedVector.x > 180.f)
		{
			clampedVector.x -= 360.f;
		}
		while (clampedVector.x < -89.0f)
		{
			clampedVector.x = -89.0f;
		}
		while (clampedVector.y > 180.f)
		{
			clampedVector.y -= 360.f;
		}
		while (clampedVector.y < -180.f)
		{
			clampedVector.y += 360.f;
		}
		return clampedVector;
	}

	vectors::Vector currentAngles(HANDLE processHandle, DWORD clientState) { //get current view anglesssss
		vectors::Vector angles;
		ReadProcessMemory(processHandle, (LPCVOID)(clientState + hazedumper::signatures::dwClientState_ViewAngles), &angles, sizeof(angles), NULL);
		//cout << "ANGLES: " << angles.y << endl;
		return angles;
	}


	float FOV(vectors::Vector angle, vectors::Vector myAngle) { //return fov from my crosshair to enemy head
		return sqrt(pow(angle.x - myAngle.x, 2) + pow(angle.y - myAngle.y, 2));
	}


};


