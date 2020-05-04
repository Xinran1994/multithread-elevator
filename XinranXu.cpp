//HW3 by XinranXu
// 762193445
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <mutex>
#include <thread> 
#include <deque>
#include <chrono>
#include <condition_variable>
#include <vector>
#include <time.h>
#include <random>
#include <sstream>
using namespace std;

using Us = std::chrono::microseconds;
static int max_buffer[4] = { 6, 5, 4, 3 };
static int buffer[4] = { 0, 0, 0, 0 };
static int make_time[4] = { 50, 70, 90, 110 };
static int move_time[4] = { 20, 30, 40, 50 };
static int assemble_time[4] = { 80, 100, 120, 140 };
static int completedProducts = 0;
ofstream fs;
ostringstream stream;
chrono::system_clock::time_point tbegin;

class Buffer {
private:
    mutex m1;
    condition_variable cv1, cv2;

public:
    /*void push(int k) {
        unique_lock<mutex> ulock(m1);
        while (queue.size() == 10) cv1.wait(ulock);
        queue.push_back(k);
        cv2.notify_one();
    }

    int pop() {
        unique_lock<mutex> ulock(m1);
        while (queue.empty()) cv2.wait(ulock);
        int temp;
        temp = queue.front();
        queue.pop_front();
        cv1.notify_one();
        return temp;
    }*/
	void rand_part(int* a, int len) {
		random_device rd; // Non-determinstic seed source
		default_random_engine e{ rd() };
		int res = 4;
		for (int i = 0; i < 4; i++) {
			uniform_int_distribution<unsigned> u(0, res);
			a[i] = u(e);
			res -= a[i];
		}
		a[3] = res;
		
	}
	void rand_product(int* a, int len) {
		random_device rd; // Non-determinstic seed source
		default_random_engine e{ rd() };
		int res = 5;
		int count = 0, prev = 0;
		uniform_int_distribution<unsigned> w(0, 3);
		int m = w(e); //The index of the only 0
		a[m] = 0;

		for (int i = 0; i < 4; i++) {
			if (i == m)
				continue;
			if (count == 0) {
				uniform_int_distribution<unsigned> u(1, 3);
				a[i] = u(e);
				prev = a[i];
				res -= a[i];
			}
			else if (count == 1) {
				if (prev == 3)
					a[i] = 1;
				else if (prev == 2) {
					uniform_int_distribution<unsigned> u(1, 2);
					a[i] = u(e);
				}
				else {
					uniform_int_distribution<unsigned> u(1, 3);
					a[i] = u(e);
				}
				res -= a[i];
			}
			else {
				a[i] = res;
			}
			count++;
		}
	}

	int loadOrder(int load[]) {
		int moveTime = 0;
		for (int i = 0; i < 4; i++) {
			if (buffer[i] + load[i] > max_buffer[i]) {
				moveTime += (max_buffer[i] - buffer[i]) * move_time[i];
				load[i] = (buffer[i] + load[i]) - max_buffer[i];
				buffer[i] = max_buffer[i];
			}
			else {
				moveTime += load[i] + move_time[i];
				buffer[i] += load[i];
				load[i] = 0;
			}
		}
		return moveTime;
	}

	int pickupOrder(int pickup[]) {
		int moveTime = 0;
		for (int i = 0; i < 4; i++) {
			if (buffer[i] == 0 || pickup[i] == 0)
				continue;
			else if (buffer[i] >= pickup[i]) {
				completedProducts += pickup[i];
				moveTime += pickup[i] * move_time[i];
				buffer[i] -= pickup[i];
				pickup[i] = 0;
			}
			else {
				completedProducts += buffer[i];
				moveTime += buffer[i] + move_time[i];
				pickup[i] -= buffer[i];
				buffer[i] = 0;
			}
		}
		return moveTime;
	}

