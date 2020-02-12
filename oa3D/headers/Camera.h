#ifndef _MODELCAMERA_H
#define _MODELCAMERA_H

#include "Shader.h"
#include "OA3D.h"

#include "GLFW/glfw3.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CAMERA_MOVEMENT {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Define camera types
enum CAMERA_TYPE {
    FIRST_PERSON,
    THIRD_PERSON,
    ORBIT_LEFT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float FOV         =  90.0f;
const float NEAR_PLANE  =  0.1f;
const float FAR_PLANE   = 350.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Depth
    float Frustum = (FAR_PLANE - NEAR_PLANE); // was that his name...!? :))

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    CAMERA_TYPE CameraType = ORBIT_LEFT; // Default camera type

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 up = glm::vec3(0.0f, 0.5f, 0.0f),
                float yaw = YAW, float pitch = PITCH) :
                Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                MovementSpeed(SPEED),
                MouseSensitivity(SENSITIVITY),
                Fov(FOV)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns view matrix based on set camera type
    glm::mat4 GetViewMatrix()
    {
        switch(CameraType) {
            case FIRST_PERSON:
                return GetFirstPersonViewMatrix();
                break;
            case THIRD_PERSON:
                return GetThirdPersonViewMatrix();
                break;
            case ORBIT_LEFT:
                return GetOrbitViewMatrix();
                break;
            default:
                return GetThirdPersonViewMatrix();
        }
    }

    glm::mat4 GetPerspective()
    {
        return glm::perspective(glm::radians(this->Fov), (float) mWidth / (float) mHeight, NEAR_PLANE, FAR_PLANE);
    }

    // Returns the third person view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetThirdPersonViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetFirstPersonViewMatrix()
    {
        // TODO: implement first person camera
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Returns the orbital view matrix based on time
    glm::mat4 GetOrbitViewMatrix()
    {
        // glm::lookAt = POSITION (position of camera in world coords),
        //               TARGET   (target position/direction),
        //               UP       (positive Y-direction)

        const float radius = 2.0f;
        float time = glfwGetTime();
        float camX = sin ( time ) * radius * 4;
        //float camY = sin ( glfwGetTime() ) * radius / cos( glfwGetTime()) * radius;
        float camZ = cos ( time ) * radius * 4;

        float modelPosX = 3.0f,
        modelPosY = 0.1f,
        modelPosZ = 50.0f;
        glm::vec3 targetPos = glm::vec3(modelPosX, modelPosY, modelPosZ);

        glm::vec3 position = glm::vec3(camX, 3.0, camZ);
        glm::vec3 target   = targetPos; //glm::vec3(0.0, 0.0, 0.0);
        Position = position;
        glm::vec3 up       = glm::vec3(0.0, 2.0, 0.0);
        return glm::lookAt(position, target, up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CAMERA_MOVEMENT direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseLeftPress(float xoffset, float yoffset, float deltaTime)
    {
        (void) xoffset;
        (void) yoffset;
        (void) deltaTime;
    }

    bool ToggleCamera()
    {
        // TODO: remove this later. this is just for switching between cameras
        if (this->CameraType == THIRD_PERSON)
        {
            this->CameraType = ORBIT_LEFT;
            return true;
        }
        else
        {
            this->CameraType = THIRD_PERSON;
            return false;
        }
    }


    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Fov >= 5.0f && Fov <= 90.0f)
            Fov -= yoffset;
        if (Fov <= 5.0f)
            Fov = 5.0f;
        if (Fov >= 90.0f)
            Fov = 90.0f;
    }

    void SetCameraType(CAMERA_TYPE cameraType, glm::vec3 newPosition = glm::vec3(0))
    {
        this->Position = newPosition;
        this->CameraType = cameraType;
    }

    float getFov()
    {
        return this->Fov;
    }

    glm::vec3 getPosition()
    {
        return this->Position;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
