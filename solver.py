from parse import read_input_file, write_output_file
import Task
import os
import SA_solver as sa

# TODO[chenyu]: After implement in sa.cpp, uncomment next line
# import _sa

def solve(tasks):
    """
    Args:
        tasks: list[Task], list of igloos to polish
    Returns:
        output: list of igloos in order of polishing  
    """
    sol,end = sa.seed_sol(tasks)
    print("final get:",sa.eval_sol(sol,tasks,end))
    return sol[:end]
    # TODO[chenyu]: After implement in sa.cpp, replace above code with below:
    sol, profit = _sa.solve(tasks)
    print(f"result: profit {profit}, solution len {len(sol)}")
    return sol

def dp_solver(tasks):
    n = len(tasks)
    w = 1440
    val = [[0 for _ in range(w+1)] for _ in range(n+1)]
    path = [[[] for _ in range(w+1)] for _ in range(n+1)]

    val[1][tasks[0].get_duration()] = tasks[0].get_max_benefit()
    path[1][tasks[0].get_duration()].append(1)
    for i in range(1,n):  #initialization
        for j in range(0,w+1):
            if tasks[i].get_duration() > j:
                val[i+1][j] = val[i][j]
                path[i+1][j] = path[i][j].copy()
            else:
                val[i+1][j] = val[i][j-tasks[i].get_duration()] + tasks[i].get_max_benefit()
                if val[i][j] > val[i+1][j]:
                    val[i+1][j] = val[i][j]
                    path[i+1][j] = path[i][j].copy()
                else:
                    path[i+1][j] = path[i][j].copy()
                    path[i+1][j].append(i+1)
    
    idx = -1
    max = 0
    for j in range(0,w):
        if val[n][j] > max:
            idx = j
            max = val[n][j]
    return path[n][idx]


# Here's an example of how to run your solver.
if __name__ == '__main__':
    # for folder in os.listdir('inputs'):
    #     if not folder.startswith('.'):
    #         for input_path in os.listdir("inputs/" + folder):
    #             output_path = 'outputs/' + folder+'/' + input_path[:-3] + '.out'
    #             print("now is:",output_path)
    #             tasks = read_input_file('inputs/'+ folder+'/'+input_path)
    #             output = solve(tasks)
    #             write_output_file(output_path, output)
    input_path = 'small-181.in'
    output_path = 'outputs/' + input_path[:-3] + '.out'
    print("now is:",output_path)
    tasks = read_input_file('inputs/'+ input_path)
    output = dp_solver(tasks)
    write_output_file(output_path, output)