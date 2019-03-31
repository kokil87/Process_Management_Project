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
	semaphore(int init): s(init) {}
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

CustomMux io;
semaphore w(1);
std::queue<int> Buffer;
int i = 0;
atomic<int> rc{0};

void WriteBuffer() {
	int item = i++;
	printf("Write data: %d to Buffer\n", item);
	fflush(stdout);
	usleep(35000);
	Buffer.push(item);
}

int ReadBuffer() {
	int item = Buffer.front();
	printf("Read data: %d from Buffer\n", Buffer.front());
	fflush(stdout);
	usleep(20000);
	Buffer.pop();
	return item;
}

void *Write(void *B)
{
	while (true)
	{
		w.Wait();
		WriteBuffer();
		w.Signal();
	}
}

void *Read(void *B)
{
	while (true)
	{
		io.lock();
		rc++;
		if (rc == 1) w.Wait();
		io.unlock();

		//////////////////Read
		ReadBuffer();

		io.lock();
		rc--;
		if (rc == 0) w.Signal();
		io.unlock();
	}
}

int32_t main()
{
	ios::sync_with_stdio(false); cin.tie(NULL); cout.precision(10);
	srand(time(NULL));
	pthread_t Reader;
	pthread_t Writer;
	pthread_create(&Writer, NULL, Write, NULL);
	pthread_create(&Reader, NULL, Read, NULL);
	pthread_exit(NULL);
}