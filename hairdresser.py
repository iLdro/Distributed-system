from concurrent.futures import thread
import threading
import time
import random
import queue


NUM_HAIRDRESSERS = 4
NUM_WAITING_SEATS = 15
AIRLOCK_CAPACITY = 2

waiting_seats_semaphore = threading.Semaphore(NUM_WAITING_SEATS)
airlock_semaphore = threading.Semaphore(AIRLOCK_CAPACITY)
waiting_queue_lock = threading.Lock()
waiting_queue = []


class Hairdresser(threading.Thread):
    def __init__(self, hairdresser_id):
        threading.Thread.__init__(self)
        self.hairdresser_id = hairdresser_id

    def run(self):
        while True:
            with waiting_queue_lock:
                if waiting_queue:
                    client_id = waiting_queue.pop(0)
                else:
                    client_id = None
            if client_id is None:
                time.sleep(1)
            else:
                self.style_hair(client_id)

    def style_hair(self, client_id):
        print(f"Hairdresser {self.hairdresser_id} is styling Client {client_id}")
        time.sleep(random.uniform(2, 3.5))
        print(f"Hairdresser {self.hairdresser_id} has finished styling Client {client_id}")
        waiting_seats_semaphore.release()

class Client(threading.Thread):
    def __init__(self, client_id):
        threading.Thread.__init__(self)
        self.client_id = client_id

    def run(self):
        print(f"Client {self.client_id} tries to enter the salon")
        if not waiting_seats_semaphore.acquire(blocking=False):
            print(f"Client {self.client_id} leaves as there are no free seats")
            return
        print(f"Client {self.client_id} enters the airlock")
        if not airlock_semaphore.acquire(blocking=True):
            print(f"Client {self.client_id} waits for the airlock to be free")
        time.sleep(random.uniform(0.1, 0.5))
        print(f"Client {self.client_id} leaves the airlock")
        airlock_semaphore.release()
        print(f"Client {self.client_id} enters the salon there are {waiting_seats_semaphore._value} free seats")
        with waiting_queue_lock:
            waiting_queue.append(self.client_id)


# Create and start hairdresser threads
hairdressers = [Hairdresser(i) for i in range(NUM_HAIRDRESSERS)]
for hairdresser in hairdressers:
    hairdresser.start()

# Create and start client threads
clients = [Client(i) for i in range(30)]  # Assuming 20 clients for testing
for client in clients:
    time.sleep(random.uniform(0.1, 0.6))
    client.start()

# Wait for all clients to be processed
for client in clients:
    client.join()

# Stop hairdresser threads
for hairdresser in hairdressers:
    hairdresser.join()

print("All clients have been processed.")