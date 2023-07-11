#include <GL/glut.h>
#include "structs.h"


long double bound_min_x = -10000000000;
long double bound_min_y = -10000000000;
long double bound_min_z = 10000000000;
long double bound_max_x = 10000000000;
long double bound_max_y = 10000000000;
long double bound_max_z = 10000000000;

long double force_x[PRTCNT] ;
long double force_y[PRTCNT] ;
long double force_z[PRTCNT] ;

system_node *s1;


// void drawGrid(int size, int step)
// {
//   glLineWidth(1.0); // set line width
//   glBegin(GL_LINES);
//   glColor3f(1, 1, 1); // set color
//   // draw lines along x-axis
//   for (int i = -size; i <= size; i += step)
//   {
//     glVertex3f(i, -size, 0);
//     glVertex3f(i, size, 0);
//   }
//   // draw lines along z-axis
//   for (int i = -size; i <= size; i += step)
//   {
//     glVertex3f(-size, i, 0);
//     glVertex3f(size, i, 0);
//   }
//   glEnd();
// }

void display()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100, 0.5, 0.1, 100000000);
  gluLookAt(0, 0, 4884000.0, 0, 0, 0, 1.0, 0, 0);

  // draw objects
  // drawGrid(10000000, 800000);
  //  glColor3f(0.5, 0.5, 0.5);
  // glBegin(GL_LINES);
  // for (int i = -10; i <= 10; i++) {
  //  printf("Drawing line %d\n", i); // Debug output
  // glVertex3f(i, -10, 0);
  //      glVertex3f(i, 10, 0);
  //    glVertex3f(-10, i, 0);
  //  glVertex3f(10, i, 0);
  //   }
  //   glEnd();

  for (int i = 0; i < PRTCNT; i++)
  {
    glPushMatrix();
    glTranslatef(s1->p[i]->pos[0], s1->p[i]->pos[1], s1->p[i]->pos[2]);
    // glutSolidCube(500000);
    
    glutSolidSphere(50000 , 200, 20);
    glPopMatrix();

  }

  glutSwapBuffers();
}

void update(int value)
{
  printf("----updating----\n");
  BarnesHut *bh = BarnesHut_malloc(bound_min_x, bound_max_x, bound_min_y, bound_max_y, bound_min_z, bound_max_z);

  for (int i = 0; i < PRTCNT; i++)
  {
    printf("---calling barnes hut AND---\n");
    BarnesHut_add(bh, s1->p[i]->pos[0], s1->p[i]->pos[1], s1->p[i]->pos[2], s1->p[i]->mass , s1->p[i]->vel[0] , s1->p[i]->vel[1] ,s1->p[i]->vel[2] );
  }
    
  BarnesHut_make(bh);


  for (int i = 0; i < PRTCNT; i++)
  {
    BarnesHut_getNewPos(bh, s1, s1->p[i]->pos[0], s1->p[i]->pos[1], s1->p[i]->pos[2], s1->p[i]->vel[0], s1->p[i]->vel[1], s1->p[i]->vel[2],s1->p[i]->mass, &force_x[i], &force_y[i], &force_z[i] , i);
  }
  BarnesHut_free(bh);


  for (int i = 0; i < PRTCNT; i++)
  {
    printf("updated position : %Lf, %Lf, %Lf\n",s1->p[i]->pos[0] , s1->p[i]->pos[1] , s1->p[i]->pos[2]);
  }

  // Call display function to redraw the cube
  
  glutPostRedisplay();

  // Call update function again after 10 milliseconds
  glutTimerFunc(10, update, 0);
}

int main(int argc, char **argv)
{

  s1 = createsystem(s1);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Gravity Simulation");
  glutDisplayFunc(display);
  glutTimerFunc(0, update, 0);

  glutMainLoop();
  return 0;
}
