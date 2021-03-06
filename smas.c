#include <stdio.h>
#define GLFW_INCLUDE_ES3
#define GLFW_INCLUDE_GLEXT
#include "GLFW/glfw3.h"
#include "nanovg.h"
#define NANOVG_GLES3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"
#include "perf.h"
#include <unistd.h>

int blowup = 0;
int screenshot = 0;
int premult = 1;

void errorcb(int error, const char* desc)
{
 printf("GLFW error %d: %s\n", error, desc);
}

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
 NVG_NOTUSED(scancode);
 NVG_NOTUSED(mods);
 if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  glfwSetWindowShouldClose(window, GL_TRUE);
 if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
  blowup = !blowup;
 if (key == GLFW_KEY_S && action == GLFW_PRESS)
  screenshot = 1;
 if (key == GLFW_KEY_P && action == GLFW_PRESS)
  premult = !premult;
}

int init(void)
{
	GLFWwindow* window;
	NVGcontext* vg = NULL;
	PerfGraph fps;
	double prevt = 0;

	if (!glfwInit()) {
        printf("Failed to init GLFW.");
        return -1;
    }

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	glfwSetErrorCallback(errorcb);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);

	if (!window) {
        glfwTerminate();
        return -1;
    }

	glfwSetKeyCallback(window, key);
	glfwMakeContextCurrent(window);
	
	vg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    if (vg == NULL) {
        printf("Could not init nanovg.\n");
        return -1;
    }

	glfwSwapInterval(0);
	glfwSetTime(0);
	prevt = glfwGetTime();

	int fontNormal, fontBold, fontIcons, fontEmoji;
	fontIcons = nvgCreateFont(vg, "icons", "../7thwok/nanovg/example/entypo.ttf");
	fontNormal = nvgCreateFont(vg, "sans", "../7thwok/nanovg/example/Roboto-Regular.ttf");
	fontBold = nvgCreateFont(vg, "sans-bold", "../7thwok/nanovg/example/Roboto-Bold.ttf");
	fontEmoji = nvgCreateFont(vg, "emoji", "../7thwok/nanovg/example/NotoEmoji-Regular.ttf");
	nvgAddFallbackFontId(vg, fontNormal, fontEmoji);
	nvgAddFallbackFontId(vg, fontBold, fontEmoji);

	while (!glfwWindowShouldClose(window)){
		double mx, my, t, dt;
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        float pxRatio;

		t = glfwGetTime();
  		dt = t - prevt;
		prevt = t;
		updateGraph(&fps, dt);

		glfwGetCursorPos(window, &mx, &my);
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		pxRatio = (float)fbWidth / (float)winWidth;
		glViewport(0, 0, fbWidth, fbHeight);
		if (premult)
		   	glClearColor(0,0,0,0);
		else
	   		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
		renderGraph(vg, 5,5, &fps);

const char* title = "omgbebebe";
float x = 100.0;
float y = 100.0;
float w = 100.0;
float h = 100.0;
nvgFontSize(vg, 18.0f);
nvgFontFace(vg, "sans-bold");
nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

nvgFontBlur(vg,2);
nvgFillColor(vg, nvgRGBA(0,0,0,128));
nvgText(vg, x+w/2,y+16+1, title, NULL);

nvgFontBlur(vg,0);
nvgFillColor(vg, nvgRGBA(220,220,220,160));
nvgText(vg, x+w/2,y+16, title, NULL);

		nvgEndFrame(vg);

		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
        glfwPollEvents();
		usleep(100 * 1000);
	}

	nvgDeleteGLES3(vg);
	glfwTerminate();
	return 0;
}
