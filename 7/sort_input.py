import sys

input = list(map(int, sys.stdin.readlines()[0].split(',')))
print(",".join(map(str, sorted(input))))
