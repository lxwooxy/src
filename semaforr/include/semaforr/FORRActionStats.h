/*!
 * FORRActionStats.h
 *
 * Class that collects data about a decision
 *
 * \author Raj Korpan <rkorpan@gradcenter.cuny.edu>
 */
#ifndef FORRACTIONSTATS_H
#define FORRACTIONSTATS_H

#include <string>
#include <iostream>
#include <set>

class FORRActionStats {

  public:
    int decisionTier;
    std::string vetoedActions;
    std::string advisors;
    std::string advisorComments;

    FORRActionStats(int decTier, std::string vActions, std::string adv, std::string advComments) : decisionTier(decTier), vetoedActions(vActions), advisors(adv), advisorComments(advComments) {};
    FORRActionStats(): decisionTier(0), vetoedActions(" "), advisors(" "), advisorComments(" ") {};

};


#endif
