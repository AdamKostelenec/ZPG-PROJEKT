#include "Controller.h"
#include <glm/gtc/matrix_transform.hpp>

Controller::Controller(GLFWwindow* win,
    const glm::vec3& camPos,
    float yawDeg, float pitchDeg,
    float fovDeg, float zNear, float zFar)
    : win_(win),
    cam_(camPos, yawDeg, pitchDeg, fovDeg, zNear, zFar) 
{
    prevTime_ = glfwGetTime();
    glfwGetFramebufferSize(win_, &fbw_, &fbh_);
}

void Controller::initializeCallbacks(bool /*lockCursor*/) {
    installCallbacks_();
  //  if (lockCursor) {
	glfwSetInputMode(win_, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
  //}
}

FrameState Controller::update() {
    // čas
    double now = glfwGetTime();
    float dt = static_cast<float>(now - prevTime_);
    prevTime_ = now;

  
    int w = 0, h = 0;
    glfwGetFramebufferSize(win_, &w, &h);
    if (w != fbw_ || h != fbh_) {
        fbw_ = w; fbh_ = h;
        glViewport(0, 0, fbw_, fbh_);
    }

    // vstupy (WASD)
    int W = (glfwGetKey(win_, GLFW_KEY_W) == GLFW_PRESS);
    int A = (glfwGetKey(win_, GLFW_KEY_A) == GLFW_PRESS);
    int S = (glfwGetKey(win_, GLFW_KEY_S) == GLFW_PRESS);
    int D = (glfwGetKey(win_, GLFW_KEY_D) == GLFW_PRESS);

    cam_.processKeyboard(W, A, S, D, dt); 

    bool k1 = (glfwGetKey(win_, GLFW_KEY_1) == GLFW_PRESS);
    bool k2 = (glfwGetKey(win_, GLFW_KEY_2) == GLFW_PRESS);
    bool k3 = (glfwGetKey(win_, GLFW_KEY_3) == GLFW_PRESS);
	bool k4 = (glfwGetKey(win_, GLFW_KEY_4) == GLFW_PRESS);
    bool k5 = (glfwGetKey(win_, GLFW_KEY_5) == GLFW_PRESS);
	bool k6 = (glfwGetKey(win_, GLFW_KEY_6) == GLFW_PRESS);
	bool k7 = (glfwGetKey(win_, GLFW_KEY_7) == GLFW_PRESS);
	bool k8 = (glfwGetKey(win_, GLFW_KEY_8) == GLFW_PRESS);
    if (k1 && !lastKey1_) activeScene_ = 0;
    if (k2 && !lastKey2_) activeScene_ = 1;
    if (k3 && !lastKey3_) activeScene_ = 2;
	if (k4 && !lastKey4_) activeScene_ = 3;
	if (k5 && !lastKey5_) activeScene_ = 4;
	if (k6 && !lastKey6_) activeScene_ = 5;
	if (k7 && !lastKey7_) activeScene_ = 6;
	if (k8 && !lastKey8_) activeScene_ = 7;
	lastKey1_ = k1; lastKey2_ = k2; lastKey3_ = k3; lastKey4_ = k4; lastKey5_ = k5, lastKey6_ = k6, lastKey7_ = k7,lastKey8_ =k8;

    bool kF = (glfwGetKey(win_, GLFW_KEY_F) == GLFW_PRESS);
    if (kF && !lastFlashKey_) {
        flashlightOn_ = !flashlightOn_;
    }
    lastFlashKey_ = kF;

	bool kC = (glfwGetKey(win_, GLFW_KEY_C) == GLFW_PRESS);
    if (kC && !freeMoveMode_) {
        cam_.setFreeMove(!cam_.isFreeMove());
    }
	freeMoveMode_ = kC;

    //Edit controls 

	bool kDel = (glfwGetKey(win_, GLFW_KEY_DELETE) == GLFW_PRESS);
    if (kDel && !deleteMode_) {
        deleteMode_ = true;
		std::printf("Delete mode ON\n");
    };
    if (!kDel && deleteMode_) {
        deleteMode_ = false;
	};

    bool kI = (glfwGetKey(win_, GLFW_KEY_I) == GLFW_PRESS);
    if (kI && !insertMode_) {
        insertMode_ = true;
        std::printf("Insert mode ON\n");
    };
    if (!kI && insertMode_) {
        insertMode_ = false;
    };

	bool kM = (glfwGetKey(win_, GLFW_KEY_M) == GLFW_PRESS);
    if (kM && !moveMode_) {
        moveMode_ = true;
        std::printf("Move mode ON\n");
    };
    if (!kM && moveMode_) {
        moveMode_ = false;
	};

    bool kB = (glfwGetKey(win_, GLFW_KEY_B) == GLFW_PRESS);
    if (kB && !lastKeyB_) {
        bezierMode_ = !bezierMode_;
        std::printf("Bezier mode %s\n", bezierMode_ ? "ON" : "OFF");
    }
    lastKeyB_ = kB;
	bool kR = (glfwGetKey(win_, GLFW_KEY_R) == GLFW_PRESS);
    if (kR && !bezierReset_) {
        bezierReset_ = true;
    }
    if(!kR && bezierReset_) {
        bezierReset_ = false;
	}


	bool upArrow = (glfwGetKey(win_, GLFW_KEY_UP) == GLFW_PRESS);
    if (upArrow && !upArrow_) {
        upArrow_ = true;
	}
    if (!upArrow && upArrow_) {
        upArrow_ = false;
    }

	bool downArrow = (glfwGetKey(win_, GLFW_KEY_DOWN) == GLFW_PRESS);
    if (downArrow && !downArrow_) {
		downArrow_ = true;
	}
    if (!downArrow && downArrow_) {
        downArrow_ = false;
	}
	bool leftArrow = (glfwGetKey(win_, GLFW_KEY_LEFT) == GLFW_PRESS);
	if (leftArrow && !leftArrow_) {
		leftArrow_ = true;
	}
    if (!leftArrow && leftArrow_) {
		leftArrow_ = false;
	}
	bool rightArrow = (glfwGetKey(win_, GLFW_KEY_RIGHT) == GLFW_PRESS);
	if (rightArrow && !rightArrow_) {
		rightArrow_ = true;
	}
	if (!rightArrow && rightArrow_) {
		rightArrow_ = false;
	}

	
    

    // build view/proj
    float aspect = (fbh_ > 0) ? (float)fbw_ / (float)fbh_ : 1.0f;
    FrameState st;
    st.view = cam_.view();           
    st.proj = cam_.proj(aspect);    
    st.time = static_cast<float>(now);
    st.dt = dt;
    st.activeScene = activeScene_;
    st.fbw = fbw_; st.fbh = fbh_;
	st.flashlightOn = flashlightOn_;
	st.pickrequest = pickRequest_;
	st.pickx = pickX_;
	st.picky = pickY_;
	pickRequest_ = false; // reset
    st.deletemode = deleteMode_;
	st.insertmode = insertMode_;
	st.movemode = moveMode_;
    st.beziermode = bezierMode_;
	st.bezierreset = bezierReset_;
	st.uparrow = upArrow_;
	st.downarrow = downArrow_;
	st.leftarrow = leftArrow_;
	st.rightarrow = rightArrow_;

    

    CameraUpdate cu;
    cu.view = st.view;          
    cu.proj = st.proj;
    cu.camPos = cam_.position();   
    cu.time = st.time;
    subject_.notifyObservers(cu);
    return st;
}



void Controller::installCallbacks_() {
    glfwSetWindowUserPointer(win_, this);

    glfwSetCursorPosCallback(win_, [](GLFWwindow* w, double x, double y) {
        sCursorPos(w, x, y);
        });

    glfwSetFramebufferSizeCallback(win_, [](GLFWwindow* w, int width, int height) {
        sFramebufferSize(w, width, height);
        });


    glfwSetMouseButtonCallback(win_, [](GLFWwindow* w, int button, int action, int) {
        auto* self = static_cast<Controller*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS) { self->rmbDown_ = true;  self->firstMouse_ = true; }
            if (action == GLFW_RELEASE) { self->rmbDown_ = false; self->firstMouse_ = true; }
        }
        if (button == GLFW_MOUSE_BUTTON_LEFT && action== GLFW_PRESS) {
            
            double xpos, ypos;
            glfwGetCursorPos(w, &xpos, &ypos);
            self->pickRequest_ = true;
            self->pickX_ = static_cast<int>(xpos);
            self->pickY_ = static_cast<int>(ypos);
            
        }
        });

    glfwSetCursorEnterCallback(win_, [](GLFWwindow* w, int entered) {
        auto* self = static_cast<Controller*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        self->hovered_ = (entered == GLFW_TRUE);
        self->firstMouse_ = true; 
        });

    glfwSetWindowFocusCallback(win_, [](GLFWwindow* w, int focused) {
        auto* self = static_cast<Controller*>(glfwGetWindowUserPointer(w));
        if (!self) return;
        self->focused_ = (focused == GLFW_TRUE);
        self->firstMouse_ = true;
        });
}





void Controller::sCursorPos(GLFWwindow* w, double x, double y) {
    auto* self = static_cast<Controller*>(glfwGetWindowUserPointer(w));
    if (!self) return;

    if (!self->rmbDown_ || !self->hovered_ || !self->focused_) {
        self->lastX_ = x; self->lastY_ = y;
        self->firstMouse_ = true;
        return; 
    }

    if (self->firstMouse_) {
        self->lastX_ = x; self->lastY_ = y;
        self->firstMouse_ = false;
        return;
    }

    float dx = static_cast<float>(x - self->lastX_);
    float dy = static_cast<float>(y - self->lastY_);
    self->lastX_ = x; self->lastY_ = y;

    self->cam_.processMouse(dx, dy, self->invertX_, self->invertY_);
}



void Controller::sFramebufferSize(GLFWwindow* w, int width, int height) {
    auto* self = static_cast<Controller*>(glfwGetWindowUserPointer(w));
    if (!self) return;
    self->fbw_ = width; self->fbh_ = height;
    glViewport(0, 0, width, height);
}
