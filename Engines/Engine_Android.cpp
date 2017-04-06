#include"Engine_Android.h"
#include"Engine_EGL.h"
#include"Engine_GLES.h"
#include<android/native_activity.h>
#include<android/log.h>
#include<android_native_app_glue.h>

Engine_EGL engineEGL;
Engine_GLES engineGLES;

void redraw(){
	engineGLES.redraw();
	engineEGL.swap();
}

/**
 * Process the next input event.
 */
int32_t onInputEvent(struct android_app* app, AInputEvent* event){
	int32_t deviceID=AInputEvent_getDeviceId(event);
	int32_t source=AInputEvent_getSource(event);
	int32_t type=AInputEvent_getType(event);
	//__android_log_print(ANDROID_LOG_INFO,"advance.wars","%d,%d,%d",deviceID,source,type);
	switch(type){
		case AINPUT_EVENT_TYPE_KEY:{
			int32_t action=AKeyEvent_getAction(event);
			int32_t flags=AKeyEvent_getFlags(event);
			int32_t keyCode=AKeyEvent_getKeyCode(event);
			int32_t scanCode=AKeyEvent_getScanCode(event);
			int32_t metaState=AKeyEvent_getMetaState(event);
			int32_t repeatCount=AKeyEvent_getRepeatCount(event);
			int64_t downTime=AKeyEvent_getDownTime(event);
			int64_t eventTime=AKeyEvent_getEventTime(event);
			switch(action){
				case AKEY_EVENT_ACTION_DOWN:break;
				case AKEY_EVENT_ACTION_UP:break;
				case AKEY_EVENT_ACTION_MULTIPLE:break;
			}
			switch(flags){
				case AKEY_EVENT_FLAG_WOKE_HERE:break;
				case AKEY_EVENT_FLAG_SOFT_KEYBOARD:break;
				case AKEY_EVENT_FLAG_KEEP_TOUCH_MODE:break;
				case AKEY_EVENT_FLAG_FROM_SYSTEM:break;
				case AKEY_EVENT_FLAG_EDITOR_ACTION:break;
				case AKEY_EVENT_FLAG_CANCELED:break;
				case AKEY_EVENT_FLAG_VIRTUAL_HARD_KEY:break;
				case AKEY_EVENT_FLAG_LONG_PRESS:break;
				case AKEY_EVENT_FLAG_CANCELED_LONG_PRESS:break;
				case AKEY_EVENT_FLAG_TRACKING:break;
				case AKEY_EVENT_FLAG_FALLBACK:break;
			}
			switch(keyCode){}
			switch(scanCode){}
		}break;
		case AINPUT_EVENT_TYPE_MOTION:{
			float x=AMotionEvent_getX(event,0);
			float y=AMotionEvent_getY(event,0);
			__android_log_print(ANDROID_LOG_INFO,"advance.wars","%d,%d",(int)x,(int)y);
		}
		break;
	}
	switch(source){
		case AINPUT_SOURCE_UNKNOWN:break;
		case AINPUT_SOURCE_KEYBOARD:break;
		case AINPUT_SOURCE_DPAD:break;
		case AINPUT_SOURCE_GAMEPAD:break;
		case AINPUT_SOURCE_TOUCHSCREEN:break;
		case AINPUT_SOURCE_MOUSE:break;
		case AINPUT_SOURCE_STYLUS:break;
		case AINPUT_SOURCE_TRACKBALL:break;
		case AINPUT_SOURCE_TOUCHPAD:break;
		case AINPUT_SOURCE_TOUCH_NAVIGATION:break;
		case AINPUT_SOURCE_JOYSTICK:break;
		case AINPUT_SOURCE_ANY:break;
	}
	return 0;
}

/**
 * Process the next main command.
 */
