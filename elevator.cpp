#include"elevator.h"

Elevator::Elevator(int id){
	stopping = 2; // take 2 sec to stop
	stopped = 5; // stopped 5 sec 
	max_passenger = 7;
	status = "idle";
	direction = "up";
	curFloor = 1; // starting at floor 1
	door = "closed"

}

Elevator::~Elevator(){}

Elevator::get_id(){
	return id;
}

Elevator::get_floor(){
	return curFloor;
}

Elevator::up_floor(){
	return curFloor + 1;
}

Elevator::down_floor(){
	return curFloor - 1;
}