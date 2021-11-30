import numpy as np
import numpy.random as rd
import Task
import math

def seed_sol(tasks):
    """
    Args:
        tasks: list[Task], get_task_id, get_deadline,get_duration, get_max_benefit
    Returns:
        output: list of igloos in order of polishing  
                end index
    """
    #greedy by profit/duration
    res = []
    n = len(tasks)
    curr_time = 0
    residual_list = [i+1 for i in range(n)]
    for i in range(n):
        ratio = 0
        idx = -1
        for j in range(n-i):
            if tasks[residual_list[j]-1].get_duration() + curr_time >= 1440:
                continue
            exceed_time = tasks[residual_list[j]-1].get_duration() + curr_time - tasks[residual_list[j]-1].get_deadline()
            curr_ratio = tasks[residual_list[j]-1].get_late_benefit(exceed_time)
            if curr_ratio > ratio:
                ratio = curr_ratio
                idx = j
        if idx == -1:           # if all task are not valid
            res += residual_list
            i -= 1
            break
        else:
            curr_time += tasks[residual_list[idx]-1].get_duration()
            res.append(residual_list.pop(idx))
        
    return (res,i+1)      #i is the number of real tasks

def trans_sol(sol,tasks,end):
    res = sol.copy()
    #swap
    if end < len(sol) and end > 0:
        idx1 = rd.randint(0,end)
        idx2 = idx1 + rd.randint(1,len(sol)-idx1)
        res[idx1],res[idx2] = res[idx2],res[idx1]
    else:
        idx = rd.choice(len(sol),2)
        res[idx[1]],res[idx[0]] = res[idx[0]],res[idx[1]]
    #insert

    #reverse
    return res
    

def eval_sol(sol,tasks,n=None):
    """
    Args:
        sol:   a list of idx of tasks to process
        tasks: list[Task], tasks[0] idx, tasks[1] deadline, tasks[2] duration, tasks[3] profit
        n: end index
    Returns:
        output: the total profit in such solution  
        end index
    """
    curr_time = 0
    total_profit = 0
    if n == None:
        n = len(sol)
        assert n == len(tasks)   #error..
        for i in range(n):          #iter all task
            if tasks[sol[i]-1].get_duration() + curr_time >= 1440:
                i -= 1
                break
            exceed_time = tasks[sol[i]-1].get_duration() + curr_time - tasks[sol[i]-1].get_deadline()
            total_profit +=  tasks[sol[i]-1].get_late_benefit(exceed_time)
            curr_time += tasks[sol[i]-1].get_duration() 
    else:
        for i in range(n):          #iter all real task
            if tasks[sol[i]-1].get_duration() + curr_time >= 1440:
                i -= 1
                break
            exceed_time = tasks[sol[i]-1].get_duration() + curr_time - tasks[sol[i]-1].get_deadline()
            total_profit +=  tasks[sol[i]-1].get_late_benefit(exceed_time)
            curr_time += tasks[sol[i]-1].get_duration() 

    return (total_profit,i+1)

def SA(tasks):
    """
    Args:
        tasks: list[Task], list of igloos to polish, tasks[0] idx...etc
    Returns:
        output: list of igloos in order of polishing  
    """
    epochs = 100
    M = 200
    solution,end = seed_sol(tasks)
    #solution = (rd.choice(len(tasks),len(tasks),replace=False)+1).tolist()
    temperature_list = []
    initial_acceptance = 0.4
    profit,end = eval_sol(solution,tasks)

    for i in range(10):     #generate the initial temperates
        next_sol    = trans_sol(solution,tasks,end)
        next_profit, next_end = eval_sol(next_sol,tasks)
        temperature_list.append(-abs(next_profit-profit)/math.log(initial_acceptance))

        if rd.uniform(0,1) < initial_acceptance:
            solution = next_sol            
            profit = next_profit
            end = next_end
    temperature_list = np.array(temperature_list)
    
    for i in range(epochs):
        flag = 0
        for m in range(M):
            next_sol    = trans_sol(solution,tasks,end)
            next_profit, next_end = eval_sol(next_sol,tasks)
            if next_profit > profit:
                solution = next_sol            
                profit = next_profit
                end = next_end
            else:
                r=rd.uniform(0,1)
                if r < math.exp((next_profit-profit)/temperature_list.max()):
                    flag += 1
                    solution = next_sol            
                    profit = next_profit
                    end = next_end
        if flag > 0:
            temperature_list = np.delete(temperature_list,temperature_list.argmax())    #pop the max
            temperature_list = np.append(temperature_list,temperature_list.mean()/flag)
    
    return solution,end

if __name__ == '__main__':      #unit tests here
    tasks = []
    sol = SA(tasks)
    print(sol)