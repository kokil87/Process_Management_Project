CC := g++
pthread = -pthread
cs = ./CigaretteSmoker/main.cpp
dp = ./DiningPhilosopher/main.cpp
pc = ./ProducerConsumer/main.cpp
rw = ./ReadersWriters/main.cpp
sb = ./SleepingBarber/main.cpp

# Build beast
build: 
	$(CC) -o ./executables/cigarette_smoker $(pthread) $(cs)
	$(CC) -o ./executables/dining_philosopher $(pthread) $(dp)
	$(CC) -o ./executables/producer_consumer $(pthread) $(pc)
	$(CC) -o ./executables/readers_writers $(pthread) $(rw)
	$(CC) -o ./executables/sleeping_barber $(pthread) $(sb)

clean:
	rm ./executables/*