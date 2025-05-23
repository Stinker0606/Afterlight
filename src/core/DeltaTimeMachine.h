//
// Created by Kruse on 10/04/2025.
//

#ifndef RAYLIBSTARTER_DELTATIMEMACHINE_H
#define RAYLIBSTARTER_DELTATIMEMACHINE_H

#include <chrono>

namespace DT {
    struct timemachine {           //if you want to use delta Time you have to before starting your game loop make an Object timemachine and call its start function
                                   //do not call the start function inside the game loop as this will destroy your reference point
                                   //then at the end of your game loop you call the update function
                                   // if at any point in the game loop you need the delta time variable call the getdt function which will give you a float
        double dt=1;
        typedef std::chrono::time_point<std::chrono::system_clock,std::chrono::duration<long long,std::ratio<1,1000000000>>> ctime;
        ctime frame1;
        ctime frame2;


        void Start();

        void Update();

        float Get_Dt();

    };
}
#endif //RAYLIBSTARTER_DELTATIMEMACHINE_H
