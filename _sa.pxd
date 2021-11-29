# distutils: sources = sa.cpp
from libcpp.vector cimport vector
from libcpp.utility cimport pair

cdef extern from "sa.cpp":
    pass

cdef extern from "sa.h":
    cdef struct CTask "Task":
        int task_id
        int deadline
        int duration
        float perfect_benefit
    
    cdef cppclass CSolution "Solution":
        vector[int] sols
        int end

    pair[CSolution, float] SA_solve(const vector[CTask]& tasks)
