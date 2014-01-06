#include "FUSense.h"
#include "vmath.h"
#include "SFML/Graphics/Image.hpp"
#include <math.h>
#include <iostream>

using namespace nite;
using namespace openni;

FUSense::FUSense()
{
}

int FUSense::getVisibleUserCount(const nite::Array<nite::UserData>& users)
{
    int visibleUserCount = 0;
    for (int i = 0;i < users.getSize();i++)
    {
        if (users[i].isVisible())
            visibleUserCount++;
    }
    return visibleUserCount;
}

bool FUSense::detectGreaterPosture(const nite::UserData& user)
{
    bool isGreaterPosture = false;
    if (!user.isVisible())
        return false;
    else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
        nite::Skeleton skeleton = user.getSkeleton();
        const nite::Point3f& rightHandPosition = skeleton.getJoint(nite::JOINT_RIGHT_HAND).getPosition();
        const nite::Point3f& leftShoulderPosition = skeleton.getJoint(nite::JOINT_LEFT_SHOULDER).getPosition();
        if (rightHandPosition.x > leftShoulderPosition.x || rightHandPosition.y < leftShoulderPosition.y)
            return false;
        else
            isGreaterPosture = true;
    }
    else
        printf("SKELETON NOT TRACKED!!!!\n");
    return isGreaterPosture;
}

bool FUSense::detectLesserPosture(const nite::UserData& user)
{
    bool isLesserPosture = false;
    if (!user.isVisible())
        return false;
    else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
        nite::Skeleton skeleton = user.getSkeleton();
        const nite::Point3f& leftHandPosition = skeleton.getJoint(nite::JOINT_LEFT_HAND).getPosition();
        const nite::Point3f& rightShoulderPosition = skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER).getPosition();
        if (leftHandPosition.x < rightShoulderPosition.x || leftHandPosition.y < rightShoulderPosition.y)
            return false;
        else
            isLesserPosture = true;
    }
    else
        printf("SKELETON NOT TRACKED!!!!\n");
    return isLesserPosture;
}

bool FUSense::isUserOnRight(const nite::UserData& userOne, const nite::UserData& userTwo)
{
    bool isUserOnRight = false;
    if (!userOne.isVisible() || !userTwo.isVisible() || userOne.getId() == userTwo.getId())
        return false;
    else
    {
        nite::Skeleton skeletonOne = userOne.getSkeleton();
        nite::Skeleton skeletonTwo = userTwo.getSkeleton();
        if (skeletonOne.getState() == nite::SKELETON_TRACKED && skeletonTwo.getState() == nite::SKELETON_TRACKED)
        {
            if (skeletonOne.getJoint(nite::JOINT_TORSO).getPosition().x > skeletonTwo.getJoint(nite::JOINT_TORSO).getPosition().x)
                isUserOnRight = true;
        }
        else
            printf("SKELETON NOT TRACKED!!!!\n");
    }
    return isUserOnRight;
}

bool FUSense::detectRightHandUpPosture(const nite::UserData& user)
{
    bool isRightHandUp = false;
    if (!user.isVisible())
        return false;
    else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
        const nite::Skeleton& skeleton = user.getSkeleton();
        //Elbow should be under hand
        if (skeleton.getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().y > skeleton.getJoint(nite::JOINT_RIGHT_HAND).getPosition().y)
            return false;
        if (skeleton.getJoint(nite::JOINT_RIGHT_HAND).getPosition().y - skeleton.getJoint(nite::JOINT_HEAD).getPosition().y > 200)
            isRightHandUp = true;
        else
            isRightHandUp = false;
    }
    else
        printf("SKELETON NOT TRACKED!!!!\n");
    return isRightHandUp;
}

