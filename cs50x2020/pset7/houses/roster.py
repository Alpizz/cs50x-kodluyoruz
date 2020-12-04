# Prints all of the student's information for desired house
import cs50
import sys
import csv

# Check for commandline arguments
if len(sys.argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# Get house name from commandline
house_name = sys.argv[1]

# Open sqlite database
db = cs50.SQL("sqlite:///students.db")

# Execute query, generate a list of student dictionaries for given house name
# ordered by their last name
student_table = db.execute("SELECT * FROM students WHERE students.house = ? ORDER BY students.last, students.first", house_name)

# Go through every student dictionary in the list
for student in student_table:

    # Check for None values in the middle
    name_list = [n for n in [student['first'], student['middle'], student['last']] if n != None]

    # Join first (middle) last names in name string
    name = " ".join(name_list)

    # Print student name and birth year from dictionary
    print(f"{name}, born {student['birth']}")