//Question 2 - threads & synchronization

//Task: Improve execution time by using multi-threading instead of calling operation1 and operation2 serially, make sure that sum=EXPECTED_SUM after using threads
// please explain the changes you made and what new aspects you had to deal with when shifting from serial execution to parallel execution 

// Make sure you compile the code in "Release" configuration (e.g O2 optimization level).
// Do not modify the constexpr variables

#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

constexpr size_t ITERATIONS = 1000 * 1000ULL;
constexpr size_t OP1_PARAM = 2ULL;
constexpr size_t OP2_PARAM = 1ULL;
constexpr size_t EXPECTED_SUM = 1000001000000ULL;

size_t gSum = 0;
std::mutex mtx;

void operation1(size_t arg) {
	std::cout << "Performing operation1" << std::endl;
	mtx.lock();
	for (size_t i = 0; i < ITERATIONS; i++) {
		gSum += (arg + i);
	}
	mtx.unlock();
	std::this_thread::sleep_for(std::chrono::seconds(5)); //Simulate some heavy work
	std::cout << "Operation1 Performed" << std::endl;
}


void operation2(size_t arg) {
	std::cout << "Performing operation2" << std::endl;
	mtx.lock();
	for (size_t i = 0; i < ITERATIONS; i++) {
		gSum += (arg * i);
	}
	mtx.unlock();
	std::this_thread::sleep_for(std::chrono::seconds(10));  //Simulate some heavy work
	std::cout << "Operation2 Performed" << std::endl;
}


int main(int argc, char** argv)
{

	auto start = std::chrono::steady_clock::now();
	//operation1(OP1_PARAM);
	//operation2(OP2_PARAM);
	std::thread th1(operation1, OP1_PARAM);
	std::thread th2(operation2, OP2_PARAM);
	th1.join();
	th2.join();
	auto end = std::chrono::steady_clock::now();
	size_t sum = gSum;
	std::cout << "Total execution duration in milliseconds: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	std::cout << "Result:  " << sum << ", " << (sum == EXPECTED_SUM ? "success" : "failure!") << std::endl;
	std::cout << "Press enter to exit" << std::endl;
	getchar();
	return 0;
}

/*
* Using threads results:
* Total execution duration in milliseconds: 10018
Result:  3568587328, success

Not using threads results:
Total execution duration in milliseconds: 15034
Result:  3568587328, success

My idea was to improve execution by using threads for the "sleeping" part of the operation, because gSum is a common resource i used mutex to lock
it for only one thread at the time. and i used two thread to run operation1 and operation2.  

*/