bool FUSense::detectLeftHandUpPosture(const nite::UserData& user)
{
    bool isLeftHandUp = false;
    if (!user.isVisible())
        return false;
    else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
        const nite::Skeleton& skeleton = user.getSkeleton();
        //Elbow should be under hand
        if (skeleton.getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y > skeleton.getJoint(nite::JOINT_LEFT_HAND).getPosition().y)
            return false;
        if (skeleton.getJoint(nite::JOINT_LEFT_HAND).getPosition().y - skeleton.getJoint(nite::JOINT_HEAD).getPosition().y > 200)
            isLeftHandUp = true;
        else
            isLeftHandUp = false;
    }
    else
        printf("SKELETON NOT TRACKED!!!!!!\n");
    return isLeftHandUp;
}

bool FUSense::detectLeftHandDownPosture(const nite::UserData &user)
{
    //TODO: You might want to fine tune this
    if (!user.isVisible())
        return false;
    else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
        const nite::Skeleton& skeleton = user.getSkeleton();
        //Elbow should be below hand
        if (skeleton.getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y < skeleton.getJoint(nite::JOINT_LEFT_HAND).getPosition().y)
            return false;
        if (skeleton.getJoint(nite::JOINT_LEFT_HAND).getPosition().y > skeleton.getJoint(nite::JOINT_LEFT_HIP).getPosition().y)
            return false;
        if (std::abs(skeleton.getJoint(nite::JOINT_LEFT_HIP).getPosition().x) - std::abs(skeleton.getJoint(nite::JOINT_LEFT_HAND).getPosition().x) > 30)
            return false;
    }
    return true;
}

bool FUSense::detectRightHandDownPosture(const nite::UserData &user)
{
    //TODO: You might want to fine tune this
    if (!user.isVisible())
        return false;
    else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
        const nite::Skeleton& skeleton = user.getSkeleton();
        //Elbow should be over hand
        if (skeleton.getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().y < skeleton.getJoint(nite::JOINT_RIGHT_HAND).getPosition().y)
            return false;
        if (skeleton.getJoint(nite::JOINT_RIGHT_HAND).getPosition().y >= skeleton.getJoint(nite::JOINT_RIGHT_HIP).getPosition().y)
            return false;
        if (std::abs(skeleton.getJoint(nite::JOINT_RIGHT_HIP).getPosition().x) - std::abs(skeleton.getJoint(nite::JOINT_RIGHT_HAND).getPosition().x) > 30)
            return false;
    }
    return true;
}

bool FUSense::detectOpenRightArm(const nite::UserData &user)
{
    if (!user.isVisible())
        return false;
    bool armOpen = true;
    const nite::Skeleton& skeleton = user.getSkeleton();
    const nite::Point3f &rightHandPoint = skeleton.getJoint(nite::JOINT_RIGHT_HAND).getPosition();
    const nite::Point3f &rightElbowPoint = skeleton.getJoint(nite::JOINT_RIGHT_ELBOW).getPosition();
    const nite::Point3f &rightShoulderPoint = skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER).getPosition();
    //If elbow is below the shoulder, return false
    if (rightShoulderPoint.y - rightElbowPoint.y > 150)
        return false;
    //If elbow is over shoulder, return false
    if (rightShoulderPoint.y - rightElbowPoint.y < -150)
        return false;
    //If hand is below elbow, return false
    if (rightHandPoint.y - rightElbowPoint.y > 150)
        return false;
    //If the hand is over elbow, return false
    if (rightHandPoint.y - rightElbowPoint.y < -150)
        return false;
    return armOpen;
}

