# Compiler Parser
## Intro
This repository record my assignment progress, which focuses on building up a Parser.

<!-- ## Pipeline -->
## Algorithms
- Elimination of left recursion
![img](imgs/alg-elimination_left_recursion.png)

- Left Factoring
![img](imgs/alg-left_factoring1.png)
![img](imgs/alg-left_factoring2.png)

- First Table
![img](imgs/alg-build_first.png)

- Follow Table
![img](imgs/alg-build_follow1.png)
![img](imgs/alg-build_follow2.png)

- Parsing Table
![img](imgs/alg-build_parsing_table1.png)
![img](imgs/alg-build_parsing_table2.png)

- Table-driven predictive parsing
![img](imgs/alg-parsing1.png)
![img](imgs/alg-parsing2.png)

## TODO
- [x] Rules input
- [x] Elimination of left recursion
- [x] Left factoring
- [x] Build up FISRT
- [x] Build up FOLLOW
- [x] Build up parsing table (LL(1))
- [x] Table-driven predictive parsing
- [ ] Error Recovery?