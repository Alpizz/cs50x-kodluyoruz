# Imports csv file into database
import cs50
import sys
import csv

# Check for commandline arguments
if len(sys.argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

# Open sqlite database
db = cs50.SQL("sqlite:///students.db")

# Get filename
csv_filename = sys.argv[1]

# Read csv file and import into database row by row

# Open csv file for reading
with open(csv_filename, 'r') as csvf:
    # Read every row of csv file as dictionaries
    reader = csv.DictReader(csvf)

    # Go through every line of csv file
    for line in reader:
        # String of first, (middle), last names
        student_name = line['name']

        # Split string into names list
        names = student_name.split()

        # Insert None value if there is no middle name
        if len(names) == 2:
            names.insert(1, None)

        # Execute query which inserts first, middle, last, house, birth
        # information into students table in the database
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?,?,?,?,?)",
                   names[0], names[1], names[2], line['house'], line['birth'])
        # Clear names list for next iteration
        names.clear()