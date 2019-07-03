from collections import deque
import random
import math
import itertools


def poisson(rate):
	return -1 * (1 / rate) * math.log(1 - random.random())


def generate_packets(rate):
	current_time = 0
	while True:
		current_time += poisson(rate)
		yield current_time


# Params
Tsim = 1000
node_count = 4  # [20,40,60,80,100]
arrival_rate = 7  # [7,10,20]
channel_speed = 8 * (10 ** 6)
packet_length = 1500
adjacent_node_distance = 10
propagation_speed = 2 * (10 ** 8)


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
		return self.buffer[-1]

	# Return true is channel is open
	def sense(self):
		pass


nodes = [Node(i) for i in range(node_count)]


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
		propagation_delay = abs(n.head() - node.head()) / propagation_speed
		if n.head() < node.head() + propagation_delay:
			collisions.append(n)

	return collisions

def update_collision(time):


transmitted_packet_count = 0
successful_transmitted_packet_count = 0


def main():
	while True:
		node = next_node()
		collisions = detect_collision(node)





