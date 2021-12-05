#ifndef _SA_H
#define _SA_H

#include <vector>
#include <utility>
#include <cmath>

#include <assert.h> 
#include <algorithm>
#include <random>
#include <numeric>


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
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(rng);
}

double rand_01(){
    static std::random_device rd;
    static std::default_random_engine rng {rd()};
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

float mean(std::vector<float>& a){    
    return std::accumulate(a.begin(), a.end(), 0) / a.size();
}

#endif
