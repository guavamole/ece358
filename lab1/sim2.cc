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
	int maxBuffer = 10;
	cout << "p,usage rate, idle rate, packet loss" << endl;
	//Generate data for question 3
	for(float p = 0.5; p < 1.5; p+=0.1) {
		//Uncomment for question 4 data
		

		//Calculate arrival rate based on p
		float arrivalRate = p * serviceRate / averagePacketSize;

		//Generate random packets
		vector<Packet> packets;
		for(int i = 0; i < arrivalRate; i++) {
			packets.push_back(Packet(expRand(arrivalRate), expRand(1/averagePacketSize)));
		}

		//Generate observers
		vector<float> observers;
		for(int i = 0; i < arrivalRate * 5; i++) {
			observers.push_back(expRand(arrivalRate*5));
		}

		//Sort packets by arrival times
		sort(packets.begin(), packets.end(), compPacket);

		//Sort observers by times
		sort(observers.begin(), observers.end());

		//Calculate arrival and departure time for first packet
		packets[0].departure = packets[0].arrival + packets[0].size/serviceRate;

		//Calculate departure times for all other packets based on previous packet

		//j points to the next packet that will be moved out of the buffer
		int buffer = 1;
		int j = 0;
		for(int i = 1; i < arrivalRate; i++) {

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
					packets[i].departure = packets[i-1].departure;
					packets[i].dropped = true;
				}
			} else {
				packets[i].departure = packets[i].arrival + packets[i].size/serviceRate;
				buffer = 1;
				j = i;
			}
		}

		////Print out arrival times
		//for(int i = 0; i < arrivalRate; i++) {
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
		int dropped = 0;
		double totalUsage = 0;
		double totalIdle = 0;
		for(int i = 0; i < arrivalRate * 5; i++) {
			float obsTime = observers[i];
			while(na < arrivalRate) {
				if(packets[na].arrival > obsTime) break;
				na++;
			}

			while(nd + dropped < arrivalRate) {
				if(packets[nd].departure > obsTime) break;
				if(packets[nd].dropped) {
					dropped++;
				} else {
					nd++;
				}
			}

			if(na == (nd + dropped)) {
				totalIdle++;
			} else if(na > (nd + dropped)) {
				totalUsage += na - (nd + dropped);
			}

			//Prints out observer values
			//cout << "Observer," << obsTime << "," << i << "," << na << "," << nd << endl;
		}
		double usageRate = totalUsage / (arrivalRate * 5);
		double idleRate = totalIdle / (arrivalRate * 5);
		cout << dropped << endl;
		double packetLoss = (1.0 * dropped) / arrivalRate;
		cout << p << "," << usageRate << "," << idleRate << "," << packetLoss << endl;
	}
	return 0;
};