#ifndef _SA_H
#define _SA_H

#include <vector>
#include <utility>
#include <cmath>

#include <assert.h> 

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
    int end = -1;
};

std::pair<Solution, float> SA_solve(const std::vector<Task>& tasks);

int rand_int(int a, int b){
    return a + ( std::rand() % ( b - a + 1 ) );
}

float mean(std::vector<float> a){
     float return_value = 0.0;
        int n = a.size();
       
        for ( int i=0; i < n; i++)
        {
            return_value += a[i];
        }
       
        return ( return_value / n);
}

#endif
