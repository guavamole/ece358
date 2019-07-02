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

	Packet(float a, float s) {
		arrival = a;
		size = s;
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
	cout << "p,usage rate, idle rate" << endl;

	float simTime = 1000.0;

	//Generate data for question 3
	for(float p = 0.25; p < 1; p+=0.1) {
		//Uncomment for question 4 data
		//float p = 1.2;

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

		//Calculate arrival and departure time for first packet
		packets[0].departure = packets[0].arrival + packets[0].size/serviceRate;

		//Calculate departure times for all other packets based on previous packet
		for(int i = 1; i < numPackets; i++) {
			if(packets[i-1].departure > packets[i].arrival) {
				packets[i].departure = packets[i-1].departure + packets[i].size/serviceRate;
			} else {
				packets[i].departure = packets[i].arrival + packets[i].size/serviceRate;
			}
		}

		// //Print out arrival times
		// for(int i = 0; i < arrivalRate; i++) {
		// 	cout << "Arrival," << packets[i].arrival << endl;
		// }

		// //Print out departure times
		// for(int i = 0; i < arrivalRate; i++) {
		// 	cout << "Departure," << packets[i].departure << endl;
		// }

		//Calculate observer values
		int na = 0;
		int nd = 0;
		double totalUsage = 0;
		double totalIdle = 0;
		for(int i = 0; i < numObservers; i++) {
			float obsTime = observers[i];
			while(na < numPackets) {
				if(packets[na].arrival > obsTime) break;
				na++;
			}

			while(nd < numPackets) {
				if(packets[nd].departure > obsTime) break;
				nd++;
			}

			if(na == nd) {
				totalIdle++;
			} else if(na > nd) {
				totalUsage += na - nd;
			}

			//Prints out observer values
			//cout << "Observer," << obsTime << "," << i << "," << na << "," << nd << endl;
		}
		double usageRate = totalUsage / numObservers;
		double idleRate = totalIdle / numObservers;

		cout << p << "," << usageRate << "," << idleRate << endl;
	}
	return 0;
};