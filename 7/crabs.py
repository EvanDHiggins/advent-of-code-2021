import sys
import math

with open('input.txt') as f:
    crabs = list(map(int, f.readline().split(',')))

best = math.inf
for pos in range(max(crabs)):
    cost = 0
    for crab in crabs:
        cost += abs(crab - pos)
    best = min(cost, best)
print(best)
