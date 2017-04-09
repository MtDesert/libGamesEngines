#include "TimeSlice.h"

TimeSlice::TimeSlice():timeSlice(0),minTimeSlice(1000){}

bool TimeSlice::addTimeSlice(uint usec){
	timeSlice += usec;
	if(timeSlice >= minTimeSlice){
		timeSlice -= minTimeSlice;
		consumeTimeSlice();
		return true;
	}
	return false;
}
void TimeSlice::consumeTimeSlice(){}