#pragma once
#include "NiTE.h"
#include <map>

class FUSense
{
public:
    FUSense();

    /**
     * @brief Takes an array of UserData and check all of the users if they are visible then returns the count.
     *Note that a user might be invisible but still be in the array. After 10 seconds NiTE removes it from the array.
     * @param users
     * @return Number of visible users
     */
    static int getVisibleUserCount(const nite::Array<nite::UserData> &users);
    
    static bool detectGreaterPosture(const nite::UserData& user);
    static bool detectLesserPosture(const nite::UserData& user);

    static bool detectRightHandUpPosture(const nite::UserData& user);
    static bool detectLeftHandUpPosture(const nite::UserData& user);
    
    static bool isUserOnRight(const nite::UserData& userOne, const nite::UserData& userTwo);
    
    static bool detectLeftHandDownPosture(const nite::UserData& user);
    static bool detectRightHandDownPosture(const nite::UserData &user);
    
    static bool detectOpenRightArm(const nite::UserData &user);
    static bool detectOpenLeftArm(const nite::UserData &user);
    static bool detectOpenArms(const nite::UserData &user);
    
    static bool isUserTracked(const nite::UserData &user);
    
    static bool detectWaveGesture(nite::HandTracker &handTracker, nite::HandTrackerFrameRef &handTrackerFrame);
    static bool detectPushGesture(nite::HandTracker &handTracker, nite::HandTrackerFrameRef &handTrackerFrame);
    
    static bool detectBothArmsDown(const nite::UserData &user);
    
    static bool detectLeanRight(const nite::UserData &user);
    static bool detectLeanLeft(const nite::UserData &user);
    /**
     * @brief Gets the floor plane from the nite::UserTrackerFrameRef and checks the distance from both of the feet to the floor.
     *If the distance is greater than 200 mm returns true otherwise false
     * @param userTrackerFrame
     * @param user
     * @return
     */
    static bool detectJump(nite::UserTrackerFrameRef &userTrackerFrame, const nite::UserData &user);
    /**
     * @brief Takes the right shoulder as the origion of the circle and as the hand turns around it
     *returns the angle. When the right hand is down near the legs, the angle is 0 and it increases clockwise.
     * @param user
     * @return Returns the angle relative to the right shoulder as the origin
     */
    static float getRightHandAngle(const nite::UserData &user);
    /**
     * @brief Takes the left shoulder as the origion of the circle and as the hand turns around it
     *returns the angle. When the left hand is down near the legs, the angle is 0 and it increases clockwise.
     * @param user
     * @return Returns the angle relative to the left shoulder as the origin
     */
    static float getLeftHandAngle(const nite::UserData &user);
    /**
     * @brief Takes a const openni::RGB888Pixel and converts it to a sf::Image and saves it to the path
     * @param const openni::RGB888Pixel pixels
     * @param File path including the name of the file with the extension
     */
    static void saveRGBImage(const openni::RGB888Pixel *pixels, std::string path);
};

