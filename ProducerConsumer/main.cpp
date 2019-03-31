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

CustomMux Buff;
semaphore ProductsCount(0);
semaphore EmptyCount(10);
queue<int> Buffer;
atomic<int> i(0);

void ItemProduction() {
}

void ItemConsumption(int j) {
	///////////////Consumption
}

void BufferInsert() {
	int item = i++;
	printf("Item #%d pushed in the buffer\n", item);
	fflush(stdout);
	usleep(35000);
	Buffer.push(item);
}

int BufferRemove() {
	int item = Buffer.front();
	printf("Item #%d pulled for consumption from the buffer\n", Buffer.front());
	fflush(stdout);
	usleep(20000);
	Buffer.pop();
	return item;
}

void *Produce(void *id)
{
	while (true)
	{
		////////////////////////Produce Item
		ItemProduction();

		EmptyCount.Wait();
		Buff.lock();

		///////////////////////Push into Buffer
		BufferInsert();

		Buff.unlock();
		ProductsCount.Signal();
	}
}

void *Consume(void *id)
{
	while (true)
	{
		ProductsCount.Wait();
		Buff.lock();

		////////////////////////Remove Item
		int item = BufferRemove();

		Buff.unlock();
		EmptyCount.Signal();
		////////////////////////Consume Item
		ItemConsumption(item);
	}
}

int32_t main()
{
	ios::sync_with_stdio(false); cin.tie(NULL); cout.precision(10);
	pthread_t Prod[7];
	pthread_t Cons[7];
	pthread_create(&Prod[0], NULL, Produce, NULL);
	pthread_create(&Cons[0], NULL, Consume, NULL);
	pthread_create(&Prod[1], NULL, Produce, NULL);
	pthread_create(&Cons[1], NULL, Consume, NULL);
	pthread_create(&Prod[2], NULL, Produce, NULL);
	pthread_create(&Cons[2], NULL, Consume, NULL);
	pthread_create(&Prod[3], NULL, Produce, NULL);
	pthread_create(&Cons[3], NULL, Consume, NULL);
	pthread_create(&Prod[4], NULL, Produce, NULL);
	pthread_create(&Cons[4], NULL, Consume, NULL);
	pthread_create(&Prod[5], NULL, Produce, NULL);
	pthread_create(&Cons[5], NULL, Consume, NULL);
	pthread_create(&Prod[6], NULL, Produce, NULL);
	pthread_create(&Cons[6], NULL, Consume, NULL);
	pthread_exit(NULL);
}