# Vigenere
from cs50 import get_string
import sys


def shift(c):
    if c.isupper():
        return ord(c) - ord('A')
    else:
        return ord(c) - ord('a')


# Check for command line arguments and key
if len(sys.argv) != 2:
    print("Usage: python vigenere k")
    exit(1)
if not sys.argv[1].isalpha():
    print("Usage: python vigenere k")
    exit(1)

# Get key from cmdline, and plaintext from input
keyword = sys.argv[1]
plaintext = get_string("plaintext: ")

# Initialize ordinary numbers for letter counting
j = 0
nlm = 0
print("ciphertext: ", end="")

# Go through every letter in plaintext
for i in plaintext:
    # Check for alphabetical letters
    if i.isalpha():
        key = shift(keyword[j % len(keyword)])
        # Case for lower letter
        if i.islower():
            nlm = (ord(i) + key - ord('a')) % 26

            while nlm < 0:
                nlm += 26
            print(f"{chr(nlm+ord('a'))}", end="")
            j += 1
        # Uppercase
        else:
            nlm = (ord(i) + key - ord('A')) % 26
            while nlm < 0:
                nlm += 26
            print(f"{chr(nlm+ord('A'))}", end="")
            j += 1
    # Non-alpha
    else:
        print(f"{i}", end="")

print()

