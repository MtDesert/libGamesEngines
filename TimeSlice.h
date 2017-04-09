#ifndef TIMESLICE_H
#define TIMESLICE_H

#include"typedef.h"

/** TimeSlice 时间片
In the game, object's movement is controlled by the time slice
在游戏中，物体的运动是由时间来控制的
For example, constant speed object, the time-elapsed will determine how far the object moved
比如匀速移动的物体，时间逝去的量决定了这个物体运动了多远
If we wish a (class)object can control self(or other objects), we can inherit this class, and then rewrite consumeTimeSlice()
如果我们希望一个类对象能根据时间去控制自己(或者别的对象)，可以继承此类，然后重写consumeTimeSlice()
*/
class TimeSlice
{
public:
	TimeSlice();
	
	bool addTimeSlice(uint usec);//以毫秒为单位增加时间片,并且试图消耗时间片,返回是否消耗成功
protected:
	virtual void consumeTimeSlice();//消耗时间片,返回是否
	uint timeSlice;//目前的时间片
	uint minTimeSlice;//运动的最小时间片
};
#endif