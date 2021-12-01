#include "sa.h"
#include <assert.h> 
#include <iostream>
#include <algorithm>

Solution seed_sol(const std::vector<Task> tasks) {
    auto sol = Solution();
    // sol.sols = {1, 2, 3, 4};
    // sol.end = 2;
    // return sol;
    int n = tasks.size();
    float curr_time = 0;
    std::vector<int> residual_list;
    for (int i = 0; i < n; i++){
        residual_list.push_back(i+1); 
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
        if (idx == -1){  
            std::cout<<"line 30 "<<  sol.end <<"\n";     // if all task are not valid
            for (int k = sol.end; k < sol.end + n; k++ ){
            sol.sols[k] = residual_list[k-sol.end];
            sol.end ;
            std::cout<<"line 34 "<<  sol.end <<"\n"; 
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

std::vector<int>  trans_sol(Solution& sol, const std::vector<Task> tasks) {
    std::vector<int> res = sol.sols;
    if (sol.end < sol.sols.size() && sol.end > 0){
        int idx1 = rand_int(0,sol.end);
        int idx2 = idx1 + rand_int(1,(sol.sols.size())-idx1);
        res[idx1],res[idx2] = res[idx2],res[idx1];
    }
    else{
        int idx1 = rand_int(0,sol.end);
        int idx2 = rand_int(0,sol.end);

        while(idx1 ==idx2){
            idx2 = rand_int(0,sol.end);
        }

        res[idx2] = res[idx1];
        res[idx1]  = res[idx2];
    }
    // #insert

    // reverse
    std::cout<<"line 70"<<std::endl;
    return res;
    
}

float eval_sol(Solution& sol, const std::vector<Task> tasks) {
    std::cout<<"line 76"<<std::endl;
    float curr_time = 0;
    float total_profit = 0;
    int i;
    if (sol.end == -1){
        sol.end = sol.sols.size();
        std::cout<<"line 85 "<<  sol.end <<"\n"; 
        assert (sol.end == tasks.size());  
        for (i=0;i<sol.end;i++){
            if (tasks[sol.sols[i]-1].duration + curr_time >= 1440){
                i -= 1;
                break;
            }
            float exceed_time = tasks[sol.sols[i]-1].duration + curr_time - tasks[sol.sols[i]-1].deadline;
            Task tsk = tasks[sol.sols[i]-1];
            total_profit +=  tsk.get_late_benefit(exceed_time);
            curr_time += tasks[sol.sols[i]-1].duration ;
        }
    }

    else{
        std::cout<<"line 95"<<std::endl;
        for (i=0;i<sol.end;i++){
            // std::cout<<i<<' '<< sol.sols[i]<<"\n";
        }
        for (i=0;i<sol.end;i++){
            float exceed_time = tasks[sol.sols[i]-1].duration + curr_time - tasks[sol.sols[i]-1].deadline;
            Task tsk = tasks[sol.sols[i]-1];
            // std::cout<<"line 99"<<std::endl;
            total_profit +=  tsk.get_late_benefit(exceed_time);
            curr_time += tasks[sol.sols[i]-1].duration ;
            // std::cout<<"line 102 "<< i <<std::endl;
        }
        // std::cout<<"line 105 "<< i <<std::endl;

    }
    sol.end = i;
    std::cout<<"line 106"<<std::endl;
    return total_profit;
}

std::pair<Solution, float> SA_solve(const std::vector<Task>& tasks) {
    // auto sol = seed_sol(tasks);
    // trans_sol(sol, tasks);
    // float profit = eval_sol(sol, tasks);
    // return std::make_pair(sol, profit);
    
    int epochs = 100;
    int M = 500;
    Solution solution;
    int n = tasks.size();
    for (int i = 0; i < n; i++){
        solution.sols.push_back(i+1); 
    }
    solution.end = n;
    std::random_shuffle ( solution.sols.begin(), solution.sols.end() );
   
    std::vector<float> temperature_list;
    float initial_acceptance = 0.4;
    std::cout<<"line 138 "<<  solution.end <<"\n"; 
    float profit = eval_sol(solution,tasks);
    std::cout<<"line 140 "<<  solution.end <<"\n"; 
    for (int i=0;i<10;i++){
        std::vector<int>  next_sol  = trans_sol(solution,tasks);
        Solution Next_sol;
        Next_sol.sols = next_sol;
        Next_sol.end = solution.end;
        std::cout<<"line 146 "<<  solution.end <<"\n"; 
        float next_profit = eval_sol(Next_sol,tasks);
        temperature_list.push_back(-abs(next_profit-profit)/log(initial_acceptance));

        if (rand_int(0,10)/10.0 < initial_acceptance){
            solution.sols = next_sol;            
            profit = next_profit;
            solution.end = Next_sol.end;
        }
    }
    
    for (int i=0; i < epochs; i++) {
        int flag = 0;
        for (int m = 0; m < M ;m++){
            std::vector<int> next_sol  = trans_sol(solution,tasks);

            Solution Next_sol;
            Next_sol.sols = next_sol;
            Next_sol.end = solution.end;
            float next_profit = eval_sol(Next_sol,tasks);

            if (next_profit > profit){
                solution.sols = next_sol ;           
                profit = next_profit;
                solution.end = Next_sol.end;
            }
            
            else{
                float r = rand_int(0,10)/10.0;
                float max = *std::max_element(temperature_list.begin(),temperature_list.end());
                if (r < exp((next_profit-profit)/max)){
                    flag += 1;
                    solution.sols = next_sol ;           
                    profit = next_profit;
                    solution.end = Next_sol.end;
                }
            }
        }
        if (flag > 0){
            // temperature_list = np.delete(temperature_list,temperature_list.argmax());    //#pop the max
            temperature_list.erase(std::max_element(temperature_list.begin(),temperature_list.end()));
            temperature_list .push_back(mean(temperature_list)/flag);
        }
    }

}