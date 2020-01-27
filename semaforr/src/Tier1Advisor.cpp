
#include "Tier1Advisor.h"


void Tier1Advisor::advisorNotOpposite(){
  ROS_DEBUG("COntroller::advisorNotOpposite > Entering function");
  vector<FORRAction> actions = beliefs->getAgentState()->getCurrentTask()->getPreviousDecisions();
  set<FORRAction> *rotation_set = beliefs->getAgentState()->getRotationActionSet();
  int size = actions.size();
  if(actions.size() < 2){
    ROS_DEBUG("actions list less than 2. Exiting not opposite");
    return;
  }
  FORRAction lastAction = actions[size - 1];
  FORRAction lastlastAction = actions[size - 2];
  FORRAction lastlastlastAction = actions[size - 3];
  ROS_DEBUG_STREAM("Controller::advisorNotOpposite > " << lastAction.type << " " << lastAction.parameter << ", " << lastlastAction.type << " " << lastlastAction.parameter << ", " << lastlastlastAction.type << " " << lastlastlastAction.parameter); 
  if(((lastlastAction.type == RIGHT_TURN or lastlastAction.type == LEFT_TURN) and lastAction.type == PAUSE) or lastAction.type == RIGHT_TURN or lastAction.type == LEFT_TURN){
    ROS_DEBUG("Not opposite active ");
    if((lastlastAction.type == RIGHT_TURN and lastAction.type == PAUSE) or lastAction.type == RIGHT_TURN or (lastlastAction.type == LEFT_TURN and lastAction.type == LEFT_TURN)){
      for(int i = 1; i < rotation_set->size()/2+1 ; i++){
        (beliefs->getAgentState()->getVetoedActions()->insert(FORRAction(LEFT_TURN, i)));
        ROS_DEBUG_STREAM("Vetoed action : " << FORRAction(LEFT_TURN, i).type << " " << i);
      }
    }
    if((lastlastAction.type == LEFT_TURN and lastAction.type == PAUSE) or lastAction.type == LEFT_TURN or (lastlastAction.type == RIGHT_TURN and lastAction.type == RIGHT_TURN)){
      for(int i = 1; i < rotation_set->size()/2+1 ; i++){
        (beliefs->getAgentState()->getVetoedActions()->insert(FORRAction(RIGHT_TURN, i)));
        ROS_DEBUG_STREAM("Vetoed action : " << FORRAction(RIGHT_TURN, i).type << " " << i);
      }
    }
  }
  // if(lastlastAction.type == RIGHT_TURN or lastlastAction.type == LEFT_TURN){
  //   if(lastAction.type == PAUSE){
  //     ROS_DEBUG("Not opposite active ");
  //     if(lastlastAction.type == RIGHT_TURN)    for(int i = 1; i < rotation_set->size()/2+1 ; i++)   (beliefs->getAgentState()->getVetoedActions()->insert(FORRAction(LEFT_TURN, i)));
  //     else                                     for(int i = 1; i < rotation_set->size()/2+1 ; i++)   (beliefs->getAgentState()->getVetoedActions()->insert(FORRAction(RIGHT_TURN, i)));
  //   }
  // }
  ROS_DEBUG("leaving notOpposite");
  return;
}

