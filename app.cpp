#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <time.h>
#include <random>
#include <sstream>
#include <mutex>

using namespace std;

using Us = std::chrono::microseconds;
chrono::system_clock::time_point tbegin;
mutex m1;

const int end_time = 1000;
const int max_floor = 30;
const int min_floor = 1;
const int elevator_number = 4;
const int moving_bwflr = 10; // take 10 sec between each floor 
const int passenger_interval = 5000;
const int elevator_interval = 7000;

vector<elevator*> E;
list<floor> building; 
// list<passenger*> up_request; // a list of passengers going up, each floor have one up request_list,one down request_list
// list<passenger*> down_request; // a list of passengers going down 
map<int, mutex*> building_waitinglist;
map<int, vector<passenger*>> passengers_list;



enum status{"up", "down", "idle"};

int cur_time;
int total_generated;
int total_completed;
ofstream fs;
ostringstream stream;


void generate(int i ){ // generating passenger
    chrono::system_clock::time_point tend = chrono::system_clock::now(); // now
    auto end_time = tend + Us(1000000);// 1000s

    while(chrono::system_clock::now() < end_time){
        start_floor = 1 + rand()%30;
        end_floor = 1 + rand()%30;
        auto lock_floor = building_waitinglist[start_floor];
        passenger p = new passenger(tend, i, start_floor, end_floor); // generate a random passenger
        p->print_passenger();
        bool direc = start_floor < end_floor;// true is up, false is down 
        lock_floor->lock();
        passengers_going_down[start_floor].push_back(p); 
        lock_floor->unlock();

        //find a closest elevator
        int distance = calculate_distance(E[0], p, direc);
        auto picked_elevator = E[0];
        for(int i = 1; i < E.size(); i++){
            if(calculate_distance(E[1], p, direc) < distance){
                picked_elevator = E[i];
            }
        }
        schedule(picked_elevator, p); 

        total_generated++;
        i++;
        this_thread::sleep_for(Us(passenger_interval));
    }
}

int calculate_distance(elevator *el, passenger *p){
    int distance = INT_MAX

    if(el->status == "idle"){
        distance = min(distance, el->cur_floor - p->start_floor);
    }else if(el->status == p->direction){ //same direction
        distance = min(distance, abs(el->cur_floor - p->start_floor));
    }else{
        if(p->direction == "up"){
            distance = min(distance, el->cur_floor - min_floor + p->start_floor - min_floor);
        }else{
            distance = min(distance, max_floor - el->cur_floor + max_floor - p->start_floor);
        }
    }
    return distance;
}

void schedule(elevator *el, passenger *p){
    if(el->cur_floor > p->start_floor){ //going down target
        el->target_down-floor = min(el->target_down-floor, p->start_floor);
    }else{
        el->target_down-floor = max(el->target_down-floor, p->start_floor);
    }
}

void scheduler(){
    if(!finish){
        for(int i = 1; i <= 30; i++){
            for(auto p: passengers_list[i]){
                int distance = calculate_distance(E[0], p, direc);
                auto picked_elevator = E[0];
                for(int i = 1; i < E.size(); i++){
                    if(calculate_distance(E[1], p, direc) < distance){
                        picked_elevator = E[i];
                    }
                }
                schedule(picked_elevator, p);
            }
        }
        this_thread::sleep_for(Us(1000));
    }
}

void elevator_process(Elevator *el, int id){
    cout<< "elevator "<< el->id << "start serving !!!";
    while(!finish){ //haven't finish all request
        unique_lock<std::mutex> lock2(m1, defer_lock);
        if(el->status == "up"){ //el is up
            el->going_up();
        }else if(el.status == "down"){ // el is down
            el->going_down();
        }else{ //el is idle
            el->idle();
        }
        this_thread::sleep_for(Us(elevator_interval));
    }
    cout<< "elevator "<< el->id << "exit !!!";
}

void process_request(Elevator elevator, Passenger passenger, int time){
    int passenger_floor = p.get_startfloor();
    int cur_floor = elevator.get_floor();
    int distance  = abs(passenger_floor - cur_floor);
    if(passenger_floor > cur_floor){
        elevator.direction = "up";        
    }
    else{
        elevator.direction = "down"
        }
    door = "closed";

    while(distance > 0){
            // 5s / floor
        cur_time += 5;
        if(elevator.direction == "up"){
            elevator.cur_floor += 1;
        }else{
            elevator.cur_floor -= 1;
        }
        distance--;
    }
    elevator.door = "opened";
    printf("Elevator %d Arrived at floor %d\n", elevator.get_id(), elevator.get_floor());        cur_time += 5; // stopped 5s at each floor to load or unload
    elevator.door = "closed";    

}

int main()
{
    tbegin = chrono::system_clock::now();
    for(int i = 0; i < 4; i++){ // push all elevator carts into the vector
        elevator e = new elevator(i);
        E.push_back(e);
    }
    thread ele[elevator_number];
    thread passenger_generate[2];
    thread scheduler;

    printf("-----------running the simulation----------\n");
    printf("---------------start serving---------------\n");

    for(int i = 0; i < 2; i++){
        passenger_generate[i] = thread(generate, i);
    }
    for(int i = 0; i < elevator_number; i++){
        ele[i] = thread(elevator_process, E[i], i);
    }
    scheduler = thread(schedule);

    for(int i = 0; i < 2; i++){
        passenger_generate[i].join();
    }
    for(int i = 0; i < elevator_number; i++){
        ele[i].join();
    }
    scheduler.join();

    while(!finish){
        for(auto el: E){
            this->print_elevator();
        }
        this_thread::sleep_for(Us(1000));
    }
    cout<< "Total completed request: "<< total_completed;
    cout<< ""--------------------Finish!!!--------------------""
    return 0;   
}