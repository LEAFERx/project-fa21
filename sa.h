#ifndef _SA_H
#define _SA_H

#include <vector>
#include <utility>
#include <cmath>
struct Task {
    int task_id;
    int deadline;
    int duration;
    float perfect_benefit;
    
    float get_late_benefit(float minutes_late){
        float m_minutes_late = std::max(float(0), minutes_late); 
        return  perfect_benefit * exp(-0.0170 * m_minutes_late);
    }

};

class Solution {
public:
    std::vector<int> sols;
    int end;
};

std::pair<Solution, float> SA_solve(const std::vector<Task>& tasks);

#endif