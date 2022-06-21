# 42 Philosophers

#### Overview

This is a 42 project whose aim it to simulate the famous dining philosophers problem.
#### Introduced concepts
⭐ Multithreads processing  
⭐ Mutexes  
⭐ C time management  
 
#### Description
- X philosophers sit at a round dining table
- There are X forks around the table, each between two philosophers
- A philosopher needs two forks to eat
- They alternatively think --> eat --> sleep --> think--> eat --> sleep
- The simulation stops when a philosophers dies not having eaten
- If a 5th argument is provided, the simulation may stop if all philosophers have eaten that many times before one dies.
#### Restrictions
- Global variables forbidden
#### Usage
    make && ./philo NB_PHIL TIME_BEFORE_DYING TIME_TO_EAT TIME_TO_SLEEP [NB_MEALS_BEFORE_END]

    make && ./philo 4 400 200 100 [3]  
#### Example
    ./philo 4 300 200 100
    
![phil](https://user-images.githubusercontent.com/49759654/174774281-434e3817-8870-40c0-a55e-4ab5ceaa8fca.jpg)


