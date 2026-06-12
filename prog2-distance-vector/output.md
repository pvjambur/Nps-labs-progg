# Program 2 – Sample Execution Output

---

## Sample 1 – 3 Nodes

**Input:**
```
Enter the no of nodes: 3
Enter the cost matrix:
0 2 7
2 0 1
7 1 0
```

**Output:**
```
For router 1
  For node 1 via 1 distance is 0
  For node 2 via 2 distance is 2
  For node 3 via 2 distance is 3

For router 2
  For node 1 via 1 distance is 2
  For node 2 via 2 distance is 0
  For node 3 via 3 distance is 1

For router 3
  For node 1 via 2 distance is 3
  For node 2 via 2 distance is 1
  For node 3 via 3 distance is 0
```

> Router 1 → Node 3: goes via Router 2 (cost 2+1=3), cheaper than the direct link (cost 7).

---

## Sample 2 – 4 Nodes

**Input:**
```
Enter the no of nodes: 4
Enter the cost matrix:
0   1   3   999
1   0   1   4
3   1   0   2
999 4   2   0
```

**Output:**
```
For router 1
  For node 1 via 1 distance is 0
  For node 2 via 2 distance is 1
  For node 3 via 2 distance is 2
  For node 4 via 2 distance is 5

For router 2
  For node 1 via 1 distance is 1
  For node 2 via 2 distance is 0
  For node 3 via 3 distance is 1
  For node 4 via 3 distance is 3

For router 3
  For node 1 via 2 distance is 2
  For node 2 via 2 distance is 1
  For node 3 via 3 distance is 0
  For node 4 via 4 distance is 2

For router 4
  For node 1 via 3 distance is 5
  For node 2 via 3 distance is 3
  For node 3 via 3 distance is 2
  For node 4 via 4 distance is 0
```

---

## Notes

- Use `999` to represent infinity (no direct link).
- The diagonal of the cost matrix is always 0 (self-distance).
- Output shows the **next-hop** router and the **total path cost**.
