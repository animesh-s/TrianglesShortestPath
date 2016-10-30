# Triangles Shortest Path

## Problem Statement

The first part of the application finds the shortest path between all pairs of triangles in a randomly chosen subset from a set of triangles specified in an .obj file. The second part allows the user to view the rendered object from various angles with the help of keyboard controls.

## Compilation and Running

Compile the program by the command g++ ShortestPath.cpp 

Run the program by the command ./a.out ../resources/filename.obj

Compile the second program by the command g++ Navigation.cpp -lglut -lGLU

Run the program by the command ./a.out

Sample Input :
./a.out ../resources/mug.obj
