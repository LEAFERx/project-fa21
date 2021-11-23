import numpy as np
import numpy.random as rd
import math

eps = 1

def seed_sol(tasks):
    #greedy by profit/duration
    res = []
    n = len(tasks[0])
    curr_time = 0
    residual_list = tasks[0].copy()
    for i in range(n):
        ratio = 0
        idx = -1
        for j in range(n-i):
            exceed_time = tasks[2][residual_list[j]] + curr_time - tasks[1][residual_list[j]]
            if exceed_time > 0:
                curr_ratio = math.exp(−0.017*exceed_time) * tasks[3][residual_list[j]]/tasks[2][residual_list[j]]
                if curr_ratio > ratio:
                    ratio = curr_ratio
                    idx = j
            else
                curr_ratio = tasks[3][residual_list[j]]/tasks[2][residual_list[j]]
                if curr_ratio > ratio:
                    ratio = curr_ratio
                    idx = j
        curr_time += tasks[2][residual_list[idx]]
        assert idx != -1    # it doesn't find a task what can be? 
        res.append(residual_list.pop(idx))
        if curr_time >= 1440:
            res += residual_list
            break
    return res

def trans_sol(sol,tasks):
    pass

def eval_sol(sol,tasks):
    curr_time = 0
    n = len(sol)
    total_profit = 0

    assert n == len(tasks[0])   #error..

    for i in range(n):          #iter all task
        exceed_time = tasks[2][sol[i]] + curr_time - tasks[1][sol[i]]
        if exceed_time > 0:
            total_profit +=  math.exp(−0.017*exceed_time) * tasks[3][sol[i]]
        else
            total_profit +=  tasks[3][sol[i]]
        curr_time += tasks[2][sol[i]]
        if curr_time >= 1440:
            break

    return total_profit

def SA(tasks):
    """
    Args:
        tasks: list[Task], list of igloos to polish, tasks[0] idx...etc
    Returns:
        output: list of igloos in order of polishing  
    """
    epochs = 10000
    solution = seed_sol(tasks)
    profit = eval_sol(solution)
    for i in range(epochs):
        next_sol    = trans_sol(solution)
        next_profit = eval_sol(next_sol)
        if rd.uniform(0,1,size=1) < np.exp(eps*(next_profit-profit)/i):
            solution = next_sol
            profit = next_profit
    
    return solution

if __name__ == '__main__':      #unit tests here
    tasks = []
    sol = SA(tasks)
    print(sol)