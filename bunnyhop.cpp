#include  <Windows.h>
#include  "offsets.hpp"
class BunnyHop {	
public:
	void jump(int flags,  DWORD  clientDLL, HANDLE processHandle) {	
		int valueon = 6;
		if (((flags == 257 || flags == 259 || flags == 261 || flags == 263))){
			WriteProcessMemory(processHandle, (LPVOID)(clientDLL + hazedumper::signatures::dwForceJump), &valueon, sizeof(valueon), NULL);
		}}};