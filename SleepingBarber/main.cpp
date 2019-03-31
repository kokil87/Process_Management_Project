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

semaphore BarberReady(0);
semaphore AccessSeats(1);
semaphore CBReady(0);  
int VacantSeats = 3;    

void *Barber(void *b) {
  while (true) {
    CBReady.Wait();
    AccessSeats.Wait();
    VacantSeats += 1;
    string s = "Barber is cutting hair\n";
    cout << s;
    fflush(stdout);
    usleep(rand() % 1000 * 1000);
    BarberReady.Signal();
    AccessSeats.Signal();
  } 

}
void *Customer(void *b) {
  string s;
  while (true) {
    AccessSeats.Wait();
    s = "New Customer Arrived\n";
    cout << s;
    fflush(stdout);
    if (VacantSeats > 0) {
      VacantSeats--;
      s = "Vacant Seat Available\n";
      cout << s;
      fflush(stdout);
      CBReady.Signal();
      AccessSeats.Signal();
      BarberReady.Wait();
    } else {
      s = "No Seats Available\n";
      usleep(rand() % 1000 * 1000);
      cout << s;
      fflush(stdout);
      AccessSeats.Signal();
    }   
  } 
}

int main()
{
  int i;
  pthread_t customer[4], barber;
  pthread_create(&customer[0], NULL, Customer, NULL);
  pthread_create(&customer[1], NULL, Customer, NULL);
  pthread_create(&customer[2], NULL, Customer, NULL);
  pthread_create(&customer[3], NULL, Customer, NULL);
  pthread_create(&barber, NULL, Barber, NULL);
  pthread_join(customer[0], NULL);
  pthread_join(customer[1], NULL);
  pthread_join(customer[2], NULL);
  pthread_join(customer[3], NULL);
  pthread_join(barber, NULL);
}