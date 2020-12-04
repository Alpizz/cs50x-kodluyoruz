# Bleep
from cs50 import get_string
import sys

# Main function
def main():
    # Check argument count
    if len(sys.argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)
    
    # Get file name from cmdline
    filename = sys.argv[1]
    # Initialize banned words list
    banned_list = list()
    
    # Open txt of banned words
    with open(filename, 'r') as censor_words:
        # Add every word in censor_words, lowered
        for word in censor_words:
            banned_list.append(word.lower().strip())
            
    # Get text from the user
    user_text = get_string("What message would you like to censor?\n")
    # Split string to text_list
    text_list = user_text.split()
    # Initalize censored string
    censored = ""
    
    # Go through every word in user input
    for word in text_list:
        # Add exact word if not banned
        if word.lower() not in banned_list:
            censored += word
        # Add the words length number of stars if the word is banned
        else:
            censored += "*" * len(word)
        # Add space between words
        censored += " "
    # Print stripped censored string
    print(censored.strip())

# Execute main
if __name__ == "__main__":
    main()
