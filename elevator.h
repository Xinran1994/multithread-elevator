class Elevator{
	int id;
	int max_passenger;
	int finished_requests; 
	int cur_floor;
	int cur_passengers;
	int load_time = 5000;
	
	target_up_floor = 0;
	target_down_floor = 31;
	list<passenger*> cur_queue; //elevator queue, passengers in elevator
	list<passenger*> finished_request; 

public:
	Elevator(int id);
	~Elevator();

	int get_id();
	int get_floor();

	void load();
	void unload();

	void going_up();
	void going_down();
	void idle();
}