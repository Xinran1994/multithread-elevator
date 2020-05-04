class Elevator{
	int id;
	int max_passenger;
	int number_orders;
	list<int> cur_queue; // get off the elevator sorted by floor 
	string status;
	string direction;
	string door;

public:
	Elevator(int id);
	~Elevator();

	int get_id();
	int get_floor();

}