#ifndef COST_HPP_
#define COST_HPP_

struct Cost {
  int insertCost;
  int deleteCost;
  int replaceCost;

  Cost() : insertCost(1), deleteCost(1), replaceCost(1) {}
  Cost(int insertCost, int deleteCost, int replaceCost)
      : insertCost(insertCost),
        deleteCost(deleteCost),
        replaceCost(replaceCost) {}
};

#endif  // COST_HPP_