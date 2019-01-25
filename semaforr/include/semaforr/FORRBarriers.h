/************************************************
FORRBarriers.h 
This file contains the class which contains information about the Barriers that FORR learns and uses

Written by Raj Korpan, adapted from Sarah Mathew and Gil Dekel, 2018
**********************************************/

#ifndef FORRBARRIERS_H
#define FORRBARRIERS_H

#include <FORRGeometry.h>
#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include <cmath>        //for atan2 and M_PI
#include <algorithm>
#include <map>

using namespace std;

//=========================================================//=========================================================//

/* FORRBarriers class
 *
 * FORRBarriers represent the Barriers that are learned by SemaFORR.
 *
 */
class FORRBarriers{
public:
    FORRBarriers(){
        barriers = vector<LineSegment>();
    };
    vector<LineSegment> getBarriers(){return barriers;}
    ~FORRBarriers(){};

    void clearAllBarriers(){
        barriers.clear();
    }

    void updateBarriers(vector< vector<CartesianPoint> > *laser_hist, CartesianPoint curr_pos) {
        laser_history.clear();
        for(int i = 0 ; i < laser_hist->size() ; i++){
          laser_history.push_back((*laser_hist)[i]);
        }
        current_position = curr_pos;

        vector<LineSegment> laser_segments;
        CreateSegments(laser_segments, current_position, laser_history);
        cout << "num of segments " << laser_segments.size() << endl;

        for(int i = 0 ; i < barriers.size() ; i++){
          laser_segments.push_back(barriers[i]);
        }
        vector<vector<double> > segments_similarities;
        ListSimilarities(segments_similarities, laser_segments);
        cout << "num of segments similarities " << segments_similarities.size() << endl;

        vector<vector<double> > most_similar_segments;
        FindMostSimilarSegments(most_similar_segments, segments_similarities);
        cout << "num of most similar segments " << most_similar_segments.size() << endl;

        vector<LineSegment> initial_barriers;
        CreateInitialSegments(initial_barriers, most_similar_segments, laser_segments);
        cout << "num of initial_barriers " << initial_barriers.size() << endl;

        vector<LineSegment> merged_barriers;
        MergeNearbyBarriers(merged_barriers, initial_barriers);
        barriers = merged_barriers;
    }

private:
    vector<LineSegment> barriers;
    vector<vector<CartesianPoint> > laser_history;
    CartesianPoint current_position;

    void CreateSegments(vector<LineSegment> &segments, CartesianPoint current_position, vector < vector <CartesianPoint> > laser_history);

    void ListSimilarities(vector<vector<double> > &similarities, const vector<LineSegment> &laser_segments);
    double ComputeDistance(LineSegment first_segment, LineSegment second_segment);

    void FindMostSimilarSegments(vector<vector<double> > &most_similar,const vector<vector<double> > &similarities);

    void CreateInitialSegments(vector<LineSegment> &initial_barriers,const vector<vector<double> > &most_similar,const vector<LineSegment> &segments);

    void MergeNearbyBarriers(vector<LineSegment> &merged_barriers, const vector<LineSegment> &initial_barriers);
};

#endif
