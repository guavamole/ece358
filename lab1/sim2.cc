#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <time.h> 

using namespace std;

struct Packet{
	float arrival;
	float departure;
	float size;
	bool dropped;

	Packet(float a, float s) {
		arrival = a;
		size = s;
		dropped = false;
	}
};

bool compPacket (Packet i, Packet j) { 
	return (i.arrival < j.arrival); 
}

float expRand(float lambda) {
	float lambdaInverse = 1.0/lambda;
    float random = (float)rand() / (float)RAND_MAX;
    return -1 * lambdaInverse * log(1 - random);
}

int main() {
	//Start with random seed
	srand(time(0));
	float averagePacketSize = 2000;
	float serviceRate = 1048576;

	float simTime = 1000.0;
	int maxBuffer = 50;
	cout << "p,E[N],Packet Loss" << endl;

	//Generate data for question 3
	for(float p = 0.5; p < 1.5; p += 0.1) {
		//float p = 0.9;
		//Uncomment for question 4 data
		
		//Calculate arrival rate based on p
		float arrivalRate = p * serviceRate / averagePacketSize;

		//Generate random packets
		vector<Packet> packets;
		int numPackets = 0;
		float curTime = 0;
		do {
			packets.push_back(Packet(curTime, expRand(1/averagePacketSize)));
			curTime += expRand(arrivalRate);
			numPackets++;
		} while(curTime < simTime);

		int numObservers = 0;
		//Generate random observers
		vector<float> observers;
		curTime = 0;
		do {
			observers.push_back(curTime);
			curTime += expRand(arrivalRate * 5);
			numObservers++;
		} while(curTime < simTime);

		//Holds number of dropped packets
		int numDropped = 0;
		//Calculate arrival and departure time for first packet
		packets[0].departure = packets[0].arrival + packets[0].size/serviceRate;

		//Calculate departure times for all other packets based on previous packet
		//j points to the next packet that will be moved out of the buffer
		int buffer = 1;
		int j = 0;
		for(int i = 1; i < numPackets; i++) {
			if(packets[i-1].departure > packets[i].arrival) {
				while(packets[j].departure < packets[i].arrival && j < i) {
					buffer--;
					while(packets[j].dropped && j < i) j++;
					j++;
				}

				if(buffer < maxBuffer) {
					packets[i].departure = packets[i-1].departure + packets[i].size/serviceRate;
					buffer++;
				} else {
					//We store departure time of previous packet (so next non-dropped packet can use it)
					packets[i].departure = packets[i-1].departure;
					//Packet is dropped. 
					packets[i].dropped = true;
					numDropped++;
				}
			} else {
				packets[i].departure = packets[i].arrival + packets[i].size/serviceRate;
				buffer = 1;
				j = i;
			}
		}

		////Print out arrival times
		//for(int i = 0; i < numPackets; i++) {
		//	cout << "Arrival," << packets[i].arrival;
		//	if (packets[i].dropped) {
		//		cout << ", dropped" << endl;
		//	} else {
		//		cout << ", " << packets[i].departure << endl;
		//	}
		//}

		//Calculate observer values
		int na = 0;
		int nd = 0;

		//Holds number of dropped packets inbetween na and nd
		int dropped = 0;
		double totalUsage = 0;
		double totalIdle = 0;
		for(int i = 0; i < numObservers; i++) {
			float obsTime = observers[i];

			while(na < numPackets) {
				if(packets[na].arrival > obsTime) break;
				if(packets[na].dropped) dropped++;
				na++;
			}

			while(nd < numPackets) {
				
				if(packets[nd].departure > obsTime) break;
				if(packets[nd].dropped) dropped--;
				nd++;
			}

			//cout << "na: " << na << " nd: " << nd << " dropped " << dropped << endl;

			// while(nd + dropped < numPackets) {
			// 	if(packets[nd].departure > obsTime) break;
			// 	if(packets[nd].dropped) {
			// 		dropped++;
			// 	} else {
			// 		nd++;
			// 	}
			// }

			if(na == (nd + dropped)) {
				totalIdle++;
			} else if(na > (nd + dropped)) {
				totalUsage += na - (nd + dropped);
			}

			//Prints out observer values
			//cout << "Observer," << obsTime << "," << i << "," << na << "," << nd << endl;
		}
		double usageRate = totalUsage / (numObservers);
		double idleRate = totalIdle / (numObservers);
		double packetLoss = (1.0 * numDropped) / numPackets;
		cout << p << "," << usageRate << "," << packetLoss << endl;
	}
	return 0;
};