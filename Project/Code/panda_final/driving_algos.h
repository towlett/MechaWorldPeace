/****************************************************
 * 
 *	Driving Algorithms - Robot navigation
 *
 ****************************************************/

#ifndef DRIVE_ALGOS
#define DRIVE_ALGOS

void driving_algo(unsigned int distanceright, unsigned int distanceleft);
void drive_back_wall(void);
void find_side_wall(void);
void drive_side_wall(void);

//ULTRASOUND
unsigned int ultrasonicPing_left(void);
unsigned int ultrasonicPing_right(void);

//IR Finding
void FindIR(void);
void spin_reverse(void);

#endif