/*
// This advisor should ban all forward moves not in the direction of the exit unless all of the exits
// are already blocked by other robots
void Tier1Advisor::advisorCircle(Beliefs *beliefs){

  // Initialize variables
  vector<FORRCircle> circles = (beliefs->abstractMap).getCircles();
  Position curr_pos = beliefs->getCurrentPosition();
  Task *task = beliefs->getCurrentTask();
  CartesianPoint targetPoint (task->getX() , task->getY());
  CartesianPoint currentPosition (curr_pos.getX(), curr_pos.getY());
  bool targetInCircle = false;
  bool currPosInCircleWithExit = false;
  int robotCircle=-1, targetCircle = -1;
  //cout << "Controller::advisorCircle >> Initializing variables " << endl;
  // check the preconditions for activating the advisor
  for(int i = 0; i < circles.size() ; i++){
    // check if the target point is in circle
    if(circles[i].inCircle(targetPoint.get_x(), targetPoint.get_y())){
      targetInCircle = true;
      targetCircle = i;
    }
    // check if the rob_pos is in a circle and the circle has atleast one exit
    if(circles[i].inCircle(curr_pos.getX(), curr_pos.getY()) and ((circles[i]).getExits().size() >= 1)){
      currPosInCircleWithExit = true;
      robotCircle = i;
    }
  }

  // if preconditions are met, veto forward moves in the direction of non exits
  if(targetInCircle == true and currPosInCircleWithExit == true and robotCircle != targetCircle){
    //cout << "Controller::advisorCircle >> Activating tier 1 get out of circle advisor" << endl;
    vector<FORRExit> exits = circles[robotCircle].getExits(); 
    bool facingExit = false;
    double forward_distance = beliefs->wallDistanceVector[0];
    for(int i = 0; i< exits.size(); i++){
      CartesianPoint exitPoint = exits[i].getExitPoint();
      double exitDistance = Utils::get_euclidian_distance(exitPoint.get_x(), exitPoint.get_y(), curr_pos.getX() , curr_pos.getY());
      if(!(beliefs->abstractMap).isExitToLeaf(exits[i]) || exits[i].getExitCircle() == targetCircle){
	if( ( isFacing(curr_pos, exitPoint, circles[robotCircle].getRadius()) and forward_distance - exitDistance > 20 ) or exitDistance < 40 ){
	  //cout << "Controller::advisorCircle >> Robot is facing exit " << exitPoint.get_x() << exitPoint.get_y() << endl;
	  facingExit = true;
	  break;
	}
      }
    }
    // Robot is not facing any of the exits ban all forward moves
    if(facingExit == false){
      //cout << "Controller::advisorCircle >> Vetoing all forward moves" << endl;
      for(int i = 1; i < 6; ++i){
	(beliefs->vetoedActions)->insert(FORRAction(FORWARD, i));
      }
    }
  }
  return;
}

bool Tier1Advisor::isFacing(Position robotPos , CartesianPoint point, double radius){
  bool isFacing = false;
  double robot_point_angle = atan2(point.get_y() - robotPos.getY(), point.get_x() - robotPos.getX());
  double angleDiff = robotPos.getTheta() - robot_point_angle;
  //cout << "In function isFacing " << endl;
  //cout << "Robot angle " << robotPos.getTheta() << endl;
  //cout << "Angle made by the robot and the position with x axis" << robot_point_angle << endl;
  double ratio = Utils::get_euclidian_distance(robotPos.getX(), robotPos.getY() ,point.get_x(), point.get_y())/(2*radius);
  double min_delta = 0.72; //30 degrees on each side
  double max_delta = 3.14/(2); //60 degrees on each side
  double delta = ratio*(min_delta) + (1-ratio)*(max_delta) ;

  if(abs(angleDiff) < delta){
    isFacing = true;
  }
  return isFacing;
}


*/

