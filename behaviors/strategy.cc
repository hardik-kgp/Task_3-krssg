#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"

extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    beamX = -HALF_FIELD_X + worldModel->getUNum();
    beamY = 0;
    beamAngle = 0;
}


SkillType NaoBehavior::selectSkill() {
    // My position and angle
    //cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    //cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    //return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    //return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    //return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    //return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    //return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    //return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    //return goToTargetRelative(VecPdosition(0,-1,0), -90); // Turn clockwise
    //return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    //return goToTarget(ball);

    // Turn in place to face ball
    /*double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }*/

    // Walk to ball while always facing forward
    //return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    //return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    //return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0,0)); // IK kick

    // Just stand in place
    //return SKILL_STAND;

    // Demo behavior where players form a rotating circle and kick the ball
    // back and forth
    return demoKickingCircle();
}


/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */
SkillType NaoBehavior::demoKickingCircle() {
    // Parameters for circle
    VecPosition center = VecPosition(HALF_FIELD_X, 0, 0);
    

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int i =WO_OPPONENT1; i < WO_OPPONENT1+4; ++i)
    {
        
        int playerNum = i - WO_OPPONENT1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } 
        else 
        {
            WorldObject* opponent = worldModel->getWorldObject( i );
            if (opponent->validPosition) 
            {
                temp = opponent->pos;
            } 
            else
            {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) 
        {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    if(me.getDistanceTo(temp)<0.25)
    {
        return SKILL_STAND;
    }

    if (if(me.getDistanceTo(ball)<closestDistanceToBall)) {
        // Have closest player kick the ball toward the center
        return kickBall(KICK_FORWARD, center);
    } 
    else {
        // If Player is away from the ball then approach the ball
    if(me.getDistanceTo(ball)>2)
    {
        return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());
    }

    if(me.getDistanceTo(center)<1&&me.getDistanceTo(ball)<0.5)
    {
        return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0,0));
    }


     
    kickTarget = VecPosition(HALF_FIELD_X, 0, 0);     

    kickDirection = (kickTarget - ball).normalize();

    kickType = KICK_DRIBBLE;

    if (me.getDistanceTo(ball) > 1) {
        // Far away from the ball so walk toward target offset from the ball
        VecPosition approachBallTarget = ball - kickDirection*atof(namedParams.find("drib_target")->second.c_str());
        return goToTarget(approachBallTarget);
    }
    
    return kickBallAtPresetTarget();
        }
    }
}


