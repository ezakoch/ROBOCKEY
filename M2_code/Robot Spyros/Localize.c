/* Name: Localize.c
 * Author: Spyridon Karachalios
 */

#include "Localize.h"

//For 0 and 180 compute the mean of the camera pixels given and make the mean between them and added to the x0/y0 offset. Do it once, 
//get the mean and flip signs. The do it again and add values.
//Then for the offset in cm, after applying the offset explained before, we check the mean in cm of the position given in Matlab and subtract those values
#define x0_offset 81  // ROBOT SPYROS 115   ----   ALEX 64
#define y0_offset -94  // ROBOT SPYROS -31   ----   ALEX 16
#define P_vertical 29
#define x0_offset_cm -10 	// ROBOT SPYROS -9  ---- ALEX -8
#define y0_offset_cm 8		// ROBOT SPYROS 5 -----  ALEX 7


// Variables for Localization
int u0 = 512 - x0_offset, v0 = 384 - y0_offset;     // Center of camera optical axis

// Fixed stars in global coordinates
float P0[2] = {0, 0}, P1[2] = {0, 14.5}, P2[2] = {11.655, 8.741}, P3[2] = {0, -14.5}, P4[2] = {-10.563, 2.483};
int QA[2] = {0, 0}, QB[2] = {0, 0}, QC[2] = {0, 0}, QD[2] = {0, 0}, S0[2], S1[2] ,S2[2], S3[2], S4[2];
int *Q1 ,*Q2 ,*Q3, *Q4;
float AB = 0, AC = 0, AD = 0, BC = 0, BD = 0, CD = 0;
float distances[6] = {0}, radius, theta, theta2, alpha;
int orientation_prev = 0, x_robot_prev = 0, y_robot_prev = 0, orientation_current = 0, x_robot_current = 0, y_robot_current = 0;
float beta = 0.0;
int flag_no_good_stars = 0;


// --------------------------------------------------------------
// Localize the robot
// --------------------------------------------------------------

