#ifndef ENGINE_ANDROID_H
#define ENGINE_ANDROID_H

#include <android/input.h>
#include <android/sensor.h>
#include "android_native_app_glue.h"

// Prepare to monitor accelerometer
/*engine.sensorManager = ASensorManager_getInstance();
engine.sensorAmount = ASensorManager_getSensorList(engine.sensorManager,0);
engine.sensorAccelerometer = ASensorManager_getDefaultSensor(engine.sensorManager,ASENSOR_TYPE_ACCELEROMETER);
engine.sensorMagneticField = ASensorManager_getDefaultSensor(engine.sensorManager,ASENSOR_TYPE_MAGNETIC_FIELD);
engine.sensorGyroscope = ASensorManager_getDefaultSensor(engine.sensorManager,ASENSOR_TYPE_GYROSCOPE);
engine.sensorLight = ASensorManager_getDefaultSensor(engine.sensorManager,ASENSOR_TYPE_LIGHT);
engine.sensorProximity = ASensorManager_getDefaultSensor(engine.sensorManager,ASENSOR_TYPE_PROXIMITY);*/

struct Engine_Android {
	struct android_app* app;
	ASensorManager* sensorManager;
	int sensorAmount;
	const ASensor* sensorAccelerometer;
	const ASensor* sensorMagneticField;
	const ASensor* sensorGyroscope;
	const ASensor* sensorLight;
	const ASensor* sensorProximity;
};

#endif // ENGINE_ANDROID_H
