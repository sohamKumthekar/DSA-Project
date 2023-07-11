#include "structs.h"

BarnesHut *BarnesHut_malloc(long double min_x, long double max_x, long double min_y, long double max_y, long double min_z, long double max_z)
{   
    printf("---Barnes hut mallocing---\n");
    BarnesHut *bh = (BarnesHut *)malloc(sizeof(BarnesHut));
    if (!bh)
        return NULL;
    bh->octree_root = Octree_malloc_node(min_x, max_x, min_y, max_y, min_z, max_z);
    if (!bh->octree_root)
    {
        free(bh);
        return NULL;
    }
    return bh;
}

void BarnesHut__free(OctreeNode *node)
{
    if (!node)
        return;
    free(node->bhn);
    BarnesHut__free(node->children[0]);
    BarnesHut__free(node->children[1]);
    BarnesHut__free(node->children[2]);
    BarnesHut__free(node->children[3]);
    BarnesHut__free(node->children[4]);
    BarnesHut__free(node->children[5]);
    BarnesHut__free(node->children[6]);
    BarnesHut__free(node->children[7]);
}
void BarnesHut_free(BarnesHut *bh)
{
    BarnesHut__free(bh->octree_root);
    destroy_Octree(bh->octree_root);
    free(bh);
}

int BarnesHut_add(BarnesHut *BH, long double x, long double y, long double z, long double mass , long double velx ,long double vely ,long double velz )
{
    printf("barneshut add\n");
    BarnesHut_node *temp = (BarnesHut_node *)malloc(sizeof(BarnesHut_node));
    if (!temp)
        return 0;

    temp->mass = mass;
    temp->com_pos[0] = x;
    temp->com_pos[1] = y;
    temp->com_pos[2] = z;
    temp->com_vel[0] = velx;
    temp->com_vel[1] = vely;
    temp->com_vel[2] = velz;

    insert_Octree_node(BH->octree_root, temp, x, y, z);

    return 1;
}

void BarnesHut_Tree(OctreeNode *node)
{
    printf("barnes hut tree\n");
    if (!node)
        return;
    if (node->elements == 1){
        return;}
    /* The mass and COM can be determined from the mass and COM of the node's
       children */
    else
    {
        /* The node is an empty non-leaf, so add the custom data to it */
        node->bhn = malloc(sizeof(BarnesHut_node));
        BarnesHut_node *pt = (BarnesHut_node *)(node->bhn);
        pt->mass = 0;
        pt->com_pos[0] = 0;
        pt->com_pos[1] = 0;
        pt->com_pos[2] = 0;
        pt->com_vel[0] = 0;
        pt->com_vel[1] = 0;
        pt->com_vel[2] = 0;
        for (int i = 0; i < 8; i++)
        {
            if (!node->children[i])
                continue;
            else{ 
                printf("////child exists///////\n");
            BarnesHut_Tree(node->children[i]);
            BarnesHut_node *child_pt = (BarnesHut_node *)node->children[i]->bhn;
            long double child_mass = child_pt->mass;
            pt->mass += child_mass;
            for (int j = 0; j < 3; j++)
            {   

                printf("body com = %Lf , %Lf , %Lf , %Lf\n" , child_pt->com_vel[0] ,child_pt->com_vel[1] ,child_pt->com_vel[2]  , child_mass);
                pt->com_pos[j] += child_mass * (child_pt->com_pos[j]);
                pt->com_vel[j] += child_mass * (child_pt->com_vel[j]);
            }
            }
        }
        for (int k = 0; k < 3; k++)
        {
            pt->com_pos[k] /= pt->mass;
            pt->com_vel[k] /= pt->mass;
        }
    }
    
}

void BarnesHut_make(BarnesHut *bh)
{   
    if (!bh) return;
    BarnesHut_Tree(bh->octree_root);
}

void BarnesHut_force(OctreeNode *node, system_node *s, BarnesHut_node *bhn, long double *fx, long double *fy, long double *fz)
{
    printf("barnes hut force\n");
    if (!node)
        return;
    BarnesHut_node node_bhn = *(BarnesHut_node *)(node->bhn); // this should give all the elements(cluster) not just 1 particle
    printf("particle in root  = %Lf, %Lf, %Lf\n" , node_bhn.com_pos[0],node_bhn.com_pos[1],node_bhn.com_pos[2]);

    long double radius = sqrtf(powf(node_bhn.com_pos[0] - bhn->com_pos[0], 2) + powf(node_bhn.com_pos[1] - bhn->com_pos[1], 2) + powf(node_bhn.com_pos[2] - bhn->com_pos[2], 2));
    printf("radius = %Lf\n" , radius);
    if (radius == 0)
        return;

    float width = ((node->bound_top[0] - node->bound_bottom[0]) + (node->bound_top[1] - node->bound_bottom[1]) + (node->bound_top[2] - node->bound_bottom[2])) / 3;
    printf("width = %f\n" ,width );
    if (width / radius < 0.5)
    {
        // call update function of integration here , no need to keep the updated force
        value_update(&node_bhn, bhn , fx , fy , fz);
        
        // *fx = *force_arr;
        // *fy = force_arr[1];
        // *fz = force_arr[2];
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            long double cfx = 0;
            long double cfy = 0;
            long double cfz = 0;
            if (node->children[i])
            {
                BarnesHut_force(node->children[i], s, bhn, &cfx, &cfy, &cfz);
                *fx += cfx;
                *fy += cfy;
                *fz += cfz;
            }
        }
    }
   
}

void BarnesHut_getNewPos(BarnesHut *bh, system_node *s, long double x, long double y, long double z, long double velx,long double vely,long double velz, long double mass, long double *fx, long double *fy, long double *fz , int i)
{
    printf("barnes hut new pos\n");
    if (!bh)
        return;
    BarnesHut_node bhn;
    bhn.mass = mass;
    bhn.com_pos[0] = x;
    bhn.com_pos[1] = y;
    bhn.com_pos[2] = z;
    bhn.com_vel[0] = velx;
    bhn.com_vel[1] = vely;
    bhn.com_vel[2] = velz;
    BarnesHut_force(bh->octree_root, s, &bhn, fx, fy, fz);
    long double *force_arr[3] = {fx, fy, fx};
    printf("pos = %Lf , %Lf , %Lf\n" , bhn.com_pos[0] , bhn.com_pos[1] , bhn.com_pos[2]);
    calculate_acc_velocity(&bhn, force_arr);

    for (int j = 0; j < 3; j++)
    {
        s->p[i]->pos[j] = bhn.com_pos[j];
        s->p[i]->vel[j] = bhn.com_vel[j];
    }
    

}
