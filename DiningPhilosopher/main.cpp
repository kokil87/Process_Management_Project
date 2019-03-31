#include <iostream>
#include <queue>
#include <atomic>
#include <unistd.h>
#include <pthread.h>
using namespace std;

class CustomMux
{ 
    std::atomic<int> m; 
public:
    CustomMux(): m(0) {} 
    void lock() 
    { 
      while ((m++) > 1) m--;
    } 
    void unlock() 
    { 
        m--;
    } 
};

class semaphore {
private:
	CustomMux m;
	atomic<int> s;
public:
	semaphore(int init = 1): s(init) {}
	void Wait() {
		while (true) {
			while (s <= 0);
			m.lock();
			if (s <= 0) { m.unlock(); continue; }
			s--;
			m.unlock();
			break;
		}
	}
	void Signal() { s++; }
};

std::vector<semaphore> S(100000);
int n;
CustomMux mu;

std::vector<int> phil;

void* philospher(void* num)
{
	int i = *(int *)num;
	while (true) {
		mu.lock();
		S[i].Wait();
		S[(i + 1) % n].Wait();
		mu.unlock();
		string s = "Philospher #" + to_string(i) + " is eating\n";
		cout << s;
		fflush(stdout);
		S[i].Signal();
		S[(i + 1) % n].Signal();
		usleep((rand()) % 1000 * 1000);
	}
}

int main()
{
	cout << "Give number of philosophers : \t" << endl;
	cin >> n;
	std::vector<pthread_t> thr_id(n);
	phil.resize(n);
	for (int i = 0; i < n; ++i)
		phil[i] = i;
	for (int i = 0; i < n; i++)
		pthread_create(&thr_id[i], NULL, philospher, &phil[i]);
	for (int i = 0; i < n; i++)
		pthread_join(thr_id[i], NULL);
}