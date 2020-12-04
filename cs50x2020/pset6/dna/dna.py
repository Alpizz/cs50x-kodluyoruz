# DNA
import sys
import csv

# Check for argument count
if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# Get filenames from cmdline args
filename_csv = sys.argv[1]
filename_txt = sys.argv[2]

# Initialize dictionaries for storing DNA information
big_dict = {}
tmp_dict = {}

# Open DNA database and read them into dictionary
with open(filename_csv, 'r', newline="") as csvfile:
    reader = csv.DictReader(csvfile)
    # Populate big dictionary with names as keys
    # and DNA subsequence counts (a dictionary too) as values
    for line in reader:
        for key in line.keys():
            if key != 'name':
                tmp_dict[key] = line[key]
        big_dict[line['name']] = tmp_dict
        # Reset temporary dictionary
        tmp_dict = {}

# Extract DNA subsequences as list for comparison convenience
dna_parts = list(list(big_dict.values())[1].keys())
# Initialize sequence count dictionary for reading from txt
seq_counts = dict()

# Open txt sequence and read the most repeated subsequences into
# seq_cts dictionary (Keys: DNA subsqs, Values: Counts)
with open(filename_txt, 'r', newline="") as txtfile:
    for sequence in txtfile:
        for sub_seq in dna_parts:
            count = 1
            while(True):
                if sub_seq * count in sequence:
                    count += 1
                    continue
                else:
                    count -= 1
                    seq_counts[sub_seq] = f'{count}'
                    break
            count = 0
# Check for matching, print and exit if name's DNA matches
for name, seq_ct in big_dict.items():
    if seq_ct == seq_counts:
        print(name)
        exit(0)

# No match if loop above doesn't exit the program
print("No match")
exit(0)