#include "structs.h"

OctreeNode *Octree_malloc_node(long double x1, long double x2, long double y1,long double y2, long double z1, long double z2)
{
    printf("----octree malloced----\n");
    OctreeNode *oct = (OctreeNode *)malloc(sizeof(OctreeNode));
    if (!oct)
    {
        return NULL;
    }
    oct->bhn = NULL;

    for (int i = 0; i < 8; i++)
    {
        oct->children[i] = NULL;
    }

    oct->bound_top[0] = x1 > x2 ? x1 : x2;
    oct->bound_top[1] = y1 > y2 ? y1 : y2;
    oct->bound_top[2] = z1 > z2 ? z1 : z2;

    oct->bound_bottom[0] = x1 < x2 ? x1 : x2;
    oct->bound_bottom[1] = y1 < y2 ? y1 : y2;
    oct->bound_bottom[2] = z1 < z2 ? z1 : z2;

    oct->bound_mid[0] = (x1 + x2) / 2;
    oct->bound_mid[1] = (y1 + y2) / 2;
    oct->bound_mid[2] = (z1 + z2) / 2;

    oct->elements = 0;
    return oct;
}

int insert__Octree_node(OctreeNode *oct, BarnesHut_node *BHN, long double x, long double y, long double z)
{   
    printf("insert -- octree\n");
    // Case 1: If unsuccessful malloc -
    if (!oct)
    {
        return 0;
    }
    // Case 2: If elements = 0, i.e., Number of leaf nodes of current node are 0 -
    if (oct->elements == 0)
    {
        // oct->bhn->com_pos[0] = x;
        // oct->bhn->com_pos[1] = y;
        // oct->bhn->com_pos[2] = z;
        oct->bhn = BHN;
    }
    // Else it may happen that there are more leaf nodes, so to handle that
    // we need to call the function again
    else
    {
        // Case A: There is one leaf node -
        if (oct->elements == 1)
        {
            printf("in ele 1\n");
            // Taking the inital node's position to reallocate it to another subnode
            insert_Octree_node(oct, oct->bhn, oct->bhn->com_pos[0], oct->bhn->com_pos[1], oct->bhn->com_pos[2]);
            // oct->bhn = BHN;
        }
        insert_Octree_node(oct, BHN, x, y, z);
    }
    (oct->elements)++;
    return (oct->elements);
}

int insert_Octree_node(OctreeNode *oct, BarnesHut_node *BHN, long double x, long double y, long double z)
{
    printf("inserting _ octree\n");
    int flag = 0; /* Cases for insertion */
    long double bot_x, bot_y, bot_z;
    long double top_x, top_y, top_z;
    if (x >= oct->bound_mid[0])
    {
        flag += 1;
        bot_x = oct->bound_mid[0];
        top_x = oct->bound_top[0];
    }
    else
    {
        top_x = oct->bound_mid[0];
        bot_x = oct->bound_bottom[0];
    }
    if (y >= oct->bound_mid[1])
    {
        flag += 2;
        bot_y = oct->bound_mid[1];
        top_y = oct->bound_top[1];
    }
    else
    {
        top_y = oct->bound_mid[1];
        bot_y = oct->bound_bottom[1];
    }
    if (z >= oct->bound_mid[2])
    {
        flag += 4;
        bot_z = oct->bound_mid[2];
        top_z = oct->bound_top[2];
    }
    else
    {
        top_z = oct->bound_mid[2];
        bot_z = oct->bound_bottom[2];
    }
    if (!oct->children[flag])
    {
        //allocating space to new node
        printf("flag = %d\n" , flag);
        oct->children[flag] = Octree_malloc_node(bot_x, top_x, bot_y, top_y, bot_z, top_z);
    }
    // Verifies successful malloc as well
    return insert__Octree_node(oct->children[flag], BHN, BHN->com_pos[0], BHN->com_pos[1], BHN->com_pos[2]);
}

void destroy_Octree(OctreeNode *oct)
{
    if (!oct)
    {
        return;
    }
    free(oct->children[2]);
    free(oct->children[1]);
    free(oct->children[0]);
    free(oct->children[3]);
    free(oct->children[4]);
    free(oct->children[5]);
    free(oct->children[7]);
    free(oct->children[6]);
    free(oct);
}