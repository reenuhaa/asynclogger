#include<queue>
#include<mutex>
#include<condition_variable>
#include<optional>

template<typename T>
class BoundedQueue{
public:
	BoundedQueue(size_t s):maxCapacity(s){
	}

	template<typename U>
	bool push(U&& item){
		{std::unique_lock<std::mutex> ul{m};
		cv_notFull.wait(ul,[this](){ return stop || q.size() < maxCapacity ;});
		if(stop) return false;

		q.push(std::forward<U>(item));}
		cv_notEmpty.notify_one();
		return true;
	}

	std::optional<T> pop(){

	std::optional<T> res;

	{std::unique_lock<std::mutex> ul{m};
	cv_notEmpty.wait(ul,[this](){return stop || !q.empty();});
	if(stop && q.empty()) return {};
	if(!q.empty()){
	   res = std::move(q.front());
	   q.pop();
	}
	}
	cv_notFull.notify_one();
	return res;
	}

	void shutdown(){
		{
		std::lock_guard<std::mutex> lg{m};
		stop = true;}
		cv_notEmpty.notify_all();
		cv_notFull.notify_all();
	}
private:
 std::queue<T> q;
 std::mutex m;
 std::condition_variable cv_notEmpty, cv_notFull;
 size_t maxCapacity{0};
 bool stop{false};
};