bool Tier1Advisor::advisorVictory(FORRAction *decision) {
  ROS_DEBUG("Begin victory advisor");
  // if the robot is oriented towards the goal and the robot actions which are not vetoed allows the robot to reach the goal then take that action.
  bool decisionMade = false;
  CartesianPoint task(beliefs->getAgentState()->getCurrentTask()->getTaskX(),beliefs->getAgentState()->getCurrentTask()->getTaskY());
  ROS_DEBUG("Check if target can be spotted using laser scan");
  cout << "Target = " << task.get_x() << " " << task.get_y() << endl;
  bool targetInSight = beliefs->getAgentState()->canSeePoint(task, 20);
  if(targetInSight == false){
    ROS_DEBUG("Target not in sight, check if waypoint can be spotted using laser scan");
    CartesianPoint waypoint(beliefs->getAgentState()->getCurrentTask()->getX(),beliefs->getAgentState()->getCurrentTask()->getY());
    cout << "Waypoint = " << waypoint.get_x() << " " << waypoint.get_y() << endl;
    bool waypointInSight = beliefs->getAgentState()->canSeePoint(waypoint, 20);
    if(waypointInSight == false){
      ROS_DEBUG("Waypoint not in sight, Victory advisor skipped");
    }
    else{
      ROS_DEBUG("Waypoint in sight , victory advisor active");
      (*decision) = beliefs->getAgentState()->moveTowards(waypoint);
      FORRAction forward = beliefs->getAgentState()->maxForwardAction();
      Position expectedPosition = beliefs->getAgentState()->getExpectedPositionAfterAction((*decision));
      if(((decision->type == RIGHT_TURN or decision->type == LEFT_TURN) or (forward.parameter >= decision->parameter)) and decision->parameter != 0 and expectedPosition.getDistance(beliefs->getAgentState()->getCurrentPosition()) >= 0.1){
        ROS_DEBUG("Waypoint in sight and no obstacles, victory advisor to take decision");
        decisionMade = true;
        Position currentPosition = beliefs->getAgentState()->getCurrentPosition();
        beliefs->getAgentState()->getCurrentTask()->updatePlanPositions(currentPosition.getX(), currentPosition.getY());
        // if(decision->type == FORWARD)
        //   beliefs->getAgentState()->setGetOutTriggered(false);
      }
    }
  }
  else{
    ROS_DEBUG("Target in sight , victory advisor active");
    (*decision) = beliefs->getAgentState()->moveTowards(task);
    FORRAction forward = beliefs->getAgentState()->maxForwardAction();
    Position expectedPosition = beliefs->getAgentState()->getExpectedPositionAfterAction((*decision));
    if(((decision->type == RIGHT_TURN or decision->type == LEFT_TURN) or (forward.parameter >= decision->parameter)) and decision->parameter != 0 and expectedPosition.getDistance(beliefs->getAgentState()->getCurrentPosition()) >= 0.1){
      ROS_DEBUG("Target in sight and no obstacles, victory advisor to take decision");
      decisionMade = true;
      Position currentPosition = beliefs->getAgentState()->getCurrentPosition();
      // beliefs->getAgentState()->getCurrentTask()->updatePlanPositions(currentPosition.getX(), currentPosition.getY());
      // if(decision->type == FORWARD)
      //   beliefs->getAgentState()->setGetOutTriggered(false);
    }
    /*vector<FORRAction> actions = beliefs->getAgentState()->getCurrentTask()->getPreviousDecisions();
    if(actions.size() > 1){
      FORRAction lastAction = actions[actions.size() - 1];
      if((lastAction.type == RIGHT_TURN or lastAction.type == LEFT_TURN) and (decision->type == RIGHT_TURN or decision->type == LEFT_TURN)){
        decisionMade = false;
      }
      else if((lastAction.type == FORWARD or lastAction.type == PAUSE) and (decision->type == FORWARD or decision->type == PAUSE)){
        decisionMade = false;
      }
      else if(((decision->type == RIGHT_TURN or decision->type == LEFT_TURN) or (forward.parameter >= decision->parameter)) and decision->parameter != 0){
        ROS_DEBUG("Target in sight and no obstacles , victory advisor to take decision");
        decisionMade = true;
      }
    }*/
  }
  return decisionMade;
}


/*
 * This advisor has to do prevent all the actions that will result in robot hitting the obstacles.
 * Straight forward thing is to check for collsion in the orientation.
 */
bool Tier1Advisor::advisorAvoidWalls(){
  ROS_DEBUG("In advisor avoid walls");
  FORRAction max_forward = beliefs->getAgentState()->maxForwardAction();
  ROS_DEBUG_STREAM("Max allowed forward action : " << max_forward.type << " " << max_forward.parameter);
  int intensity = max_forward.parameter;
  set<FORRAction> *vetoedActions = beliefs->getAgentState()->getVetoedActions();
  set<FORRAction> *forward_set = beliefs->getAgentState()->getForwardActionSet();
  for(int i = forward_set->size()-1 ; i > 0; i--){
	FORRAction a(FORWARD,i);
	if(i > intensity){
		ROS_DEBUG_STREAM("Vetoed action : " << a.type << " " << a.parameter);
		vetoedActions->insert(a);
	}				
  } 
  return false; 
}

bool Tier1Advisor::advisorDontGoBack(){
  ROS_DEBUG("In advisor don't go back");
  set<FORRAction> *vetoedActions = beliefs->getAgentState()->getVetoedActions();
  set<FORRAction> *action_set = beliefs->getAgentState()->getActionSet();
  set<FORRAction>::iterator actionIter;
  for(actionIter = action_set->begin(); actionIter != action_set->end(); actionIter++){
    FORRAction forrAction = *actionIter;
    if(std::find(vetoedActions->begin(), vetoedActions->end(), forrAction) != vetoedActions->end()){
      continue;
    }
    else if(forrAction.type == PAUSE or forrAction.type == FORWARD){
      continue;
    }
    else{
      Position expectedPosition = beliefs->getAgentState()->getExpectedPositionAfterAction(forrAction);
      if(beliefs->getAgentState()->getCurrentTask()->getPlanPositionValue(expectedPosition.getX(), expectedPosition.getY())){
        FORRAction a(forrAction.type,forrAction.parameter);
        ROS_DEBUG_STREAM("Vetoed action : " << a.type << " " << a.parameter);
        vetoedActions->insert(a);
      }
    }
  }
  cout << "Don't go back number of vetoes" << vetoedActions->size() << " " << action_set->size() << endl;
  // if(vetoedActions->size()+1 == action_set->size()){
  //   beliefs->getAgentState()->getCurrentTask()->resetPlanPositions();
  // }
  return false; 
}

