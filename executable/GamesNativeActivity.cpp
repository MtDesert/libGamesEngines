#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/asset_manager.h>
#include <android/bitmap.h>
#include <android/configuration.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android/log.h>
#include <android/looper.h>
#include <android/native_window.h>
#include <android/obb.h>
#include <android/rect.h>
#include <android/sensor.h>
#include <android/storage_manager.h>
#include <android/tts.h>
#include <android/window.h>
#include "android_native_app_glue.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "games.engines", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "games.engines", __VA_ARGS__))

#define SENSOR_AMOUNT 32
ASensorManager* sensorManager=NULL;
const ASensor* allSensors[SENSOR_AMOUNT]={NULL};
ASensorEventQueue* sensorEventQueue=NULL;

typedef struct EngineEGL{
	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
}EngineEGL;

static int engine_init_display(struct android_app *app,EngineEGL* engine) {
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint format,numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;
	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);
	surface = eglCreateWindowSurface(display,config,app->window, NULL);
	context = eglCreateContext(display,config,NULL,NULL);
	
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}
	
	eglQuerySurface(display,surface, EGL_WIDTH, &engine->width);
	eglQuerySurface(display,surface, EGL_HEIGHT, &engine->height);
	
	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	
	// Initialize GL state.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	
	return 0;
}

//重绘
static void engine_draw_frame(EngineEGL* engine) {
	if (engine->display == NULL)return;
	glClearColor(0,0,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(engine->display, engine->surface);
}

static void engine_term_display(EngineEGL* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app,AInputEvent* event){
	int32_t type=AInputEvent_getType(event);
	int32_t deviceID=AInputEvent_getDeviceId(event);
	int32_t source=AInputEvent_getSource(event);
	switch(type){
		case AINPUT_EVENT_TYPE_KEY:{
			int32_t action=AKeyEvent_getAction(event);
			int32_t flags=AKeyEvent_getFlags(event);
			int32_t keyCode=AKeyEvent_getKeyCode(event);
			int32_t scanCode=AKeyEvent_getScanCode(event);
		}break;
		case AINPUT_EVENT_TYPE_MOTION:{
			int32_t action=AMotionEvent_getAction(event);
			switch(action){
				case AMOTION_EVENT_ACTION_DOWN:LOGI("下");break;
				case AMOTION_EVENT_ACTION_UP:LOGI("上");break;
				case AMOTION_EVENT_ACTION_MOVE:LOGI("移动");break;
				case AMOTION_EVENT_ACTION_CANCEL:LOGI("取消");break;
				case AMOTION_EVENT_ACTION_OUTSIDE:LOGI("外部");break;
				case AMOTION_EVENT_ACTION_POINTER_DOWN:LOGI("指针下");break;
				case AMOTION_EVENT_ACTION_POINTER_UP:LOGI("指针上");break;
				case AMOTION_EVENT_ACTION_HOVER_MOVE:LOGI("盘旋移动");break;
				case AMOTION_EVENT_ACTION_SCROLL:LOGI("滚动");break;
				case AMOTION_EVENT_ACTION_HOVER_ENTER:LOGI("盘旋进入");break;
				case AMOTION_EVENT_ACTION_HOVER_EXIT:LOGI("盘旋退出");break;
			}
			size_t pointerCount=AMotionEvent_getPointerCount(event);
			size_t i=0;
			for(;i<pointerCount;++i){
				float x=AMotionEvent_getX(event,i);
				float y=AMotionEvent_getY(event,i);
				LOGI("x,y==%d,%d",(int)x,(int)y);
			}
		}break;
	}
	return 0;//返回0表示系统也进行处理
}

void printSensorInfo(const ASensor *sensor){//打印传感器信息
	if(!sensor)return;
	LOGI("传感器--------");
	LOGI("名称: %s",ASensor_getName(sensor));
	LOGI("供应商: %s",ASensor_getVendor(sensor));
	LOGI("类型: %d",ASensor_getType(sensor));
	LOGI("分辨率: %f",ASensor_getResolution(sensor));
	LOGI("最小延时: %d",ASensor_getMinDelay(sensor));
}
void startupSensor(const ASensor *sensor){//启动传感器
	if(!sensor)return;
	ASensorEventQueue_enableSensor(sensorEventQueue,sensor);
	ASensorEventQueue_setEventRate(sensorEventQueue,sensor,1000000/60);
}
void stopSensor(const ASensor *sensor){//关闭传感器
	if(sensor)ASensorEventQueue_disableSensor(sensorEventQueue,sensor);
}

static void engine_handle_cmd(struct android_app *app, int32_t cmd) {
	EngineEGL* engine = (EngineEGL*)app->userData;
	switch (cmd) {
		case APP_CMD_SAVE_STATE:
			// The system has asked us to save our current state.  Do so.
			app->savedState = NULL;
			app->savedStateSize = 0;
		break;
		case APP_CMD_INIT_WINDOW:
			// The window is being shown, get it ready.
			if (app->window != NULL) {
				engine_init_display(app,engine);
				engine_draw_frame(engine);
			}
		break;
		case APP_CMD_TERM_WINDOW:
			// The window is being hidden or closed, clean it up.
			engine_term_display(engine);
		break;
		case APP_CMD_GAINED_FOCUS:{//获得焦点
			//启动所有传感器
			int i=0;
			for(;i<SENSOR_AMOUNT;++i){
				startupSensor(allSensors[i]);
			}
		}break;
		case APP_CMD_LOST_FOCUS:{//失去焦点
			//停止所有传感器
			int i=0;
			for(;i<SENSOR_AMOUNT;++i){
				stopSensor(allSensors[i]);
			}
			//停止动画
			engine_draw_frame(engine);
		}break;
	}
}

void android_main(struct android_app* app) {
	LOGI("主函数开始启动");
	EngineEGL engine;
	app_dummy();//删除此函数可能导致启动失败
	memset(&engine, 0, sizeof(engine));
	app->userData = &engine;
	app->onAppCmd = engine_handle_cmd;
	app->onInputEvent = engine_handle_input;
	
	//显示所有传感器信息
	sensorManager = ASensorManager_getInstance();
	int i=0;
	for(;i<SENSOR_AMOUNT;++i){
		allSensors[i]=ASensorManager_getDefaultSensor(sensorManager,i);
		printSensorInfo(allSensors[i]);
	}
	sensorEventQueue = ASensorManager_createEventQueue(sensorManager,app->looper,LOOPER_ID_USER,NULL,NULL);
	LOGI("事件循环启动");
	int ident=0;
	int events=0;
	struct android_poll_source* source=NULL;
	while (1) {
		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,(void**)&source)) >= 0) {
			//处理事件
			if (source)source->process(app,source);
			//有传感器事件,可以处理
			if (ident == LOOPER_ID_USER) {
				ASensorEvent event;
				while (ASensorEventQueue_getEvents(sensorEventQueue,&event,1) > 0) {
					switch(event.type){
						case ASENSOR_TYPE_ACCELEROMETER:
							//LOGI("accelerometer: x=%f y=%f z=%f",event.acceleration.x, event.acceleration.y,event.acceleration.z);
						break;
						default:;
					}
				}
			}
			
			//需要退出的时候调用此代码
			if (app->destroyRequested != 0) {
				engine_term_display(&engine);
				ASensorManager_destroyEventQueue(sensorManager,sensorEventQueue);
				return;
			}
		}
		engine_draw_frame(&engine);
	}
}
