#include "game.h"
#include "glwrapperAndroid.h"
#include "GameCore.h"

GameCore* pGameCore = new GameCore;

void on_surface_created() {
	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	pGameCore->OneTimeInit();
}

void on_surface_changed(int width, int height) {
	 //No-op
	pGameCore->OnSurfaceChanged(width,height);
}

void on_draw_frame() {
	//glClear(GL_COLOR_BUFFER_BIT);
	pGameCore->OnDrawFrame();
}


