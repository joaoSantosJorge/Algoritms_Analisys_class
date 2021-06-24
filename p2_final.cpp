
//g++ -std=c++11 -O3 -Wall p2.cpp -lm

//vector with index

#include <cstdio>
#include <iostream>
#include <vector>
#include <chrono>


typedef struct Connection {
    int vertex;
    int cap;
    int flux;
} Connection;

std::vector<std::vector<Connection>> graph;

std::vector<int> color;

void readInput() {
    int n, m;

    int j = scanf("%d %d", &n, &m);
    if(j < 0)
        printf("error\n");
    

    graph = std::vector<std::vector<Connection>>(n+2, std::vector<Connection>());
    
    int x, y, z;
    for (int i = 0; i < n; i++) {
        j = scanf("%i %i", &x, &y);
        
        //vertex source and sink
        Connection conn_1;
        Connection conn_2;
        conn_1.vertex = i+2;
        conn_1.cap = x;
        conn_1.flux = 0;
        conn_2.vertex = i+2;
        conn_2.cap = 0;
        conn_2.flux = 0;

        graph[0].push_back(conn_1);
        graph[1].push_back(conn_2);

        //residual path of vertex 2 to n. Initial capacity = 0;
        Connection conn_1_b;
        Connection conn_2_b;
        conn_1_b.vertex = 0;
        conn_1_b.cap = 0;
        conn_1_b.flux = 0;
        conn_2_b.vertex = 1;
        conn_2_b.cap = y;
        conn_2_b.flux = 0;

        graph[i+2].push_back(conn_1_b);
        graph[i+2].push_back(conn_2_b);
    }

    for (int i = 0; i < m; i++) {
        j = scanf("%i %i %i", &x, &y, &z);

        Connection conn_1;
        Connection conn_2;

        conn_1.vertex = x+1;
        conn_1.cap = z;
        conn_1.flux = 0;
        conn_2.vertex = y+1;
        conn_2.cap = z;
        conn_2.flux = 0;

        graph[y+1].push_back(conn_1);
        graph[x+1].push_back(conn_2);
        
    }

}

void printGraph() {
    for (int i = 0; i < (int) graph.size(); i++) {
        printf("%d: ", i);
        for (int j = 0; j < (int) graph[i].size(); j++) {
            printf("[%d %d %d] ", graph[i][j].vertex, graph[i][j].cap, graph[i][j].flux);
        }
        printf("\n");
    }
    printf("\n");
}

void dfs_visit(int v, int pos, std::vector<int> *path_aux, std::vector<int> *index_aux) {

    if (color[1] == 1)
        return;

    color[v]++;
    (*path_aux).push_back(v);
    (*index_aux).push_back(pos);
    for (int i = 0; i < (int) graph[v].size(); i++) {
        if (graph[v][i].vertex == 1 && (graph[v][i].cap - graph[v][i].flux) > 0){
            color[graph[v][i].vertex] = 1;
            (*path_aux).push_back(graph[v][i].vertex);
            (*index_aux).push_back(i);
            break;
            }
        else if ((color[graph[v][i].vertex] == 0) && (graph[v][i].cap > 0) && (graph[v][i].cap - graph[v][i].flux) > 0) {
            dfs_visit(graph[v][i].vertex, i, *(&path_aux), *(&index_aux));
        }
    }
    color[v]++;

    while (color[(*path_aux).back()] > 1) {
        (*path_aux).pop_back();
        (*index_aux).pop_back();
    }

}


void compute_path(std::vector<int> *path, std::vector<int> *index) {

    for (int i = 0; i < (int) color.size(); i++) {
        color[i] = 0;
    }

    color[0] = 1;
    (*path).push_back(0);
    for (int i = 0; i < (int) graph[0].size(); i++) {
        if (color[1] == 1)
            break;
        else if ((graph[0][i].cap - graph[0][i].flux)  > 0) {
            dfs_visit(graph[0][i].vertex, i, *(&path), *(&index));
            }        
    }

    if ((*path).back() != 1) {
        (*path).clear();
    }

}

void compute_flow(std::vector<int> *path, std::vector<int> *index) {
    int max_flux = 1000;



    for (int i = 0; i < (int) (*path).size()-1; i++) {
        if ((graph[(*path)[i]][(*index)[i]].cap - graph[(*path)[i]][(*index)[i]].flux) < max_flux)
            max_flux = graph[(*path)[i]][(*index)[i]].cap - graph[(*path)[i]][(*index)[i]].flux;
    }

    //printf("max_flux: %d\n", max_flux);

    for (int i = 0; i < (int) (*path).size()-1; i++) {
        graph[(*path)[i]][(*index)[i]].flux +=max_flux;
    }


}

void ford_fulkerson() {

    while (true) {
        std::vector<int> path;
        std::vector<int> index;
        
        compute_path(&path, &index);

        //return max_flow
        if (path.size() < 1)
            return;

        compute_flow(&path, &index);

    }

}

int compute_flux_max() {
    int flux_max = 0;

    for (int i = 0; i < (int) graph[0].size(); i++)
        flux_max += graph[0][i].flux;
    
    return flux_max;
}


int main() {

    auto start = std::chrono::high_resolution_clock::now();

    readInput();

    //printGraph();

    //initialize color
    for (int i = 0; i < (int) graph.size(); i++) {
        color.push_back(0);
    }
    
    ford_fulkerson();
    //printGraph();
    


    int flux_max = compute_flux_max();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    printf("%d\n", flux_max);
    std::cout << duration.count() << std::endl;

    return 0;
}
