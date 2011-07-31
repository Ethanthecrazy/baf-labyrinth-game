using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Xml.Linq;

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using SGD;

namespace AnimationEditor
{
    public partial class Form1 : Form
    {
        CAnimation currAnim;
        List<CAnimation> animations;
        AnimationEditor.Timer myTimer = new AnimationEditor.Timer();
        ManagedDirect3D dev = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;
        Point Selected = new Point();
        Size Moving = new Size();
        int FrameIndex;
        string SystemPath;
        int isRightClick;
        public bool Exit;
        

        public Form1()
        {
            InitializeComponent();
            //Init DirectX
            dev.InitManagedDirect3D(this.panelFrames, false);
            dev.InitManagedDirect3D(this.panel1, false);
            currAnim = new CAnimation(new CFrame());
            TM.InitManagedTextureManager(dev.Device, dev.Sprite);
            //Init Variables
            Exit = false;
            animations = new List<CAnimation>();
            buttonRemoveFrame.Enabled = false;
            isRightClick = -1;
            FrameIndex = 0;
            //If we cant load settings create a new path
            if (!LoadSettings())
            {
                CreateAppDataPath();
            }

            //Start App Controls
            UpdateControls();
            UpdateFrameList();
        }
        public void Render() 
        {          
            RenderAnimationPanel();
            RenderFramePanel();                               
        }
        new public void Update() 
        {
            base.Update();
            myTimer.Update();
            UpdateAnimation(myTimer.DeltaTime);
        }

