// the code is referencing the dijkstra's algorithm from week 5

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "PQueue.h"


// #################    complexity  ###############    
// the highest complexity goes the main function, as m^2 * n^2, the breakdown for each function is listed above the function


#define VERY_HIGH_VALUE 999999


// printResult is the function to print out result
// #################    complexity  ###############
// complexity m*n (length of pred[]) (k<m*n)
void printResult(int start, int list[], char names[][32], char times[][5], int result) {
    
        if (list[start] <0){
            printf("%s ", times[start]);
            printf("%s\n", names[start]);
        }
        else if (list[start]>=0){
            // this result refers to the previous value, its checking if the adjacant node have the same str name, if so, print change at.
            result = start;
            printResult(list[start], list, names, times, result);
            if(strcmp(names[result],names[list[start]])==false){
                printf("Change at %s\n",names[start]);
            }
            printf("%s ", times[start]);
            printf("%s\n", names[start]);
            
            
        }
}




// algorithm dijkstra, refers to week 5 content. the difference is to pass the dictionary to printResult.
// #################    complexity  ###############
// complexity n*m * log(n*m)
// the maximum number of vertice would be m*n as we put everything one array, by using PQueue, it is O ( V + E l o g V ), V could be m*n
// and ElogV could get to n*m * log(n*m)
int dijkstraSSSP(Graph g, Vertex source, int dest, int print_flag, char names[][32], char times[][5]) {
    int result=-1;
    int dist[MAX_NODES];
    int pred[MAX_NODES];
    bool vSet[MAX_NODES];  // vSet[v] = true <=> v has not been processed
    int s;

    PQueueInit();
    int nV = numOfVertices(g);
    for (s = 0; s < nV; s++) {
        joinPQueue(s);
        dist[s] = VERY_HIGH_VALUE;
        pred[s] = -1;
        vSet[s] = true;
    }
    dist[source] = 0;

    
    while (PQueueIsEmpty() == false) {
        // find s∈vSet with minimum dist[s]
        s = leavePQueue(dist);
        for (int t = 0; t < nV; t++) {
            
            if (vSet[s] == true) {
                int weight = adjacent(g, s, t);
                if (weight > 0 && dist[s] + weight < dist[t]) {
                    dist[t] = dist[s] + weight;
                    pred[t] = s;
                }
            }
        }
        vSet[s] = false;
    }
    
    // if we see print_flag, the program print out stuff rather than returning distance, its written inside because we need pred[]
    if(print_flag==1){
        printResult(dest, pred, names, times, result);
        return 99;
    }
    else if (dist[dest] < VERY_HIGH_VALUE) {
        return dist[dest];
        // this is to return the current distance, for comparing later
        
    } 
    else {
        // it means no path found.
        return -1;
    }
}




