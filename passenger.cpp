#include <random>
#include"passenger.h"


Passenger::Passenger(generate_time, id, start_floor, end_floor){
	this->start_floor = start_floor;
	this->end_floor = end_floor;
	this->id = id;
	this->generate_time = generate_time;
}
Passenger::~Passenger(){}


