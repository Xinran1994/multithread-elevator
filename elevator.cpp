#include"elevator.h"

Elevator::Elevator(int id){
	max_passenger = 7;
	status = "idle";
	cur_floor = 1; // starting at floor 1
	finished_requests = 0;
}

Elevator::~Elevator(){}


Elevator::idle(){
	if(target_up_floor != 0){
		this->status = "up";
		this->load();

	}else if(target_down_floor != 31){
		this->status = "down";
		this->load();
	}
}

Elevator::load(){
	auto floor = this->cur_floor;
	auto lock = building_waitinglist[floor];
	list<passenger*> new_passengers_list; // update passengers in the waitinglist
	lock->lock();
	for(auto p: passengers_list[floor]){
		if(this->cur_queue.size() >= max_passenger){
			new_passengers_list.push_back(p);
		}
		if(p->direction == this->status){
			this->cur_queue.push_back(p);
			sort_queue();
			p->load_time = chrono::system_clock::now();
			cur_passengers++;
		}else{
			new_passengers_list.push_back(p);
		}
		this_thread::sleep_for(Us(load_time));
	}
	passengers_list[floor] = new_passengers_list;
	lock->unlock();
}

Elevator::unload(){
	auto floor = this->cur_floor;
	auto lock = building_waitinglist[floor];
	list<passenger*> new_cur_queue; //update passengers in the elevator
	lock->lock();
	for(auto p: cur_queue){
		if(p->end_floor = floor){
			finished_request.push_back(p);
			p->unload_time = chrono::system_clock::now();
			cur_passengers--;
		}else{
			new_cur_queue.push_back(p);
		}
		this_thread::sleep_for(Us(unload_time));
	}
	this->cur_queue = new_cur_queue;
	lock->unllock();
	if(cur_passengers == 0){

	}
}

Elevator::going_up(){
	if(this->cur_floor != cur_floor){
		this->cur_floor = cur_floor;
		this->load();
		this->unload();
	}
	if(this->cur_floor == target_up_floor || this->cur_floor == max_floor){
		this->target_up_floor == 0;
		if(this->target_down_floor == 31 && this->cur_queue.size() == 0){
			this->status = "idle";
		}else{
			this->status = "down";
			this->load();
		}
	}

}

Elevator::going_down(){
	if(this->cur_floor != cur_floor){
		this->cur_floor = cur_floor;
		load();
		unload();
	}
	if(this->cur_floor == target_down_floor || this->cur_floor == min_floor){
		this->target_up_floor == 0;
		if(this->target_up_floor == 31 && this->cur_queue.size() == 0){
			this->status = "idle";
		}else{
			this->status = "up";
			this->load();
		}
	}

}
Elevator::print_elevator(){
    cout<< "time : "<< chrono::system_clock::now() << "elevator "<< this->id << "at floor "<<this->cur_floor << " direction " << this->direction;
    cout<< "completed "<< this->number_orders <<endl; 
}