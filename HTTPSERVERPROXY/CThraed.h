#pragma once
#include"pch.h"
class CThread
{
public:
	CThread(size_t numthreads);
	~CThread();

	template<class F, class...Args>
	std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(
		F&& f, Args&& ... args);
private:
	size_t num_threads_;
	// 총 Worker 쓰레드의 개수.
	// Worker 쓰레드를 보관하는 벡터.
	std::vector<std::thread> worker_threads_;
	// 할일들을 보관하는 job 큐.
	std::queue<std::function<void()>> jobs_;
	// 위의 job 큐를 위한 cv 와 m.
	std::condition_variable cv_job_q_;
	std::mutex m_job_q_;
	bool stop_all;
	void WorkerThread();
};

template<class F, class ...Args>
inline std::future<typename std::result_of<F(Args...)>::type> CThread::EnqueueJob(F&& f, Args&& ...args)
{
	if (stop_all) {
		throw std::runtime_error("ThreadPool 사용 중지됨");
	}
	using return_type = typename std::result_of<F(Args...)>::type;
	auto job = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	std::future<return_type> job_result_future = job->get_future();
	{
		std::lock_guard<std::mutex> lock(m_job_q_);
		jobs_.push([job]() { (*job)(); });
	}
	cv_job_q_.notify_one();

	return job_result_future;
}
