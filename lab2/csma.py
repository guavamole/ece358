from collections import deque
import random
import math
import itertools


def poisson(rate):
	return -1 * (1.0 / rate) * math.log(1 - random.random())


def generate_packets(rate):
	current_time = 0
	while True:
		current_time += poisson(rate)
		yield current_time


# Params
Tsim = 200
node_count = 30  # [20,40,60,80,100]
arrival_rate = 12  # [7,10,20]
channel_speed = 10.0 ** 6
packet_length = 1500
adjacent_node_distance = 10
propagation_speed = 2.0 * (10 ** 8)
Tp = 512 / channel_speed
transmission_delay = packet_length / channel_speed

big_num = 9999999999999999

class Node:

	def __init__(self, position):
		self.id = position
		self.position = position * adjacent_node_distance
		self.collision_count = 0
		self.buffer = deque()

		gen = generate_packets(arrival_rate)
		while True:
			self.buffer.append(next(gen))
			if self.buffer[-1] > Tsim:
				break

	def head(self):
		if len(self.buffer) == 0:
			return big_num
		return self.buffer[0]

	# Return true is channel is open
	def sense(self):
		pass


nodes = [Node(i) for i in range(node_count)]
transmitted_packet_count = 0
successful_transmitted_packet_count = 0

def next_node():
	node = nodes[0]
	for n in nodes:
		if n.head() < node.head():
			node = n
	return node

def detect_collision(node):
	collisions = []

	for n in nodes:
		if n.id == node.id:
			continue
		propagation_delay = (abs(n.position - node.position) / propagation_speed)
		if n.head() < node.head() + propagation_delay:
			collisions.append(n)

	return collisions

def update_collision(colliding_nodes, original_node):
	global transmitted_packet_count
	transmitted_packet_count += (len(colliding_nodes) + 1)
	for node in colliding_nodes:
		node.collision_count += 1
		if node.collision_count == 10:
			node.buffer.popleft()
			node.collision_count = 0
		else:
			propagation_delay = abs(original_node.position - node.position) / propagation_speed
			node_time = node.head() + propagation_delay
			wait_time = (2 ** random.randint(0, node.collision_count - 1)) * Tp
			new_time = node_time + wait_time
			for idx in range(len(node.buffer)):
				if node.buffer[idx] > new_time:
					break
				node.buffer[idx] = new_time


last_t = 0

while True:
	node = next_node()
	if len(node.buffer) == 0 or last_t > Tsim:
		break
	last_t = node.head()

	collisions = detect_collision(node)
	if len(collisions) == 0:
		successful_transmitted_packet_count += 1
		transmitted_packet_count += 1
		arrival_time = node.head()
		node.collision_count = 0
		node.buffer.popleft()

		for n in nodes:
			propagation_delay = abs(node.position - n.position) / propagation_speed
			departure_time = arrival_time + propagation_delay + transmission_delay

			for idx in range(len(n.buffer)):
				if n.buffer[idx] >= departure_time:
					break

				n.buffer[idx] = departure_time
	else:
		update_collision(collisions, node)

print successful_transmitted_packet_count
print transmitted_packet_count
print 1.0 * successful_transmitted_packet_count / transmitted_packet_count
print (successful_transmitted_packet_count * packet_length) / (last_t * channel_speed)



