#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Helper function prototype
bool helper(const AvailabilityMatrix& avail, 
               const size_t dailyNeed,
               const size_t maxShifts,
               DailySchedule& sched,
               std::vector<size_t>& numShifts,
               size_t day, size_t slot);

// Schedule function
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0){
        return false;
    }
    sched.clear();

    // Add your code below

    size_t numDays = avail.size(); 
    size_t numWorkers = avail[0].size();

    // initialize schedule with n days with d invalid (empty) slots each
    for (size_t i = 0; i < numDays; ++i)
    {
        std::vector<Worker_T> daySched(dailyNeed, INVALID_ID);
        sched.push_back(daySched);
    }

    // initialize vec to keep count of how many shifts each worker has so far 
    std::vector<size_t> numShifts(numWorkers, 0);

    // recursive call 
    return helper(avail, dailyNeed, maxShifts, sched, numShifts, 0, 0);

}


// Helper function implementation
bool helper(const AvailabilityMatrix& avail, 
               const size_t dailyNeed,
               const size_t maxShifts,
               DailySchedule& sched,
               std::vector<size_t>& numShifts,
               size_t day, size_t slot)
{
    size_t numDays = avail.size(); 
    size_t numWorkers = avail[0].size(); 

    // base case: all days are scheduled
    if (day == numDays)
    {
        return true; 
    }

    // day is filled, move onto the next one
    if (slot == dailyNeed)
    {
        return helper(avail, dailyNeed, maxShifts, sched, numShifts, day + 1, 0);
    }

    // try each worker for slot on current day 
    for (Worker_T worker = 0; worker < numWorkers; ++worker)
    {
        // make sure worker is available on day, have not exceeded shift limit
        // also make sure worker is not already scheduled for current day
        if (avail[day][worker] && numShifts[worker] < maxShifts
            && std::find(sched[day].begin(), sched[day].end(), worker) == sched[day].end())
        {
            // assign worker to slot 
            sched[day][slot] = worker; 
            numShifts[worker]++; 

            // recursive call to check the next slot 
            if (helper(avail, dailyNeed, maxShifts, sched, numShifts, day, slot + 1))
            {
                return true; 
            }

            // backtrack if assignment did not work out 
            numShifts[worker]--;
            sched[day][slot] = INVALID_ID;
        }
    }

    // cannot find a way to fit worker into this slot 
    return false; 
}

