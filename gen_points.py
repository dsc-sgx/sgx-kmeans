from math import pi, sin, cos
from random import random, choice
import sys
import os

FLOAT_MAX = 1e100
 
class Point:
    __slots__ = ["x", "y", "group"]
    def __init__(self, x=0.0, y=0.0, group=0):
        self.x, self.y, self.group = x, y, group
 
 
def generate_points(npoints, radius):
    points = [Point() for _ in xrange(npoints)]
 
    # note: this is not a uniform 2-d distribution
    for p in points:
        r = random() * radius
        ang = random() * 2 * pi
        p.x = r * cos(ang)
        p.y = r * sin(ang)
 
    return points

def write_points_to_file(fname, points):
    #if not os.path.isfile(fname):
        
    with open(fname, "a") as fout:
        for point in points:
            fout.write("%s %s\n" % (point.x, point.y))
                    
def main():
    NMAX = 1000000
    argc = len(sys.argv) - 1
    if argc != 2:
        print ("PROG nPoints file")
        exit (1)
    else:
        npoints = int(sys.argv[1])
        fname = sys.argv[2]
        nleft = npoints
        while nleft > NMAX:
            nleft -= NMAX
            points = generate_points(NMAX, 10)
            write_points_to_file(fname, points)
        points = generate_points(nleft, 10)
        write_points_to_file(fname, points)

if __name__ == "__main__":
    main()