bool Tier1Advisor::advisorSituation(){
  ROS_DEBUG("In advisor situation");
  // Find closest situation based on similarity to median
  set<FORRAction> *vetoedActions = beliefs->getAgentState()->getVetoedActions();
  set<FORRAction> *action_set = beliefs->getAgentState()->getActionSet();
  double accuracy = beliefs->getSpatialModel()->getSituations()->getAccuracyForSituation(beliefs->getAgentState());
  cout << "Situation accuracy: " << accuracy << endl;
  if(accuracy >= 0.75){
    set<FORRAction>::iterator actionIter;
    for(actionIter = action_set->begin(); actionIter != action_set->end(); actionIter++){
      FORRAction forrAction = *actionIter;
      if(std::find(vetoedActions->begin(), vetoedActions->end(), forrAction) != vetoedActions->end()){
        continue;
      }
      else if(forrAction.type == PAUSE){
        continue;
      }
      else{
        double action_weight = beliefs->getSpatialModel()->getSituations()->getWeightForAction(beliefs->getAgentState(), forrAction);
        cout << "action_weight " << action_weight << endl;
        if(action_weight < 0.25){
          FORRAction a(forrAction.type,forrAction.parameter);
          ROS_DEBUG_STREAM("Vetoed action : " << a.type << " " << a.parameter);
          vetoedActions->insert(a);
        }
      }
    }
  }
  // vector<int> current_situation = beliefs->getSpatialModel()->getSituations()->identifySituation(beliefs->getAgentState()->getCurrentLaserScan());
  // vector< vector<int> > grid;
  // for(int i = 0; i < 51; i++){
  //   vector<int> col;
  //   for(int j = 0; j < 51; j++){
  //     col.push_back(0);
  //   }
  //   grid.push_back(col);
  // }
  // for (int i = 0; i < current_situation.size(); i++){
  //   int row = i / 51 + 16;
  //   int col = i %51;
  //   grid[row][col] = current_situation[i];
  // }
  // for(int i = 16; i < grid.size(); i++){
  //   for(int j = 0; j < grid[i].size(); j++){
  //     cout << grid[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // // Veto actions not allowed based on situation
  // set<FORRAction> *vetoedActions = beliefs->getAgentState()->getVetoedActions();
  // set<FORRAction> *action_set = beliefs->getAgentState()->getActionSet();
  // Position initialPosition = beliefs->getAgentState()->getCurrentPosition();
  // set<FORRAction>::iterator actionIter;
  // for(actionIter = action_set->begin(); actionIter != action_set->end(); actionIter++){
  //   FORRAction forrAction = *actionIter;
  //   if(std::find(vetoedActions->begin(), vetoedActions->end(), forrAction) != vetoedActions->end()){
  //     continue;
  //   }
  //   else{
  //     Position expectedPosition = beliefs->getAgentState()->getExpectedPositionAfterAction(forrAction);
  //     double x = 25.0 - initialPosition.getX() + expectedPosition.getX();
  //     double y = 25.0 - initialPosition.getY() + expectedPosition.getY();
  //     int x1 = (int)(round(x));
  //     int y1 = (int)(round(y));
  //     int x2 = (int)(x);
  //     int y2 = (int)(y);
  //     int x3 = (int)(floor(x));
  //     int y3 = (int)(floor(y));
  //     int x4 = (int)(ceil(x));
  //     int y4 = (int)(ceil(y));
  //     // cout << initialPosition.getX() << " " << initialPosition.getY() << " " << expectedPosition.getX() << " " << expectedPosition.getY() << " " << x << " " << y << " " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << " " << x4 << " " << y4 << " " << grid[x1][y1] << " " << grid[x2][y2] << " " << grid[x3][y3] << " " << grid[x4][y4] << endl;
  //     if(grid[x1][y1] == 0 or grid[x2][y2] == 0 or grid[x3][y3] == 0 or grid[x4][y4] == 0){
  //       FORRAction a(forrAction.type,forrAction.parameter);
  //       ROS_DEBUG_STREAM("Vetoed action : " << a.type << " " << a.parameter);
  //       vetoedActions->insert(a);
  //     }
  //   }
  // }
  return false;
}

