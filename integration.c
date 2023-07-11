#include <math.h>
#include "structs.h"
#define DELTA_T 1

long double G = 0.00000000067;

long double acc_x[2] = {0 , 0 };
long double acc_y[2] = {0 , 0 };
long double acc_z[2] = {0 , 0 };


void calculate_acc_velocity(BarnesHut_node *bhn2, long double * force[])
{
	printf("bhn2 mass = %Lf\n" , bhn2->mass);

	acc_x[1] = *force[0] / (bhn2->mass);
	printf("acc %d = %Lf , %Lf\n", 1, acc_x[1] , *force[0]);
	acc_y[1] = *force[1] / (bhn2->mass);
	printf("acc %d = %Lf , %Lf\n", 1, acc_y[1] , *force[1]);
	acc_z[1] = *force[2] / (bhn2->mass);
	printf("acc %d = %Lf , %Lf\n", 1, acc_z[1] , *force[2]);

	bhn2->com_vel[0] = bhn2->com_vel[0] + 0.5 * acc_x[1] * DELTA_T;
	bhn2->com_vel[1] = bhn2->com_vel[1] + 0.5 * acc_y[1] * DELTA_T;
	bhn2->com_vel[2] = bhn2->com_vel[2] + 0.5 * acc_z[1] * DELTA_T;

	return;
}

void calculate_position( BarnesHut_node *bhn2)
{	
	

	for (int i = 0; i < 3; i++)
	{
		bhn2->com_pos[i] +=  bhn2->com_vel[i] * DELTA_T;
		printf("position is calculated %Lf\n" , bhn2->com_pos[i]);
	}

	
}

float calculate_distance(long double pos1[], long double pos2[])
{

	long double x = pos1[0] - pos2[0];
	long double y = pos1[1] - pos2[1];
	long double z = pos1[2] - pos2[2];

	long double distance = sqrtf(x * x + y * y + z * z);

	return distance;
}

void calculate_force(BarnesHut_node *bhn1, BarnesHut_node *bhn2 , long double *force[])
{

		printf("the vel of bhn2 = %Lf , and the position = %Lf\n" , bhn2->com_vel[0] , bhn2->com_pos[0]);
	long double distance = calculate_distance(bhn1->com_pos, bhn2->com_pos);
	printf("distance = %Lf\n" , distance);
	long double r_vector[3];

	for (int i = 0; i < 3; i++)
	{
		r_vector[i] = bhn1->com_pos[i] - bhn2->com_pos[i];
	}

	long double mag_r_vector = sqrtl(r_vector[0] * r_vector[0] + r_vector[1] * r_vector[1] + r_vector[2] * r_vector[2]);

	long double force_vec = (G * bhn1->mass * bhn2->mass) / (long double)(pow(mag_r_vector, 3));

	printf("force_vec mag = %Lf\n" , force_vec);

	*force[0] = force_vec * r_vector[0];
	*force[1] = force_vec * r_vector[1];
	*force[2] = force_vec * r_vector[2];
	printf("force %d = %Lf\n", 1, *force[0]);
	printf("force %d = %Lf\n", 1, *force[1]);
	printf("force %d = %Lf\n", 1, *force[2]);
}

long double *value_update(BarnesHut_node *bhn1, BarnesHut_node *bhn2 , long double * fx , long double * fy , long double * fz)
{	
	long double * force[3];
	force[0] = fx;
	force[1] = fy;
	force[2] = fz;

	printf("printing all the info of bhn1 : %Lf , %Lf , %Lf, %Lf , %Lf , %Lf , %Lf\n", bhn1->mass , bhn1->com_pos[0], bhn1->com_pos[1], bhn1->com_pos[2], bhn1->com_vel[0], bhn1->com_vel[1],bhn1->com_vel[2]);
	printf("printing all the info of bhn2 : %Lf , %Lf , %Lf, %Lf , %Lf , %Lf , %Lf\n", bhn2->mass , bhn2->com_pos[0], bhn2->com_pos[1], bhn2->com_pos[2], bhn2->com_vel[0], bhn2->com_vel[1],bhn2->com_vel[2]);
	calculate_acc_velocity(bhn2, force);
	calculate_position( bhn2);
	calculate_force(bhn1, bhn2 , force);
	printf("force in integration : %Lf , %Lf , %Lf\n", *force[0] , *force[1] , *force[2]);
	// calculate_acc_velocity(bhn1 , bhn2);
	// return force;
}

// int main(){
// 	Particle p[0];
// 	p[0].mass = 6 * pow(10 , 24);
// 	p[0].pos[0] = 0;
// 	p[0].pos[1] = 0;
// 	p[0].pos[2] = 0;
// 	p[0].vel[0] = 0;
// 	p[0].vel[1] = 0;
// 	p[0].vel[2] = 0;
// 	Particle p[1];
// 	p[1].mass = 7.4 * pow(10 , 22);
// 	p[1].pos[0] = 3844000;
// 	p[1].pos[1] = 0;
// 	p[1].pos[2] = 0;
// 	p[1].vel[0] = -10;
// 	p[1].vel[1] = 108;
// 	p[1].vel[2] = 0;
// 	printf("mass 1 = %Lf , mass2 = %Lf , G = %.12Lf\n" , p[0].mass , p[1].mass , G );
// 	system_node s1;
// 	s1.p[0] = &p[0];
// 	s1.p[1] = &p[1];
// 	s1.acc[0] = s1.acc[1] = s1.acc[2] = s1.force[0] = s1.force[1] = s1.force[2] = 0;
// 	for(int i = 0 ; i < 100 ; i++){
// 		value_update(&s1);
// 		printf("%d th iteration\n" , i);
// 		printf("pos 0 = %f , pos 1 = %f , pos 2 = %f\n" , p[1].pos[0] , p[1].pos[1] , p[1].pos[3]);
// 		printf("vel 0 = %f , vel 1 = %f , vel 2 = %f\n" , p[1].vel[0] , p[1].vel[1] , p[1].vel[3]);
// 	}
// }
