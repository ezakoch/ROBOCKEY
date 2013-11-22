/* Name: Localize.c
 * Author: Spyridon Karachalios
 */

#include "Localize.h"

#define USB 1               // If using the USB set to 1
#define F_M2 16000000       // Frequency of the M2


#define x0_offset 0
#define y0_offset 0
#define P_vertical 29


// Variables for Localization
int u0 = 512 - x0_offset, v0 = 384 - y0_offset;     // Center of camera optical axis

// Fixed stars in global coordinates
float P0[2] = {0, 0}, P1[2] = {0, 14.5}, P2[2] = {11.655, 8.741}, P3[2] = {0, -14.5}, P4[2] = {-10.563, 2.483};
int QA[2] = {0, 0}, QB[2] = {0, 0}, QC[2] = {0, 0}, QD[2] = {0, 0}, S0[2], S1[2] ,S2[2], S3[2], S4[2];
int *Q1 ,*Q2 ,*Q3, *Q4;
float AB = 0, AC = 0, AD = 0, BC = 0, BD = 0, CD = 0;
float distances[6] = {0}, radius, theta, alpha;


// --------------------------------------------------------------
// Localize the robot
// --------------------------------------------------------------

unsigned char localize(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, int* x_robot, int* y_robot, int* orientation){
    
    
    
    
    float max_distance = 0, min_distance = 10000;
    int max_id = 0, min_id = 0;
    
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
                    return 0;
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
                    return 0;
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
                    return 0;
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
                    return 0;
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
                    return 0;
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
                    return 0;
                    break;
            }
            break;
    }

    
    float scale = P_vertical/max_distance;
    S1[0] = Q1[0] * scale; S1[1] = Q1[1] * scale;
    S3[0] = Q3[0] * scale; S3[1] = Q3[1] * scale;
    
    S0[0] = 0.5*(S1[0]+S3[0]);
    S0[1] = 0.5*(S1[1]+S3[1]);
    radius = sqrt(S0[0]*S0[0] + S0[1]*S0[1]);
    
    
    theta = atan2( (Q3[1] - Q1[1]), (Q3[0] - Q1[0])  );
    alpha = -atan2(S0[0],S0[1]);
    
    *orientation = theta;
    
    *x_robot = -radius * cos(theta - alpha);
    *y_robot = -radius * sin(theta - alpha);
    
    
    return 1;
    
}
// --------------------------------------------------------------









