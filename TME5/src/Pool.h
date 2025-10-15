#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	// create a pool with a queue of given size
	Pool(int qsize) {/*TODO*/
	}
	// start the pool with nbthread workers
	void start (int nbthread) {
		/*TODO*/
		for (int i = 0; i < nbthread; i++) {
			// syntaxe pour passer une methode membre au thread
			threads.emplace_back(&Pool::worker, this);
		}
	}

	// submit a job to be executed by the pool
	void submit (Job * job) { /*TODO*/}
	
	// initiate shutdown, wait for threads to finish
	void stop() { }
	

private:
	// worker thread function
	void worker() { //todo
	}
};

}
