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
    sol,end = sa.SA(tasks)
    print("final get:",sa.eval_sol(sol,tasks,end))
    return sol[:end]



# Here's an example of how to run your solver.
if __name__ == '__main__':
    for folder in os.listdir('inputs'):
        if not folder.startswith('.'):
            for input_path in os.listdir("inputs/" + folder):
                output_path = 'outputs/' + folder+'/' + input_path[:-3] + '.out'
                print("now is:",output_path)
                tasks = read_input_file('inputs/'+ folder+'/'+input_path)
                output = solve(tasks)
                write_output_file(output_path, output)
    # input_path = 'large-112.in'
    # output_path = 'outputs/' + input_path[:-3] + '.out'
    # print("now is:",output_path)
    # tasks = read_input_file('inputs/'+ input_path)
    # output = solve(tasks)
    # write_output_file(output_path, output)