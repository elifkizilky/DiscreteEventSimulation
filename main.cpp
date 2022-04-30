#include "Hacker.h"
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <iomanip>

using namespace std;

struct Event {
	float time;
	string type;
	int hackerId;
	int lenOfCommits;
	int deskId;

	Event(float time, string type, int hackerId){
		this->time = time;
		this->type = type;
		this->hackerId = hackerId;
	}

	Event(float time, string type, int hackerId, int lenOfCommits){
		this->time = time;
		this->type = type;
		this->hackerId = hackerId;
		this->lenOfCommits = lenOfCommits;
	}
	Event(float time, int deskId, string type, int hackerId){
		this->time = time;
		this->type = type;
		this->hackerId = hackerId;
		this->deskId = deskId;
	}
	Event(const Event& event){
		this->time = event.time;
		this->type = event.type;
		this->hackerId = event.hackerId;
		this->lenOfCommits = event.lenOfCommits;
		this->deskId = event.deskId;

	}
	Event& operator=(const Event& event){
		
		if(this == &event){
		return *this;
	}
	    this->time = event.time;
		this->type = event.type;
		this->hackerId = event.hackerId;
		this->lenOfCommits = event.lenOfCommits;
		this->deskId = event.deskId;

		return *this;

	}
};

struct Desk {
	int ID;
	float serviceTime;
	bool availability;

	Desk(int ID, float serviceTime, bool availability){
		this->ID = ID;
		this->serviceTime = serviceTime;
		this->availability = availability;
	}
	Desk(const Desk& desk){
		
		this->ID = desk.ID;
		this->serviceTime = desk.serviceTime;
		this->availability = desk.availability;
	}

	Desk& operator=(const Desk& desk){
		if(this == &desk){
			return *this;
		}

		this->ID = desk.ID;
		this->serviceTime = desk.serviceTime;
		this->availability = desk.availability;

		return *this;
	}

};

class CompareEvent {
public:
    int operator() (const Event* e1, const Event* e2){
        if(abs(e1->time - e2->time) < 0.00001){
        	return e1->hackerId > e2->hackerId;
        }

        if(e1->time - e2->time > 0.00001){
        	return true;
        }

        return false;
    }
};

class CompareHacker {
public:
    int operator() (const Hacker* h1, const Hacker* h2)
    {
    	if(h1->valCommit == h2->valCommit){
    		if(abs(h1->entryHoodie-h2->entryHoodie) < 0.00001){
    			return h1->ID > h2->ID;
    		}
    		return (h1->entryHoodie - h2->entryHoodie) > 0.00001;
    	}
        return h1->valCommit < h2->valCommit;
    }
};

