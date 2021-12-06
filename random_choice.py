from parse import read_input_file, write_output_file, read_output_file
import Task
import os
import SA_solver as sa
from dp import dp_solver



from itertools import permutations
a = list(range(142,151))
b = list(permutations(a))
old_profit = 0
for i in range (len(b)):
    tasks = read_input_file('./inputs/medium/medium-93.in')
    sol = b[i]
    print(sol)
    profit = sa.eval_sol(sol, tasks, len(sol))[0]
    # profit, new_sol = _sa.eval(sol, tasks)
    print(profit)
    old_profit = profit
    if old_profit< profit:
        f = open('./outputs/medium/medium-'+ str(930) +'.out', 'w')
        for j in range(0,len(a)):
            f.writelines(b[i][j])
        
    