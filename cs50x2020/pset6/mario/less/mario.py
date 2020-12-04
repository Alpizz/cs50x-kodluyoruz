# Mario less
from cs50 import get_int

# Init height
height = 0

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
    # Newline character at the end of each row
    print("\n", end="")