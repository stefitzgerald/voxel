#pragma once

#include <string>

struct GLFWwindow;

class Application {
public:
    Application();

    static Application& getInstance();

    GLFWwindow* getWindow() const;

    void exit();

    float getFrameDeltaTime() const;
    float getTime() const;

    void run();

    int getWidth();
    int getHeight();
    float getWindowRatio();
    bool windowDimensionChanged();

private:
    enum State { STATE_READY, STATE_RUN, STATE_EXIT };

    State state;

    Application& operator=(const Application&) { return *this; }

    GLFWwindow* window;

    float time;
    float deltaTime;

    int width, height;
    bool dimensionChanged;
    void detectWindowDimensionChange();

protected:
    Application(const Application&) {};

    std::string title;

    virtual void loop();
};