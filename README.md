## Design Strategies for Computer Algorithms

###　Implement four computer algorithms
* the longest common subsequence problem (Dynamic Programming)
* two dimentional linear programming problem (Prune and Search)
* 0/1 knapsack problem (Branch and Bound)
* two dimentional closet pair problem (Divide and Conquer)

#### The Longest Common Subsequence Problem (Dynamic Programming)
Given two strings `X=x1x2...xn` and `Y=y1y2...ym`, find their longest common subsequence.Denote as LCS(n,m)<br />
Consider the following cases:<br />
* If xi = yj, the LCS(i,j) = LCS(i-1,j-1) + 1
* If xi != yj, then LCS(i,j) = max( LCS(i-1,j), LCS(i, j-1) )
* If i = 0 or j = 0, LCS(i,j) = 0
`Time Complexity: O(nm)`

#### Two Dimentional Linear Programming Problem (Prune and Search)
Minimize the value of `a * x + b * y` subject to `a_i * x + b_i * y + c_i <= 0    for i = 1, 2, .... n` <br />
Which is equivalent to compute the smallest y' coordinate in the feasible region (a_i' * x' + b_i' * y' + c_i' <= 0    for i = 1, 2, .... n).<br />
* Partition n constraints into three classes, according to whether b_i' is positive, zero, and negative.
* Find the optimal x coordinate is on the left or right side of the median of x coordinate, or no feasible region.
* Iteratively prune redundant constraints.
`Time Complexity: T(n) <= T(3n/4) + O(n) <= O(n)`

#### 0/1 Knapsack Problem (Branch and Bound)
Given n objects and a knapsack. Each object has a weight `w_i` and a price value `p_i`. Obtain the maximum profit to fill the knapsack.<br />
* Branch: explore the node with greatest upper bound.
* Bound: if encounter with lower bound >= upper bound, this node could not obtain better solution.

#### Two Dimentional Closet Pair Problem (Divide and Conquer)
Given a set of n points in the plane, find the pair of points that are closet.<br />
* Divide: divide n points into two equal-size subsets L and R according to x coordinate. Define the median of x coordinate as `x_median`.
* Conquer: Recursive to find the closet pair in two subsets L and R. Denote as `d_l` and `d_r`.
* Merge: Find the closet pair across two subsets L and R. Let `d = min(d_l, d_r)`. Only need to examine seven points located in the range of `[ x_midian- d, x_median + d ]`. 
`Time Complexity: T(n) = 2T(n/2) + O(n) = O(nlogn)`