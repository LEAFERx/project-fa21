# distutils: language = c++

from typing import List
from Task import Task

from libcpp.vector cimport vector

from _sa cimport CTask, CSolution, SA_solve

def solve(tasks: List[Task]):
    cdef vector[CTask] _tasks = vector[CTask](len(tasks))
    for i, task in enumerate(tasks):
        _tasks[i].task_id = task.task_id
        _tasks[i].deadline = task.deadline
        _tasks[i].duration = task.duration
        _tasks[i].perfect_benefit = task.perfect_benefit
    result = SA_solve(_tasks)
    solution = result.first
    profit = result.second
    return solution.sols[:solution.end], profit