# Caesar
from cs50 import get_string
import sys

# Check for command line arguments and key
if len(sys.argv) != 2:
    print("Usage: python caesar k")
    exit(1)
if not sys.argv[1].isnumeric():
    print("Key must be a valid integer")
    exit(2)

# Get key from cmdline, and plaintext from input
key = int(sys.argv[1])
plaintext = get_string("plaintext: ")

# Initialize ciphertext string
ciphertext = ""

# Go through every char in plaintext
for letter in plaintext:
    # Cipher if char is an alphabetic letter
    if letter.isalpha():
        if letter.isupper():
            ciphertext += chr((ord(letter) + key - ord("A")) % 26 + ord("A"))
        else:
            ciphertext += chr((ord(letter) + key - ord("a")) % 26 + ord("a"))
    # Skip if letter is not alphabetic
    else:
        ciphertext += letter

# Print ciphertext
print(f"ciphertext: {ciphertext}")