#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#define BSIZE 1<<15

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

long readLong() 
{
	long d = 0L, x = 0L;
	char c;

	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(stdin)) return x;
			bsize = fread(buffer, 1, BSIZE, stdin);
		}
		c = buffer[bpos++];
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return x;
	}
	return -1;
}

unsigned long long int acc, min ;
unsigned long int V, E ; 
long int *edg1, *edg2, *A;
long int *T, *TC;
long int *C;
long int K, M;

struct Edge
{
    unsigned long int src, dest ;
    long int weight;
};

struct Graph
{
    struct Edge* edge;
};


struct subset
{
    long int ancestor;
    long int deg;
    unsigned long long int node;
};
//union find data stucture 
long int find(struct subset subsets[], long int i)
{
    if (subsets[i].ancestor != i)
        subsets[i].ancestor = find(subsets, subsets[i].ancestor);

    return subsets[i].ancestor;
}

void Union(struct subset subsets[],long int x,long int y)
{
    long int xroot = find(subsets, x);
    long int yroot = find(subsets, y);

    if (subsets[xroot].deg < subsets[yroot].deg){
        subsets[xroot].ancestor = yroot;
        subsets[yroot].node += subsets[xroot].node;
    }
    else if (subsets[xroot].deg > subsets[yroot].deg){
        subsets[yroot].ancestor = xroot;
        subsets[xroot].node += subsets[yroot].node;
    }
    else
    {
        subsets[yroot].ancestor = xroot;
        subsets[xroot].deg++;
        subsets[xroot].node += subsets[yroot].node;
    }
}

int comp(const void* a, const void* b)
{
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return (a1->weight - b1->weight);
}

int main()
{
	long i;
    int k;
    struct Graph* graph ;
   	V = readLong();
    K = readLong();
    M = readLong();
 
    E = M + K;

    T = (long int *) malloc(K * sizeof(long int));
    TC = (long int *) malloc(K * sizeof(long int));

    for (i = 0; i<K; i++) {
        T[i] = readLong();
        TC[i] = readLong();
    }

    edg1 = (long int *) malloc(E * sizeof(long int));
    edg2 = (long int *) malloc(E * sizeof(long int));
    A = (long int *) malloc(E * sizeof(long int));

    for (i = 0; i<M; i++) {
        edg1[i] = readLong();
        edg2[i] = readLong();
        A[i] = readLong();
    }

    k = M;
    for (i = 0; i<K; i++) {
        edg1[i+M] = T[i];
        edg2[i+M] = V+1;
        A[k] = TC[i];
        k++;
    }
	graph = (struct Graph*) malloc( sizeof(struct Graph) );

    graph->edge = (struct Edge*) malloc( E * sizeof( struct Edge ) );
    for (i = 0; i < (E); i++) {
		graph->edge[i].src = edg1[i];
		graph->edge[i].dest = edg2[i];
		graph->edge[i].weight = A[i];
    }
    long int x,y;
	long int weig;
    struct Edge next_edge ;
    int counter = 0;
    qsort(graph->edge, M, sizeof(graph->edge[0]), comp);
    struct subset *subsets = (struct subset*) malloc( (V+1) * sizeof(struct subset) );
    i=1;
	while (i<=V){
		subsets[i].ancestor = i;
        subsets[i].deg = 0;
        subsets[i].node = 1 ;
		i++;
	}
	
    i = 0 ; 
    while (counter < V-1)
    {
        next_edge = graph->edge[i++];
        x = find(subsets, next_edge.src);
        y = find(subsets, next_edge.dest);
        weig = next_edge.weight;
        if (x != y) {
            acc += weig;
            counter++;
            Union(subsets, x, y);
        }
    }
	min = acc;
    acc = 0;
    V++;
    counter = 0;
    qsort(graph->edge, E, sizeof(graph->edge[0]), comp);
    i=1;
	while (i<=V){
		subsets[i].ancestor = i;
        subsets[i].deg = 0;
        subsets[i].node = 1 ;
		i++;
	}
	
    i = 0 ; 
    while (counter < V-1)
    {
        next_edge = graph->edge[i++];
        x = find(subsets, next_edge.src);
        y = find(subsets, next_edge.dest);
        weig = next_edge.weight;
        if (x != y) {
            acc += weig;
            counter++;
            Union(subsets, x, y);
        }
    }
    if (acc < min) min = acc;
    printf("%llu\n",min);
    return 0 ;
}
