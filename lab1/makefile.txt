generator:
	gcc -o generator generator.c
	./generator
	rm generator

sim1:
	g++ -o sim1 sim1.cc
	./sim1
	rm sim1

sim2:
	g++ -o sim2 sim2.cc
	./sim2
	rm sim2