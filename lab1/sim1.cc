#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>

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
	int n = 20;
	float arrivalTime = 20;
	float averagePacketSize = 2000;
	float serviceRate = 1048576;

	vector<Packet> packets;
	vector<float> observers;

	//Generate random packets
	for(int i = 0; i < n; i++) {
		packets.push_back(Packet(expRand(arrivalTime), expRand(1/averagePacketSize)));
	}

	//Generate observers
	for(int i = 0; i < n * 5; i++) {
		observers.push_back(expRand(arrivalTime*5));
	}

	//Sort packets by arrival times
	sort(packets.begin(), packets.end(), compPacket);

	//Sort observers by times
	sort(observers.begin(), observers.end());

	//Calculate arrival and departure time for first packet
	packets[0].departure = packets[0].arrival + packets[0].size/serviceRate;

	//Calculate departure times for all other packets based on previous packet
	for(int i = 1; i < n; i++) {
		if(packets[i-1].departure > packets[i].arrival) {
			packets[i].departure = packets[i-1].departure + packets[i].size/serviceRate;
		} else {
			packets[i].departure = packets[i].arrival + packets[i].size/serviceRate;
		}
	}

	//Print out arrival times
	for(int i = 0; i < n; i++) {
		cout << "Arrival," << packets[i].arrival << endl;
	}

	//Print out departure times
	for(int i = 0; i < n; i++) {
		cout << "Departure," << packets[i].departure << endl;
	}

	//Print out observer times and values
	int na = 0;
	int nd = 0;
	for(int i = 0; i < n * 5; i++) {
		float obsTime = observers[i];
		while(na < n) {
			if(packets[na].arrival > obsTime) break;
			na++;
		}

		while(nd < n) {
			if(packets[nd].departure > obsTime) break;
			nd++;
		}
		cout << "Observer," << obsTime << "," << i << "," << na << "," << nd << endl;
	}	

	return 0;
};