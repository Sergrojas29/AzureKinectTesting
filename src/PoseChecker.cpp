#include "PoseChecker.h"

bool PoseChecker::check(PoseState Pose, const k4abt_skeleton_t &skeleton)
{

    switch (Pose)
    {
    case PoseState::POSE_EMPOWERED:
    {

        /*
        Stand Up Straight
        Lift Right Heal (Right Left Angle: Hip -> Knee -> Ankle)
        Hands at the bottom of the rib cage (Left & Right Arm Angle : Shoulder -> Elbow -> Wrist  )
        Hand Thumbs facing down
        Don't Lean in * could change that

        LegRads = 2.26f
        L R ArmRads = 0.87f

        tolerance = .20f

        Spine_Chest = get z value of _SPINE_CHEST
        tolerance = 10.0f
        L R wrist z value == ~ Spine_chest z value

        */

        float rightLegAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_HIP_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_ANKLE_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_KNEE_RIGHT].position);

        float leftArmAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_SHOULDER_LEFT].position,
                                        skeleton.joints[K4ABT_JOINT_WRIST_LEFT].position,
                                        skeleton.joints[K4ABT_JOINT_ELBOW_LEFT].position);

        float rightArmAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_SHOULDER_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_WRIST_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_ELBOW_RIGHT].position);

        float tolerance = .20f; // Radians

        float z_chest = skeleton.joints[K4ABT_JOINT_SPINE_CHEST].position.xyz.z;
        float z_lelft_wrist = skeleton.joints[K4ABT_JOINT_WRIST_LEFT].position.xyz.z;
        float z_right_wrist = skeleton.joints[K4ABT_JOINT_WRIST_RIGHT].position.xyz.z;

        float tolerance_mm = 20.0f;

        // Check values fall with in the range with +- tolerance
        if (std::abs(rightLegAngle - 2.26f) < tolerance &&
            std::abs(leftArmAngle - 0.87f) < tolerance &&
            std::abs(rightArmAngle - 0.87f) < tolerance &&
            std::abs(z_lelft_wrist - z_chest) < tolerance_mm &&
            std::abs(z_right_wrist - z_chest) < tolerance_mm)
        {
            return true;
        }
        // Else Flase
        return false;
    }

    case PoseState::POSE_MORGAN:
    {
        /*
        From Standing Straight
        Move right leg slightly past the shoulder
        Raise Right Heel
        Bring Both hand above to reset on the neck
        Both hand should be on top of one another
        Relax left hip slightly to tilt

        leftRightLegs ( Left Foot -> right hip -> right foot) RAD = .52

        L R ArmAngle ( shoulder -> elbow -> wrist) RAD = .62

        */

        float leftRightLegs = get_Rad_3D(skeleton.joints[K4ABT_JOINT_FOOT_LEFT].position,
                                         skeleton.joints[K4ABT_JOINT_FOOT_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_HIP_RIGHT].position);

        float leftArmAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_SHOULDER_LEFT].position,
                                        skeleton.joints[K4ABT_JOINT_WRIST_LEFT].position,
                                        skeleton.joints[K4ABT_JOINT_ELBOW_LEFT].position);

        float rightArmAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_SHOULDER_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_WRIST_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_ELBOW_RIGHT].position);

        float tolerance = .1f;

        if (std::abs(leftRightLegs - .52f) < tolerance &&
            std::abs(leftArmAngle - .62f) < tolerance &&
            std::abs(rightArmAngle - .62f) < tolerance)
        {
            return true;
        }

        return false;
    }
    case PoseState::POSE_LEAN_LEFT:
    {
        /*



        rightLeg ( Right Hip -> Right Knee -> right foot) RAD = 

        L ArmAngle ( shoulder -> elbow -> wrist) RAD = .

        */

        float hipAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_ANKLE_LEFT].position,
                                         skeleton.joints[K4ABT_JOINT_ANKLE_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_PELVIS].position);

        float RightArmAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_SHOULDER_RIGHT].position,
                                        skeleton.joints[K4ABT_JOINT_WRIST_RIGHT].position,
                                        skeleton.joints[K4ABT_JOINT_ELBOW_RIGHT].position);
        float leanAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_WRIST_LEFT].position,
                                        skeleton.joints[K4ABT_JOINT_PELVIS].position,
                                        skeleton.joints[K4ABT_JOINT_SHOULDER_LEFT].position);

        float tolerance = .1f;




        if (std::abs(hipAngle - .65f) < tolerance &&
            std::abs(leanAngle - .55f) < (tolerance + .05f) &&
            std::abs(RightArmAngle - 2.11f) < tolerance 
        )
        {
            return true;
        }

        return false;
    }
    case PoseState::POSE_CUTE_STANDING:
    {
        /*
        Place Left hands on neck. Above the top of your neck
        Lift your right foot above your ankle and fold your leg.
        Slightly move the right knee to the left thigh.
        Right Hand should be at hip level and straight


        rightLeg ( Right Hip -> Right Knee -> right foot) RAD = 1.52

        L ArmAngle ( shoulder -> elbow -> wrist) RAD = .62

        */

        float rightLegAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_HIP_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_ANKLE_RIGHT].position,
                                         skeleton.joints[K4ABT_JOINT_KNEE_RIGHT].position);

        float leftArmAngle = get_Rad_3D(skeleton.joints[K4ABT_JOINT_SHOULDER_LEFT].position,
                                        skeleton.joints[K4ABT_JOINT_WRIST_LEFT].position,
                                        skeleton.joints[K4ABT_JOINT_ELBOW_LEFT].position);

        float tolerance = .1f;



        if (std::abs(rightLegAngle - 1.52f) < tolerance &&
            std::abs(leftArmAngle - .62f) < tolerance
        )
        {
            return true;
        }

        return false;
    }
    case PoseState::LOOK_LEFT:
    {
        float x_head = skeleton.joints[K4ABT_JOINT_HEAD].position.xyz.x;
        float x_nose = skeleton.joints[K4ABT_JOINT_NOSE].position.xyz.x;
        if (x_head < x_nose - 30)
        {
            return true;
        }
        return false;
    }
    case PoseState::LOOK_RIGHT:
    {
        float x_head = skeleton.joints[K4ABT_JOINT_HEAD].position.xyz.x;
        float x_nose = skeleton.joints[K4ABT_JOINT_NOSE].position.xyz.x;
        if (x_head > x_nose + 30)
        {
            return true;
        }
        return false;
    }
    default:
        throw std::runtime_error("No pose was selected");
        return false;
    }
}
