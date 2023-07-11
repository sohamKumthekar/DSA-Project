#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define PRTCNT 15

typedef struct Particle
{ /* Particle Node */
    long double mass;
    long double vel[3];
    long double pos[3];
} Particle;

typedef struct BarnesHut_node
{
    long double mass;
    long double com_pos[3];
    long double com_vel[3];
} BarnesHut_node;
typedef struct OctreeNode
{ /* Octree Node */
    // void *cluster;
    BarnesHut_node *bhn;
    struct OctreeNode *children[8];
    // float position[3];
    long double bound_top[3];
    long double bound_bottom[3];
    long double bound_mid[3];
    int elements;
} OctreeNode;

typedef struct system_node
{ /* System Node */
    Particle *p[PRTCNT];
    // long double acc_x[2];
    // long double acc_y[2];
    // long double acc_z[2];
    // long double force_x[2];
    // long double force_y[2];
    // long double force_z[2];
} system_node;

typedef struct BarnesHut
{
    OctreeNode *octree_root;
} BarnesHut;

// positions *par[PRTCNT];


system_node *createsystem(system_node *s1);
BarnesHut *BarnesHut_malloc(long double min_x, long double max_x, long double min_y, long double max_y, long double min_z, long double max_z);
void BarnesHut__free(OctreeNode *node);
void BarnesHut_free(BarnesHut *bh);
int BarnesHut_add(BarnesHut *BH, long double x, long double y, long double z, long double mass , long double velx ,long double vely ,long double velz) ;
void BarnesHut_Tree(OctreeNode *node);
void BarnesHut_make(BarnesHut *bh);
void BarnesHut_force(OctreeNode *node, system_node *s, BarnesHut_node *bhn, long double *fx, long double *fy, long double *fz);
void BarnesHut_getNewPos(BarnesHut *bh, system_node *s, long double x, long double y, long double z,long double velx,long double vely,long double velz, long double mass, long double *fx, long double *fy, long double *fz , int i);
OctreeNode *Octree_malloc_node(long double x1, long double x2, long double y1, long double y2, long double z1, long double z2);
int insert__Octree_node(OctreeNode *oct, BarnesHut_node *BHN, long double x, long double y, long double z);
int insert_Octree_node(OctreeNode *oct, BarnesHut_node *BHN, long double x, long double y, long double z);
void destroy_Octree(OctreeNode *oct);
void update(int value);
void display();
long double *value_update(BarnesHut_node *bhn1, BarnesHut_node *bhn2 , long double * fx , long double * fy , long double * fz);
void calculate_acc_velocity(BarnesHut_node *bhn2, long double *force[]);
// void initParticles();