bool Tier1Advisor::advisorGetOut(FORRAction *decision) {
  ROS_DEBUG("Begin get out advisor");
  // if the robot is in a confined space and it sees a way out then take the action that does that.
  bool decisionMade = false;
  if(beliefs->getAgentState()->getRobotConfined(20, beliefs->getAgentState()->getDistanceToNearestObstacle(beliefs->getAgentState()->getCurrentPosition()), 20)){
    vector<FORRAction> actions = beliefs->getAgentState()->getCurrentTask()->getPreviousDecisions();
    set<FORRAction> *rotation_set = beliefs->getAgentState()->getRotationActionSet();
    int size = actions.size();
    // cout << "actions size " << size << " rotation size " << rotation_set->size()/2 << endl;
    if(size >= 5){
      FORRAction lastAction = actions[size - 1];
      FORRAction lastlastAction = actions[size - 2];
      FORRAction lastlastlastAction = actions[size - 3];
      FORRAction lastlastlastlastAction = actions[size - 4];
      // cout << "lastAction " << lastAction.type << " " << lastAction.parameter << " lastlastAction " <<  lastlastAction.type << " " << lastlastAction.parameter << " lastlastlastAction " <<  lastlastlastAction.type << " " << lastlastlastAction.parameter<< endl;
      if(lastlastAction.type == RIGHT_TURN and lastlastAction.parameter == rotation_set->size()/2 and lastAction.type == RIGHT_TURN and lastAction.parameter == rotation_set->size()/2 and lastlastlastAction.type == RIGHT_TURN and lastlastlastAction.parameter == rotation_set->size()/2 and lastlastlastlastAction.type == RIGHT_TURN and lastlastlastlastAction.parameter == rotation_set->size()/2){
        vector< sensor_msgs::LaserScan > laser_scan_hist = beliefs->getAgentState()->getAllLaserScanHistory();
        vector<Position> *positionHis = beliefs->getAgentState()->getCurrentTask()->getPositionHistory();
        CartesianPoint current_position = CartesianPoint((*positionHis)[positionHis->size()-1].getX(), (*positionHis)[positionHis->size()-1].getY());
        // cout << "current_position " << current_position.get_x() << " " << current_position.get_y() << endl;
        vector<Position> last_positions;
        vector< sensor_msgs::LaserScan > last_lasers;
        vector< vector<CartesianPoint> > last_endpoints;
        int previous_count = 4;
        for(int i = 1; i < previous_count+1; i++){
          last_positions.push_back((*positionHis)[positionHis->size()-i]);
          last_lasers.push_back(laser_scan_hist[laser_scan_hist.size()-i]);
          last_endpoints.push_back(beliefs->getAgentState()->transformToEndpoints((*positionHis)[positionHis->size()-i], laser_scan_hist[laser_scan_hist.size()-i]));
        }
        vector< vector<int> > grid = beliefs->getSpatialModel()->getSituations()->overlaySituations(last_lasers, last_positions);
        double cos_curr = cos((*positionHis)[positionHis->size()-1].getTheta());
        double sin_curr = sin((*positionHis)[positionHis->size()-1].getTheta());
        double x_curr = (*positionHis)[positionHis->size()-1].getX();
        double y_curr = (*positionHis)[positionHis->size()-1].getY();
        vector<CartesianPoint> grid_potential_destinations;
        for(int i = 1; i < grid.size()-1; i++){
          for(int j = 1; j < grid[i].size()-1; j++){
            int neigh = 0;
            if(grid[i-1][j] == -1)
              neigh++;
            if(grid[i][j-1] == -1)
              neigh++;
            if(grid[i+1][j] == -1)
              neigh++;
            if(grid[i][j+1] == -1)
              neigh++;
            if(grid[i][j] == 1 and neigh < 3){
              grid_potential_destinations.push_back(CartesianPoint(i,j));
              cout << "Potential grid position " << i << " " << j << endl;
            }
          }
        }
        CartesianPoint current_grid_position = CartesianPoint(25,25);
        CartesianPoint farthest_grid_position;
        double max_distance = 0;
        for(int i = 0; i < grid_potential_destinations.size(); i++){
          double dist_to_potential = grid_potential_destinations[i].get_distance(current_grid_position);
          if(dist_to_potential > max_distance){
            max_distance = dist_to_potential;
            farthest_grid_position = grid_potential_destinations[i];
          }
        }
        cout << "farthest_grid_position " << farthest_grid_position.get_x() << " " << farthest_grid_position.get_y() << endl;
        vector<CartesianPoint> sequence_of_grid_positions;
        while(!(current_grid_position == farthest_grid_position)){
          int curr_x = current_grid_position.get_x();
          int curr_y = current_grid_position.get_y();
          if(grid[curr_x][curr_y] == 1){
            sequence_of_grid_positions.push_back(current_grid_position);
            cout << "current_grid_position " << current_grid_position.get_x() << " " << current_grid_position.get_y() << endl;
          }
          vector<CartesianPoint> free_neighbors;
          if(grid[curr_x+1][curr_y] == 1){
            free_neighbors.push_back(CartesianPoint(curr_x+1,curr_y));
          }
          if(grid[curr_x][curr_y+1] == 1){
            free_neighbors.push_back(CartesianPoint(curr_x,curr_y+1));
          }
          if(grid[curr_x-1][curr_y] == 1){
            free_neighbors.push_back(CartesianPoint(curr_x-1,curr_y));
          }
          if(grid[curr_x][curr_y-1] == 1){
            free_neighbors.push_back(CartesianPoint(curr_x,curr_y-1));
          }
          if(free_neighbors.size() == 0){
            if(grid[curr_x+1][curr_y+1] == 1){
            free_neighbors.push_back(CartesianPoint(curr_x+1,curr_y+1));
            }
            if(grid[curr_x-1][curr_y+1] == 1){
              free_neighbors.push_back(CartesianPoint(curr_x-1,curr_y+1));
            }
            if(grid[curr_x+1][curr_y-1] == 1){
              free_neighbors.push_back(CartesianPoint(curr_x+1,curr_y-1));
            }
            if(grid[curr_x-1][curr_y-1] == 1){
              free_neighbors.push_back(CartesianPoint(curr_x-1,curr_y-1));
            }
          }
          CartesianPoint closest_neighbor;
          double min_to_farthest = 100000;
          for(int i = 0; i < free_neighbors.size(); i++){
            double dist_to_farthest = free_neighbors[i].get_distance(farthest_grid_position);
            if(dist_to_farthest < min_to_farthest){
              min_to_farthest = dist_to_farthest;
              closest_neighbor = free_neighbors[i];
            }
          }
          current_grid_position = closest_neighbor;
        }
        sequence_of_grid_positions.push_back(farthest_grid_position);
        cout << "Length of plan to leave " << sequence_of_grid_positions.size() << endl;
        vector<CartesianPoint> sequence_of_positions;
        for(int i = 0; i < sequence_of_grid_positions.size(); i++){
          double new_x = (sequence_of_grid_positions[i].get_x()-25)*cos_curr - (sequence_of_grid_positions[i].get_y()-25)*sin_curr + x_curr;
          double new_y = (sequence_of_grid_positions[i].get_y()-25)*cos_curr + (sequence_of_grid_positions[i].get_x()-25)*sin_curr + y_curr;
          cout << "pos " << i << " " << new_x << " " << new_y << endl;
          sequence_of_positions.push_back(CartesianPoint(new_x, new_y));
        }
        for(int i = sequence_of_positions.size()-1; i >= 0; --i){
          beliefs->getAgentState()->getCurrentTask()->createNewWaypoint(sequence_of_positions[i]);
        }
      }
      else{
        // cout << "else make turn" << endl;
        (*decision) = FORRAction(RIGHT_TURN, rotation_set->size()/2);
        decisionMade = true;
      }
    }
  }
  cout << "decisionMade " << decisionMade << endl;
  return decisionMade;

  // bool decisionMade = false;
  // if(beliefs->getAgentState()->getGetOutTriggered()){
  //   // cout << "current_position " << beliefs->getAgentState()->getCurrentPosition().getX() << " " << beliefs->getAgentState()->getCurrentPosition().getY() << " farthest_position " << beliefs->getAgentState()->getFarthestPoint().get_x() << " " << beliefs->getAgentState()->getFarthestPoint().get_y() << endl;
  //   if(beliefs->getAgentState()->getFarthestPoint().get_distance(CartesianPoint(beliefs->getAgentState()->getCurrentPosition().getX(), beliefs->getAgentState()->getCurrentPosition().getY())) < 1.0){
  //     // cout << "farthest_position achieved" << endl;
  //     beliefs->getAgentState()->setGetOutTriggered(false);
  //   }
  //   // else if(beliefs->getAgentState()->canSeePoint(beliefs->getAgentState()->getFarthestPoint(), 30)){
  //   else{
  //     // cout << "farthest_position not achieved" << endl;
  //     (*decision) = beliefs->getAgentState()->moveTowards(beliefs->getAgentState()->getFarthestPoint());
  //     FORRAction forward = beliefs->getAgentState()->maxForwardAction();
  //     Position expectedPosition = beliefs->getAgentState()->getExpectedPositionAfterAction((*decision));
  //     if(((decision->type == RIGHT_TURN or decision->type == LEFT_TURN) or (forward.parameter >= decision->parameter)) and decision->parameter != 0 and expectedPosition.getDistance(beliefs->getAgentState()->getCurrentPosition()) >= 0.25){
  //       ROS_DEBUG("farthest_position in sight and no obstacles, get out advisor to take decision");
  //       decisionMade = true;
  //     }
  //     // else{
  //     //   beliefs->getAgentState()->setGetOutTriggered(false);
  //     // }
  //   }
  //   // else{
  //   //   cout << "else make small turn" << endl;
  //   //   (*decision) = FORRAction(RIGHT_TURN, 2);
  //   //   decisionMade = true;
  //   // }
  //   if(decisionMade == false and beliefs->getAgentState()->getGetOutTriggered() == true){
  //     // cout << "stuck in place" << endl;
  //     beliefs->getAgentState()->setGetOutTriggered(false);
  //   }
  // }
  // else if(beliefs->getAgentState()->getRobotConfined(50, max(beliefs->getAgentState()->getDistanceToNearestObstacle(beliefs->getAgentState()->getCurrentPosition()), 1.0 + beliefs->getAgentState()->getCurrentTask()->getDecisionCount()/500.0), 20)){
  //   vector<FORRAction> actions = beliefs->getAgentState()->getCurrentTask()->getPreviousDecisions();
  //   set<FORRAction> *rotation_set = beliefs->getAgentState()->getRotationActionSet();
  //   int size = actions.size();
  //   // cout << "actions size " << size << " rotation size " << rotation_set->size()/2 << endl;
  //   if(size >= 4){
  //     FORRAction lastAction = actions[size - 1];
  //     FORRAction lastlastAction = actions[size - 2];
  //     FORRAction lastlastlastAction = actions[size - 3];
  //     // cout << "lastAction " << lastAction.type << " " << lastAction.parameter << " lastlastAction " <<  lastlastAction.type << " " << lastlastAction.parameter << " lastlastlastAction " <<  lastlastlastAction.type << " " << lastlastlastAction.parameter<< endl;
  //     if(lastlastAction.type == RIGHT_TURN and lastlastAction.parameter == rotation_set->size()/2 and lastAction.type == RIGHT_TURN and lastAction.parameter == rotation_set->size()/2 and lastlastlastAction.type == RIGHT_TURN and lastlastlastAction.parameter == rotation_set->size()/2){
  //       // cout << "overlay turned poses" << endl;
  //       vector< sensor_msgs::LaserScan > laser_scan_hist = beliefs->getAgentState()->getAllLaserScanHistory();
  //       vector<Position> *positionHis = beliefs->getAgentState()->getCurrentTask()->getPositionHistory();
  //       CartesianPoint current_position = CartesianPoint((*positionHis)[positionHis->size()-1].getX(), (*positionHis)[positionHis->size()-1].getY());
  //       // cout << "current_position " << current_position.get_x() << " " << current_position.get_y() << endl;
  //       vector<Position> last_positions;
  //       vector< sensor_msgs::LaserScan > last_lasers;
  //       vector< vector<CartesianPoint> > last_endpoints;
  //       int previous_count = 3;
  //       if(size > 5){
  //         previous_count = 5;
  //       }
  //       for(int i = 1; i < previous_count+1; i++){
  //         last_positions.push_back((*positionHis)[positionHis->size()-i]);
  //         last_lasers.push_back(laser_scan_hist[laser_scan_hist.size()-i]);
  //         last_endpoints.push_back(beliefs->getAgentState()->transformToEndpoints((*positionHis)[positionHis->size()-i], laser_scan_hist[laser_scan_hist.size()-i]));
  //       }
  //       // for(int i = 0; i < last_endpoints.size(); i++){
  //       //   for(int j = 0; j < last_endpoints[i].size(); j++){
  //       //     cout << last_endpoints[i][j].get_x() << " " << last_endpoints[i][j].get_y() << endl;
  //       //   }
  //       // }
  //       vector< vector<int> > grid = beliefs->getSpatialModel()->getSituations()->overlaySituations(last_lasers, last_positions);
  //       double cos_curr = cos((*positionHis)[positionHis->size()-1].getTheta());
  //       double sin_curr = sin((*positionHis)[positionHis->size()-1].getTheta());
  //       double x_curr = (*positionHis)[positionHis->size()-1].getX();
  //       double y_curr = (*positionHis)[positionHis->size()-1].getY();
  //       vector<CartesianPoint> potential_destinations;
  //       for(int i = 0; i < grid.size(); i++){
  //         for(int j = 0; j < grid[i].size(); j++){
  //           if(grid[i][j] >= 1){
  //             double new_x = (i-25)*cos_curr - (j-25)*sin_curr + x_curr;
  //             double new_y = (j-25)*cos_curr + (i-25)*sin_curr + y_curr;
  //             int anyVisible = 0;
  //             for(int k = 0; k < last_endpoints.size(); k++){
  //               if(beliefs->getAgentState()->canSeePoint(last_endpoints[k], CartesianPoint(last_positions[k].getX(), last_positions[k].getY()), CartesianPoint(new_x, new_y), 10)){
  //                 anyVisible++;
  //               }
  //             }
  //             // cout << i << " " << j << " " << i-25 << " " << j-25 << " " << new_x << " " << new_y << " " << anyVisible << endl;
  //             if(anyVisible > 1 and !beliefs->getAgentState()->getCurrentTask()->getPlanPositionValue(new_x, new_y)){
  //               potential_destinations.push_back(CartesianPoint(new_x, new_y));
  //             }
  //           }
  //         }
  //       }
        
  //       CartesianPoint farthest_position;
  //       double max_distance = 0;
  //       for(int i = 0; i < potential_destinations.size(); i++){
  //         double dist_to_potential = 0;
  //         for(int j = 0; j < last_positions.size(); j++){
  //           dist_to_potential += potential_destinations[i].get_distance(CartesianPoint(last_positions[j].getX(), last_positions[j].getY()));
  //         }
  //         dist_to_potential = dist_to_potential / last_positions.size();
  //         if(dist_to_potential > max_distance and dist_to_potential < 10){
  //           max_distance = dist_to_potential;
  //           farthest_position = potential_destinations[i];
  //         }
  //       }
  //       if(max_distance > 0){
  //         // cout << "Farthest Dist " << max_distance << " farthest_position " << farthest_position.get_x() << " " << farthest_position.get_y() << endl;
  //         beliefs->getAgentState()->setGetOutTriggered(true, farthest_position);
  //         (*decision) = beliefs->getAgentState()->moveTowards(farthest_position);
  //         FORRAction forward = beliefs->getAgentState()->maxForwardAction();
  //         Position expectedPosition = beliefs->getAgentState()->getExpectedPositionAfterAction((*decision));
  //         if(((decision->type == RIGHT_TURN or decision->type == LEFT_TURN) or (forward.parameter >= decision->parameter)) and decision->parameter != 0 and expectedPosition.getDistance(beliefs->getAgentState()->getCurrentPosition()) >= 0.25){
  //           ROS_DEBUG("farthest_position in sight and no obstacles, get out advisor to take decision");
  //           decisionMade = true;
  //         }
  //       }
  //       else{
  //         // cout << "No available farthest_position" << endl;
  //         beliefs->getAgentState()->setGetOutTriggered(false);
  //         beliefs->getAgentState()->getCurrentTask()->resetPlanPositions();
  //       }
  //       if(beliefs->getAgentState()->getCurrentTask()->getWaypoints().size() == 0){
  //         // cout << "No plan exists, add new farthest_position as waypoint" << endl;
  //         beliefs->getAgentState()->getCurrentTask()->createNewWaypoint(farthest_position);
  //       }
  //     }
  //     else{
  //       // cout << "else make turn" << endl;
  //       beliefs->getAgentState()->setGetOutTriggered(false);
  //       (*decision) = FORRAction(RIGHT_TURN, rotation_set->size()/2);
  //       decisionMade = true;
  //     }
  //   }
  // }
  // cout << "decisionMade " << decisionMade << endl;
  // return decisionMade;
}