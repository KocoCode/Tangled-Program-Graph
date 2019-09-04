Tangled Program Graphs
===
This project is an implementation of Tangled Program Graphs (TPG), a genetic programming framework presented in [this paper](https://www.mitpressjournals.org/doi/full/10.1162/evco_a_00232).
[An implementation](https://github.com/skellco/Tangled-Program-Graphs) already exists, here we attempt to make a more efficient version of it.
Though we have yet to prove it, our code does have some potential to run faster. Below is a summary of the improvements we introduced to the original code.

## Key Improvements

### 1. Maintenance of Population

Better way to maintain program (bidder) population and team population.

#### 1-1 Randomized Set

A randomized set provides the following operations:
* O(1) Insertion
* O(1) Removal
* O(1) Get random element
* O(n) Iterate through elements

Using this data structure improves the speed for mutating teams, which plays a big part of the TPG algorithm.

#### 1-2 Pool Proxy

The main purpose of the pool proxy is to act as a database of the program population and team population. It helps avoid unnecessary argument passing and copying of complex structures.
Besides, it's responsible for maintaining the "reference count" of each team.

### 2. Root Team Maintenance

With help from a better way to tell if each team is also a root team (by maintaining reference count for each team), generating new teams now costs O(T) time, where T is the size of team population.

### 3. Neutrality Test

By storing a profile for each "state" (instead of a profile for each "program"), the cost for neutrality test is improved from O(P\) to O(log P), where P is the size of program population. To see how this is the case, we briefly elaborate how neutrality test is implemented in both works.

In the original work, a profile for each program is kept. A profile for a program is the bids it make for 50 previous states. Each time a program is changed, it's to be ensured that its new profile be significantly different from all other profiles. This is checked by an element-to-element comparison between profiles.

In our work, profiles for 50 states are kept. A profile for a state is the bids all programs made for it (sorted). Each time a program is changed, it's to be ensured that its bid for each state is significantly different from those in the corresponding profile, and this can be done by a binary search.

## So does our version really run faster?

Unfortunately, it's difficult to compare the speed for both implementations. Since 1) better agent may play for a longer time within each game run, and 2) model complexity fluctuates during the course of evolving, we have yet to find a way to properly assess efficiency through actual experiments.

Any feedback or contribution is welcomed.