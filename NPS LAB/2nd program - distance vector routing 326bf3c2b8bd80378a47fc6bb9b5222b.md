# 2nd program - distance vector routing

- sample input output
    
    ✅ Sample 1 (3 Nodes)
    🔹 Input
    Enter the no of nodes: 3
    Enter the matrix:
    0 2 7
    2 0 1
    7 1 0
    🔹 Output
    For router 1
    For node 1 via 1 distance is 0
    For node 2 via 2 distance is 2
    For node 3 via 2 distance is 3
    
    For router 2
    For node 1 via 1 distance is 2
    For node 2 via 2 distance is 0
    For node 3 via 3 distance is 1
    
    For router 3
    For node 1 via 2 distance is 3
    For node 2 via 2 distance is 1
    For node 3 via 3 distance is 0
    
    ✅ Sample 2 (4 Nodes)
    🔹 Input
    Enter the no of nodes: 4
    Enter the matrix:
    0 1 3 999
    1 0 1 4
    3 1 0 2
    999 4 2 0
    🔹 Output
    For router 1
    For node 1 via 1 distance is 0
    For node 2 via 2 distance is 1
    For node 3 via 2 distance is 2
    For node 4 via 2 distance is 5
    
    For router 2
    For node 1 via 1 distance is 1
    For node 2 via 2 distance is 0
    For node 3 via 3 distance is 1
    For node 4 via 3 distance is 3
    
    For router 3
    For node 1 via 2 distance is 2
    For node 2 via 2 distance is 1
    For node 3 via 3 distance is 0
    For node 4 via 4 distance is 2
    
    For router 4
    For node 1 via 3 distance is 5
    For node 2 via 3 distance is 3
    For node 3 via 3 distance is 2
    For node 4 via 4 distance is 0
    
    ✅ Sample 3 (5 Nodes)
    🔹 Input
    Enter the no of nodes: 5
    Enter the matrix:
    0 2 999 1 999
    2 0 3 2 999
    999 3 0 4 5
    1 2 4 0 3
    999 999 5 3 0
    🔹 Output
    For router 1
    For node 1 via 1 distance is 0
    For node 2 via 2 distance is 2
    For node 3 via 4 distance is 5
    For node 4 via 4 distance is 1
    For node 5 via 4 distance is 4
    
    For router 2
    For node 1 via 1 distance is 2
    For node 2 via 2 distance is 0
    For node 3 via 3 distance is 3
    For node 4 via 4 distance is 2
    For node 5 via 4 distance is 5
    
    For router 3
    For node 1 via 4 distance is 5
    For node 2 via 2 distance is 3
    For node 3 via 3 distance is 0
    For node 4 via 4 distance is 4
    For node 5 via 5 distance is 5
    
    For router 4
    For node 1 via 1 distance is 1
    For node 2 via 2 distance is 2
    For node 3 via 3 distance is 4
    For node 4 via 4 distance is 0
    For node 5 via 5 distance is 3
    
    For router 5
    For node 1 via 4 distance is 4
    For node 2 via 4 distance is 5
    For node 3 via 3 distance is 5
    For node 4 via 4 distance is 3
    For node 5 via 5 distance is 0
    

```c
#include <stdio.h>

struct node{
    unsigned front[20];
    unsigned dist[20];
}t[10];

int main()
{
    int costmat[20][20];
    int nodes,i,j,k,count=0;
    printf("\nEnter the no of nodes: ");
    scanf("%d",&nodes);
    printf("Enter the matrix:\n ");

    for(i=0;i<nodes;i++){
        for(j=0;j<nodes;j++){
            scanf("%d",&costmat[i][j]);
            costmat[i][i]=0;
            t[i].dist[j]=costmat[i][j];
            t[i].front[j]=j;
        }
    }

    // Relaxation
    do{
        count=0;
        for(i=0;i<nodes;i++)
            for(j=0;j<nodes;j++)
                for(k=0;k<nodes;k++)
                    if(t[i].dist[j]>costmat[i][k]+t[k].dist[j]){
                        t[i].dist[j] = costmat[i][k] + t[k].dist[j];
                        t[i].front[j]=k;
                        count++;
                    }
    }while(count!=0);

    for(i=0;i<nodes;i++){
        printf("\nFor router %d ",i+1);
        for(j=0;j<nodes;j++){
            printf("\nFor node %d via %d distance is %d",j+1,t[i].front[j]+1,t[i].dist[j]);
        }
    }
    printf("\n\n");
    
   
}
```

---

##