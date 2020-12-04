#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();

    // Prints sorted pairs (extra)
    printf("sorted pairs: [");
    for (int k = 0; k < pair_count; k++)
    {
        printf("(%i, %i)", pairs[k].winner, pairs[k].loser);
    }
    printf("]\n");

    lock_pairs();

    // Prints locked array (extra)
    printf("Locked pairs array\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Go through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Find candidate, and mark ranks
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    // Invalid vote
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Fills preferences array
    // preferences[i][j] is how many vote prefers i over j
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j)
            {
                preferences[i][j] = 0;
            }
            else
            {
                // Go through ranks of all candidates on votes
                for (int k = 0; k < candidate_count; k++)
                {
                    for (int l = 0; l < candidate_count; l++)
                    {
                        // If candidate i's rank is higher than cand j, add 1
                        // to the preferences[i][j]
                        if (ranks[k] == i && ranks[l] == j && l > k)
                        {
                            preferences[i][j] += 1;
                        }
                    }
                }
            }
            //printf("%i ", preferences[i][j]);
        }
        //printf("\n");
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Goes through all candidates on preferences array
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Fills pairs array by comparing preferences
            if (preferences[i][j] > preferences[j][i])
            {
                //printf("Winner: candidate %i\n", i);
                //printf("Loser: candidate %i\n", j);
                pair_count += 1;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1].loser = j;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // For each pair, array will be filled with margins of victories
    int margin_of_vic[pair_count];
    for (int k = 0; k < pair_count; k++)
    {
        margin_of_vic[k] = preferences[pairs[k].winner][pairs[k].loser];
    }

    // Goes through all pairs
    // Sorts them by descending order of margin of victories, using bubble sort
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (margin_of_vic[i] < margin_of_vic[j])
            {
                int tmp_w = pairs[i].winner;
                pairs[i].winner = pairs[j].winner;
                pairs[j].winner = tmp_w;

                int tmp_l = pairs[i].loser;
                pairs[i].loser = pairs[j].loser;
                pairs[j].loser = tmp_l;
            }
        }
    }
    return;
}

// Checks if a cycle happens (new function to be used in lock_pairs)
// Inputs are index of pair in checking process and indices of winner and loser
bool check_cycle(int k, int winner, int loser)
{
    // Goes through all sorted pairs
    for (int n = 0; n < pair_count; n++)
    {
        // Checks if loser of pair k is winner of another locked pair
        if (pairs[n].winner == loser && locked[pairs[n].winner][pairs[n].loser])
        {
            // Checks recursively if this locked pair creates a cycle
            // Note that initial pair index k is saved throughout
            // entire recursion
            if (check_cycle(k, pairs[n].winner, pairs[n].loser))
            {
                return true;
            }
            // If no cycle has been created, we can safely check for another pair
            else
            {
                continue;
            }
        }
        // Base case of recursion, if this recursion ends up where it started,
        // a cycle is generated
        else if ((pairs[k].winner == loser) && n == k)
        {
            return true;
        }
    }
    // If none of the conditions have been met, no cycle happens in graph
    return false;
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Go through every sorted pair
    for (int k = 0; k < pair_count; k++)
    {
        // Based on the result from check_cycle, lock or skip pair k
        locked[pairs[k].winner][pairs[k].loser] = !check_cycle(k, pairs[k].winner, pairs[k].loser);
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Initial count of how many locked pairs in a column
    int column = 0;

    // Goes through every candidate in locked array
    // Counts how many people loses in comparison
    for (int j = 0; j < candidate_count; j++)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            column += locked[i][j];
        }
        // If no one wins over specific candidate, prints this candidate as winner
        if (column == 0)
        {
            printf("%s\n", candidates[j]);
        }
        else
        {
            // Reset value for next iteration
            column = 0;
        }
    }

    return;
}