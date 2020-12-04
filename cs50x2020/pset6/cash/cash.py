# Cash Python by Alpizz
from cs50 import get_float

# Init variables and get owe amount from user
count = 0
owe = get_float("How much owe: ")

# Simplify with multiplying by 100 and rounding
# to nearest integer
owe = round(owe * 100)

# Subtract maximum possible
# Loop until owe drops to zero
# Count every subtraction
while owe != 0:
    
    if owe >= 25:
        owe -= 25
        count += 1
        
    elif owe >= 10:
        owe -= 10
        count += 1
        
    elif owe >= 5:
        owe -= 5
        count += 1
    elif owe >= 1:
        owe -= 1
        count += 1
    else:
        pass
    
# Print how many coins are used
print("{}".format(count))