unsigned char localize(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, int* x_robot, int* y_robot, int* orientation, int* CAMERA_CENTER_X, int* CAMERA_CENTER_Y){
    
    float max_distance = 0, min_distance = 10000;
    int max_id = 0, min_id = 0;
    
    flag_no_good_stars = 0;
    
    if ((x1==1023)||(x2==1023)||(x3==1023)||(x4==1023)||(y1==1023)||(y2==1023)||(y3==1023)||(y4==1023)) {
        flag_no_good_stars = 1;
    }
    
    if (!flag_no_good_stars) {
        
        QA[0] = x1 - u0; QA[1] = y1 - v0;
        QB[0] = x2 - u0; QB[1] = y2 - v0;
        QC[0] = x3 - u0; QC[1] = y3 - v0;
        QD[0] = x4 - u0; QD[1] = y4 - v0;
        distances[0] = sqrt((QB[0] - QA[0])*(QB[0] - QA[0]) + (QB[1] - QA[1])*(QB[1] - QA[1]));
        distances[1] = sqrt((QC[0] - QA[0])*(QC[0] - QA[0]) + (QC[1] - QA[1])*(QC[1] - QA[1]));
        distances[2] = sqrt((QD[0] - QA[0])*(QD[0] - QA[0]) + (QD[1] - QA[1])*(QD[1] - QA[1]));
        distances[3] = sqrt((QC[0] - QB[0])*(QC[0] - QB[0]) + (QC[1] - QB[1])*(QC[1] - QB[1]));
        distances[4] = sqrt((QD[0] - QB[0])*(QD[0] - QB[0]) + (QD[1] - QB[1])*(QD[1] - QB[1]));
        distances[5] = sqrt((QD[0] - QC[0])*(QD[0] - QC[0]) + (QD[1] - QC[1])*(QD[1] - QC[1]));
        
        int i;
        for (i=0; i<6; i++) {
            if (distances[i] > max_distance) {
                max_distance = distances[i];
                max_id = i;
            }
            if (distances[i] < min_distance) {
                min_distance = distances[i];
                min_id = i;
            }
        }
        
        
        switch (max_id) {
            case 0:
                switch (min_id) {
                    case 1:
                        Q1=QA; Q2=QC; Q3=QB; Q4=QD;
                        break;
                    case 2:
                        Q1=QA; Q2=QD; Q3=QB; Q4=QC;
                        break;
                    case 3:
                        Q1=QB; Q2=QC; Q3=QA; Q4=QD;
                        break;
                    case 4:
                        Q1=QB; Q2=QD; Q3=QA; Q4=QC;
                        break;
                    default:
                        Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                        flag_no_good_stars = 1;
                        break;
                }
                break;
                
            case 1:
                switch (min_id) {
                    case 0:
                        Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                        break;
                    case 2:
                        Q1=QA; Q2=QD; Q3=QC; Q4=QB;
                        break;
                    case 3:
                        Q1=QC; Q2=QB; Q3=QA; Q4=QD;
                        break;
                    case 5:
                        Q1=QC; Q2=QD; Q3=QA; Q4=QB;
                        break;
                    default:
                        Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                        flag_no_good_stars = 1;
                        break;
                }
                break;
                
            case 2:
                switch (min_id) {
                    case 0:
                        Q1=QA; Q2=QB; Q3=QD; Q4=QC;
                        break;
                    case 1:
                        Q1=QA; Q2=QC; Q3=QD; Q4=QB;
                        break;
                    case 4:
                        Q1=QD; Q2=QB; Q3=QA; Q4=QC;
                        break;
                    case 5:
                        Q1=QD; Q2=QC; Q3=QA; Q4=QB;
                        break;
                    default:
                        Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                        flag_no_good_stars = 1;
                        break;
                }
                break;
                
            case 3:
                switch (min_id) {
                    case 0:
                        Q1=QB; Q2=QA; Q3=QC; Q4=QD;
                        break;
                    case 1:
                        Q1=QC; Q2=QA; Q3=QB; Q4=QD;
                        break;
                    case 4:
                        Q1=QB; Q2=QD; Q3=QC; Q4=QA;
                        break;
                    case 5:
                        Q1=QC; Q2=QD; Q3=QB; Q4=QA;
                        break;
                    default:
                        Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                        flag_no_good_stars = 1;
                        break;
                }
                break;
                
            case 4:
                switch (min_id) {
                    case 0:
                        Q1=QB; Q2=QA; Q3=QD; Q4=QC;
                        break;
                    case 2:
                        Q1=QD; Q2=QA; Q3=QB; Q4=QC;
                        break;
                    case 3:
                        Q1=QB; Q2=QC; Q3=QD; Q4=QA;
                        break;
                    case 5:
                        Q1=QD; Q2=QC; Q3=QB; Q4=QA;
                        break;
                    default:
                        Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                        flag_no_good_stars = 1;
                        break;
                }
                break;
                
                
            case 5:
                switch (min_id) {
                    case 1:
                        Q1=QC; Q2=QA; Q3=QD; Q4=QB;
                        break;
                    case 2:
                        Q1=QD; Q2=QA; Q3=QC; Q4=QB;
                        break;
                    case 3:
                        Q1=QC; Q2=QB; Q3=QD; Q4=QA;
                        break;
                    case 4:
                        Q1=QD; Q2=QB; Q3=QC; Q4=QA;
                        break;
                    default:
                        Q1=QA; Q2=QB; Q3=QC; Q4=QD;
                        flag_no_good_stars = 1;
                        break;
                }
                break;
        }
    }
    
    if (flag_no_good_stars) {
        *orientation = orientation_prev;
        *x_robot = x_robot_prev;
        *y_robot = y_robot_prev;
        return 0;
    }
    
    
    float scale = P_vertical/max_distance;
    S1[0] = Q1[0] * scale; S1[1] = Q1[1] * scale;
    S3[0] = Q3[0] * scale; S3[1] = Q3[1] * scale;
    
	*CAMERA_CENTER_X = (int)(0.5*(Q1[0]+Q3[0]));
	*CAMERA_CENTER_Y = (int)(0.5*(Q1[1]+Q3[1]));
    S0[0] = 0.5*(S1[0]+S3[0]);
    S0[1] = 0.5*(S1[1]+S3[1]);
    radius = sqrt(S0[0]*S0[0] + S0[1]*S0[1]);
    
    theta = atan2( (Q3[1] - Q1[1]), (Q3[0] - Q1[0])  );
	theta2 = atan2( -(Q3[1] - Q1[1]), -(Q3[0] - Q1[0])  );
    alpha = -atan2(S0[0],S0[1]);
    
    x_robot_current     = -radius * cos(theta - alpha)+x0_offset_cm;
    y_robot_current     = -radius * sin(theta - alpha)+y0_offset_cm;
    orientation_current = (theta2) * 180/M_PI;
	//if (orientation_current > 180)
		//orientation_current -= 360;
    
    *x_robot     = beta * x_robot_prev      +  (1-beta) * x_robot_current;
    *y_robot     = beta * y_robot_prev      +  (1-beta) * y_robot_current;
    *orientation = beta * orientation_prev  +  (1-beta) * orientation_current;
    
    x_robot_prev     = *x_robot;
    y_robot_prev     = *y_robot;
    orientation_prev = *orientation;
    
    return 1;
    
}
// --------------------------------------------------------------









