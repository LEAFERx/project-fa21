import numpy as np
import numpy.random as rd
import Task
import math

eps = 1

def seed_sol(tasks):
    """
    Args:
        tasks: list[Task], get_task_id, get_deadline,get_duration, get_max_benefit
    Returns:
        output: list of igloos in order of polishing  
    """
    #greedy by profit/duration
    res = []
    n = len(tasks)
    curr_time = 0
    residual_list = [i for i in range(n)]
    for i in range(n):
        ratio = 0
        idx = -1
        for j in range(n-i):
            if tasks[residual_list[j]].get_duration() + curr_time >= 1440:
                continue
            exceed_time = tasks[residual_list[j]].get_duration() + curr_time - tasks[residual_list[j]].get_deadline()
            if exceed_time > 0:
                curr_ratio = tasks[residual_list[j]].get_late_benefit(exceed_time)
                if curr_ratio > ratio:
                    ratio = curr_ratio
                    idx = j
            else:
                curr_ratio = tasks[residual_list[j]].get_max_benefit()/tasks[residual_list[j]].get_duration()
                if curr_ratio > ratio:
                    ratio = curr_ratio
                    idx = j
        if idx == -1:           # if all task are not valid
            res += residual_list
            break
        else:
            curr_time += tasks[residual_list[idx]].get_duration()
            res.append(residual_list.pop(idx)+1)
            if curr_time >= 1440:
                res += residual_list
                break
    return res

def trans_sol(sol,tasks):
    pass

def eval_sol(sol,tasks):
    """
    Args:
        sol:   a list of idx of tasks to process
        tasks: list[Task], tasks[0] idx, tasks[1] deadline, tasks[2] duration, tasks[3] profit
    Returns:
        output: the total profit in such solution  
    """
    curr_time = 0
    n = len(sol)
    total_profit = 0

    assert n == len(tasks[0])   #error..

    for i in range(n):          #iter all task
        if tasks[2][sol[i]] + curr_time >= 1440:
            break
        exceed_time = tasks[2][sol[i]] + curr_time - tasks[1][sol[i]]
        if exceed_time > 0:
            total_profit +=  math.exp(-0.017*exceed_time) * tasks[3][sol[i]]
        else:
            total_profit +=  tasks[3][sol[i]]
        curr_time += tasks[2][sol[i]]        

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