bool FUSense::detectOpenLeftArm(const nite::UserData &user)
{
    if (!user.isVisible())
        return false;
    bool armOpen = true;
    const nite::Skeleton& skeleton = user.getSkeleton();
    const nite::Point3f &leftHandPoint = skeleton.getJoint(nite::JOINT_LEFT_HAND).getPosition();
    const nite::Point3f &leftElbowPoint = skeleton.getJoint(nite::JOINT_LEFT_ELBOW).getPosition();
    const nite::Point3f &leftShoulderPoint = skeleton.getJoint(nite::JOINT_LEFT_SHOULDER).getPosition();
    //If elbow is below the shoulder, return false
    if (leftShoulderPoint.y - leftElbowPoint.y > 150)
        return false;
    //If elbow is over shoulder, return false
    if (leftShoulderPoint.y - leftElbowPoint.y < -150)
        return false;
    //If hand is below elbow, return false
    if (leftHandPoint.y - leftElbowPoint.y > 150)
        return false;
    //If the hand is over elbow, return false
    if (leftHandPoint.y - leftElbowPoint.y < -150)
        return false;
    return armOpen;
}

bool FUSense::detectOpenArms(const nite::UserData &user)
{
    return detectOpenLeftArm(user) && detectOpenRightArm(user);
}

bool FUSense::isUserTracked(const nite::UserData &user)
{
    return user.getSkeleton().getState() == nite::SKELETON_TRACKED;
}

bool FUSense::detectWaveGesture(nite::HandTracker &handTracker, nite::HandTrackerFrameRef &handTrackerFrame)
{
    if (!handTracker.isValid())
        return false;
    bool isGestureDetected = false;
    handTracker.startGestureDetection(nite::GESTURE_WAVE);
    if (handTracker.readFrame(&handTrackerFrame) != nite::STATUS_OK)
    {
        printf("Get next frame failed\n");
        return isGestureDetected;
    }
    const nite::Array<nite::GestureData>& gestures = handTrackerFrame.getGestures();
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        if (gestures[i].isComplete())
            isGestureDetected = true;
    }
    return isGestureDetected;
}

bool FUSense::detectPushGesture(nite::HandTracker &handTracker, nite::HandTrackerFrameRef &handTrackerFrame)
{
    if (!handTracker.isValid())
        return false;
    bool isGestureDetected = false;
    handTracker.startGestureDetection(nite::GESTURE_CLICK);
    if (handTracker.readFrame(&handTrackerFrame) != nite::STATUS_OK)
    {
        printf("Get next frame failed\n");
        return isGestureDetected;
    }
    const nite::Array<nite::GestureData>& gestures = handTrackerFrame.getGestures();
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        if (gestures[i].isComplete())
            isGestureDetected = true;
    }
    return isGestureDetected;
}

bool FUSense::detectBothArmsDown(const nite::UserData &user)
{
    return detectRightHandDownPosture(user) && detectLeftHandDownPosture(user);
}

bool FUSense::detectLeanRight(const nite::UserData &user)
{
    if (!user.isVisible())
        return false;
    bool isLeaningRight = true;
    const nite::Point3f &headPosition = user.getSkeleton().getJoint(nite::JOINT_HEAD).getPosition();
    const nite::Point3f &torsoPosition = user.getSkeleton().getJoint(nite::JOINT_TORSO).getPosition();
    if (headPosition.x <= torsoPosition.x || headPosition.x - torsoPosition.x < 100)
        return false;
    return isLeaningRight;
}

bool FUSense::detectLeanLeft(const nite::UserData &user)
{
    if (!user.isVisible())
        return false;
    bool isLeaningLeft = true;
    const nite::Point3f &headPosition = user.getSkeleton().getJoint(nite::JOINT_HEAD).getPosition();
    const nite::Point3f &torsoPosition = user.getSkeleton().getJoint(nite::JOINT_TORSO).getPosition();
    if (headPosition.x >= torsoPosition.x || torsoPosition.x - headPosition.x < 100)
        return false;
    return isLeaningLeft;
}

bool FUSense::detectJump(nite::UserTrackerFrameRef &userTrackerFrame, const nite::UserData &user)
{
    bool isJumping = false;
    if ((user.getSkeleton().getJoint(nite::JOINT_LEFT_FOOT).getPosition().y - userTrackerFrame.getFloor().point.y > 200)
            && (user.getSkeleton().getJoint(nite::JOINT_RIGHT_FOOT).getPosition().y - userTrackerFrame.getFloor().point.y > 200))
        isJumping = true;
    return isJumping;
}

