class passenger
{
	int id;
	int time_ask_elevator;
	int start_floor;
	int end_floor;
	int load_time;
	int unload_time;
	string direction;

public:
	passenger(chrono::system_clock::time_point generate_time, int id, int start_floor,int end_floor);
	~passenger();
	
};