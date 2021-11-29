#ifndef _SA_H
#define _SA_H

#include <vector>
#include <utility>

struct Task {
    int task_id;
    int deadline;
    int duration;
    float perfect_benefit;
};

class Solution {
public:
    std::vector<int> sols;
    int end;
};

std::pair<Solution, float> SA_solve(const std::vector<Task>& tasks);

#endif