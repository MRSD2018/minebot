import random

count = 0
hits = 0

while True:
    ones, twos, threes, fours, fives, sixes = [0]*6
    for x in range(420):
        thing = random.randint(1, 6)
        if thing == 1:
            ones += 1
        elif thing == 2:
            twos += 1
        elif thing == 3:
            threes += 1
        elif thing == 4:
            fours += 1
        elif thing == 5:
            fives += 1
        elif thing == 6:
            sixes += 1
    if ones == twos and twos == threes and threes == fours and fours == fives and fives == sixes:
        hits += 1
    count += 1
    if (count % 1000) == 0:
        print(count, float(hits)/count)