	void PartWorker(int i) {
		// put part to the buffer 
		int iteration = 1;
		int rpart[4];// load_order
		while (iteration <= 5) {
			// random generate part order
			rand_part(rpart, 4);
			//sleep for makae parts
			this_thread::sleep_for(Us(make_time[0] * rpart[0] + make_time[1] * rpart[1] + make_time[2] * rpart[2] + make_time[3] * rpart[3]));

			int wait_time = 0;
			chrono::system_clock::time_point tend = chrono::system_clock::now();
			chrono::system_clock::duration elapse = tend - tbegin;
			auto until = tend + Us(3000);
			if (m1.try_lock()) {
				stream << "Current Time:" << (elapse).count() << "us" << endl;
				stream << "Product Worker ID:" << i << endl;
				stream << "Iteration:" << iteration << endl;
				stream << "Status: New Load Order" << endl;
				stream << "Accumulate Wait Time: 0us" << endl;
				stream << "Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
				stream << "Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				int loadTime = loadOrder(rpart);
				this_thread::sleep_for(Us(loadTime)); // put patial parts to the buffer
				stream << "Updated Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				stream << "Updated Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
				stream << endl;
				m1.unlock();
				cv1.notify_all();
				//break to next iteration when finished
				if (rpart[0] == 0 && rpart[1] == 0 && rpart[2] == 0 && rpart[3] == 0) {
					continue;
				}
			}
			//unique_lock<mutex> ulock1(m1);
			//cout << "Current Time:" << (elapse).count() << "us" << endl;
			//cout << "Product Worker ID:" << i << endl;
			//cout << "Iteration:" << iteration << endl;
			//cout << "Status: New Load Order" << endl;
			//cout << "Accumulate Wait Time: 0us" << endl;
			//cout << "Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
			//cout << "Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
			//int loadTime = loadOrder(rpart);
			//this_thread::sleep_for(Us(loadTime)); // put patial parts to the buffer
			//cout << "Updated Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
			//cout << "Updated Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
			//cout << endl;
			//m1.unlock();
			//cv1.notify_all();
			////break to next iteration when finished
			//if (rpart[0] == 0 && rpart[1] == 0 && rpart[2] == 0 && rpart[3] == 0) {
			//	continue;
			//}

			while (chrono::system_clock::now() < until) {
				unique_lock<mutex> ulock1(m1); // lock
				chrono::system_clock::time_point startWait = chrono::system_clock::now();
				if (cv1.wait_until(ulock1, until) == cv_status::timeout) {    // timeout
					auto now = chrono::system_clock::now();
					stream << "Current Time:" << (chrono::duration_cast<std::chrono::microseconds>(now - tbegin).count()) << "us" << endl;
					stream << "Part Worker ID:" << i << endl;
					stream << "Iteration:" << iteration << endl;
					stream << "Status: Wakeup-Timeout" << endl;
					stream << "Accumulate Wait Time:" << (chrono::system_clock::now() - startWait).count() << "us" << endl;
					stream << "Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
					stream << "Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
					//try once at last
					int loadTime = loadOrder(rpart);
					stream << "Updated Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
					stream << "Updated Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
					stream << endl;
					this_thread::sleep_for(Us(loadTime)); // put patial parts to the buffer
					cv1.notify_all();
					if (rpart[0] == 0 && rpart[1] == 0 && rpart[2] == 0 && rpart[3] == 0) {
						break;
					}
					else {
						for (int i = 0; i < 4; i++) {
							this_thread::sleep_for(Us(rpart[i]));
							rpart[i] = 0;
						}
						break;
					}
				}
				// try to put when obtained lock
				auto now = chrono::system_clock::now();
				stream << "Current Time:" << (chrono::duration_cast<std::chrono::microseconds>(now - tbegin).count()) << "us" << endl;
				stream << "Part Worker ID:" << i << endl;
				stream << "Iteration:" << iteration << endl;
				stream << "Status: Wakeup-Notified" << endl;
				stream << "Accumulate Wite Time:" << (chrono::system_clock::now() - startWait).count() << "us" << endl;
				stream << "Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
				stream << "Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				int loadTime = loadOrder(rpart);
				this_thread::sleep_for(Us(loadTime)); // put patial parts to the buffer
				stream << "Updated Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				stream << "Updated Load Order: (" << rpart[0] << "," << rpart[1] << "," << rpart[2] << "," << rpart[3] << ")" << endl;
				stream << endl;
				cv2.notify_one();
				if (rpart[0] == 0 && rpart[1] == 0 && rpart[2] == 0 && rpart[3] == 0) {
					break;
				}
			}
			iteration++;
		}
	}

