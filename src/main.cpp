#include<iostream>
#include<thread>
#include<chrono>
#include<string>
#include "AsyncLogger.hpp"
#include <array>
#include <utility>

using namespace std;

int main(){
	cout<<"My Claude Project\n";
	
	AsyncLogger logger{"Traces.log"};

	std::array<std::pair<std::string,std::string>,3> arr{
		std::make_pair("sender1","OverHeating"),std::make_pair("sender2","OverCooling"),std::make_pair("sender3","powerloss")};
	std::jthread jt1{[&arr, &logger](){ for(int i=0 ; i< 3 ;++i){
		cout<<"pushing msg num:"<<i<<"\n";
		logger.log(TelemetryMessage{std::chrono::steady_clock::now(),arr[i].first,arr[i].second});
	}}};

	using namespace std::chrono_literals;
	std::this_thread::sleep_until(std::chrono::steady_clock::now() + 500ms);

	logger.shutdown();
}
