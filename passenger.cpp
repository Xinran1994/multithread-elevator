#include <random>
#include"passenger.h"


Passenger::Passenger(generate_time, id, start_floor, end_floor){
	this->start_floor = start_floor;
	this->end_floor = end_floor;
	this->id = id;
	this->generate_time = generate_time;
	if(start_floor < end_floor){
		this->direction = "up";
	}else{
		this->direction = "down";
	}
}
Passenger::~Passenger(){}

Passenger::print_passenger(){
    cout<< "time : "<<this->generate_time <<" passenger "<< this->id << "from "<< this->start_floor << "to "<< this->end_floor<<endl;
}



