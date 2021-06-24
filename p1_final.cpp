//g++ -std=c++11 -O3 -Wall p1.cpp -lm

#include <cstdio>
#include <iostream>
#include <vector>



std::vector<std::vector<int>> graph;
std::vector<int> sources;

std::vector<int> stack;
std::vector<int> color;

std::vector<int> topological_order;
std::vector<int> cost;



void printGraph() {
    int line = 0;
    std::cout << std::endl;
    for (std::vector<int> list : graph) {
        
        std::cout << line << ": "; 
        for (int i : list) {
            std::cout << i << " ";
        }

        std::cout << std::endl;
        ++line;
    }

    std::cout << std::endl;
}

void readInput() {
    int n, m;

    int j = scanf("%d %d", &n, &m);
    if(j < 0)
        printf("error\n");
    

    graph = std::vector<std::vector<int>>(n, std::vector<int>());

    int x, y;
    for (int i = 0; i < m; i++) {
        j = scanf("%i %i", &x, &y);
        graph[--x].push_back(--y);
    }


}

void computeSources() {
    std::vector<int> tmp;

    for (int m = 0; m < (int)graph.size(); m++) {
        tmp.push_back(0);
    }

    
    for (int i = 0; i < (int) graph.size(); i++){
        for (int j = 0; j < (int) graph[i].size(); j++) {
            tmp[graph[i][j]] = 1;
        }
    }

    
    for(int k = 0; k < (int)graph.size(); k++) {
        if (tmp[k] == 0)
            sources.push_back(k);
    }
}



void depth_first_search() {

    for (int i = 0; i < (int) graph.size(); i++)
        color.push_back(0);
    
    for (int i = 0; i < (int)sources.size(); i++)
        stack.push_back(sources[i]);

    bool check = false;

    while (!stack.empty()) {
        
        if (color[stack.back()] == 2) {
            stack.pop_back();
        }
        
        else {
            color[stack.back()] = 1;
            check = false;

            if ((int) graph[stack.back()].size() == 0) {
                check = true;
                
            }

            for (int i = 0; i < (int)graph[stack.back()].size(); i++) {
                if (color[graph[stack.back()][i]] != 2)
                    break;
                else if (i+1 == (int)graph[stack.back()].size()) {
                    if (color[graph[stack.back()][i]] == 2) {
                        check = true;
                        
                    }
                }

            }

            if (check) {
                color[stack.back()] = 2;
                topological_order.push_back(stack.back());
                stack.pop_back();
            }

            for (int i = 0; i < (int)graph[stack.back()].size(); i++) {
                if (color[graph[stack.back()][i]] != 2)
                    stack.push_back(graph[stack.back()][i]);
            }
        }
    }



    
}

int sort_topological_order() {
    int max_cost = 1;

    for (int i = 0; i < (int)graph.size(); i++)
        cost.push_back(1);

    for (int i = topological_order.size() -1 ; i >= 0; i--) {
        for (int j = 0; j < (int)graph[topological_order[i]].size(); j++) {
            if (cost[graph[topological_order[i]][j]] < cost[topological_order[i]] + 1) {
                cost[graph[topological_order[i]][j]] = cost[topological_order[i]] + 1;
                if (cost[graph[topological_order[i]][j]] > max_cost)
                    max_cost = cost[graph[topological_order[i]][j]];
            }
        }

    }
    

    return max_cost;
}




int main() {
    readInput();
    
    computeSources();
    
    //printGraph();

    depth_first_search();
    
    printf("%d %d\n", (int) sources.size(), sort_topological_order());




    return 0;
}