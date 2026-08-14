#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Observer.h"

struct FrameState {
    glm::mat4 view{ 1.0f };
    glm::mat4 proj{ 1.0f };
    float     time = 0.0f;
    float     dt = 0.0f;
    int       activeScene = 0;
    int       fbw = 0, fbh = 0;
    bool      flashlightOn = false;
    bool      pickrequest = false;
    int       pickx = 0, picky = 0;
    bool      deletemode = false;
    bool      insertmode = false;
    bool 	  movemode = false;
    bool      beziermode = false;  
	bool      bezierreset = false;  
	bool      uparrow = false; 
	bool	  downarrow = false;
	bool      leftarrow = false;
	bool      rightarrow = false;
};

class Controller {
public:
    Controller(GLFWwindow* win,
        const glm::vec3& camPos = { 0.0f, 0.0f, 5.0f },
        float yawDeg = 90.0f, float pitchDeg = 0.0f,
        float fovDeg = 50.0f, float zNear = 0.1f, float zFar = 100.0f);

    void initializeCallbacks(bool lockCursor = true);

    FrameState update();

    Camera& camera() { return cam_; }
    const Camera& camera() const { return cam_; }

    void setInvertX(bool inv) { invertX_ = inv; }
    void setInvertY(bool inv) { invertY_ = inv; }
    void setActiveScene(int idx) { activeScene_ = idx; }
    int  activeScene() const { return activeScene_; }

    void addObserver(IObserver* o) { subject_.addObserver(o); }
    void removeObserver(IObserver* o) { subject_.removeObserver(o); }
    void clearObservers() { subject_.clearObservers(); }

private:
    GLFWwindow* win_ = nullptr;
    Camera      cam_;
    bool invertX_ = true, invertY_ = true;
    bool rmbDown_ = false;
    bool hovered_ = false;
    bool focused_ = true;

    
    double prevTime_ = 0.0;

    double lastX_ = 0.0, lastY_ = 0.0;
    bool   firstMouse_ = true;

    int fbw_ = 0, fbh_ = 0;

    int  activeScene_ = 0;
	bool lastKey1_ = false, lastKey2_ = false, lastKey3_ = false, lastKey4_ = false, lastKey5_ = false, lastKey6_ = false, lastKey7_ = false, lastKey8_ = false;

    // flashlight
    bool flashlightOn_ = false;
    bool lastFlashKey_ = false;  // edge detect pro F


    //Delete
	bool deleteMode_ = false;

    //Insert
	bool insertMode_ = false;

	bool moveMode_ = false;
    //picking
	bool pickRequest_ = false;
	int pickX_ = 0, pickY_ = 0;

	//freemove mode
	bool freeMoveMode_ = false;

    bool bezierMode_ = false;
    bool lastKeyB_ = false;
	bool bezierReset_ = false;
	bool lastKeyR_ = false;


	bool upArrow_ = false;
	bool downArrow_ = false;
	bool leftArrow_ = false;
	bool rightArrow_ = false;

    void installCallbacks_();

    static void sCursorPos(GLFWwindow* w, double x, double y);
    static void sScroll(GLFWwindow* w, double xoff, double yoff);
    static void sFramebufferSize(GLFWwindow* w, int width, int height);

    CameraSubject subject_;
};
