using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AnimationEditor
{
    public class Timer
    {
        //data members
        private int prevTime;
        private int currTime;
        private float deltaTime;
        //constructors
        public Timer() 
        {
            currTime = Environment.TickCount;
            prevTime = currTime;
            DeltaTime = 0.0f;
        }
        //encapulations
        public float DeltaTime
        {
            get { return deltaTime; }
            set { deltaTime = value; }
        }
        //functions
        public void Update()
        {
            currTime = Environment.TickCount;
            //Get difference of time in milliseconds
            int diffTime = currTime - prevTime;
            //convert to a float in milliseconds
            DeltaTime = (float)(diffTime * 0.001f);
            //Set the prev time
            prevTime = currTime;
        }
    }
}
