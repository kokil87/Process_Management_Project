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

semaphore PaperAndTobacco(0);
semaphore MatchesAndPaper(0);
semaphore TobaccoAndMatches(0);
semaphore SmokingCompleted(1);

void Smoke()
{
  usleep(rand() % 1000 * 1000);
}

void *Agent(void *B)
{
  while (true) {
    SmokingCompleted.Wait();
    int r = rand() % 3;
    switch (r) {
    case 0: PaperAndTobacco.Signal();
      break;
    case 1: MatchesAndPaper.Signal();
      break;
    case 2: TobaccoAndMatches.Signal();
      break;
    }
  }
}

void *S1(void *B)
{
  while (true) {
    PaperAndTobacco.Wait();
    Smoke();
    string s = "S1 is smoking\n";
    cout << s;
    fflush(stdout);
    SmokingCompleted.Signal();
  }
}

void *S2(void *B)
{
  while (true) {
    MatchesAndPaper.Wait();
    Smoke();
    string s = "S2 is smoking\n";
    cout << s;
    fflush(stdout);
    SmokingCompleted.Signal();
  }
}

void *S3(void *B)
{
  while (true) {
    TobaccoAndMatches.Wait();
    Smoke();
    string s = "S3 is smoking\n";
    cout << s;
    fflush(stdout);
    SmokingCompleted.Signal();
  }
}


int main()
{
  int i;
  pthread_t s1, s2, s3, agent;
  pthread_create(&s1, NULL, S3, NULL);
  pthread_create(&s2, NULL, S2, NULL);
  pthread_create(&s3, NULL, S1, NULL);
  pthread_create(&agent, NULL, Agent, NULL);
  pthread_join(s1, NULL);
  pthread_join(s2, NULL);
  pthread_join(s3, NULL);
  pthread_join(agent, NULL);
}