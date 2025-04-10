#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} NODE;

typedef struct Graph {
    int vertices;
    int *visited;
    struct Node **adjacency_lists;
} GPH;

/// parcurgere graf cu DFS/BFS

int dfs_first = 1;

NODE *create_node(int v) {
    NODE *new_node = malloc(sizeof(NODE));
    if (new_node == NULL) {
        printf("Eroare alocare memorie pentru nod!\n");
        exit(1);
    }
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

GPH *create_graph(int vertices) {
    GPH *graph = malloc(sizeof(GPH));
    if (graph == NULL) {
        printf("Eroare alocare memorie pentru graf!\n");
        exit(1);
    }
    graph->vertices = vertices;
    graph->adjacency_lists = malloc(vertices * sizeof(NODE *));
    graph->visited = malloc(vertices * sizeof(int));

    if (graph->adjacency_lists == NULL || graph->visited == NULL) {
        printf("Eroare alocare memorie pentru listele grafului!\n");
        exit(1);
    }

    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void add_edge(GPH *graph, int src, int dest) {
    if (src >= graph->vertices || dest >= graph->vertices || src < 0 || dest < 0) {
        printf("Indicele nodului este invalid!\n");
        return;
    }

    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

void insedg(int nr_of_vertices, int nr_of_edges, GPH *graph) {
    int src, dest;
    printf("Adauga %d muchii (noduri intre 0 si %d):\n", nr_of_edges, nr_of_vertices - 1);
    for (int i = 0; i < nr_of_edges; i++) {
        printf("Muchia %d: ", i + 1);
        scanf("%d %d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

/// bfs utils
int is_empty(NODE *queue) {
    return queue == NULL;
}

void enqueue(NODE **queue, int data) {
    NODE *new_node = create_node(data);
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        NODE *temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int dequeue(NODE **queue) {
    if (is_empty(*queue)) {
        printf("Coada este goala!\n");
        return -1;
    }
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

void print_graph(GPH *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        NODE *temp = graph->adjacency_lists[i];
        printf("%d: ", i);
        while (temp) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

void wipe_visited_list(GPH *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

// parcurgeri
void DFS(GPH *graph, int vertex_nr) {
    graph->visited[vertex_nr] = 1;

    if (dfs_first) {
        printf("%d", vertex_nr);
        dfs_first = 0;
    } else {
        printf("->%d", vertex_nr);
    }

    NODE *temp = graph->adjacency_lists[vertex_nr];
    while (temp != NULL) {
        int connected_vertex = temp->data;

        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

void BFS(GPH *graph, int start) {
    NODE *queue = NULL;

    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        NODE *temp = graph->adjacency_lists[current];
        while (temp) {
            int adj_vertex = temp->data;

            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

int main() {
    int nr_of_vertices, nr_of_edges, starting_vertex;

    printf("Cate noduri are graful?\n");
    scanf("%d", &nr_of_vertices);

    printf("Cate muchii are graful?\n");
    scanf("%d", &nr_of_edges);

    GPH *graph = create_graph(nr_of_vertices);

    insedg(nr_of_vertices, nr_of_edges, graph);

    printf("\nDe unde plecam in DFS?\n");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu DFS:\n");
    DFS(graph, starting_vertex);
    printf("\n");

    wipe_visited_list(graph);
    dfs_first = 1;

    printf("\nDe unde plecam in BFS?\n");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu BFS:\n");
    BFS(graph, starting_vertex);

    return 0;
}
