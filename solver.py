from parse import read_input_file, write_output_file, read_output_file
import Task
import os
import SA_solver as sa
from dp import dp_solver

import click

import _sa

@click.command()
@click.option('-s', '--solver',
    type=click.Choice(['sapy', 'dp', 'sa'], case_sensitive=False),
    default='sapy')
@click.option('-e', '--eval', default=False, is_flag=True)
@click.option('-f', '--force-replace', default=False, is_flag=True)
@click.argument('case')
def cli(solver, eval, force_replace, case):
    """CASE format are [s/m/l]+number, e.g. l1; all for solve all"""
    if case != "all":
        case_type = {"l": "large", "m": "medium", "s": "small"}
        input_path = f"inputs/{case_type[case[0]]}/{case_type[case[0]]}-{case[1:]}.in"
        output_path = f"outputs/{case_type[case[0]]}/{case_type[case[0]]}-{case[1:]}.out"
    else:
        input_path = None
        output_path = None
    
    if eval:
        if output_path == None:
            print("Provide a case name for eval")
            return
        tasks = read_input_file(input_path)
        sol = read_output_file(output_path)
        profit = sa.eval_sol(sol, tasks, len(sol))[0]
        print(f"Profit: {profit}")
        return
    
    if input_path == None:
        for folder in os.listdir('inputs'):
            if not folder.startswith('.'):
                for input_path in os.listdir("inputs/" + folder):
                    output_path = 'outputs/' + folder+'/' + input_path[:-3] + '.out'
                    print("now is:", output_path)
                    tasks = read_input_file('inputs/'+ folder+'/'+input_path)
                    if os.path.exists(output_path):
                        old_sol = read_output_file(output_path)
                        old_profit = sa.eval_sol(old_sol, tasks, len(old_sol))[0]
                    else:
                        old_profit = None
                    if solver == 'sapy':
                        sol, end = sa.SA(tasks)
                        profit = sa.eval_sol(sol, tasks, end)[0]
                        sol = sol[:end]
                    elif solver == 'dp':
                        sol = dp_solver(tasks)
                        profit = sa.eval_sol(sol, tasks, len(sol))[0]
                    else:
                        sol, profit = _sa.solve(tasks)
                    print(f"Get Profit: {profit}")
                    print(f"Old Profit: {old_profit}")
                    if old_profit < profit or force_replace:
                        print(f"Replace existing file {output_path}")
                        write_output_file(output_path, sol)
    else:
        tasks = read_input_file(input_path)
        if os.path.exists(output_path):
            old_sol = read_output_file(output_path)
            old_profit = sa.eval_sol(old_sol, tasks, len(old_sol))[0]
        else:
            old_profit = None
        if solver == 'sapy':
            sol, end = sa.SA(tasks)
            profit = sa.eval_sol(sol, tasks, end)[0]
            sol = sol[:end]
        elif solver == 'dp':
            sol = dp_solver(tasks)
            profit = sa.eval_sol(sol, tasks, len(sol))[0]
        else:
            sol, profit = _sa.solve(tasks)
            print("finish, the sol:",sol)
        print(f"Get Profit: {profit}")
        print(f"Old Profit: {old_profit}")
        if old_profit < profit or force_replace:
            print(f"Replace existing file {output_path}")
            write_output_file(output_path, sol)
            

def solve_sapy(tasks):
    """
    Args:
        tasks: list[Task], list of igloos to polish
    Returns:
        output: list of igloos in order of polishing  
    """
    sol,end = sa.seed_sol(tasks)
    print("final get:",sa.eval_sol(sol,tasks,end))
    return sol[:end]
    

def solve_sa(tasks):
    """
    Args:
        tasks: list[Task], list of igloos to polish
    Returns:
        output: list of igloos in order of polishing  
    """
    sol, profit = _sa.solve(tasks)
    print(f"result: profit {profit}, solution len {len(sol)}")
    return sol

# Here's an example of how to run your solver.
if __name__ == '__main__':
    cli()
