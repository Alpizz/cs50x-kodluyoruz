# Credit Python by Alpizz
from cs50 import get_int
# Init variables
card_num = 0
total_even = 0
total_odd = 0
grand_tot = 0

# Get card number from user as an int
card_num = get_int("Enter card number: ")

# Convert this integer value to string
card_num = str(card_num)

# Iterate through each digit from last to end
# And count digit numbers using enum
for i, digit in enumerate(card_num[::-1]):
    # print("{} | {}".format(digit, i+1))
    # Convert each number to int
    digit = int(digit)
    
    # Luhn's algorithm
    if (i+1) % 2 == 0:
        # If multiplication exceeds 9, sum up its digits
        if digit * 2 >= 10:
            total_even += (((digit*2) % 10) + 1)
        else:
            total_even += digit * 2
    else:
        total_odd += digit

# Total value after Luhn's algo
grand_tot = total_even + total_odd

# Checksum
if grand_tot % 10 == 0:
    
    # print("VALID: {}".format(grand_tot))
    # Print card's type according to given description
    if int(card_num[0]) == 4:
        print("VISA\n", end="")
        
    elif int(card_num[:2]) == 37:
        print("AMEX\n", end="")
        
    elif int(card_num[:2]) <= 55 or int(card_num[:2]) >= 51:
        print("MASTERCARD\n", end="")
        
    else:
        print("INVALID\n", end="")
    
else:
    print("INVALID\n", end="")