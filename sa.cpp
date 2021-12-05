#include "sa.h"
#include <assert.h> 
#include <iostream>
#include <cmath>


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
            // // std::cout<<"line 30 "<<  sol.end <<"\n";     // if all task are not valid
            for (int k = sol.end; k < sol.end + n; k++ ){
            sol.sols[k] = residual_list[k-sol.end];
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

std::vector<int>  insert_task(Solution& sol, const std::vector<Task> tasks)
{
    std::vector<int> res = sol.sols;
    int idx1,idx2;

    if (sol.end < int(sol.sols.size()) && sol.end > 0)
    {
        idx1 = rand_int(0,sol.end-1);                       //here need to be -1 since we need index (n exclusive)
        idx2 = idx1 + rand_int(1,(sol.sols.size()-1)-idx1);
        if(res[idx1] > 100 || res[idx2]>100)
        {
            // std::cout<<"first 1:"<<res[idx1]<<" 2:"<<res[idx2]<<std::endl;
            // std::cout<<"idx1:"<<idx1<<" idx2:"<<idx2<<std::endl;
        }
        std::swap(res[idx1],res[idx2]);
    }
    else
    {
        idx1 = rand_int(0,sol.end-1);
        idx2 = rand_int(1,sol.end-1);

        if(idx1 == idx2){
            idx2 = 0;
        }
        if(res[idx1] > 100 || res[idx2]>100)
        {
            // std::cout<<"sol end:"<<sol.end<<std::endl;
            // std::cout<<"second case 1:"<<res[idx1]<<" 2:"<<res[idx2]<<std::endl;
            // std::cout<<"idx1:"<<idx1<<" idx2:"<<idx2<<std::endl;
        }
        std::swap(res[idx1],res[idx2]);
    }
    return res;
}

std::vector<int>  reverse_task(std::vector<int>& res,int end, const std::vector<Task> tasks)
{
    int curr_time = 0;
    int idx1=0,idx2;
    int max1 = 0, max2 = 0;
    int checkpoint_time = 0;

    for(int i=0;i<end-1;i++)
    {
        if(tasks[res[i]].deadline - curr_time - tasks[res[i]].duration > max1 && rand_01()>0.6)  //may be delayed
        {
            max1 = tasks[res[i]].deadline - curr_time + tasks[res[i]].duration;
            idx1 = i;
            checkpoint_time = curr_time;
        }
        if( curr_time>=1440)
            break;
    }
    curr_time = checkpoint_time;
    idx2 = idx1 + 1;
    for(int i=idx1;i<end;i++)
    {
        if( curr_time + tasks[res[i]].duration - tasks[res[i]].deadline > max2)  //may be pre
        {
            max2 = curr_time + tasks[res[i]].duration - tasks[res[i]].deadline;
            idx2 = i;
        }
        if( curr_time>=1440)
            break;
    }

    std::swap(res[idx1],res[idx2]);
    // // std::cout<<"1:"<<idx1<<" 2:"<<idx2<<std::endl;
    return res;
}

std::vector<int>  trans_sol(Solution& sol, const std::vector<Task> tasks) 
{    
    std::vector<int> res = sol.sols;
    int operation;
    int end = sol.end != -1 ? sol.end : res.size();
    int repeat_times;

    operation = rand_int(0,4);
    //operation = 0;

    if(operation < 2)
        return insert_task(sol,tasks);
    else if (operation < 4)
    {
        repeat_times = rand_int(1,5);
        for(int j=0;j<repeat_times;j++)
            res = reverse_task(res,end,tasks);
        return res;
    }
    else
    {
        int idx1,idx2;
        //// std::cout<<"here"<<std::endl;

        idx1 = rand_int(0,end-1);                       //here need to be -1 since we need index (n exclusive)
        idx2 = rand_int(1,end-1);
        if(idx1 == idx2)
        {
            idx2 = 0;
        }
        std::swap(res[idx1],res[idx2]);
        return res;
    }
}

float eval_sol(Solution& sol, const std::vector<Task>& tasks) {
    float curr_time = 0;
    float total_profit = 0;
    int i;

    for(std::vector<int>::iterator iter = sol.sols.begin();iter != sol.sols.end();iter++)
    {
        if(*iter > tasks.size() || *iter <= 0)
        {
            // std::cout<<"not valid solution"<<std::endl;
            return -1.0;
        }
        if(std::count(sol.sols.begin(),sol.sols.end(),*iter) > 1)       // cannot repeat tasks
        {
            // std::cout<<"not valid solution"<<std::endl;
            return -2.0;
        }
    }

    if (sol.end == -1){
        sol.end = sol.sols.size();
        for (i=0;i<sol.end;i++){
            if (tasks[sol.sols[i]-1].duration + curr_time >= 1440){
                i -= 1;
                break;
            }
            float exceed_time = tasks[sol.sols[i]-1].duration + curr_time - tasks[sol.sols[i]-1].deadline;
            Task tsk = tasks[sol.sols[i]-1];
            total_profit +=  tsk.get_late_benefit(exceed_time);
            curr_time += tasks[sol.sols[i]-1].duration;
        }
    }

    else{
        for (i=0;i<sol.end;i++){
            float exceed_time = tasks[sol.sols[i]-1].duration + curr_time - tasks[sol.sols[i]-1].deadline;
            Task tsk = tasks[sol.sols[i]-1];
            // // std::cout<<"line 99"<<std::endl;
            total_profit +=  tsk.get_late_benefit(exceed_time);
            curr_time += tasks[sol.sols[i]-1].duration ;
            // // std::cout<<"line 102 "<< i <<std::endl;
        }
    }
    sol.end = i;
    return total_profit;
}

std::pair<Solution, float> SA_solve(const std::vector<Task>& tasks) {
    // auto sol = seed_sol(tasks);
    // trans_sol(sol, tasks);
    // float profit = eval_sol(sol, tasks);
    // return std::make_pair(sol, profit);
    
    int epochs = 100;
    int M = 1500;
    Solution solution;
    int n = tasks.size();
    for (int i = 0; i < n; i++){
        solution.sols.push_back(i+1); 
    }
    solution.end = n;
    std::random_shuffle ( solution.sols.begin(), solution.sols.end() );

    // // std::cout<<"the initial:"<<std::endl;
    // for (int i = 0; i < n; i++){
    //     // std::cout<<solution.sols[i]<<std::endl;
    // }
   
    std::vector<float> temperature_list;
    float initial_acceptance = 0.4;
    float profit = eval_sol(solution,tasks);
    for (int i=0;i<10;i++){
        std::vector<int>  next_sol  = trans_sol(solution,tasks);
        Solution Next_sol;
        Next_sol.sols = next_sol;
        Next_sol.end = solution.end;
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
                float r = rand_01();
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
            temperature_list.push_back(mean(temperature_list)/flag);
        }
    }
    return std::pair<Solution, float>(solution,profit);
}