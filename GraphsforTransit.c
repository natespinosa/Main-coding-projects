// This project uses graphs in order to estimate the most optical route from a place to another. It takes as input the "traffic" that exists
// between any node and analyzes it (among other constraints) to give the most optimal route. It also adds, updates and removes desired nodes 
// (or stops) accordingy. Among other functions. 
#define infinity 1000000

int get_idx(Graph * gr, char * node){
    int node_idx = 0;
    for (int i = 0; i < gr->count; i++){
        if (strcmp(gr->adj_list[i]->station, node) == 0){
            node_idx = i;
        }        
    }
    return node_idx;   
}

char **plan_route(Graph *gr, char *start, char *dest){
    //initializing the parameters
    int path_size = 1;
    char ** path = (char**)malloc(sizeof(char*)); // at this point, assume there is no path
    path[0] = (char*)calloc(MAX_LEN,sizeof(char));
    strcpy(path[0], dest);
    Enode * curr_Enode;
    Vnode * curr_Vnode, * next_Vnode;
    int min_dist = 0;

    if(gr->count == 0){
        return NULL;
    }

    //reseting the graph
    int count = 0;
    while(count < gr->count){
        gr->adj_list[count]->cost = infinity;
        gr->adj_list[count]->visited = 0;
        gr->adj_list[count]->prev = NULL;
        count ++;
    }
    

    gr->adj_list[get_idx(gr, start)]->cost = 0;
    curr_Vnode = gr->adj_list[get_idx(gr, start)];
    next_Vnode = gr->adj_list[get_idx(gr, start)];

    //Starting Dimamis algorithm

    int  i = 0;
    while (curr_Vnode->visited == 0){
        curr_Vnode->visited = 1;
        curr_Enode = curr_Vnode->edges;

        while(curr_Enode != NULL){
            next_Vnode = gr->adj_list[get_idx(gr, curr_Enode->vertex)];

            //check for the minium distance from prev node and update prev from station
            if(curr_Enode->weight + curr_Vnode->cost < next_Vnode->cost){
                next_Vnode->cost = curr_Enode->weight + curr_Vnode->cost;
                next_Vnode->prev = curr_Vnode;
            }

            curr_Enode = curr_Enode->next;
        }


        //reset to calculate the min distance
        curr_Vnode = gr->adj_list[0];
        min_dist = infinity;

        for(int i = 0; i < gr->count; i++){
            if(gr->adj_list[i]->visited == 0 && (min_dist > gr->adj_list[i]->cost)){
                min_dist = gr->adj_list[i]->cost;
                curr_Vnode = gr->adj_list[i];
            }
        }
    }
    //dfs is done

    Vnode * end_Node = gr->adj_list[get_idx(gr, dest)]->prev;
    Vnode * start_Node = gr->adj_list[get_idx(gr, start)];

    //constructing the path 
    while(end_Node != NULL){ 
        path_size++;
        path = realloc(path, path_size*sizeof(char*));
        path[path_size-1] = (char*)calloc(MAX_LEN, sizeof(char));     
        strcpy(path[path_size-1],end_Node->station); //pointer to duplicated string
        end_Node = end_Node->prev; 
    }
    if(path_size == 1) return NULL;
    return path;
}

void add(Graph *gr, char *station){

    //looping through the adj list to verify the existance of station
	for (int i = 0; i < gr->count; i++){
        if (strcmp(gr->adj_list[i]->station, station) == 0){
            return;
        }
    }

    //otherwise add another Vnode
    //first, make more room in the array
    Vnode * new_node;
    new_node = malloc(sizeof(Vnode));
    new_node->cost = 0;
    new_node->visited = 0;
    new_node->edges = NULL;
    strcpy(new_node->station, station);
    //add the node to the array

    gr->adj_list = realloc(gr->adj_list, (gr->count+1) * sizeof(Vnode*));
    gr->adj_list[gr->count] = new_node;
    gr -> count++;
    //I believe we don't need to change anything else
}