	void ProductWorker(int i) {
		int iteration = 1;
		int rproduct[4]; // pickup_order
		while (iteration <= 5) {
			// random generate pickup order
			rand_product(rproduct, 4);
			chrono::system_clock::time_point tend = chrono::system_clock::now();  //this order's start time
			chrono::system_clock::duration elapse = tend - tbegin;  // current time
			auto until = tend + Us(6000);
			if (m1.try_lock()) {
				stream << "Current Time:" << (elapse).count() << "us" << endl;
				stream << "Product Worker ID:" << i << endl;
				stream << "Iteration:" << iteration << endl;
				stream << "Status: New Pickup Order" << endl;
				stream << "Accumulate Wait Time: 0us" << endl;
				stream << "Pickup Order: (" << rproduct[0] << "," << rproduct[1] << "," << rproduct[2] << "," << rproduct[3] << ")" << endl;
				stream << "Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				int movetime = pickupOrder(rproduct);
				this_thread::sleep_for(Us(movetime)); // put patial parts to the buffer
				stream << "Updated Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				stream << "Updated pickup Order: (" << rproduct[0] << "," << rproduct[1] << "," << rproduct[2] << "," << rproduct[3] << ")" << endl;
				stream << "Total Completed Products: " << completedProducts << endl;
				stream << endl;
				m1.unlock();
				cv1.notify_all();
				//break to next iteration when finished
				if (rproduct[0] == 0 && rproduct[1] == 0 && rproduct[2] == 0 && rproduct[3] == 0) {
					continue;
				}
			}

			while (chrono::system_clock::now() < until) {
				unique_lock<mutex> ulock1(m1); // lock
				chrono::system_clock::time_point startWait = chrono::system_clock::now();
				if (cv2.wait_until(ulock1, until) == cv_status::timeout) {// timeout
					auto now = chrono::system_clock::now();
					stream << "Current Time:" << (chrono::duration_cast<std::chrono::microseconds>(now - tbegin).count()) << "us" << endl;
					stream << "Product Worker ID:" << i << endl;
					stream << "Iteration:" << iteration << endl;
					stream << "Status: Wakeup-Timeout" << endl;
					stream << "Accumulate Wait Time:" << (chrono::system_clock::now() - startWait).count() << "us" << endl;
					stream << "pickup Order: (" << rproduct[0] << "," << rproduct[1] << "," << rproduct[2] << "," << rproduct[3] << ")" << endl;
					stream << "Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
					//try once at last
					int movetime = pickupOrder(rproduct);
					stream << "Updated Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
					stream << "Updated pickup Order: (" << rproduct[0] << "," << rproduct[1] << "," << rproduct[2] << "," << rproduct[3] << ")" << endl;
					stream << "Total Completed Products: " << completedProducts << endl;
					stream << endl;
					this_thread::sleep_for(Us(movetime)); // put patial parts to the buffer
					cv1.notify_all();
					if (rproduct[0] == 0 && rproduct[1] == 0 && rproduct[2] == 0 && rproduct[3] == 0) {
						break;
					}
					else {
						for (int i = 0; i < 4; i++) {
							this_thread::sleep_for(Us(rproduct[i]));
							rproduct[i] = 0;
						}
						break;
					}
				}
				// try to put when obtained lock
				auto now = chrono::system_clock::now();
				stream << "Current Time:" << (chrono::duration_cast<std::chrono::microseconds>(now - tbegin).count()) << "us" << endl;
				stream << "Product Worker ID:" << i << endl;
				stream << "Iteration:" << iteration << endl;
				stream << "Status: Wakeup-Notified" << endl;
				stream << "Accumulate Wait Time:" << (chrono::system_clock::now() - startWait).count() << "us" << endl;
				stream << "pickup Order: (" << rproduct[0] << "," << rproduct[1] << "," << rproduct[2] << "," << rproduct[3] << ")" << endl;
				stream << "Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				int movetime = pickupOrder(rproduct);
				//for (int i = 0; i < 4; i++) {
				//	last_updated_order[i] = updated_pickup_order[i];
				//	updated_pickup_order[i] = buffer[i] - rproduct[i] >= 0 ? 0 : buffer[i] - rproduct[i];  rproduct[i] - buffer[i];// update load order
				//	buffer[i] = buffer[i] - rproduct[i] >= 0 ? buffer[i] - rproduct[i] : 0;// update buffer
				//}
				this_thread::sleep_for(Us(movetime));
				stream << "Updated Buffer State: (" << buffer[0] << "," << buffer[1] << "," << buffer[2] << "," << buffer[3] << ")" << endl;
				stream << "Updated pickup Order: (" << rproduct[0] << "," << rproduct[1] << "," << rproduct[2] << "," << rproduct[3] << ")" << endl;
				stream << "Total Completed Products: " << completedProducts << endl;
				stream << endl;
				cv1.notify_all();
				if (rproduct[0] == 0 && rproduct[1] == 0 && rproduct[2] == 0 && rproduct[3] == 0) {
					break;
				}
			}
			this_thread::sleep_for(Us(make_time[0] * rproduct[0] + make_time[1] * rproduct[1] + make_time[2] * rproduct[2] + make_time[3] * rproduct[3]));
			iteration++;
		}
		cv1.notify_all();
	}
};

void put(Buffer& buffer, int i) {
    buffer.PartWorker(i);
}

void get(Buffer& buffer, int i) {
	buffer.ProductWorker(i);
}

int main() {
	Buffer buffer;
	const int m = 20, n = 16; //m: number of Part Workers
	/*n: number of Product Workers
	m>n*/

	thread partW[m]; 
	thread prodW[n];
	tbegin = chrono::system_clock::now();
	for (int i = 0; i < n; i++) {
		partW[i] = thread(put, ref(buffer), i);
		prodW[i] = thread(get, ref(buffer), i);
	}
	for (int i = n; i < m; i++) {
		partW[i] = thread(put, ref(buffer), i);
	}


	/* Join the threads to the main threads */ for (int i = 0; i < n; i++) {
		partW[i].join();
		prodW[i].join();
	}
	for (int i = n; i < m; i++) {
		partW[i].join();
	}

	stream << "Total Completed Products: " << completedProducts << endl;
	stream << "Finish!" << endl;
	ofstream fs("log.txt");
	cout << stream.str() << endl;
	fs << stream.str() << endl;
	fs.close();
	
	return 0;

}