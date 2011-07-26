using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Timers;
using System.Windows.Forms;

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using SGD;

namespace AnimationEditor
{
    public class CAnimation
    {
        //data members
        public List<CFrame> frames;
        private int imageID;
        private string szFilenameWithPath;
        private string szFilename;
        private string szName;
        private int nCurFrame;
        private float fSpeed;
        private float fTimeWaited;
        private bool bIsPlaying;
        private bool bIsLooping;
        private bool bIsReversed;
        //constructors
        public CAnimation() 
        {
            frames = new List<CFrame>();
            SzName = "";
            SzFilename = "";
            SzFilenameWithPath = "";
            ImageID = -1;
            nCurFrame = 0;
            FSpeed = 1.0f;
            fTimeWaited = 0.0f;
            BIsPlaying = false;
            BIsLooping = false;
            BIsReversed = false;
        }
        public CAnimation(CFrame f) 
        {
            frames = new List<CFrame>();
            frames.Add(f);
            SzName = "";
            SzFilename = "";
            SzFilenameWithPath = "";
            ImageID = -1;
            nCurFrame = 0;
            FSpeed = 1.0f;
            fTimeWaited = 0.0f;
            BIsPlaying = false;
            BIsLooping = false;
            BIsReversed = false;
        }
        public CAnimation(CAnimation a) 
        {
            //Frame List
            frames = new List<CFrame>(a.frames);
            for (int i = 0; i < frames.Count; i++) 
            {
                frames[i] = new CFrame(a.frames[i]);
            }
            //Name
            SzName = a.SzName.ToString();
            SzFilename = a.SzFilename.ToString();
            SzFilenameWithPath = a.SzFilenameWithPath.ToString();
            //Texture
            ImageID = a.ImageID;
            //Speed
            FSpeed = a.FSpeed;
            nCurFrame = 0;
            fTimeWaited = 0.0f;
            BIsPlaying = false;
            BIsLooping = false;
            BIsReversed = false;
        }
        //encapsulations
        public int ImageID
        {
            get { return imageID; }
            set { imageID = value; }
        }
        public string SzName
        {
            get { return szName; }
            set { szName = value; }
        }
        public string SzFilename
        {
            get { return szFilename; }
            set { szFilename = value; }
        }
        public string SzFilenameWithPath
        {
            get { return szFilenameWithPath; }
            set { szFilenameWithPath = value; }
        }
        public float FSpeed
        {
            get { return fSpeed; }
            set { fSpeed = value; }
        }
        public bool BIsPlaying
        {
            get { return bIsPlaying; }
            set { bIsPlaying = value; }
        }
        public bool BIsLooping
        {
            get { return bIsLooping; }
            set { bIsLooping = value; }
        }
        public bool BIsReversed
        {
            get { return bIsReversed; }
            set { bIsReversed = value; }
        }
        //helper-functions
        private void AddFrames(int size)
        {
            for (int i = 0; i < size; i++)
            {
                frames.Add(new CFrame());
            }
        }
        private void RemoveFrames(int amount)
        {
            //Get our target new count
            int target = ((frames.Count - 1) - amount);
            for (int i = (frames.Count - 1); i > target; i--) 
            {
                CFrame f = frames[(i)];
                frames.Remove(f);
            }
        }
        private void Draw(int frameindex) 
        {
            if (ImageID == -1)
                return;

            Point pAnchor = frames[frameindex].PAnchor;
            Rectangle DrawRect = frames[frameindex].RDrawRect;
            ManagedTextureManager TM = ManagedTextureManager.Instance;
            Color color = Color.White;

            //Bug - Anchor doesnt work properly
            //Draw Bitmap
            TM.Draw(ImageID, 
                    150 - pAnchor.X,
                    70 - pAnchor.Y,
                    1.0f, 1.0f, DrawRect,
                    0, 0, 0, color.ToArgb());
        }
        private void ToNextFrame() 
        {
            if (BIsReversed)
                --nCurFrame;
            else
                ++nCurFrame;
        }
        private bool AtLastFrame() 
        {
            if (BIsReversed)
            {
                if (nCurFrame < 0)
                    return true;
                else
                    return false;
            }
            else 
            {
                if(nCurFrame >= frames.Count)
                    return true;
                else
                    return false;
            }
        }
        //functions
        public void DrawAnimation()
        {
            Draw(nCurFrame); 
        }
        public void DrawFrame(int frameindex)
        {
            Draw(frameindex); 
        }
        public bool Update(float interval)
        {
            if (!BIsPlaying)
                return false;

            //increased waited time by delta - affected by speed
            fTimeWaited += (interval * FSpeed);
            if (fTimeWaited > frames[nCurFrame].FDuration) 
            {
                //Reset based of current frames duration
                fTimeWaited -= frames[nCurFrame].FDuration;
                ToNextFrame();
                if (AtLastFrame()) 
                {
                    if (BIsLooping)
                        Reset();
                    else 
                    {
                        Stop();
                        Reset();
                    }
                }
                return true;
            }
            return false;
        }
        public void Play() 
        {
            Resume();
            Reset();
        }
        public void Stop() 
        {
            BIsPlaying = false;
        }
        public void Resume() 
        {
            BIsPlaying = true;
        }
        private void Reset() 
        {
            fTimeWaited = 0.0F;
            if (BIsReversed)
                nCurFrame = frames.Count - 1;
            else
                nCurFrame = 0;
        }
        public void ChangeNumOfFrames(int size) 
        {
            if (size > frames.Count)
                AddFrames((size - frames.Count));
            else if (size < frames.Count)
                RemoveFrames((frames.Count - size));
        }
        public override string ToString() 
        {
            return this.SzName;
        }
    }
}
