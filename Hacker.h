using namespace std;

#ifndef HACKER_H
#define HACKER_H

class Hacker {

public:
	int ID;
	int numGifts;
	float stickerWait;
	float hoodieWait;
	int totCommit; //number of total commits
	int valCommit; //number of valid commits
	float entrySticker; //time of entrance to the Sticker queue
	float leaveSticker; //time of leaving the Sticker queue also the time going to the sticker desk
	float entryHoodie; //time of entrance to the hoodie queue
	float leaveHoodie; //time of leaving the Hoodie queue also the time going tot the hoodie desk

	Hacker(int Id);
    Hacker(const Hacker& hacker);
    Hacker& operator=(const Hacker& hacker);
  
};

#endif