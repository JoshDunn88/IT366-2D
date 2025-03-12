#ifndef __TIME_H__
#define __TIME_H__

typedef struct Time_S
{
	//data 
	float	last_time; //previous frame time
	float	current_time; //current frame time
	float	delta_time; //time between last and current frame

}Time;

/*
 * @brief initialize time data
 * @return a Time object

*/
Time time_start();

/*
 * @brief get the time between current and last frame
 * @param the time keeper
 * @return the delta time in milliseconds

*/
float delta_time(Time time);
float current_time(Time time);

void time_update(Time* time);






#endif