float FUSense::getRightHandAngle(const nite::UserData &user)
{
    float angle = 0;
    const nite::Point3f &righthandPosition = user.getSkeleton().getJoint(JOINT_RIGHT_HAND).getPosition();
    const nite::Point3f &rightShoulderPosition = user.getSkeleton().getJoint(JOINT_RIGHT_SHOULDER).getPosition();
    float x = righthandPosition.x - rightShoulderPosition.x;
    float y = righthandPosition.y - rightShoulderPosition.y;
    Vector2<float> vector1(x, y);
    Vector2<float> vector2(x, 0);
    float vectorInnerProduct = vector1.getX() * vector2.getX() + vector1.getY() * vector2.getY();
    float lengthProduct = vector1.length() * vector2.length();
    angle = std::acos(vectorInnerProduct / lengthProduct) * 180 / 3.14159f;
    if (righthandPosition.y >= rightShoulderPosition.y && righthandPosition.x >= rightShoulderPosition.x)
        angle = 270 - angle;
    else if (righthandPosition.y <= rightShoulderPosition.y && righthandPosition.x >= rightShoulderPosition.x)
        angle += 270;
    else if (righthandPosition.y <= rightShoulderPosition.y && righthandPosition.x <= rightShoulderPosition.x)
        angle = 90 - angle;
    else if (righthandPosition.y >= rightShoulderPosition.y && righthandPosition.x <= rightShoulderPosition.x)
        angle = 90 + angle;

    printf("RIGHT ARM ANGLE: %f\n", angle);
    return angle;
}

float FUSense::getLeftHandAngle(const nite::UserData &user)
{
    float angle = 0;
    const nite::Point3f &lefthandPosition = user.getSkeleton().getJoint(JOINT_LEFT_HAND).getPosition();
    const nite::Point3f &leftShoulderPosition = user.getSkeleton().getJoint(JOINT_LEFT_SHOULDER).getPosition();
    float x = lefthandPosition.x - leftShoulderPosition.x;
    float y = lefthandPosition.y - leftShoulderPosition.y;
    Vector2<float> vector1(x, y);
    Vector2<float> vector2(x, 0);
    float vectorInnerProduct = vector1.getX() * vector2.getX() + vector1.getY() * vector2.getY();
    float lengthProduct = vector1.length() * vector2.length();
    angle = std::acos(vectorInnerProduct / lengthProduct) * 180 / 3.14159f;
    if (lefthandPosition.y >= leftShoulderPosition.y && lefthandPosition.x >= leftShoulderPosition.x)
        angle = 270 - angle;
    else if (lefthandPosition.y <= leftShoulderPosition.y && lefthandPosition.x >= leftShoulderPosition.x)
        angle += 270;
    else if (lefthandPosition.y <= leftShoulderPosition.y && lefthandPosition.x <= leftShoulderPosition.x)
        angle = 90 - angle;
    else if (lefthandPosition.y >= leftShoulderPosition.y && lefthandPosition.x <= leftShoulderPosition.x)
        angle = 90 + angle;

    printf("LEFT ARM ANGLE: %f\n", angle);
    return angle;
}

void FUSense::saveRGBImage(const RGB888Pixel *pixels, std::string path)
{
    const openni::RGB888Pixel *pImageRow = pixels;
    sf::Image sfImage;
    sfImage.create(1280, 960);
    for (unsigned int y = 0;y < sfImage.getSize().y;y++)
    {
        for (unsigned int x = 0;x < sfImage.getSize().x;x++, pImageRow++)
        {
            sf::Color color(pImageRow->r, pImageRow->g, pImageRow->b);
            sfImage.setPixel(x, y, color);
        }
    }

    sfImage.saveToFile(path);
}
