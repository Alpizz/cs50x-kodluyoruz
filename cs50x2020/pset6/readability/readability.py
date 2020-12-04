# Readability
from cs50 import get_string

# Get text from user as input
text = get_string("Text: ")

# Initialize word, letter and sentence counts
# Word count must start from one, text must has at lease one word
word_count = 1
letter_count = 0
sentence_count = 0

# Iterate through text char by char
for c in text:
    # Count char as letter if it's alphabetic
    if c.isalpha():
        letter_count += 1
        continue
    # Count spaces, there are (spacecount + 1) words in text
    if c == " ":
        word_count += 1
        continue
    # Count sentences
    if c == "." or c == "!" or c == "?":
        sentence_count += 1
        continue

# Coleman-Liau index is computed as 0.0588 * L - 0.296 * S - 15.8,
# L: avg number of letters per 100 words 
# S: avg number of sentences per 100 words
L = (100 * letter_count) / word_count
S = (100 * sentence_count) / word_count

# Calculate Coleman-Liau index for given L and S values
cl_index = round(0.0588 * L - 0.296 * S - 15.8)

# Print desired grade level
if cl_index > 16:
    print("Grade 16+")
elif cl_index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {cl_index}")

