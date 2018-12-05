
class Trigger {
public:
	void shoot(int crosshairID, DWORD clientDLL, HANDLE processHandle) { //crosshairID, clientDLL and HANDLE passed from main class
			if (crosshairID > 0) {			
				int shoot = 6; //what we will write to dwForceAttack
				WriteProcessMemory(processHandle, (LPVOID)(clientDLL + hazedumper::signatures::dwForceAttack), &shoot, sizeof(shoot), NULL); // write the shooting action, we write 6 to both +attack and -attack
				Sleep(10); //sleep to save cpu	
			}
	}
};