void onAppCmd(struct android_app* app, int32_t cmd) {
	switch (cmd) {
		case APP_CMD_INPUT_CHANGED:break;
		case APP_CMD_INIT_WINDOW:
			// The window is being shown, get it ready.
			if (app->window != NULL) {
				ANativeWindow_setBuffersGeometry(app->window,0,0,0);
				engineEGL.initial(app->window);
				__android_log_print(ANDROID_LOG_INFO,"advance.wars","%d*%d",engineEGL.width,engineEGL.height);
				engineGLES.initial();
				__android_log_print(ANDROID_LOG_INFO,"advance.wars","%s",engineGLES.glVendor);
				__android_log_print(ANDROID_LOG_INFO,"advance.wars","%s",engineGLES.glRenderer);
				__android_log_print(ANDROID_LOG_INFO,"advance.wars","%s",engineGLES.glVersion);
				__android_log_print(ANDROID_LOG_INFO,"advance.wars","%s",engineGLES.glExtension);
				redraw();
			}
		break;
		case APP_CMD_TERM_WINDOW:{
			//engineEGL.terminate();
		}
			// The window is being hidden or closed, clean it up.
		break;
		case APP_CMD_WINDOW_RESIZED:break;
		case APP_CMD_WINDOW_REDRAW_NEEDED:break;
		case APP_CMD_CONTENT_RECT_CHANGED:break;
		case APP_CMD_GAINED_FOCUS:
			// When our app gains focus, we start monitoring the accelerometer.
			/*if (engine->sensorAccelerometer != NULL) {
				ASensorEventQueue_enableSensor(engine->sensorEventQueue,
						engine->sensorAccelerometer);
				// We'd like to get 60 events per second (in us).
				ASensorEventQueue_setEventRate(engine->sensorEventQueue,
						engine->sensorAccelerometer, (1000L/60)*1000);
			}*/
		break;
		case APP_CMD_LOST_FOCUS:
			// When our app loses focus, we stop monitoring the accelerometer.
			// This is to avoid consuming battery while not being used.
			/*if (engine->sensorAccelerometer != NULL) {
				ASensorEventQueue_disableSensor(engine->sensorEventQueue,
						engine->sensorAccelerometer);
			}
			// Also stop animating.
			//redraw();*/
		break;
		case APP_CMD_CONFIG_CHANGED:break;
		case APP_CMD_LOW_MEMORY:break;
		case APP_CMD_START:break;
		case APP_CMD_RESUME:break;
		case APP_CMD_SAVE_STATE:break;
		case APP_CMD_PAUSE:break;
		case APP_CMD_STOP:break;
		case APP_CMD_DESTROY:break;
	}
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* app){
	// Make sure glue isn't stripped.
	app_dummy();
	__android_log_print(ANDROID_LOG_DEBUG,"advance.wars","android_main");

	app->onAppCmd = onAppCmd;
	app->onInputEvent = onInputEvent;

	// loop waiting for stuff to do.
	int outFd,outEvent;
	struct android_poll_source* outData;
	while(1) {
		// Read all pending events.
		int ident=ALooper_pollAll(1,&outFd,&outEvent,(void**)&outData);
		switch(ident){
			case ALOOPER_POLL_WAKE:
				//__android_log_print(ANDROID_LOG_INFO,"advance.wars","ALOOPER_POLL_WAKE");
			break;
			case ALOOPER_POLL_CALLBACK:
				//__android_log_print(ANDROID_LOG_INFO,"advance.wars","ALOOPER_POLL_CALLBACK");
			break;
			case ALOOPER_POLL_TIMEOUT:
				redraw();
				//__android_log_print(ANDROID_LOG_INFO,"advance.wars","ALOOPER_POLL_TIMEOUT");
			break;
			case ALOOPER_POLL_ERROR:
				//__android_log_print(ANDROID_LOG_INFO,"advance.wars","ALOOPER_POLL_ERROR");
			break;
			default:{
				// If not animating, we will block forever waiting for events.
				// If animating, we loop until all events are read, then continue
				// to draw the next frame of animation.
				if(ident>=0){
					// Process this event.
					if(outData)outData->process(app,outData);
					// If a sensor has data, process it now.
					if (ident == LOOPER_ID_USER){}
					// Check if we are exiting.
					if (app->destroyRequested != 0){
						return;
					}
				}
			}
		}
	}
}
