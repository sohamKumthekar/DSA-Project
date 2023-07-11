Design and implementation of N-body simulation using Octrees

Instructions to run : There is a makefile so type “make run” in the terminal.

Explanation of the output : The output is the n-body gravitational simulation (of 20 bodies). The bodies are moving according to their gravitational interactions with each other derived using the Barnes Hut algorithm for Octrees. The system of the simulation is not stable which is why the bodies disappear after some time. This is because the values of velocity , mass and position have been taken arbitrarily. Therefore the gravitational interactions are not stable. But , you can see that there are some bodies that form an ellipse, but then due to acceleration gain velocity more that escape velocity and are escaping the orbits (again this is due to arbitrary masses , velocities and positions).
In short the N-body Simulation is working properly , the problem is that there is no data set on the internet which gives accurate masses, positions and velocities of celestial bodies , hence the values were taken at random.  

Data Structures and Algorithms used : 
1) Octree data structure – used to divide the 3D space
2) Barnes Hut Algorithm – used to reduce time complexity of finding the gravitational interactions between all bodies. It reduces time complexity from O(n2) to O(nlogn)
3) Verlet Velocity Integration – to find the force of gravity between 2 bodies.
