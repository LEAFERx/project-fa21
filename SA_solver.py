import numpy as np
import numpy.random as rd

eps = 1

def seed_sol(tasks):
    pass

def trans_sol(sol,tasks):
    pass

def eval_sol(sol,tasks):
    pass

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

