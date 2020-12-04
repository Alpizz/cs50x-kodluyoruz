# Mario more
from cs50 import get_int

# Init height counter
height = 0
counter = 0

# Get user input until desired range
while height < 1 or height > 8:
    height = get_int("Height: ")

# Loop for stair shape
for row in range(1, height+1):
    for col in range(1, height+1):
        # Prints '#' for stair
        # Prints ' ' for the left of the stair
        if height - col < row:
            print("#", end="")
        else:
            print(" ", end="")
    # Double space between two stairs
    print("  ", end="")
    
    # Right stair loop, for every row
    while counter < row:
        print("#", end="")
        counter += 1
    # After each row, counter needs to be set to zero
    counter = 0
    
    # Newline character at the end of each row
    print()