int main(int argc, char* argv[]) {

	 priority_queue <Event*, vector<Event*>, CompareEvent> simulator; // holds the events
	 std::vector<Desk*> stickDesks; //holds the Sticker Desks
	 std::vector<Desk*> hoodDesks; //holds the Hoodie Desks
	 std::vector<Hacker*> hackerList;
	 int lengthCommit = 0; // holds the length of the total commits 
	 int invalEntry = 0; // holds the number of invalid attempts to enter the sticker queue
	 int invalFree = 0; // holds the number of the invalid attempts to get more than 3 gifts
	 float clock = 0.0; //shows the time
	 priority_queue<Hacker*, vector<Hacker*>, CompareHacker> hoodie; //Hoodie queue
	 queue<Hacker*> sticker; //Sticker queue
	 int maxLenSticker = 0; // holds the maximum length of the sticker queue
	 int maxLenHoodie = 0; // holds the maximum length of the hoodie queue
	 float turnTime = 0; // holds the total turnaround time

	string infile_name = argv[1];
	string outfile_name = argv[2];

	ifstream infile;
	infile.open(infile_name);

	ofstream outfile;
	outfile.open(outfile_name); 

	int numHacker; //holds the number of hackers
	infile >> numHacker;

	for (int i = 0; i < numHacker; i++) //Constructs Arrival Events
	{
		float time;
		infile >> time;
		simulator.push(new Event(time, "arrival", i+1));
	}

	int numCommits; //holds the number of Commit Events
	infile >> numCommits;

	for (int i = 0; i < numCommits; i++) //Constructs Code Commit Events
	{
		int id;
		int length;
		float time;
		infile >> id >> length >> time;
		simulator.push(new Event(time, "code commit", id,length));
	}

	int numAttempts; //holds the number of attempts to enter the queue

	infile >> numAttempts;
	for (int i = 0; i < numAttempts; i++)
	{
		int id;
		float time;
		infile >> id >> time;
		simulator.push(new Event(time, "sticker entrance attempt", id)); 
	}

	int nStickDesks; //holds the number of sticker desks
	infile >> nStickDesks;

	for (int i = 0; i < nStickDesks; i++) //Creating Sticker Desks
	{
		float time;
		infile >> time;
		stickDesks.push_back(new Desk(i+1, time, true));
	}

	int hoodieDesks;
	infile >> hoodieDesks;

	for (int i = 0; i < hoodieDesks; i++) // Creating Hoodie Desks
	{
		float time;
		infile >> time;
		hoodDesks.push_back(new Desk(i+1, time, true));
	}
	
	while(!simulator.empty()){
		Event* e = simulator.top();
		if(e->type == "arrival"){ 
			int id = e->hackerId;
			hackerList.push_back(new Hacker(id));
			clock = e->time;
		} 
		else if(e->type == "code commit"){
			int id = e->hackerId;
			int len = e->lenOfCommits;
			lengthCommit += len;
			hackerList[id-1]->totCommit++;
			
			if(len >= 20){
			hackerList[id-1]->valCommit++;
		}

		clock = e->time;
		
		}
		else if(e->type == "sticker entrance attempt"){
			int id = e->hackerId;
			float time = e->time;
			if(hackerList[id-1]->valCommit >= 3 && hackerList[id-1]->numGifts < 3){
				hackerList[id-1]->entrySticker = time;
				bool goDesk = false;
				for (int i = 0; i < stickDesks.size(); i++){
					if(stickDesks[i]->availability){
						stickDesks[i]->availability = false;
						float next = time + stickDesks[i]->serviceTime;
						goDesk = true;
						simulator.push(new Event(next, i+1, "leaving sticker desk", id));
						break;
					}
				}
				if(!goDesk){ //All desks are full, hacker is going to queue 
						sticker.push(hackerList[id-1]); 
						if(sticker.size() > maxLenSticker){
							maxLenSticker = sticker.size();
						}
					}
			} 
			if(hackerList[id-1]->valCommit < 3) { //invalid attempt
				invalEntry++;
			}
			if(hackerList[id-1]->numGifts >= 3){ //free rider is detected
				invalFree++;
			}
			clock = time;

		}
		else if(e->type == "leaving sticker desk"){ //Going into hoodie queue for current hacker, going into sticker desk for waiting hacker
			int dID = e->deskId;
			stickDesks[dID-1]->availability = true;
			int id = e->hackerId;
			float time = e->time;
			if(!sticker.empty()){
				Hacker* temp = sticker.front();
				temp->leaveSticker = time;
				temp->stickerWait += (temp->leaveSticker - temp->entrySticker);
				float next = time + stickDesks[dID-1]->serviceTime;
				stickDesks[dID-1]->availability = false;
				simulator.push(new Event(next,dID,"leaving sticker desk", temp->ID));
				sticker.pop();

			}
			bool goDesk = false;
			for(int i = 0; i < hoodDesks.size(); i++){
				if(hoodDesks[i]->availability){
						hoodDesks[i]->availability = false;
						float next = time + hoodDesks[i]->serviceTime;
						simulator.push(new Event(next, i+1, "leaving hoodie desk", id));
						goDesk = true;
						break;
					}
				}
					if(!goDesk){ //All desks are full, hacker is going to queue
						hackerList[id-1]->entryHoodie = time; 
						hoodie.push(hackerList[id-1]); 
						if(hoodie.size() > maxLenHoodie){
							maxLenHoodie = hoodie.size();
						}
					}
			
			clock = time;  

		}
		else if(e->type == "leaving hoodie desk"){ //Taking gift for current hacker, going into hoodie desk for waiting current
			float time = e->time;
			int id = e->hackerId;
			hackerList[id-1]->numGifts++;
			turnTime += (time - hackerList[id-1]->entrySticker);
			int dID = e->deskId;
			hoodDesks[dID-1]->availability = true;
			if(!hoodie.empty()){
				Hacker* temp = hoodie.top();
				hoodDesks[dID-1]->availability = false;
				temp->leaveHoodie = time;
				temp->hoodieWait += (temp->leaveHoodie - temp->entryHoodie);
				float next = time + hoodDesks[dID-1]->serviceTime;
				simulator.push(new Event(next ,dID, "leaving hoodie desk", temp->ID));
				hoodie.pop();
			}

			clock = time;

		}
		delete e;
		simulator.pop();
	}

	outfile << maxLenSticker << endl;
	outfile << maxLenHoodie << endl;

	float avgGift = 0;
	float totGift = 0;
	float stickerTime = 0;
	float avgStickerTime = 0;
	float hoodieTime = 0;
	float avgHoodieTime = 0;
	float maxWait = -1; // Should be a small number at first
	int maxID = 0; 
	float leastWait = clock; // Should be a large number at first
	int leastID = -1;

	for (int i = 0; i < hackerList.size(); i++)
	{
		totGift += hackerList[i]->numGifts;
		stickerTime += hackerList[i]->stickerWait;
		hoodieTime += hackerList[i]->hoodieWait;
		float totalWait = hackerList[i]->stickerWait + hackerList[i]->hoodieWait;

		if((totalWait - maxWait) > 0.00001){
			maxWait = totalWait;
			maxID = hackerList[i]->ID;
		}
		if(abs(totalWait - maxWait) < 0.00001){
			if(maxID > hackerList[i]->ID){
				maxID = hackerList[i]->ID;
			}

		}

		if((leastWait - totalWait) > 0.00001){
			if(hackerList[i]->numGifts == 3){
				leastWait = totalWait;
				leastID = hackerList[i]->ID;
			}
		}
		if (abs(totalWait - leastWait) < 0.00001){
			if(hackerList[i]->numGifts == 3){
			if(hackerList[i]->ID < leastID){
				leastWait = totalWait;
				leastID = hackerList[i]->ID;
			}
		}
		}
	
		
	}
	if(leastWait == clock){
		leastWait = -1;
	}

	avgGift = (totGift / numHacker);
	outfile << fixed << setprecision(3) << avgGift << endl;
	avgStickerTime = stickerTime / (numAttempts - invalFree - invalEntry);
	outfile << avgStickerTime << endl;
	avgHoodieTime = hoodieTime / (numAttempts - invalFree - invalEntry);
	outfile << avgHoodieTime << endl;
	float avgCommit = (float) numCommits / numHacker;
	outfile << avgCommit << endl;
	float avgLength = (float) lengthCommit / numCommits;
	outfile << avgLength << endl;
	float avgTurnTime = turnTime / (numAttempts - invalFree - invalEntry);
	outfile << avgTurnTime <<  "\n" << invalEntry << "\n" << invalFree << endl;
	outfile << maxID << " " << maxWait << endl;
	outfile << leastID << " " << leastWait << endl;
	outfile << clock;

	 infile.close();
	 outfile.close();

	for (auto p : stickDesks)
   {
     delete p;
   } 
    for (auto p : hoodDesks)
   {
     delete p;
   } 
    for (auto p : hackerList)
   {
     delete p;
   } 

    return 0;
}