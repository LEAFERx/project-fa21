#include "sa.h"

Solution seed_sol(const std::vector<Task> tasks) {
    auto sol = Solution();
    // sol.sols = {1, 2, 3, 4};
    // sol.end = 2;
    // return sol;
    int n = tasks.size();
    float curr_time = 0;
    std::vector<int> residual_list;
    for (int i = 0; i < n; i++){
        residual_list[i] = i+1; 
    }
    for (int i = 0; i < n ; i++){
        float ratio = 0;
        int idx = -1;
        for (int j = 0 ; j < n - i; j++){
            if (tasks[residual_list[j]-1].duration + curr_time >=1440)
            continue;
            auto tsk = tasks[residual_list[j]-1];
            float exceed_time = tsk.duration + curr_time - tsk.deadline;
            float curr_ratio = tsk.get_late_benefit(exceed_time);
            if (curr_ratio > ratio){
                ratio = curr_ratio;
                idx = j;
            }
        }
        if (idx == -1){        // if all task are not valid
            for (int k = sol.end; k < sol.end + n; k++ ){
            sol.sols[k] = residual_list[k-sol.end];
            sol.end ++;
            }
            i -= 1;
            break;
        }

        else{
            curr_time += tasks[residual_list[idx]-1].duration;
            sol.sols[sol.end] = residual_list[idx];
            residual_list.erase(residual_list.begin()+idx-1);
        }

    }
    return sol;
}




void trans_sol(Solution& sol, const std::vector<Task> tasks) {
    return;
}

float eval_sol(Solution& sol, const std::vector<Task> tasks) {
    return 0;
}

std::pair<Solution, float> SA_solve(const std::vector<Task>& tasks) {
    auto sol = seed_sol(tasks);
    trans_sol(sol, tasks);
    float profit = eval_sol(sol, tasks);
    return std::make_pair(sol, profit);
}