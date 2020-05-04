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
const moving_bwflr = 10; // take 10 sec between each floor 
const vector<elevator> E;

list<passenger> buffer; 
list<floor> building; 
list<passenger> up_request; // a list of passenger go up, sort by start floor
list<passenger> down_request; // a list of passenger go down 
map<int, mutex*> building_waitinglist;

enum status{"stopped", "moving", "idle"};
enum direction{"up", "down"};
int cur_time;
int total_completed;
ofstream fs;
ostringstream stream;


void generate(int i ){ // thread generating 
    chrono::system_clock::time_point tend = chrono::system_clock::now(); // now
    auto end_time = tend + Us(1000000);// 1000s

    while(chrono::system_clock::now() < end_time){
        start_floor = 1 + rand()%30;
        end_floor = 1 + rand()%30;
        auto lock_floor = building_waitinglist[]
        passenger p = new passenger(tend, i, start_floor, end_floor); // generate a random passenger
      
        cout<< "new request id: "<< p.id << ": start floor"<< start_floor<< " end floor "<< end_floor<<endl;
        if(start_floor > end_floor){
            p.direction = "down";
            building[passenger_floor].down_queue.push_back(p);
            addRequestToDownQueue(p);

        }else{
            p.direction = "up";
            building[passenger_floor].up_queue.push_back(p);
            addRequestToUpQueue(p);
        }
        buffer.push_back(p);
        lock1.unlock();

        this_thread::sleep_for(Us(5000));
    }
}

void addRequestToDownQueue(passenger request){
        down_request.add(request);
        int n = down_request.size();
        for(int i=0; i<n; i++){
            Request task1 = down_request.get(i);
            Request task2 = down_request.get(n-1);
            if(task2.floor > task1.floor){
                down_request.set(i, task2);
                down_request.set(n-1, task1);
            }
        }
        auto first = down_queue[0];
        first->print_passenger();
    }

void addRequestToUpQueue(passenger request){
        up_request.add(request);
        int n = up_request.size();
        for(int i=0; i<n; i++){
            Request task1 = up_request.get(i);
            Request task2 = up_request.get(n-1);
            if(task2.floor < task1.floor){
                up_request.set(i, task2);
                up_request.set(n-1, task1);
            }
        }
    }

void sort_CurQueue(Passenger p, string direc, list<passenger> cur_queue){ // passengers in elevator sorting by destination floor
    int n = cur_queue.size();
    if(direc == "down"){ // from largest to smallest
        for(int i=0; i<n; i++){
            passenger task1 = cur_queue.get(i);
            passenger task2 = cur_queue.get(n-1);
            if(task2.floor < task1.floor){
                cur_queue.set(i, task2);
                cur_queue.set(n-1, task1);
            }
        }
    }else{
        for(int i=0; i<n; i++){
            passenger task1 = cur_queue[i];
            passenger task2 = cur_queue[i];
            if(task2.floor < task1.floor){
                cur_queue.set(i, task2);
                cur_queue.set(n-1, task1);
            }
        }
    }
}

void elevator_process(Elevator el, int id){
    list<passenger> cur_queue; //elevator queue, passengers in elevator
    chrono::system_clock::time_point tend = chrono::system_clock::now(); // now
    auto end_time = tend + Us(1000000);// 1000s
    while(chrono::system_clock::now() < end_time){
        unique_lock<std::mutex> lock2(m1, defer_lock);
        if(el.status == "idle"){
            if(lock2.try_lock()){
                if(buffer.empty()){
                    continue;
                }
                Passenger p = buffer[0];
                int passenger_floor = p.get_startfloor(); // passenger's floor
                int cur_floor = el.get_floor();// elevator's floor
                if(passenger_floor > cur_floor){
                    el.direction = "up";  
                } else{
                    el.direction = "down";
                }     
                el.status = "moving";
            } // out of scope, unlock
            //find the first unload
            cur_queue.push_back(p);
            //sort the cur_queue
            sort_CurQueue(p, direc, cur_queue);
            passenger cur = cur_queue[0];
            int dist = cur.get_endfloor()- cur_floor;
            this_thread::sleep_for(Us(distance * 5 * 1000));
            el.status = "stopped";
            //el arrived the destination floor
        }else if(el.status == "moving"){ // el is moving
            Passenger p = buffer[0];
            int passenger_floor = p.get_startfloor(); // passenger's floor
            int cur_floor = el.get_floor();// elevator's floor
            if()
        }else{ //el is stopped

        }
        
        //int distance = abs(passenger_floor - cur_floor); 
        string direc = p.get_direction(); // passenger's direction
        int max_distance = INT_MAX;
        
    }

}
boolean if_schedule(){ // if the elevator be schedule for a certain passenger
    for(int i = 0; i < elevator.size(); i++){
            if(elevator[i].direction == direc || elevator[i].status == "idle" ){ // same direction or elevator is idle
                if(abs(elevator[i].get_floor() - passenger_floor) < max_distance){
                    elevator[i]
                }
            }
        }
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
void schedule(){

}

void print_passenger(){
    cout<< "time : "<<this->generate_time <<" passenger "<< this->id << "from "<< this->start_floor << "to "<< this->end_floor<<endl;
}

void print_elevator(){
    cout<< "time : "<< chrono::system_clock::now() << "elevator "<< this->id << "at floor "<<this->cur_floor << " direction " << this->direction;
    cout<< "completed "<< this->number_orders <<endl; 
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
    for(int i = 1; i <= elevator_number; i++){
        ele[i] = thread(elevator_process, ref(E), i);
    }
    scheduler = thread(schedule);

    for(int i = 0; i < 2; i++){
        passenger_generate[i].join();
    }
    for(int i = 1; i <= elevator_number; i++){
        ele[i].join();
    }
    scheduler.join();

    while(!finish){
        for(auto el: E){
            this->print_elevator();
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    cout<< "Total completed request: "<< total_completed;
    cout<< ""--------------------Finish!!!--------------------""
    return 0;   
}