        //helpers
        private void RenderAnimationPanel() 
        {
            dev.ChangeDisplayParam(this.panel1, false);
            dev.DeviceBegin();
            dev.SpriteBegin();
            dev.Clear(this.panel1, 255, 255, 255);           
            if (currAnim.BIsPlaying)
            {
                currAnim.DrawAnimation();
            }
            else
            {
                currAnim.DrawFrame(FrameIndex);
            }
            dev.SpriteEnd();
            dev.DeviceEnd();
            dev.Present(this.panel1);
        }
        private void RenderFramePanel() 
        {
            dev.ChangeDisplayParam(this.panelFrames, false);
            dev.DeviceBegin();
            dev.SpriteBegin();
            dev.Clear(this.panelFrames, 255, 255, 255);           
            //Scrolling
            Point offset = Point.Empty;
            offset.X -= panelFrames.AutoScrollPosition.X;
            offset.Y -= panelFrames.AutoScrollPosition.Y;

            DrawImageFramePanel(offset);
            DrawInfoFramePanel(offset);
            dev.SpriteEnd();
            dev.DeviceEnd();
            dev.Present(this.panelFrames);
        }
        private void UpdateFrameControls() 
        {
            FrameIndex = (int)(numericUpDownCurrFrame.Value - 1);
            //Draw Rect
            numericUpDownPosX.Value = currAnim.frames[FrameIndex].RDrawRect.X;
            numericUpDownPosY.Value = currAnim.frames[FrameIndex].RDrawRect.Y;
            numericUpDownWidth.Value = currAnim.frames[FrameIndex].RDrawRect.Width;
            numericUpDownHeight.Value = currAnim.frames[FrameIndex].RDrawRect.Height;
            //Collision Rect
            numericUpDownColX.Value = currAnim.frames[FrameIndex].RCollisionRect.X;
            numericUpDownColY.Value = currAnim.frames[FrameIndex].RCollisionRect.Y;
            numericUpDownColWidth.Value = currAnim.frames[FrameIndex].RCollisionRect.Width;
            numericUpDownColHeight.Value = currAnim.frames[FrameIndex].RCollisionRect.Height;
            //Current Frame
            numericUpDownCurrFrame.Maximum = numericUpDownFrameCount.Value;
            //Duration
            numericUpDownFrameDur.Value = (decimal)currAnim.frames[FrameIndex].FDuration;
            //Anchor Point
            numericUpDownAX.Value = currAnim.frames[FrameIndex].PAnchor.X;
            numericUpDownAY.Value = currAnim.frames[FrameIndex].PAnchor.Y;
            //Trigger
            textBoxTrigger.Text = currAnim.frames[FrameIndex].SzTriggerID;
        }
        private void UpdateAnimationControls() 
        {
            textBoxAnimName.Text = currAnim.SzName;
            numericUpDownFrameCount.Value = currAnim.frames.Count;
            //change max limit on curr frame
            numericUpDownCurrFrame.Maximum = numericUpDownFrameCount.Value;
            numericUpDownAnimSpeed.Value = (decimal)currAnim.FSpeed;
            checkBoxLooping.Checked = currAnim.BIsLooping;
            checkBoxReversed.Checked = currAnim.BIsReversed;
        }
        private void UpdateControls()
        {
            //Update all tags controls etc
            UpdateAnimationControls();
            UpdateFrameControls();
        }
        private void UpdateFrameList() 
        {
            listBoxFrames.Items.Clear();
            for (int i = 0; i < currAnim.frames.Count; i++) 
            {
                listBoxFrames.Items.Add("Frame " + (i + 1));
            }
        }
        private void UpdateAnimationList() 
        {
            listBoxAnimation.Items.Clear();
            for (int i = 0; i < animations.Count; i++) 
            {
                if(animations[i].SzName == "")
                    listBoxAnimation.Items.Add("Animation " + (i + 1));
                else
                    listBoxAnimation.Items.Add(animations[i].SzName);
            }
        }
        private void UpdateLists() 
        {
            UpdateAnimationList();
            UpdateFrameList();
        }
        private void DrawImageFramePanel(Point offset) 
        {
            if (currAnim.ImageID == -1)
                return;

            Rectangle DrawRect = new Rectangle(offset.X, offset.Y,
                                   TM.GetTextureWidth(currAnim.ImageID),//panelFrames.ClientRectangle.Width,
                                   TM.GetTextureHeight(currAnim.ImageID));//panelFrames.ClientRectangle.Height);
       
            TM.Draw(currAnim.ImageID, 0, 0, 1.0f,
                    1.0f, DrawRect, 0, 0, 0.0f, Color.White.ToArgb());
            
            TM.FlushSprites();
            
        }
        private void DrawRect(Rectangle rect, Point offset, Color color) 
        {
            //top
            dev.DrawLine(rect.X - offset.X,
                         rect.Y - offset.Y, 
                         rect.X + rect.Width - offset.X,
                         rect.Y - offset.Y,
                         (int)color.R, (int)color.G, (int)color.B);
            //bottom
            dev.DrawLine(rect.X - offset.X,
                         rect.Y + rect.Height - offset.Y,
                         rect.X + rect.Width - offset.X,
                         rect.Y + rect.Height - offset.Y,
                         (int)color.R, (int)color.G, (int)color.B);
            //left
            dev.DrawLine(rect.X - offset.X,
                         rect.Y - offset.Y,
                         rect.X - offset.X,
                         rect.Y + rect.Height - offset.Y,
                         (int)color.R, (int)color.G, (int)color.B);
            //right
            dev.DrawLine(rect.X + rect.Width - offset.X,
                         rect.Y - offset.Y,
                         rect.X + rect.Width - offset.X,
                         rect.Y + rect.Height - offset.Y,
                         (int)color.R, (int)color.G, (int)color.B);
        }
        private Rectangle FlipRect(Rectangle src) 
        {
            if (src.Width < 0) 
            {
                src.X += src.Width;
                src.Width *= -1;
            }
            if (src.Height < 0) 
            {
                src.Y += src.Height;
                src.Height *= -1;
            }
            return src;
        }
        private Rectangle SetRect(bool isDrawRect, Rectangle src) 
        {
            NumericUpDown Height, Width, X, Y;
            if(isDrawRect)
            {
                Height = numericUpDownColHeight;
                Width = numericUpDownColWidth;
                X = numericUpDownColX;
                Y = numericUpDownColY;
            }
            else
            {
                Height = numericUpDownHeight;
                Width = numericUpDownWidth;
                X = numericUpDownPosX;
                Y = numericUpDownPosY;
            }

            //Check X
            if (src.X < (int)X.Minimum)
                src.X = (int)X.Minimum;
            else if (src.X > (int)X.Maximum)
                src.X = (int)X.Maximum;

            //Check Y
            if (src.Y < (int)Y.Minimum)
                src.Y = (int)Y.Minimum;
            else if (src.Y > (int)Y.Maximum)
                src.Y = (int)Y.Maximum;

            //Check Height
            if (src.Height < (int)Height.Minimum)
                src.Height = (int)Height.Minimum;
            else if (src.Height > (int)Height.Maximum)
                src.Height = (int)Height.Maximum;

            //Check With
            if (src.Width <= (int)Width.Minimum)
                src.Width = (int)Width.Minimum;
            else if (src.Width > (int)Width.Maximum)
                src.Width = (int)Width.Maximum;

            return src;
        }
        private Point SetPoint(Point src) 
        {
            NumericUpDown X, Y;
            X = numericUpDownAX;
            Y = numericUpDownAY;
            Point point = new Point(src.X, src.Y);
            //Check X
            if (point.X < (int)X.Minimum)
                point.X = (int)X.Minimum;
            else if (point.X > (int)X.Maximum)
                point.X = (int)X.Maximum;

            //Check Y
            if (point.Y < (int)Y.Minimum)
                point.Y = (int)Y.Minimum;
            else if (src.Y > (int)Y.Maximum)
                point.Y = (int)Y.Maximum;
            return point;
        }
        private void DrawInfoFramePanel(Point offset) 
        {
            for (int i = 0; i < currAnim.frames.Count; i++) 
            {
                Color DrawColor, CollColor, APColor;
                if (i == FrameIndex)
                {
                    DrawColor = Color.LightGreen;
                    CollColor = Color.MediumVioletRed;
                    APColor = Color.Orange;
                }
                else 
                {
                    DrawColor = Color.Green;
                    CollColor = Color.Red;
                    APColor = Color.Yellow;
                }
                //Draw Rect
                DrawRect(currAnim.frames[i].RDrawRect, offset, DrawColor);
                //Collison Rect
                DrawRect(currAnim.frames[i].RCollisionRect, offset, CollColor);
                //Anchor Point
                int x = currAnim.frames[i].PAnchor.X;
                int y = currAnim.frames[i].PAnchor.Y;
                dev.DrawRect(new Rectangle(x - offset.X, y - offset.Y, 7, 7), APColor.R,
                    APColor.G, APColor.B);
            }
            //user-rect
            DrawRect(new Rectangle(Selected, Moving), offset, Color.Blue);
        }
        private void UpdateAnimation(float interval) 
        {
            if (currAnim == null)
                return;

            if(currAnim.Update(interval))
                panel1.Invalidate();
        }
        private void SaveInXML(bool saveAll, string FileName) 
        {
            TextWriter writer = new StreamWriter(FileName);
            if (saveAll)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<CAnimation>));
                serializer.Serialize(writer, animations);
            }
            else 
            {
                XmlSerializer serializer = new XmlSerializer(typeof(CAnimation));
                serializer.Serialize(writer, currAnim);
            }                
            writer.Close();
        }
        private void OpenXML(string FileName) 
        {
            List<CAnimation> list;
            CAnimation anim;
            FileStream fs = new FileStream(FileName, FileMode.Open);
            XmlReader reader = new XmlTextReader(fs);
            XmlSerializer serializerAll = new XmlSerializer(typeof(List<CAnimation>));
            XmlSerializer serializerCurr = new XmlSerializer(typeof(CAnimation));

            if (serializerAll.CanDeserialize(reader))
            {
                list = (List<CAnimation>)serializerAll.Deserialize(reader);
                //Add them to the list
                animations.Clear();
                foreach (CAnimation v in list)
                {
                    if(v.ImageID > -1)
                        v.ImageID = TM.LoadTexture(v.SzFilenameWithPath, 0);
                   
                    animations.Add(v);
                }
            }
            else if (serializerCurr.CanDeserialize(reader))
            {
                anim = (CAnimation)serializerCurr.Deserialize(reader);
                if(anim.ImageID > -1)
                    anim.ImageID = TM.LoadTexture(anim.SzFilenameWithPath, 0);

                animations.Clear();
                animations.Add(anim);
                //currAnim = anim;
            }
            else 
            {
                return;
            }         
            reader.Close();
        }
        private void SaveInBinaryAll(string FileName)
        {
            BinaryWriter writer;
            if(File.Exists(FileName))
                writer = new BinaryWriter(File.Open(FileName, FileMode.Truncate));
            else
                writer = new BinaryWriter(File.Open(FileName, FileMode.Create));

            //Animation Count
            writer.Write(animations.Count);
            for(int i = 0; i < animations.Count; i++)
            {
                //Frame Count
                writer.Write(animations[i].frames.Count);
                //Frames
                for(int j = 0; j < animations[i].frames.Count; j++)
                {
                    //Anchor Point
                    writer.Write(animations[i].frames[j].PAnchor.X);
                    writer.Write(animations[i].frames[j].PAnchor.Y);
                    //Draw Rect
                    writer.Write(animations[i].frames[j].RDrawRect.Left);
                    writer.Write(animations[i].frames[j].RDrawRect.Top);
                    writer.Write(animations[i].frames[j].RDrawRect.Right);
                    writer.Write(animations[i].frames[j].RDrawRect.Bottom);
                    //Collision Rect
                    writer.Write(animations[i].frames[j].RCollisionRect.Left);
                    writer.Write(animations[i].frames[j].RCollisionRect.Top);
                    writer.Write(animations[i].frames[j].RCollisionRect.Right);
                    writer.Write(animations[i].frames[j].RCollisionRect.Bottom);
                    //Duration
                    writer.Write(animations[i].frames[j].FDuration);
                    //Trigger
                    writer.Write(animations[i].frames[j].SzTriggerID.Length);
                    for (int h = 0; h < animations[i].frames[j].SzTriggerID.Length; h++) 
                    {
                        writer.Write(animations[i].frames[j].SzTriggerID[h]);
                    }
                }
                //Path & Filename
                writer.Write(animations[i].SzFilenameWithPath.Length);
                for (int n = 0; n < animations[i].SzFilenameWithPath.Length; n++)
                {
                    writer.Write(animations[i].SzFilenameWithPath[n]);
                }
                //Filename 
                writer.Write(animations[i].SzFilename.Length);
                for (int k = 0; k < animations[i].SzFilename.Length; k++) 
                {
                    writer.Write(animations[i].SzFilename[k]);
                }
                //Name
                writer.Write(animations[i].SzName.Length);
                for(int l = 0; l < animations[i].SzName.Length; l++)
                {
                    writer.Write(animations[i].SzName[l]);
                }
                //ImageID
                writer.Write(animations[i].ImageID);
                //Speed
                writer.Write(animations[i].FSpeed);
                //Looping
                writer.Write(animations[i].BIsLooping);
                //Reversed
                writer.Write(animations[i].BIsReversed);
            }
            writer.Close();    
        }
        private void SaveInBinaryCurr(string FileName) 
        {
            BinaryWriter writer;
            if (File.Exists(FileName))
                writer = new BinaryWriter(File.Open(FileName, FileMode.Truncate));
            else
                writer = new BinaryWriter(File.Open(FileName, FileMode.Create));

            //Animation Count
            writer.Write(1);
            //Frame Count
            writer.Write(currAnim.frames.Count);
            //Frames
            for (int j = 0; j < currAnim.frames.Count; j++)
            {
                //Anchor Point
                writer.Write(currAnim.frames[j].PAnchor.X);
                writer.Write(currAnim.frames[j].PAnchor.Y);
                //Draw Rect
                writer.Write(currAnim.frames[j].RDrawRect.Left);
                writer.Write(currAnim.frames[j].RDrawRect.Top);
                writer.Write(currAnim.frames[j].RDrawRect.Right);
                writer.Write(currAnim.frames[j].RDrawRect.Bottom);
                //Collision Rect
                writer.Write(currAnim.frames[j].RCollisionRect.Left);
                writer.Write(currAnim.frames[j].RCollisionRect.Top);
                writer.Write(currAnim.frames[j].RCollisionRect.Right);
                writer.Write(currAnim.frames[j].RCollisionRect.Bottom);
                //Duration
                writer.Write(currAnim.frames[j].FDuration);
                //Trigger
                writer.Write(currAnim.frames[j].SzTriggerID.Length);
                for (int h = 0; h < currAnim.frames[j].SzTriggerID.Length; h++)
                {
                    writer.Write(currAnim.frames[j].SzTriggerID[h]);
                }
            }
            //Path & Filename
            writer.Write(currAnim.SzFilenameWithPath.Length);
            for (int n = 0; n < currAnim.SzFilenameWithPath.Length; n++) 
            {
                writer.Write(currAnim.SzFilenameWithPath[n]);
            }
            //Filename 
            writer.Write(currAnim.SzFilename.Length);
            for (int k = 0; k < currAnim.SzFilename.Length; k++)
            {
                writer.Write(currAnim.SzFilename[k]);
            }
            //Name
            writer.Write(currAnim.SzName.Length);
            for (int l = 0; l < currAnim.SzName.Length; l++)
            {
                writer.Write(currAnim.SzName[l]);
            }
            //ImageID
            writer.Write(currAnim.ImageID);
            //Speed
            writer.Write(currAnim.FSpeed);
            //Looping
            writer.Write(currAnim.BIsLooping);
            //Reversed
            writer.Write(currAnim.BIsReversed);
            writer.Close();    
        }
        private void OpenBinary(string FileName)
        {
            List<CAnimation> list;
            CAnimation data;
            int animCount, frameCount, stringCount;
            char[] chars;
            BinaryReader reader = new BinaryReader(File.Open(FileName, FileMode.Open));
            list = new List<CAnimation>();              
            try
            {
                //Animation Count
                animCount = reader.ReadInt32();
                for (int i = 0; i < animCount; i++)
                {
                    data = new CAnimation();
                    data.frames = new List<CFrame>();
                    //Frame Count
                    frameCount = reader.ReadInt32();
                    //Frames
                    for (int j = 0; j < frameCount; j++)
                    {
                        data.frames.Add(new CFrame());
                        //Anchor Point
                        int APointX = reader.ReadInt32();
                        int APointY = reader.ReadInt32();
                        data.frames[j].PAnchor = new Point(APointX, APointY);
                        //Draw Rect
                        int left = reader.ReadInt32();
                        int top = reader.ReadInt32();
                        int right = reader.ReadInt32();
                        int bottom = reader.ReadInt32();
                        data.frames[j].RDrawRect = new Rectangle(left, top, (right - left), (bottom - top));
                        //Collision Rect
                        left = reader.ReadInt32();
                        top = reader.ReadInt32();
                        right = reader.ReadInt32();
                        bottom = reader.ReadInt32();
                        data.frames[j].RCollisionRect = new Rectangle(left, top, (right - left), (bottom - top));
                        //Duration
                        data.frames[j].FDuration = (float)reader.ReadSingle(); 
                        //Trigger
                        stringCount = reader.ReadInt32();
                        chars = reader.ReadChars(stringCount);
                        data.frames[j].SzTriggerID = new string(chars);
                    }
                    //Path & Filename
                    stringCount = reader.ReadInt32();
                    chars = reader.ReadChars(stringCount);
                    data.SzFilenameWithPath = new string(chars);
                    //Filename
                    stringCount = reader.ReadInt32();
                    chars = reader.ReadChars(stringCount);
                    data.SzFilename = new string(chars);
                    //Name
                    stringCount = reader.ReadInt32();
                    chars = reader.ReadChars(stringCount);
                    data.SzName = new string(chars);
                    //ImageID
                    data.ImageID = reader.ReadInt32();
                    //Speed
                    data.FSpeed = reader.ReadSingle(); 
                    //Looping
                    data.BIsLooping = reader.ReadBoolean();
                    //Reversed
                    data.BIsReversed = reader.ReadBoolean();
                    //add it
                    list.Add(data);
                }
                //Add them to the list
                animations.Clear();
                foreach (CAnimation v in list)
                {
                    if (v.ImageID > -1)
                        v.ImageID = TM.LoadTexture(v.SzFilenameWithPath, 0);

                    animations.Add(v);
                }
            }
            catch (EndOfStreamException e)
            {
                //If we read past end of file leave     
                e.ToString();
                return;
            }
            finally 
            {
                reader.Close();
            }
        }
        private void SaveDialog(bool saveAll) 
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.InitialDirectory = SystemPath;
            dlg.Filter = "Dat Files|*.dat|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            //dlg.DefaultExt = "xml";
            if (DialogResult.OK == dlg.ShowDialog())
            {
                //Binary
                if (dlg.FilterIndex == 1) 
                {
                    if (saveAll)
                        SaveInBinaryAll(dlg.FileName);
                    else
                        SaveInBinaryCurr(dlg.FileName);
                }
                //XML
                else if (dlg.FilterIndex == 2) 
                {
                    SaveInXML(saveAll, dlg.FileName);
                }
            }
        }
        private void OpenDialog() 
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = SystemPath;
            dlg.Filter = "Dat Files|*.dat|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            if (DialogResult.OK == dlg.ShowDialog())
            {
                //Binary
                if (dlg.FilterIndex == 1) 
                {
                    OpenBinary(dlg.FileName);
                }
                //XML
                else if (dlg.FilterIndex == 2) 
                {
                    OpenXML(dlg.FileName);
                }
            }  
            UpdateControls();
            UpdateLists();
        }
        private void CreateAppDataPath() 
        {
            try
            {
                string pathToexe = Application.UserAppDataPath;
                string newPath = Path.Combine(pathToexe, @"Settings");
                string pathTofile = Path.Combine(pathToexe, @"Settings\settings.dat");
                System.IO.Directory.CreateDirectory(newPath);
                File.Create(pathTofile);
            }
            catch (Exception) 
            {
            
            }
        }
        private bool LoadSettings() 
        {
            string pathToexe = Application.UserAppDataPath;           
            string pathTofile = Path.Combine(pathToexe, @"Settings\settings.dat");
            BinaryReader reader;
            //Opening File
            try
            {
                reader = new BinaryReader(File.Open(pathTofile, FileMode.Open));
            }
            catch (DirectoryNotFoundException)
            {
                return false;
            }
            
            //Load Settings
            try
            {
                SystemPath = reader.ReadString();
            }
            catch (EndOfStreamException)
            {
                //If we read past end of file leave
                return true;
            }
            finally 
            {
                reader.Close();           
            }
            return true;
        }
        private void SaveSettings() 
        {
            try
            {
                string pathToexe = Application.UserAppDataPath;
                string pathTofile = Path.Combine(pathToexe, @"Settings\settings.dat");
                BinaryWriter writer = new BinaryWriter(File.Open(pathTofile, FileMode.Truncate));
                writer.Write(SystemPath);
                writer.Close();
            }
            catch (DirectoryNotFoundException)
            {
                return;
            }
            
        }
        private void ExitApp() 
        {
            Exit = true;
            TM.ShutdownManagedTextureManager();
            dev.Shutdown();
        }
        
        //Menus
        private void importImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = SystemPath;
            dlg.Filter = "(*.BMP;*.JPG;*.GIF;*.PNG)|*.BMP;*.JPG;*.GIF;*.PNG";
            dlg.RestoreDirectory = true;
            if (DialogResult.OK == dlg.ShowDialog())
            {
                currAnim.SzFilenameWithPath = dlg.FileName;
                currAnim.SzFilename = dlg.SafeFileName;
                currAnim.ImageID = TM.LoadTexture(dlg.FileName, 0);
                panel1.Invalidate();
                panelFrames.Invalidate();
                panelFrames.AutoScrollMinSize = new Size(TM.GetTextureWidth(currAnim.ImageID),
                                                         TM.GetTextureHeight(currAnim.ImageID));
            }   
        }
        private void setSystemPathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.Description = "Select the directory that you want to use as the default.";
            if (DialogResult.OK == dlg.ShowDialog()) 
            {
                SystemPath = dlg.SelectedPath;
                SaveSettings();
            }
        }
        private void exitToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        private void importAnimationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDialog();
        }
        private void saveAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveDialog(true);
        }
        private void saveCurrentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveDialog(false);
        }

        //Buttons
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            ExitApp();
        }
        //Add Animation
        private void button1_Click(object sender, EventArgs e)
        {
            //add a new copy
            CAnimation temp = new CAnimation(currAnim);
            animations.Add(temp);
            UpdateAnimationList();
        }
        //Remove Selected
        private void button2_Click(object sender, EventArgs e)
        {
            if (listBoxAnimation.SelectedIndex == -1) 
            {
                return;
            }
            //remove it - currAnim is the selected animation
            animations.Remove(currAnim);
            currAnim = new CAnimation(new CFrame());
            UpdateLists();
            UpdateControls();
        }
        private void buttonPlay_Click(object sender, EventArgs e)
        {
            currAnim.Play();
            numericUpDownFrameCount.Enabled = false;
            buttonNextF.Enabled = false;
            buttonPrevF.Enabled = false;
            buttonAddFrame.Enabled = false;
            buttonRemoveFrame.Enabled = false;
        }
        private void buttonResume_Click(object sender, EventArgs e)
        {
            currAnim.Resume();
            numericUpDownFrameCount.Enabled = false;
            buttonNextF.Enabled = false;
            buttonPrevF.Enabled = false;
            buttonAddFrame.Enabled = false;
            buttonRemoveFrame.Enabled = false;
        }
        private void buttonStop_Click(object sender, EventArgs e)
        {
            currAnim.Stop();
            numericUpDownFrameCount.Enabled = true;
            buttonNextF.Enabled = true;
            buttonPrevF.Enabled = true;
            buttonAddFrame.Enabled = true;

            if(currAnim.frames.Count > 1)
                buttonRemoveFrame.Enabled = true;

        }
        private void buttonNextF_Click(object sender, EventArgs e)
        {
            if ((FrameIndex + 1) < currAnim.frames.Count)
            {
                ++FrameIndex;
                numericUpDownCurrFrame.Value = (FrameIndex + 1);
            }
        }
        private void buttonPrevF_Click(object sender, EventArgs e)
        {
            if (FrameIndex > 0)
            {
                --FrameIndex;
                numericUpDownCurrFrame.Value = (FrameIndex + 1);
            }
        }
        //Frames
        private void buttonAddFrame_Click(object sender, EventArgs e)
        {
            if (currAnim.frames.Count >= numericUpDownFrameCount.Maximum)
                return;

            currAnim.frames.Add(new CFrame());
            UpdateFrameList();
            UpdateControls();
            
            if (currAnim.frames.Count >= 1)
                buttonRemoveFrame.Enabled = true;
        }
        private void buttonRemoveFrame_Click(object sender, EventArgs e)
        {
            if (listBoxFrames.SelectedIndex == -1)
            {
                return;
            }
            //remove it
            currAnim.frames.Remove(currAnim.frames[listBoxFrames.SelectedIndex]);
            UpdateFrameList();
            UpdateControls();

            //cant remove the last frame!
            if (currAnim.frames.Count <= 1)
                buttonRemoveFrame.Enabled = false;
        }

        //NumericUpDowns
        //Frames
        //Frames - Frame Rect
        private void numericUpDownPosX_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RDrawRect;
            rect.X = (int)numericUpDownPosX.Value;
            currAnim.frames[FrameIndex].RDrawRect = rect;
        }
        private void numericUpDownPosY_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RDrawRect;
            rect.Y = (int)numericUpDownPosY.Value;
            currAnim.frames[FrameIndex].RDrawRect = rect;
        }
        private void numericUpDownWidth_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RDrawRect;
            rect.Width = (int)numericUpDownWidth.Value;
            currAnim.frames[FrameIndex].RDrawRect = rect;
        }
        private void numericUpDownHeight_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RDrawRect;
            rect.Height = (int)numericUpDownHeight.Value;
            currAnim.frames[FrameIndex].RDrawRect = rect;
        }
        //Frames - Collision Rect
        private void numericUpDownColX_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RCollisionRect;
            rect.X = (int)numericUpDownColX.Value;
            currAnim.frames[FrameIndex].RCollisionRect = rect;
        }
        private void numericUpDownColY_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RCollisionRect;
            rect.Y = (int)numericUpDownColY.Value;
            currAnim.frames[FrameIndex].RCollisionRect = rect;
        }
        private void numericUpDownColWidth_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RCollisionRect;
            rect.Width = (int)numericUpDownColWidth.Value;
            currAnim.frames[FrameIndex].RCollisionRect = rect;
        }
        private void numericUpDownColHeight_ValueChanged(object sender, EventArgs e)
        {
            Rectangle rect = currAnim.frames[FrameIndex].RCollisionRect;
            rect.Height = (int)numericUpDownColHeight.Value;
            currAnim.frames[FrameIndex].RCollisionRect = rect;
        }
        //Frames - Anchor Point
        private void numericUpDownAX_ValueChanged(object sender, EventArgs e)
        {
            Point point = currAnim.frames[FrameIndex].PAnchor;
            point.X = (int)numericUpDownAX.Value;
            currAnim.frames[FrameIndex].PAnchor = point;
        }
        private void numericUpDownAY_ValueChanged(object sender, EventArgs e)
        {
            Point point = currAnim.frames[FrameIndex].PAnchor;
            point.Y = (int)numericUpDownAY.Value;
            currAnim.frames[FrameIndex].PAnchor = point;
        }
        //Frames - Duration
        private void numericUpDownFrameDur_ValueChanged(object sender, EventArgs e)
        {
            currAnim.frames[FrameIndex].FDuration = (float)numericUpDownFrameDur.Value;
        }
        //Frames - Current Frame
        private void numericUpDownCurrFrame_ValueChanged(object sender, EventArgs e)
        {
            FrameIndex = (int)(numericUpDownCurrFrame.Value - 1);
            UpdateControls();
        }
        //Animation
        //Animation - Frame Count
        private void numericUpDownFrameCount_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownFrameCount.Value > currAnim.frames.Count)
                    buttonRemoveFrame.Enabled = true;

            currAnim.ChangeNumOfFrames((int)numericUpDownFrameCount.Value);
            UpdateFrameList();
            //change max limit on curr frame
            numericUpDownCurrFrame.Maximum = numericUpDownFrameCount.Value;

            if(numericUpDownCurrFrame.Maximum <= 1)
                buttonRemoveFrame.Enabled = false;
        }
        //Animation - Speed
        private void numericUpDownAnimSpeed_ValueChanged(object sender, EventArgs e)
        {
            currAnim.FSpeed = (float)numericUpDownAnimSpeed.Value;
        }
        
        //TextBoxes
        //Animation - Name
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            currAnim.SzName = textBoxAnimName.Text;
            UpdateAnimationList();
        }
        //Frame - Trigger
        private void textBoxTrigger_TextChanged(object sender, EventArgs e)
        {
            currAnim.frames[FrameIndex].SzTriggerID = textBoxTrigger.Text;
        }
      
        //Check Boxes
        private void checkBoxLooping_CheckedChanged(object sender, EventArgs e)
        {
            currAnim.BIsLooping = checkBoxLooping.Checked;
        }
        private void checkBoxReversed_CheckedChanged(object sender, EventArgs e)
        {
            currAnim.BIsReversed = checkBoxReversed.Checked;
        }

        //Listboxes
        private void listBoxAnimation_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBoxAnimation.SelectedIndex == -1)
                return;

            //stop animation from playing
            currAnim.Stop();
            numericUpDownFrameCount.Enabled = true;
            buttonNextF.Enabled = true;
            buttonPrevF.Enabled = true;
            buttonAddFrame.Enabled = true;

            currAnim = animations[listBoxAnimation.SelectedIndex];
            if (currAnim.frames.Count > 1)
                buttonRemoveFrame.Enabled = true;
            UpdateControls();

            if(currAnim.ImageID != -1)
                panelFrames.AutoScrollMinSize = new Size(TM.GetTextureWidth(currAnim.ImageID),
                                                         TM.GetTextureHeight(currAnim.ImageID));
            else
                panelFrames.AutoScrollMinSize = new Size(panelFrames.ClientRectangle.Width,
                                                         panelFrames.ClientRectangle.Height);
        }
        private void listBoxFrames_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBoxFrames.SelectedIndex == -1)
                return;

            numericUpDownCurrFrame.Value = (listBoxFrames.SelectedIndex + 1);
        }

        //Animation Panel
        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
        //Frame Panel
        private void panelFrames_Paint(object sender, PaintEventArgs e)
        {

        }
        private void panelFrames_Scroll(object sender, ScrollEventArgs e)
        {
            this.Render();
        }
        private void panelFrames_MouseClick(object sender, MouseEventArgs e)
        {
            
        }
        private void panelFrames_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
                isRightClick = 1;
            else if(e.Button == MouseButtons.Left)
                isRightClick = 0;

            //Scrolling
            Point offset = Point.Empty;
            offset.X -= panelFrames.AutoScrollPosition.X;
            offset.Y -= panelFrames.AutoScrollPosition.Y;
            offset.Offset(e.Location);

            if (e.Button == MouseButtons.Middle) 
            {
                isRightClick = 2;
                currAnim.frames[FrameIndex].PAnchor = SetPoint(offset);
                UpdateControls();
                return;
            }

            Selected = offset;

        }
        private void panelFrames_MouseMove(object sender, MouseEventArgs e)
        {
            if (isRightClick != 1 && isRightClick != 0)
                return;

            //Scrolling
            Point offset = Point.Empty;
            offset.X -= panelFrames.AutoScrollPosition.X;
            offset.Y -= panelFrames.AutoScrollPosition.Y;

            Moving.Width = ((e.X + offset.X) - Selected.X);
            Moving.Height = ((e.Y + offset.Y) - Selected.Y);

            DrawRect(new Rectangle(Selected, Moving), offset, Color.Blue);
        }
        private void panelFrames_MouseUp(object sender, MouseEventArgs e)
        {
            if (isRightClick == 2)
                return;

            Rectangle rect = new Rectangle(Selected, Moving);
            rect = FlipRect(rect);
            if (isRightClick == 1)
                currAnim.frames[FrameIndex].RCollisionRect = SetRect(true, rect);
            else if (isRightClick == 0)
                currAnim.frames[FrameIndex].RDrawRect = SetRect(false, rect);

            UpdateControls();
            isRightClick = -1;
            Selected = new Point(0, 0);
            Moving = new Size(0, 0);
        }
    }
}
