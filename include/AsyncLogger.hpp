#include <BoundedQueue.hpp>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream> 

using namespace std;

struct TelemetryMessage{
	TelemetryMessage(std::chrono::time_point<std::chrono::steady_clock> p, std::string s, std::string m):
		tp(p),
		sender(std::move(s)),
		message(std::move(m)){
	}
	std::chrono::time_point<std::chrono::steady_clock> tp;
	std::string sender;
	std::string message;
};

class AsyncLogger
{
  public:
	  AsyncLogger(std::string filename):fs(filename,std::ios::app){
	  	t = std::move(std::thread{[this](){write();}});
	  }

	  void log(TelemetryMessage tmsg)
	  {		
		bq.push(std::move(tmsg));
	  }

	  void shutdown(){
		  bq.shutdown();

		  stop.store(true);
		  t.join();
	  }
	
protected:
	  void write(){
		  std::optional<TelemetryMessage> msg;
		  while(!stop.load())
		  {			
		 	msg = std::move(bq.pop());
			if(!msg.has_value()) continue;

			fs << std::chrono::duration_cast<std::chrono::milliseconds>(msg.value().tp.time_since_epoch())<<" ms"
				<<" sender:"<<msg.value().sender
				<<" message:"<<msg.value().message<<"\n";
			
		  };
		  cout<<"Logger Exiting..\n";
	  }

  private:
    BoundedQueue<TelemetryMessage> bq{4};
    fstream fs;
    std::thread t;
    std::atomic<bool> stop{false};
};
