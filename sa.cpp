#include "sa.h"

Solution seed_sol(const std::vector<Task> tasks) {
    auto sol = Solution();
    sol.sols = {1, 2, 3, 4};
    sol.end = 2;
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