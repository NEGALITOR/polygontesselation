# Polygon Tesselation (Ear Clipping Algorithm)

## Algorithm
The ear clipping algorithm implemented within my program converts a user drawn polygon into multiple triangles. The program attempts to then utilize these triangles to better fill the polygon. The algorithm works by checking the vertices orientation of CCW or CW and loops for each consecutive triplet and determines if they can form a triangle. It first checks the interior to determine concavity and then checks if there is a vertex within the triangle that it will form. Valid triangles are then pushed into the triangle list and the center vertex is removed from the index list. It repeats this until no more triangles can be made.

## Compile and Run
```
make poly
./poly.out
```