// #################    complexity  ###############
// complexity m^2 * n^2
// details in program lines
int main(void) {
    Edge e;
    // n is the total vertices
    int n;
    printf("Enter the total number of stops on the network: ");
    scanf("%d", &n);
    // reading data
    char stop_names[n][32];

    for(int i=0; i<n; i++){
        scanf("%s",stop_names[i]);
    }
    printf("Enter the number of schedules: ");
    int m;
    scanf("%d",&m);
    // build up dictionary 
    char whole_names[n*m][32];
    char whole_times[n*m][5];
    // s_breaks stores the breakage between schedule, as we put all schedule in one array.
    int s_breaks[m];
    // the counter for the size of valid input total
    int cursor = 0;
    
    int from;
    int to;

    for (int i=0;i<m;i++){
        // stop in one schedule
        int n_s;
        printf("Enter the number of stops: ");
        scanf("%d",&n_s);
        s_breaks[i] = n_s + cursor;
        
        for (int j=0;j<n_s;j++){
            // store them in dictionaries
            scanf("%s",whole_times[cursor]);
            scanf("%s",whole_names[cursor]);
            cursor ++;
        }
    }

    int total_n = cursor;
    // now we build graph
    Graph g = newGraph(total_n+1);
    // #################    complexity  ###############
    // this part gives highest complexity as m^2 * n^2; as total_n could get high to m*n
    for (int i=0;i<total_n;i++){
        int flag = 1;
        for (int j=0;j<m;j++){
            if(i+1==s_breaks[j]){
                flag = 0;
            }
        }
        // if there is linkage between schedules, add edge.
        for(int k=0;k<total_n;k++){
            if(strcmp(whole_names[i],whole_names[k])==false){
                int temp_weight = atoi(whole_times[i]) - atoi(whole_times[k]);
                if(temp_weight>0){
                    e.v=k;
                    e.w=i;
                    e.weight=temp_weight;
                    insertEdge(g,e);
                }
            }
        }
        // when flag is 0, it means there is the breakage, thats between schedule, if flag ==1 , add edge.
        if (flag ==1){
            e.v = i;
            e.w = i+1;
            from = atoi(whole_times[i]);
            to = atoi(whole_times[i+1]);
            e.weight = ((to/100) - (from/100))*60 + ((to%100) - (from%100));
            insertEdge(g,e);
        }
    }
    
    // read data
    char start[32];
    char end[32];
    char stop_word [5] = "done";

    // in case of multiple feasible departure and destinations
    // store all possible start
    int multi_start [m];
    int start_count;
    // store all possible end
    int multi_end [m];
    int end_count;
    
    int depart;

    //while loop for reading data and process data
    printf("\n");
    printf("From: ");
    scanf("%s",&start);
    while(strcmp(start,stop_word)){
        printf("To: ");
        scanf("%s",&end);
        // program insert here
        // for multiple cases
        printf("Depart at: ");
        scanf("%d",&depart);

        start_count = 0;
        end_count = 0;
        // the action storing all possible start and end
        for(int i=0;i<total_n;i++){
            if(strcmp(start,whole_names[i])==0){
                multi_start[start_count] = i;
                start_count++;
            }
            else if(strcmp(end,whole_names[i])==0){
                multi_end[end_count] = i;
                end_count++;
            }
        }
        // create array of total cost for all possible route 
        int number_possible_route = start_count*end_count;

        int first_node_time;
        int to_the_first;
        // for setting the departure node as the last node in graph g
        int source = total_n;

        // get distance , start , end arrays.
        int distance_in_order [number_possible_route];
        int start_in_order [number_possible_route];
        int end_in_order [number_possible_route];
        int d_order_count = 0;
        int dest;
        int print_flag = 0;
        
        for(int i=0;i<start_count;i++){
                
                // build the final edge to the graph

            first_node_time = atoi(whole_times[multi_start[i]]);

            // add source as the final node.
            if(depart<=first_node_time){
                e.v = total_n;
                e.w = multi_start[i];
                to_the_first = depart;
                e.weight = ((first_node_time/100) - (to_the_first/100))*60 + ((first_node_time%100) - (to_the_first%100));
                insertEdge(g,e);
                // this part is to store diffrent data into the arrays of cost, start, end    
                for (int j=0;j<end_count;j++){
                    dest = multi_end[j];
                    distance_in_order[d_order_count]=dijkstraSSSP(g, source, dest, print_flag, whole_names, whole_times);
                    start_in_order[d_order_count]=multi_start[i];
                    end_in_order[d_order_count]=multi_end[j];
                    d_order_count++;
                }
                
                
                
                removeEdge(g,e);
                
            }
        }
        printf("\n");
        // get high values
        int temp_mini = 9999;
        int position = 9999;

        // to find the minimum cost (for stage 2), and if they are the same, keep the later one (for stage 3)
        for(int i=0;i<d_order_count;i++){
            if(-1<distance_in_order[i]<=temp_mini){
                if (distance_in_order[i]==temp_mini){
                    int previous =  atoi(whole_times[start_in_order[position]]);
                    int current = atoi(whole_times[start_in_order[i]]);
                    if(previous<current){
                        position = i;
                        temp_mini=distance_in_order[i];
                    }
                }
                else{
                    position = i;
                    temp_mini = distance_in_order[i];
                }
                

                
                
            }
        }

        // printing logics

        if(position==9999){
            printf("No connection found.");
            printf("\n");
        }
        else{
            print_flag=1;
            int final_start = start_in_order[position];
            int final_end = end_in_order[position];

            
            e.v = total_n;
            e.w = final_start;
            int final_depart = depart;
            int final_first = atoi(whole_times[final_start]);
            e.weight = ((final_first/100) - (final_depart/100))*60 + ((final_first%100) - (final_depart%100));
            insertEdge(g,e);
            
            
            
            int print_data = dijkstraSSSP(g, final_start, final_end, print_flag, whole_names, whole_times);
            removeEdge(g,e);
        }
        

        
        printf("\n");
        printf("From: ");
        scanf("%s",&start);
        
    }  
    
    printf("Thank you for using goNSW.");
    printf("\n");
    

    
    freeGraph(g);
    return 0;
}

