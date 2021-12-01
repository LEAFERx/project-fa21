def dp_solver(tasks):
    n = len(tasks)
    w = 1440
    val = [[0 for _ in range(w+1)] for _ in range(n+1)]
    path = [[[] for _ in range(w+1)] for _ in range(n+1)]

    val[1][tasks[0].get_duration()] = tasks[0].get_max_benefit()
    path[1][tasks[0].get_duration()].append(1)
    for j in range(tasks[0].get_duration()+1,w+1):
        val[1][j] = val[1][j-1]
        path[1][j] = path[1][j-1].copy()

    for i in range(1,n):  #initialization
        for j in range(0,w+1):
            if tasks[i].get_duration() > j:
                val[i+1][j] = val[i][j]
                path[i+1][j] = path[i][j].copy()
            else:
                val[i+1][j] = val[i][j-tasks[i].get_duration()] + tasks[i].get_max_benefit()
                if val[i][j] > val[i+1][j]:
                    val[i+1][j] = val[i][j]
                    path[i+1][j] = path[i][j].copy()
                else:
                    path[i+1][j] = path[i][j-tasks[i].get_duration()].copy()
                    path[i+1][j].append(i+1)
    
    idx = -1
    max = 0
    for j in range(0,w):
        if val[n][j] > max:
            idx = j
            max = val[n][j]
    print("max:",max)
    return path[n][idx]