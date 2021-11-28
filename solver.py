from parse import read_input_file, write_output_file
import os
import SA_solver as sa

def solve(tasks):
    """
    Args:
        tasks: list[Task], list of igloos to polish
    Returns:
        output: list of igloos in order of polishing  
    """
    sol = sa.seed_sol(tasks)
    revised_sol = []
    curr_time = 0
    for i in range(len(sol)):          #iter all task
        if tasks[sol[i]].get_duration() + curr_time >= 1440:
            break
        curr_time += tasks[sol[i]].get_duration()
        revised_sol.append(sol[i])

    return revised_sol



# Here's an example of how to run your solver.
if __name__ == '__main__':
    for input_path in os.listdir('inputs/'):
        output_path = 'outputs/' + input_path[:-3] + '.out'
        tasks = read_input_file('inputs/'+input_path)
        output = solve(tasks)
        write_output_file(output_path, output)