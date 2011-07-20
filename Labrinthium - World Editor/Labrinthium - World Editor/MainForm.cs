using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

using SGD;

namespace Labrinthium___World_Editor
{
    public partial class MainForm : Form
    {
        private bool exit;
        public bool Exit
        {
            get { return exit; }
            set { exit = value; }
        }

        private TileMap tilemap;
        public TileMap Tilemap
        {
            get { return tilemap; }
            set { tilemap = value; }
        }

        private Point cameraPos;
        public Point CameraPos
        {
            get { return cameraPos; }
            set { cameraPos = value; }
        }

        public MainForm()
        {
            InitializeComponent();
            exit = false;
            cameraPos = new Point(0, 0);
            tilemap = new TileMap();

            ManagedDirect3D.Instance.InitManagedDirect3D(splitContainer1.Panel1, false);
            ManagedTextureManager.Instance.InitManagedTextureManager(ManagedDirect3D.Instance.Device, ManagedDirect3D.Instance.Sprite);
           
        }

        new public void Update()
        {

        }

        public void Render()
        {
            ManagedDirect3D.Instance.Clear(splitContainer1.Panel1, 0, 0, 0);

            ManagedDirect3D.Instance.DeviceBegin();

            // draw verticle lines on map
            for (int i = 0; i <= tilemap.MapSize.Width * tilemap.TileSize.Width; i += tilemap.TileSize.Width)
            {
                ManagedDirect3D.Instance.DrawLine(i + cameraPos.X, cameraPos.Y, i + cameraPos.Y, tilemap.MapSize.Height * tilemap.TileSize.Height + cameraPos.Y,
                                                    255, 255, 255);
            }

            // draw horizontal lines on tilemap
            for (int i = 0; i <= tilemap.MapSize.Height * tilemap.TileSize.Height; i += tilemap.TileSize.Height)
            {
                ManagedDirect3D.Instance.DrawLine(cameraPos.X, i + cameraPos.Y, tilemap.MapSize.Width * tilemap.TileSize.Width + cameraPos.X, i + cameraPos.Y,
                                                    255, 255, 255);
            }

            ManagedDirect3D.Instance.DeviceEnd();


            ManagedDirect3D.Instance.Present(splitContainer1.Panel1);
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            exit = true;

            ManagedTextureManager.Instance.ShutdownManagedTextureManager();
        }
    }
}
