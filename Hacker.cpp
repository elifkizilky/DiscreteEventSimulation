#include "Hacker.h"


Hacker::Hacker(int Id){

	this->ID = Id;
	this->numGifts = 0;
	this->stickerWait = 0;
	this->hoodieWait = 0;
	this->totCommit = 0; //number of total commits
	this->valCommit = 0; //number of valid commits
	this->entrySticker = 0; //time of entrance to the Sticker queue
	this->leaveSticker = 0; //time of leaving the Sticker queue also the time going to the sticker desk
	this->entryHoodie = 0; //time of entrance to the hoodie queue
	this->leaveHoodie = 0; 

}

Hacker::Hacker(const Hacker& hacker){
	this->ID = hacker.ID;
	this->numGifts = hacker.numGifts;
	this->stickerWait = hacker.stickerWait;
	this->hoodieWait = hacker.hoodieWait;
	this->totCommit = hacker.totCommit;
	this->valCommit = hacker.valCommit;
	this->entrySticker = hacker.entrySticker; 
	this->leaveSticker = hacker.leaveSticker; 
	this->entryHoodie = hacker.entryHoodie; 
	this->leaveHoodie = hacker.leaveHoodie;

    }

Hacker& Hacker::operator=(const Hacker& hacker){

	if(this == &hacker){
		return *this;
	}

	this->ID = hacker.ID;
	this->numGifts = hacker.numGifts;
	this->stickerWait = hacker.stickerWait;
	this->hoodieWait = hacker.hoodieWait;
	this->totCommit = hacker.totCommit;
	this->valCommit = hacker.valCommit;
	this->entrySticker = hacker.entrySticker; 
	this->leaveSticker = hacker.leaveSticker; 
	this->entryHoodie = hacker.entryHoodie; 
	this->leaveHoodie = hacker.leaveHoodie;

	return *this;
    
}