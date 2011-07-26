using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace AnimationEditor
{
    public class CFrame
    {
        //data members   
        private Point pAnchor;
        private Rectangle rDrawRect;
        private Rectangle rCollisionRect;
        private float fDuration;
        private string szTriggerID;
        //constructors
        public CFrame()
        {
            PAnchor = new Point(0, 0);
            RDrawRect = new Rectangle(0, 0, 8, 8);
            RCollisionRect = new Rectangle(0, 0, 8, 8);
            FDuration = 0.5f;
            SzTriggerID = "";
        }
        public CFrame(Point Anchor, Rectangle DrawRect,
                      Rectangle CollisionRect, float Duration, 
                      string TriggerID) 
        {
            PAnchor = Anchor;
            RDrawRect = DrawRect;
            RCollisionRect = CollisionRect;
            FDuration = Duration;
            SzTriggerID = TriggerID;
        }
        public CFrame(CFrame f) 
        {
            PAnchor = f.PAnchor;
            RDrawRect = f.RDrawRect;
            RCollisionRect = f.RCollisionRect;
            FDuration = f.FDuration;
            SzTriggerID = f.SzTriggerID;
        }
        //encapsulations
        public Point PAnchor
        {
            get { return pAnchor; }
            set { pAnchor = value; }
        }
        public Rectangle RDrawRect
        {
            get { return rDrawRect; }
            set { rDrawRect = value; }
        }
        public Rectangle RCollisionRect
        {
            get { return rCollisionRect; }
            set { rCollisionRect = value; }
        }
        public float FDuration
        {
            get { return fDuration; }
            set { fDuration = value; }
        }
        public string SzTriggerID
        {
            get { return szTriggerID; }
            set { szTriggerID = value; }
        }
    }
}
