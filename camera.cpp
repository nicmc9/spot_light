#include "camera.h"
#include <glm/gtc/type_ptr.hpp>


glm::mat4 Camera::GetViewMatrix()
{ 
    return glm::lookAt(position_, position_ + front_, world_up_);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    //Ќа части статьи ѕ–ќ√”Ћя≈ћ—я мы ввели новые костанты и не использовали ранне рассчитанный правый вектор а здесь истользуем
    //glm::cross(front_, world_up_) но с переворотм значение так как фронт смотрит вперед  заметь glm::cross(up, cameraDirection)
    float velocity = MOVEMENT_SPEED * deltaTime;
    if (direction == FORWARD)
        position_ += front_ * velocity;
    if (direction == BACKWARD)
        position_ -= front_ * velocity;
    if (direction == LEFT)
        position_ -= right_ * velocity;
    if (direction == RIGHT)
        position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
  
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}



void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector

    //«десь wordld_up дает правилный перпендиакул€рный вектор 
    right_ = glm::normalize(glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

    //похоже это тот up_ который внутри lookAt - нигде не примен€етс€ - это наверное остаток от своей матрицы lookAt
    up_ = glm::normalize(glm::cross(right_, front_));

}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (zoom_ >= 1.0f && zoom_ <= 45.0f)
        zoom_ -= yoffset;
    if (zoom_ <= 1.0f)
        zoom_ = 1.0f;
    if (zoom_ >= 45.0f)
        zoom_ = 45.0f;
}

glm::mat4 CameraFPS::GetViewMatrix()
{
  //  return glm::lookAt(position_, position_ + front_, world_up_);
    return  look_at(position_, position_ + front_, world_up_);
}

void CameraFPS::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    //Ќа части статьи ѕ–ќ√”Ћя≈ћ—я мы ввели новые костанты и не использовали ранне рассчитанный правый вектор а здесь истользуем
    //glm::cross(front_, world_up_) но с переворотм значение так как фронт смотрит вперед  заметь glm::cross(up, cameraDirection)
    float velocity = MOVEMENT_SPEED * deltaTime;
    if (direction == FORWARD)
        position_ += front_ * velocity;
    if (direction == BACKWARD)
        position_ -= front_ * velocity;
    if (direction == LEFT)
        position_ -= right_ * velocity;
    if (direction == RIGHT)
        position_ += right_ * velocity;
}

void CameraFPS::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= SENSITIVITY;
    yaw_ += xoffset;
     // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}



void CameraFPS::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_));
    front.y = 0.0f;
    front.z = sin(glm::radians(yaw_));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector

    //«десь wordld_up дает правилный перпендиакул€рный вектор 
    right_ = glm::normalize(glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

    //похоже это тот up_ который внутри lookAt - нигде не примен€етс€ - это наверное остаток от своей матрицы lookAt
    up_ = glm::normalize(glm::cross(right_, front_));

}
glm::mat4 CameraFPS::look_at(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    //1)pos

    //2)
    glm::vec3 cameraDirection = glm::normalize(pos - target);

    //3)
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

    //4)
    glm::vec3 cameraUp = glm::cross( cameraDirection, cameraRight); //


    float vecs[16] = {   cameraRight.x, cameraUp.x, cameraDirection.x,  0.0f ,
                         cameraRight.y, cameraUp.y, cameraDirection.y,  0.0f ,
                         cameraRight.z, cameraUp.z, cameraDirection.z,  0.0f ,
                         0.0f,          0.0f,       0.0f,               1.0f
                     };
    float posic[16] = { 1.0f,    0.0f,   0.0f,  0.0f ,
                        0.0f,    1.0f,   0.0f,  0.0f ,
                        0.0f,    0.0f,   1.0f,  0.0f ,
                        -pos.x, -pos.y, -pos.z, 1.0f
                      };
    glm::mat4 aaa = glm::make_mat4(vecs);
    glm::mat4 bbb = glm::make_mat4(posic);

    glm::mat4 look = aaa * bbb;
    //Also don't forget to ensure that the source array is stored column-wise, 
    //otherwise you will need to add glm::mat4 bbbT = glm::make_mat4(aaa); glm::mat4 bbb = glm::transpose(bbbT); Ц Riaz Rizvi

    return look;

}

void CameraFPS::ProcessMouseScroll(float yoffset)
{
    if (zoom_ >= 1.0f && zoom_ <= 45.0f)
        zoom_ -= yoffset;
    if (zoom_ <= 1.0f)
        zoom_ = 1.0f;
    if (zoom_ >= 45.0f)
        zoom_ = 45.0f;
}

