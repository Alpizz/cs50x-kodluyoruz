from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    
    # Split strings into lists
    wordlist_a = a.split("\n")
    wordlist_b = b.split("\n")
    
    # Init empty list
    lines_list = []
    
    # Collect similars into one list
    for word in wordlist_a:
        if word in wordlist_b:
            lines_list.append(word)

    for word in wordlist_b:
        if word in wordlist_a:
            lines_list.append(word)

    # Eliminate duplicates and return
    return list(set(lines_list))
    

def sentences(a, b):
    """Return sentences in both a and b"""
    
    # Split sentences into lists
    sent_a = sent_tokenize(a, language='english')
    sent_b = sent_tokenize(b, language='english')
    # Init empty list
    sent_list = []
    
    # Collect similar sentences
    for sent in sent_a:
        if sent in sent_b:
            sent_list.append(sent)

    for sent in sent_a:
        if sent in sent_b:
            sent_list.append(sent)
    
    # Eliminate duplicates and return
    return list(set(sent_list))

# Splits given string into n sized substrings


def seperate_str(s, n):
    splitted = []
    # Collect n sized substrings into splitted list
    for i in range(len(s)):
        if i + n <= len(s):
            splitted.append(s[i:i+n])
    return splitted


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    subs = []
    
    # Get lists of n sized substrings for a, b
    sp_a = seperate_str(a, n)
    sp_b = seperate_str(b, n)
    
    # Collect similar substrings into substr list
    for substr in sp_a:
        if substr in sp_b:
            subs.append(substr)

    for substr in sp_b:
        if substr in sp_a:
            subs.append(substr)
    
    # Eliminate duplicates and return
    return list(set(subs))
