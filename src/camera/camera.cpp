#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "camera.h"

// Constructor initializes camera properties and updates its vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
cameraSpeed(defaultCameraSpeed),
cameraSensitivity(defaultCameraSensitivity),
cameraFOV(defaultCameraFOV)
{
    this->cameraPosition = position;
    this->worldUp = up;
    this->cameraYaw = yaw;
    this->cameraPitch = pitch;
    this->updateCameraVectors(); // Ensure initial vectors are set
}

// Destructor (empty since there are no dynamic allocations)
Camera::~Camera()
{

}

// Returns the view matrix calculated using lookAt (position, target, up direction)
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->cameraPosition, this->cameraPosition + this->cameraFront, this->cameraUp);
}

// Handle keyboard input to move the camera in a given direction
void Camera::keyboardCall(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat cameraVelocity = this->cameraSpeed * deltaTime; // Calculate velocity based on speed and delta time

    // Simplified movement control by accumulating position based on direction
    switch (direction)
    {
    case FORWARD:
        this->cameraPosition += this->cameraFront * cameraVelocity;
        break;
    case BACKWARD:
        this->cameraPosition -= this->cameraFront * cameraVelocity;
        break;
    case LEFT:
        this->cameraPosition -= this->cameraRight * cameraVelocity;
        break;
    case RIGHT:
        this->cameraPosition += this->cameraRight * cameraVelocity;
        break;
    }
}

// Handle mouse movement to rotate camera based on x and y offsets
void Camera::mouseCall(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->cameraSensitivity; // Scale offset by sensitivity
    yoffset *= this->cameraSensitivity;

    this->cameraYaw += xoffset;   // Adjust yaw
    this->cameraPitch += yoffset; // Adjust pitch

    // Constrain the pitch to avoid flipping the camera upside down
    if (constrainPitch)
    {
        if (this->cameraPitch > 89.0f)
            this->cameraPitch = 89.0f;
        if (this->cameraPitch < -89.0f)
            this->cameraPitch = -89.0f;
    }

    this->updateCameraVectors(); // Update camera direction vectors based on new angles
}

// Handle scroll input to zoom in and out by adjusting the field of view (FOV)
void Camera::scrollCall(GLfloat yoffset)
{
    // Adjust FOV, but keep it within a reasonable range (1° to 45°)
    if (this->cameraFOV >= glm::radians(1.0f) && this->cameraFOV <= glm::radians(45.0f))
        this->cameraFOV -= glm::radians(yoffset);

    if (this->cameraFOV <= glm::radians(1.0f))
        this->cameraFOV = glm::radians(1.0f);
    if (this->cameraFOV >= glm::radians(45.0f))
        this->cameraFOV = glm::radians(45.0f);
}

// Recalculates the camera vectors (front, right, and up) based on the current yaw and pitch
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector using trigonometric functions
    glm::vec3 front;
    front.x = cos(glm::radians(this->cameraYaw)) * cos(glm::radians(this->cameraPitch));
    front.y = sin(glm::radians(this->cameraPitch));
    front.z = sin(glm::radians(this->cameraYaw)) * cos(glm::radians(this->cameraPitch));

    // Normalize vectors to ensure uniform movement speed
    this->cameraFront = glm::normalize(front);
    this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->worldUp)); // Right vector
    this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraFront)); // Up vector
}
