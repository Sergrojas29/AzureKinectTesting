class PoseChecker
{
    enum class GameState
    {
        POSE_EMPOWERED
    };

public:
    PoseChecker() = delete;

    static bool Check(PoseChecker::GameState Pose){
        switch (Pose)
        {
        case GameState::POSE_EMPOWERED:
            
                

            return true;
        
        default:
            break;
        }

        return false;
    }

private:
};