void update(Graph *gr, char *start, char *dest, int weight){
    
    //since add will check if they already exist, it doesn't matter if they do
    add(gr, start);
    add(gr, dest);

    //keeping track of starting index
    int start_idx, dest_idx, i = 0;
    Enode * temp_Enode, *prev_Enode;
    //looping through the adj list to find starting node
    //add more cases here, if start or dest don't exist !
    Vnode* start_node;
	for (i = 0; i < gr->count; i++){
        if (strcmp(gr->adj_list[i]->station, start) == 0){
            start_idx = i;
        }
        if (strcmp(gr->adj_list[i]->station, dest) == 0){
            dest_idx = i;
        }
    }
    temp_Enode = gr->adj_list[start_idx]->edges;
    prev_Enode = NULL;

  if (weight == 0){
      add(gr, start);
      add(gr, dest);
      while(temp_Enode != NULL){
          if(strcmp(temp_Enode->vertex, gr->adj_list[dest_idx]->station) == 0){
              temp_Enode->weight = weight;
              return;
          }
          prev_Enode = temp_Enode;
          temp_Enode = temp_Enode->next;
      }
    }

    else{
        //missing to add the edge !!
        int edge_exists = 0;
        while(temp_Enode != NULL){
            if(strcmp(temp_Enode->vertex, gr->adj_list[dest_idx]->station) == 0){
                edge_exists = 1;
                temp_Enode->weight = weight;
                return;
            }
            prev_Enode = temp_Enode;
            temp_Enode = temp_Enode->next;
        }
        if(!edge_exists) { 
          Enode* new = (Enode*)malloc(sizeof(Enode));
          new->weight = weight;
          new->next = gr->adj_list[start_idx]->edges;
          strcpy(new->vertex, dest);
          gr->adj_list[start_idx]->edges = new;
          return;
        }
    }


}

void disrupt(Graph *gr, char *station){

    Vnode * node_to_free;
    Enode * temp_Enode, * curr_Enode, *extra_Enode, *prev;

    //strcpy(node_to_free->station, station);
    // this should work
    if((strcmp(gr->adj_list[get_idx(gr, station)]->station, station)) != 0){
        return;
    } else {
        //set the variables to work with
        curr_Enode = gr->adj_list[get_idx(gr, station)]->edges;
        node_to_free = gr->adj_list[get_idx(gr, station)];

        //iterate through all the list to check which Vnodes are conected to that station
        //delete the edges directing to that station

        // go through elements beyond this node in the array and shift up
        for(int j = (get_idx(gr,station)); j < gr->count-1; j++){
            gr->adj_list[j] = gr->adj_list[j+1];
        }
        // delete all edges attached to the disrupted node
        while(curr_Enode != NULL) {
            temp_Enode = curr_Enode;
            curr_Enode = curr_Enode->next;
            free(temp_Enode);
        }
        // free the disrupted node and adjust the memory and variable size of the graph node list
        free(node_to_free);
        (gr->count)--;
        gr->adj_list = realloc(gr->adj_list, gr->count*sizeof(Vnode*));

        // go through all graph nodes and look at all edges to see if any point to the disrupted node
        int i = 0;
        while(i < gr->count){
            prev = NULL;
            curr_Enode = gr->adj_list[i]->edges;
            //now i will check if the vertex to each Edge matches with the deleated station
            while(curr_Enode != NULL){
                // If the node goes to the station, delete it
                if(strcmp(curr_Enode->vertex, station) == 0){
                    temp_Enode = curr_Enode;

                    // treat as deleting from the head of a linked list stack
                    if(prev == NULL) {gr->adj_list[i]->edges = curr_Enode->next;}
                    else { prev->next = curr_Enode->next; }
                    curr_Enode = curr_Enode->next;
                    free(temp_Enode);
                }
                else{
                    // if this edge points to a non-disrupted node, ignore it!
                    prev = curr_Enode;
                    curr_Enode = curr_Enode->next;
                }
            }
            i++;
        }
    }
}
