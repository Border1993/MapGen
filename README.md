# MapGen
Tile-based 2d map generator that was created as a part of bigger project made in UE4. 

The purpose of it was to generate 2d city-like street layout that will be converted into actual map for a game. Each block that is shown at output will be represented by either road tile, big and special building or small random blocks composed of 1x1 sized floors.

How it is accomplished? First, a matrix-like grid is created. Then a few random grid points are selected, each one of them has to be certain distance away from each other. Then graph is created by selecting first 2 points and connecting them with a path via BFS-like pathfinding algorithm. After that we select each remaining point and connect it to random(1,3) different points that are already connected. Next, big buildings are randomized. There is a file that specifies tile representation of each building. Each of those representations has tiles reserved for road, building itself and points that must be connected to the rest of the graph. Rotations, places and amount are randomized based on application settings. After that there is a cleanup operation to eliminate certain road patterns and elliminate them so the roads won't look riddiculous sometimes. It always preserves essential roads.

License : you can do whatever you want with this.
