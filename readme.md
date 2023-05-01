# Compiler Parser
## Intro
This repository record my assignment progress, which focuses on building up a Parser using LL(1) algorithm.

<!-- ## Pipeline -->
## Algorithms

在本项目中，我使用了LL(1)算法来构建语法分析器。LL(1)算法的核心是构建预测分析表，而构建预测分析表的核心是构建First集和Follow集。因此，我首先改写输入文法规则为LL(1)文法，而后实现了构建First集和Follow集的算法，然后再根据First集和Follow集构建预测分析表，最后使用预测分析表来进行语法分析。
以下是所用到的一些算法的文字说明或伪代码结构:

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

## Pipeline

整个项目的体系结构如下图所示:

![img](imgs/pipeline.png)

## Reproduce

本项目使用makefile管理编译和测试过程，您可以使用如下命令测试本项目针对一段默认语法规则和源程序的语法分析结果：

```bash
    make clean
    make test
```

### Input

本项目的外界输入包括语法规则与以token为单位的源程序：

+ test/input/grammar.txt：用于存放语法规则的文件，您可以通过阅读该文件来了解本项目所使用的语法规则。
+ test/input/tokens.txt： 用于存放token序列的文件，您可以通过阅读该文件来了解本项目所使用的模拟token序列规则。

### Output

本项目的通过.show()方法，可以输入以下辅助信息:
  + symbol table: 用于说明所有的符号并注明了终极符和非终极符
  + Rules: 用于保存所有的产生式（若程序中执行了消除左递归和左因子化操作，则会显示消除左递归和左因子化后的产生式）
  + FIRST: 针对所有符号的FIRST集
  + FOLLOW: 针对所有非终极符的FOLLOW集
  + Parsing Table： 预测分析表

本项目针对给定语法规则，会为以token为单元的源程序给出如下两类判断结果：
  + 无错误！
  + 存在错误，并逐条打印认为错误的token，token出现的位置，错误的类型，以及进行错误修复的策略
  
## Comppiler

本项目通过本地设备编译运行测试.

测试设备操作系统信息如下:

```bash
    Distributor ID: Ubuntu
    Description:    Ubuntu 20.04.6 LTS
    Release:        20.04
    Codename:       focal
```

测试设备所用g++编译器版本信息如下:
```bash
    g++ (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
    Copyright (C) 2019 Free Software Foundation, Inc.
```

## Git

本项目的开发使用Git进行了版本管理，您可以通过git log来查看开发过程中的历史版本信息。
