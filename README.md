With the rise of SIMT optimized applications, the scope of what a program can do has risen a lot. In this example of a SIMT program,
a simple game back end will be written that controls millions of NPC characters in a game called beasts and humans.
	The game isn't actually a game as of now, nor will it ever be, but the goal of it is to see what a GPU is capable of.
The rules of the game are simple:
	Every iteration, all NPCs move in their own direction
	If two NPCs are within a range, one greets the other
	If they go out of bounds:
		The NPC is put back in bounds and rotated a random amount


Technical specifications:
	There are a certain amount of continents that are independent from eachother
		These continents are the same size
	There is a certain distance required for an interaction
	Each iteration should take 1/30 of a second (to maintain little lag)
	If an NPC is greeted by another, it increments its counter
	There is a certain amount of NPCs per continent

As per how the program functions:
	Whoever runs the program must specifiy:
		the number of continents
		the amount of NPCs per continent
		the range of interactions
		the amount of iterations
		if the program should run on the GPU or CPU
		the seed
	The program runs
	Afterwards, the results are printed
		Iterations per second
		Elapsed time
		Amount of greetings

To start, we will make a CPU implementation

This is how the program should run:
	user starts program along with specifications through arguments
	arguments are processed and checked for correctness in main
	continents are initialized
	NPCs, which belong to continents, are initialized
	for every continent:
	while i in range of 0, n_iterations:
		move all NPCs
		for every NPC:
			if out of bounds:
				move in bounds
				assign new direction
		for every NPC, cNPC: (current npc)
			for every NPC (besides cNPC), oNPC:  (other npc)
				if they are within range of eachother
					cNPC says hi to	oNPC (oNPC has its counter incremented)
	count how many interactions there were
	report iteractions per second, elapsed time and number of interactions.
