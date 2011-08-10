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
using System.Xml.Linq;

using System.IO;

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

using SGD;

namespace Labrinthium___World_Editor
{

public partial class MainForm : Form
    {

private Point cameraPos;
public Point CameraPos
        {
            get { return cameraPos; }
            set { cameraPos = value; }
        }

private bool exit;
public bool Exit
        {
            get { return exit; }
            set { exit = value; }
        }

List<Button> layerbuttons = new List<Button>();
public List<Button> Layerbuttons
        {
            get { return layerbuttons; }
            set { layerbuttons = value; }
        }

OptForm optform;
public OptForm Optform
        {
            get { return optform; }
            set { optform = value; }
        }

int renderDepth;
public int RenderDepth
        {
            get { return renderDepth; }
            set { renderDepth = value; }
        }

string relitivePath;
public string RelitivePath
{
    get { return relitivePath; }
    set { relitivePath = value; }
}

private bool saved;
public bool Saved
        {
            get { return saved; }
            set { saved = value; }
        }

int selectboxID = -1;
Rectangle selectionBox;
Point startboxPoint;

public TabPage TabPage1
        {
            get { return tabPage1; }
            set { tabPage1 = value; }
        }

bool tileFocused = true;
public bool TileFocused
{
    get { return tileFocused; }
    set { tileFocused = value; }
}

public TabPage Tb1
{
    get { return tabPage1; }
    set { tabPage1 = value; }
}

public TabPage Tb2
{
    get { return tabPage2; }
    set { tabPage2 = value; }
}

public TabPage Tb3
{
    get { return tabPage3; }
    set { tabPage3 = value; }
}

private TileMap tilemap;
public TileMap Tilemap
        {
            get { return tilemap; }
            set { tilemap = value; }
        }

private Point tilesetCamera;
public Point TilesetCamera
        {
            get { return tilesetCamera; }
            set { tilesetCamera = value; }
        }

Point tilesetselected;
public Point Tilesetselected
        {
            get { return tilesetselected; }
            set { tilesetselected = value; }
        }

public int TimerInterval
        {
            get { return timerAutoSave.Interval; }
            set { timerAutoSave.Interval = value; }
        }

public SplitContainer WorldSplitC
        {
            get { return splitContainer1; }
            set { splitContainer1 = value; }
        }

public MainForm()
        {
            InitializeComponent();

            Random rand = new Random();
            int episode = rand.Next(1, 25);

            exit = false;
            cameraPos = new Point(0, 0);
            tilemap = new TileMap(30, 30, 1);
            splitContainer1.Panel1.AutoScrollMinSize = new Size(tilemap.MapSize.Width * tilemap.TileSize.Width, tilemap.MapSize.Height * tilemap.TileSize.Height);
            tabPage1.AutoScrollMinSize = new Size(tilemap.TileSetSize.Width * tilemap.TileSize.Width, tilemap.TileSetSize.Height * tilemap.TileSize.Height);
            tabPage2.AutoScrollMinSize = new Size(tilemap.ObjectTilSet.Width * tilemap.TileSize.Width, tilemap.ObjectTilSet.Height * tilemap.TileSize.Height);
            

            ManagedDirect3D.Instance.InitManagedDirect3D(splitContainer1.Panel1, false);
            ManagedDirect3D.Instance.InitManagedDirect3D(tabPage1, false);
            ManagedDirect3D.Instance.InitManagedDirect3D(tabPage2, false);
            ManagedTextureManager.Instance.InitManagedTextureManager(ManagedDirect3D.Instance.Device, ManagedDirect3D.Instance.Sprite);

            cameraPos = new Point(0, 0);
            renderDepth = 0;
            saved = true;

            layerbuttons.Add(button_Layer1);
            layerbuttons.Add(button_Layer2);
            layerbuttons.Add(button_Layer3);
            layerbuttons.Add(button_Layer4);
            layerbuttons.Add(button_Layer5);
            layerbuttons.Add(button_Layer6);
            layerbuttons.Add(button_Layer7);
            layerbuttons.Add(button_Layer8);
            layerbuttons.Add(button_Layer9);

            for (int i = 0; i < tilemap.Tiles.Count; ++i)
                layerbuttons[i].Visible = true;


            timerAutoSave.Interval = Properties.Settings.Default.AutoSaveInterval * 1000 * 60;
            timerAutoSave.Start();

            if (!LoadSettings())
            {
                CreateAppDataPath();
            }
            ToolTip test = new ToolTip();
    
            
            string path = Path.Combine(Environment.CurrentDirectory, @"..\..\selectBox.png");
            if (File.Exists(path))
                selectboxID = ManagedTextureManager.Instance.LoadTexture(path, 0);
            else
                Properties.Settings.Default.TileSelection = false;

            tilemap.Tilesetpath = Path.Combine(Environment.CurrentDirectory, @"..\..\Tiles.png");
            if (File.Exists(tilemap.Tilesetpath))
                tilemap.TilesetID = ManagedTextureManager.Instance.LoadTexture(tilemap.Tilesetpath, 0);

            tilemap.ObjectTilesetpath = Path.Combine(Environment.CurrentDirectory, @"..\..\Objects.png");
            if (File.Exists(tilemap.ObjectTilesetpath))
                tilemap.ObjectTilesetID = ManagedTextureManager.Instance.LoadTexture(tilemap.ObjectTilesetpath, 0);
        }

private void button_AddLayer_Click(object sender, EventArgs e)
        {
            tilemap.AddLayer(renderDepth + 1);
            saved = false;
            renderDepth++;
            layerbuttons[tilemap.Tiles.Count - 1].Visible = true;
            layerbuttons[renderDepth].Focus();
        }

private void button_Layer1_Click(object sender, EventArgs e)
        {
            renderDepth = 0;
        }

private void button_Layer2_Click(object sender, EventArgs e)
        {
            renderDepth = 1;
        }

private void button_Layer3_Click(object sender, EventArgs e)
        {
            renderDepth = 2;
        }

private void button_Layer4_Click(object sender, EventArgs e)
        {
            renderDepth = 3;
        }

private void button_Layer5_Click(object sender, EventArgs e)
        {
            renderDepth = 4;
        }

private void button_Layer6_Click(object sender, EventArgs e)
        {
            renderDepth = 5;
        }

private void button_Layer7_Click(object sender, EventArgs e)
        {
            renderDepth = 6;
        }

private void button_Layer8_Click(object sender, EventArgs e)
        {
            renderDepth = 7;
        }

private void button_Layer9_Click(object sender, EventArgs e)
        {
            renderDepth = 8;
        }

private void button_RemoveLayer_Click(object sender, EventArgs e)
        {
            tilemap.RemoveLayer(renderDepth);

            if (renderDepth != 0 && renderDepth == tilemap.Tiles.Count)
                renderDepth--;

            layerbuttons[renderDepth].Visible = false;
            CascadeButtons();
            saved = false;
            layerbuttons[renderDepth].Focus();
        }

// helper function to make the buttons "fall" into place
        // aka make button 2 fall down into button 1 if button 1 was just removed and 2 is active, then make button 3 fall into button 4 ect.
        private void CascadeButtons()
        {
            for (int i = 8; i > 0; --i)
            {
                if (!layerbuttons[i - 1].Visible && layerbuttons[i].Visible)
                {
                    layerbuttons[i].Visible = false;
                    layerbuttons[i - 1].Visible = true;
                    CascadeButtons();
                }
            }
        }

private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            exit = true;
            this.Close();
        }

private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            exit = true;
            ManagedTextureManager.Instance.ShutdownManagedTextureManager();
        }

private void MainForm_Load(object sender, EventArgs e)
        {

        }

private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!saved)
            {
                if (MessageBox.Show("You have unsaved work, would you like to save before you open a new map? You will loose all unsaved work.", "WorldEditor - NOT saved",
                    MessageBoxButtons.YesNo, MessageBoxIcon.Warning)
                    == DialogResult.Yes)
                {
                    saveToolStripButton_Click(this, e);
                }

                saved = true;
            }

            tilemap.ClearMap();
            renderDepth = 0;
            splitContainer1.Panel1.AutoScrollMinSize = new Size(tilemap.MapSize.Width * tilemap.TileSize.Width, tilemap.MapSize.Height * tilemap.TileSize.Height);

            for (int i = 0; i < 9; ++i)
                Layerbuttons[i].Visible = false;

            for (int i = 0; i < tilemap.Tiles.Count; ++i)
                Layerbuttons[i].Visible = true;
        }

private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = relitivePath;
            ofd.Multiselect = false;
            
            if (DialogResult.OK == ofd.ShowDialog())
            {                
                string filetype = "";

                for (int i = 3; i > 0; --i)
                    filetype += ofd.FileName[ofd.FileName.Length - i];

                
                if (filetype == "xml")
                {
                    if (!saved)
                    {
                        if (MessageBox.Show("You have unsaved work, would you like to save before you open a new map? You will loose all unsaved work.", "WorldEditor - NOT saved",
                            MessageBoxButtons.YesNo, MessageBoxIcon.Warning)
                            == DialogResult.Yes)
                        {
                            saveToolStripButton_Click(this, e);
                        }

                        saved = true;
                    }

                    FileStream reader = new FileStream(ofd.FileName, FileMode.Open);

                    XmlSerializer serializer = new XmlSerializer(typeof(TileMap));


                    TileMap newMap = new TileMap();
                    newMap = (TileMap)serializer.Deserialize(reader);
                    newMap.TilesetID = ManagedTextureManager.Instance.LoadTexture(newMap.Tilesetpath, 0);
                    newMap.ObjectTilesetID = ManagedTextureManager.Instance.LoadTexture(newMap.ObjectTilesetpath, 0);
                    newMap.FileName = ofd.FileName;
                    tilemap = newMap;

                    renderDepth = 0;
                    splitContainer1.Panel1.AutoScrollMinSize = new Size(tilemap.MapSize.Width * tilemap.TileSize.Width, tilemap.MapSize.Height * tilemap.TileSize.Height);

                    for (int i = 0; i < 9; ++i)
                        Layerbuttons[i].Visible = false;

                    for (int i = 0; i < tilemap.Tiles.Count; ++i)
                        Layerbuttons[i].Visible = true;


                    timerAutoSave.Start();

                    reader.Close();
                }
                else if (filetype == "dat")
                {
                    if (!saved)
                    {
                        if (MessageBox.Show("You have unsaved work, would you like to save before you open a new map? You will loose all unsaved work.", "WorldEditor - NOT saved",
                            MessageBoxButtons.YesNo, MessageBoxIcon.Warning)
                            == DialogResult.Yes)
                        {
                            saveToolStripButton_Click(this, e);
                        }

                        saved = true;
                    }

                    tilemap.OpenBIN(ofd.FileName);
                    timerAutoSave.Start();
                }
                else
                {
                    if (tileFocused)
                    {
                        tilemap.Tilesetpath = ofd.FileName;
                        tilemap.TilesetID = ManagedTextureManager.Instance.LoadTexture(tilemap.Tilesetpath, 0);
                        tabPage1.AutoScrollMinSize = new Size(tilemap.TileSetSize.Width * tilemap.TileSize.Width, tilemap.TileSetSize.Height * tilemap.TileSize.Height);
                    }
                    else
                    {
                        tilemap.ObjectTilesetpath = ofd.FileName;
                        tilemap.ObjectTilesetID = ManagedTextureManager.Instance.LoadTexture(tilemap.ObjectTilesetpath, 0);
                        tabPage2.AutoScrollMinSize = new Size(tilemap.ObjectTilSet.Width * tilemap.TileSize.Width, tilemap.ObjectTilSet.Height * tilemap.TileSize.Height);
                    }
                }
            }
            
        }

private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (optform == null)
            {
                optform = new OptForm();
                optform.Owner = this;
                optform.Show();
            }
        }

public void Render()
        {
            RenderMap();

            if (tileFocused)
                RenderTileSet();
            else
                RenderObjectTiles();
        }

private void RenderMap()
        {
            ManagedDirect3D.Instance.ChangeDisplayParam(splitContainer1.Panel1, false);
            ManagedDirect3D.Instance.DeviceBegin();

            #region map
            ManagedDirect3D.Instance.Clear(splitContainer1.Panel1, 0, 0, 0);

            ManagedDirect3D.Instance.SpriteBegin();

            Rectangle cameraRect = new Rectangle(cameraPos.X * -1, cameraPos.Y * -1, splitContainer1.Panel1.Width, splitContainer1.Panel1.Height);
            
            for (int h = (cameraPos.Y*-1)/tilemap.TileSize.Height; (h <= (splitContainer1.Panel1.Height+(cameraPos.Y*-1)) / tilemap.TileSize.Height) && h < tilemap.MapSize.Height; ++h)
            {
                for (int w = (cameraPos.X * -1) / tilemap.TileSize.Width; (w <= (splitContainer1.Panel1.Width + (cameraPos.X * -1)) / tilemap.TileSize.Width) && w < tilemap.MapSize.Width; ++w)
                {
                    if (tilemap.TilesetID != -1)
                    {

                        Rectangle tileRect = new Rectangle(w * tilemap.TileSize.Width,
                                                            h * tilemap.TileSize.Height,
                                                            tilemap.TileSize.Width,
                                                            tilemap.TileSize.Height);

                        if (cameraRect.IntersectsWith(tileRect))
                        {
                            if (tilemap.Tiles[renderDepth][w][h].Type.X != -1 && tilemap.Tiles[renderDepth][w][h].Type.Y != -1)
                            {
                                ManagedTextureManager.Instance.Draw(
                                    tilemap.TilesetID,
                                    tileRect.X + cameraPos.X,
                                    tileRect.Y + cameraPos.Y,
                                    1.0f, 1.0f,
                                    new Rectangle(tilemap.Tiles[renderDepth][w][h].Type.X * tilemap.TileSize.Width,
                                    tilemap.Tiles[renderDepth][w][h].Type.Y * tilemap.TileSize.Height,
                                    tileRect.Width,
                                    tileRect.Height),
                                        0, 0, 0, 0);
                            }
                            else
                            {
                                ManagedTextureManager.Instance.Draw(
                                    tilemap.TilesetID,
                                    tileRect.X + cameraPos.X,
                                    tileRect.Y + cameraPos.Y,
                                    1.0f, 1.0f,
                                    new Rectangle(2 * tilemap.TileSize.Width,
                                    2 * tilemap.TileSize.Height,
                                    tileRect.Width,
                                    tileRect.Height),
                                        0, 0, 0, 0);

                                //tilemap.Tiles[renderDepth][w][h].Type = new Point(2, 2);

                            }
                        }
                    }


                    if (selectboxID != -1)
                    {
                        if (tilemap.Tiles[renderDepth][w][h].Selected && Properties.Settings.Default.TileSelection)
                            ManagedTextureManager.Instance.Draw(
                              selectboxID,
                              w * tilemap.TileSize.Width + cameraPos.X,
                              h * tilemap.TileSize.Height + cameraPos.Y,
                              1.0f, 1.0f,
                              new Rectangle(0, 0, tilemap.TileSize.Width, tilemap.TileSize.Height),
                                  0, 0, 0, 0);
                    }

                }
            }

            if (tilemap.ObjectTilesetID != -1)
            {
                foreach (TileObject t in tilemap.Objects)
                {
                    if (t.Type != new Point(-1, -1))
                    {
                        Rectangle tileRect = new Rectangle(t.Position.X * tilemap.TileSize.Width,
                                                            t.Position.Y * tilemap.TileSize.Height,
                                                            tilemap.TileSize.Width,
                                                            tilemap.TileSize.Height);

                        if (cameraRect.IntersectsWith(tileRect) && t.Position.Z == renderDepth)
                        {
                            ManagedTextureManager.Instance.Draw(
                                tilemap.ObjectTilesetID,
                                tileRect.X + cameraPos.X,
                                tileRect.Y + cameraPos.Y,
                                1.0f, 1.0f,
                                new Rectangle(t.Type.X * tilemap.TileSize.Width,
                                t.Type.Y * tilemap.TileSize.Height,
                                tileRect.Width,
                                tileRect.Height),
                                    0, 0, 0, 0);
                        }
                    }
                }
            }

            if (Properties.Settings.Default.TileSelection && selectionBox.Width > 0 && selectionBox.Height > 0 && selectboxID != -1)
            {
                ManagedTextureManager.Instance.Draw(
                         selectboxID,
                         selectionBox.X + cameraPos.X,
                         selectionBox.Y + cameraPos.Y,
                         1.0f, 1.0f,
                         new Rectangle(0, 0, selectionBox.Width, selectionBox.Height),
                             0, 0, 0, 0);
            }
            ManagedDirect3D.Instance.SpriteEnd();

            // draw verticle lines on map
            for (int i = 0; i <= tilemap.MapSize.Width * tilemap.TileSize.Width; i += tilemap.TileSize.Width)
            {
                if (i <= cameraPos.X * -1 + splitContainer1.Panel1.Width && i >= cameraPos.X*-1)
                    ManagedDirect3D.Instance.DrawLine(i + cameraPos.X, cameraPos.Y, i + cameraPos.X, tilemap.MapSize.Height * tilemap.TileSize.Height + cameraPos.Y,
                                                        255, 255, 255);
            }

            // draw horizontal lines on tilemap
            for (int i = 0; i <= tilemap.MapSize.Height * tilemap.TileSize.Height; i += tilemap.TileSize.Height)
            {
                if (i <= cameraPos.Y * -1 + splitContainer1.Panel1.Height && i >= cameraPos.Y * -1)
                    ManagedDirect3D.Instance.DrawLine(cameraPos.X, i + cameraPos.Y, tilemap.MapSize.Width * tilemap.TileSize.Width + cameraPos.X, i + cameraPos.Y,
                                                        255, 255, 255);
            }
            #endregion

            ManagedDirect3D.Instance.DeviceEnd();

            ManagedDirect3D.Instance.Present(splitContainer1.Panel1);
        }

private void RenderTileSet()
        {
            ManagedDirect3D.Instance.ChangeDisplayParam(tabPage1, false);
            ManagedDirect3D.Instance.DeviceBegin();

            #region tileset
            ManagedDirect3D.Instance.Clear(tabPage1, 0, 0, 0);


            // draw tileset
            ManagedDirect3D.Instance.SpriteBegin();

            if (tilemap.TilesetID != -1)
                ManagedTextureManager.Instance.Draw(tilemap.TilesetID, tilesetCamera.X, tilesetCamera.Y, 1.0f, 1.0f, Rectangle.Empty, 0, 0, 0, 0);

            if (selectboxID != -1)
                ManagedTextureManager.Instance.Draw(selectboxID, tilesetselected.X * tilemap.TileSize.Width + tilesetCamera.X,
                    tilesetselected.Y * tilemap.TileSize.Height + tilesetCamera.Y,
                    1.0f, 1.0f, new Rectangle(0, 0, tilemap.TileSize.Width, tilemap.TileSize.Height), 0, 0, 0, 0);

            ManagedDirect3D.Instance.SpriteEnd();


            // draw verticle lines on tile set
            for (int i = 0; i <= tilemap.TileSetSize.Width * tilemap.TileSize.Width; i += tilemap.TileSize.Width)
            {
                ManagedDirect3D.Instance.DrawLine(i + tilesetCamera.X, tilesetCamera.Y, i + tilesetCamera.X,
                    tilemap.TileSetSize.Height * tilemap.TileSize.Height + 1 + tilesetCamera.Y,
                                                    255, 255, 255);
            }

            // draw horizontal lines on tile set
            for (int i = 0; i <= tilemap.TileSetSize.Height * tilemap.TileSize.Height; i += tilemap.TileSize.Height)
            {
                ManagedDirect3D.Instance.DrawLine(tilesetCamera.X, i + tilesetCamera.Y,
                    tilemap.TileSetSize.Width * tilemap.TileSize.Width + 1 + tilesetCamera.X, i + tilesetCamera.Y,
                                                    255, 255, 255);
            }
            #endregion

            ManagedDirect3D.Instance.DeviceEnd();

            ManagedDirect3D.Instance.Present(tabPage1);
        }

private void RenderObjectTiles()
{
    ManagedDirect3D.Instance.ChangeDisplayParam(tabPage2, false);
    ManagedDirect3D.Instance.DeviceBegin();

    #region tileset
    ManagedDirect3D.Instance.Clear(tabPage2, 0, 0, 0);


    // draw tileset
    ManagedDirect3D.Instance.SpriteBegin();

    if (tilemap.ObjectTilesetID != -1)
        ManagedTextureManager.Instance.Draw(tilemap.ObjectTilesetID, tilesetCamera.X, tilesetCamera.Y, 1.0f, 1.0f, Rectangle.Empty, 0, 0, 0, 0);

    if (selectboxID != -1)
        ManagedTextureManager.Instance.Draw(selectboxID, tilesetselected.X * tilemap.TileSize.Width + tilesetCamera.X,
            tilesetselected.Y * tilemap.TileSize.Height + tilesetCamera.Y,
            1.0f, 1.0f, new Rectangle(0, 0, tilemap.TileSize.Width, tilemap.TileSize.Height), 0, 0, 0, 0);

    ManagedDirect3D.Instance.SpriteEnd();


    // draw verticle lines on tile set
    for (int i = 0; i <= tilemap.ObjectTilSet.Width * tilemap.TileSize.Width; i += tilemap.TileSize.Width)
    {
        ManagedDirect3D.Instance.DrawLine(i + tilesetCamera.X, tilesetCamera.Y, i + tilesetCamera.X,
            tilemap.ObjectTilSet.Height * tilemap.TileSize.Height + 1 + tilesetCamera.Y,
                                            255, 255, 255);
    }

    // draw horizontal lines on tile set
    for (int i = 0; i <= tilemap.ObjectTilSet.Height * tilemap.TileSize.Height; i += tilemap.TileSize.Height)
    {
        ManagedDirect3D.Instance.DrawLine(tilesetCamera.X, i + tilesetCamera.Y,
            tilemap.ObjectTilSet.Width * tilemap.TileSize.Width + 1 + tilesetCamera.X, i + tilesetCamera.Y,
                                            255, 255, 255);
    }
    #endregion

    ManagedDirect3D.Instance.DeviceEnd();

    ManagedDirect3D.Instance.Present(tabPage2);
}

private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();

            //"Text files (*.txt)|*.txt|All files (*.*)|*.*"
            sfd.Filter = "Binary file (*.dat)|*.dat|XML file (*.xml)|*.xml";
            sfd.InitialDirectory = relitivePath;
            sfd.DefaultExt = "dat";

            if (DialogResult.OK == sfd.ShowDialog())
            {
                tilemap.FileName = sfd.FileName;

                switch (sfd.FilterIndex)
                {
                    case 1:
                        tilemap.SaveBIN(tilemap.FileName);
                        break;
                    case 2:
                        TextWriter writer = new StreamWriter(sfd.FileName);

                        XmlSerializer serializer = new XmlSerializer(typeof(TileMap));

                        serializer.Serialize(writer, tilemap);

                        writer.Close();
                        saved = true;
                        break;
                }
            }            
        }

private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (tilemap.FileName == "Untitled")
                saveAsToolStripMenuItem_Click(this, e);
            else
            {
                string filetype = "";

                for (int i = 3; i > 0; --i)
                    filetype += tilemap.FileName[tilemap.FileName.Length - i];

                if (filetype == "xml")
                {
                    TextWriter writer = new StreamWriter(tilemap.FileName);

                    XmlSerializer serializer = new XmlSerializer(typeof(TileMap));

                    serializer.Serialize(writer, tilemap);

                    writer.Close();

                    saved = true;
                }
                else if (filetype == "dat")
                {

                }
            }
            layerbuttons[renderDepth].Focus();
        }

private void SimilarDeSelect(Tile t, int w, int h)
{
    if (h != 0 && h > (cameraPos.Y * -1) / tilemap.TileSize.Height)
    {
        t.Selected = false;
        if (tilemap.Tiles[renderDepth][w][h - 1].Selected && tilemap.Tiles[renderDepth][w][h - 1].Type == t.Type)
        {
            SimilarDeSelect(tilemap.Tiles[renderDepth][w][h - 1], w, h - 1);
        }
    }
    if (h != tilemap.MapSize.Height - 1 && h < ((cameraPos.Y * -1) + splitContainer1.Panel1.Height) / tilemap.TileSize.Height)
    {
        t.Selected = false;
        if (tilemap.Tiles[renderDepth][w][h + 1].Selected && tilemap.Tiles[renderDepth][w][h + 1].Type == t.Type)
        {
            SimilarDeSelect(tilemap.Tiles[renderDepth][w][h + 1], w, h + 1);
        }
    }

    if (w != 0 && w > (cameraPos.X * -1) / tilemap.TileSize.Width)
    {
        t.Selected = false;
        if (tilemap.Tiles[renderDepth][w - 1][h].Selected && tilemap.Tiles[renderDepth][w - 1][h].Type == t.Type)
        {
            SimilarDeSelect(tilemap.Tiles[renderDepth][w - 1][h], w - 1, h);
        }
    }
    if (w != tilemap.MapSize.Width - 1 && w < ((cameraPos.X * -1) + splitContainer1.Panel1.Width) / tilemap.TileSize.Width)
    {
        t.Selected = false;
        if (tilemap.Tiles[renderDepth][w + 1][h].Selected && tilemap.Tiles[renderDepth][w + 1][h].Type == t.Type)
        {
            SimilarDeSelect(tilemap.Tiles[renderDepth][w + 1][h], w + 1, h);
        }
    }
}

private void SimilarSelect(Tile t, int w, int h)
{
    if (h != 0 && h > (cameraPos.Y*-1) / tilemap.TileSize.Height)
    {
        t.Selected = true;
        if (!tilemap.Tiles[renderDepth][w][h - 1].Selected && tilemap.Tiles[renderDepth][w][h - 1].Type == t.Type)
        {
            SimilarSelect(tilemap.Tiles[renderDepth][w][h - 1], w, h - 1);
        }
    }

    if (h != tilemap.MapSize.Height - 1 && h < ((cameraPos.Y*-1)+splitContainer1.Panel1.Height) / tilemap.TileSize.Height)
    {
        t.Selected = true;
        if (!tilemap.Tiles[renderDepth][w][h + 1].Selected && tilemap.Tiles[renderDepth][w][h + 1].Type == t.Type)
        {
            SimilarSelect(tilemap.Tiles[renderDepth][w][h + 1], w, h + 1);
        }
    }

    if (w != 0 && w > (cameraPos.X * -1) / tilemap.TileSize.Width)
    {
        t.Selected = true;
        if (!tilemap.Tiles[renderDepth][w - 1][h].Selected && tilemap.Tiles[renderDepth][w - 1][h].Type == t.Type)
        {
            SimilarSelect(tilemap.Tiles[renderDepth][w - 1][h], w - 1, h);
        }
    }

    if (w != tilemap.MapSize.Width - 1 && w < ((cameraPos.X * -1) + splitContainer1.Panel1.Width) / tilemap.TileSize.Width)
    {
        t.Selected = true;
        if (!tilemap.Tiles[renderDepth][w + 1][h].Selected && tilemap.Tiles[renderDepth][w + 1][h].Type == t.Type)
        {
            SimilarSelect(tilemap.Tiles[renderDepth][w + 1][h], w + 1, h);
        }
    }
}

private void SimilarSelect(TileObject t, int w, int h)
{
    if (h != 0 && h > (cameraPos.Y * -1) / tilemap.TileSize.Height)
    {
        t.Selected = true;
        tilemap.Tiles[renderDepth][w][h].Selected = true;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w, h - 1, renderDepth)))
            {
                if (!tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarSelect(tilemap.Objects[i], w, h - 1);

                break;
            }
        }
    }

    if (h != tilemap.MapSize.Height - 1 && h < ((cameraPos.Y * -1) + splitContainer1.Panel1.Height) / tilemap.TileSize.Height)
    {
        t.Selected = true;
        tilemap.Tiles[renderDepth][w][h].Selected = true;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w, h + 1, renderDepth)))
            {
                if (!tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarSelect(tilemap.Objects[i], w, h + 1);

                break;
            }
        }
    }

    if (w != 0 && w > (cameraPos.X * -1) / tilemap.TileSize.Width)
    {
        t.Selected = true;
        tilemap.Tiles[renderDepth][w][h].Selected = true;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w - 1, h, renderDepth)))
            {
                if (!tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarSelect(tilemap.Objects[i], w - 1, h);

                break;
            }
        }
    }

    if (w != tilemap.MapSize.Width - 1 && w < ((cameraPos.X * -1) + splitContainer1.Panel1.Width) / tilemap.TileSize.Width)
    {
        t.Selected = true;
        tilemap.Tiles[renderDepth][w][h].Selected = true;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w + 1, h, renderDepth)))
            {
                if (!tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarSelect(tilemap.Objects[i], w + 1, h);

                break;
            }
        }
    }
}

private void SimilarDeSelect(TileObject t, int w, int h)
{
    if (h != 0 && h > (cameraPos.Y * -1) / tilemap.TileSize.Height)
    {
        t.Selected = false;
        tilemap.Tiles[renderDepth][w][h].Selected = false;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w, h - 1, renderDepth)))
            {
                if (tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarDeSelect(tilemap.Objects[i], w, h - 1);

                break;
            }
        }
    }

    if (h != tilemap.MapSize.Height - 1 && h < ((cameraPos.Y * -1) + splitContainer1.Panel1.Height) / tilemap.TileSize.Height)
    {
        t.Selected = false;
        tilemap.Tiles[renderDepth][w][h].Selected = false;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w, h + 1, renderDepth)))
            {
                if (tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarDeSelect(tilemap.Objects[i], w, h + 1);

                break;
            }
        }
    }

    if (w != 0 && w > (cameraPos.X * -1) / tilemap.TileSize.Width)
    {
        t.Selected = false;
        tilemap.Tiles[renderDepth][w][h].Selected = false;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w - 1, h, renderDepth)))
            {
                if (tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarDeSelect(tilemap.Objects[i], w - 1, h);

                break;
            }
        }
    }

    if (w != tilemap.MapSize.Width - 1 && w < ((cameraPos.X * -1) + splitContainer1.Panel1.Width) / tilemap.TileSize.Width)
    {
        t.Selected = false;
        tilemap.Tiles[renderDepth][w][h].Selected = false;

        for (int i = 0; i < tilemap.Objects.Count; ++i)
        {
            if (tilemap.Objects[i].Position.Equals(new Point3D(w + 1, h, renderDepth)))
            {
                if (tilemap.Objects[i].Selected && tilemap.Objects[i].Type == t.Type)
                    SimilarDeSelect(tilemap.Objects[i], w + 1, h);

                break;
            }
        }
    }
}


private void splitContainer1_Panel1_MouseClick(object sender, MouseEventArgs e)
        {
            Point offset = e.Location;

            offset.X -= splitContainer1.Panel1.AutoScrollPosition.X;
            offset.Y -= splitContainer1.Panel1.AutoScrollPosition.Y;

            if (offset.X < 0)
                offset.X = 0;

            if (offset.Y < 0)
                offset.Y = 0;

            if (Properties.Settings.Default.TileSelection)
            {
                if (e.Button == MouseButtons.Left)
                {
                    if (!(ModifierKeys == Keys.Shift))
                    {
                        for (int h2 = 0; h2 < tilemap.MapSize.Height; ++h2)
                        {
                            for (int w2 = 0; w2 < tilemap.MapSize.Width; ++w2)
                            {
                                tilemap.Tiles[renderDepth][w2][h2].Selected = false;
                            }
                        }

                        for (int i = 0; i < tilemap.Objects.Count; ++i)
                        {
                            tilemap.Objects[i].Selected = false;
                        }
                    }
                    
                    if (offset.X / tilemap.TileSize.Width < tilemap.MapSize.Width && offset.Y / tilemap.TileSize.Height < tilemap.MapSize.Height)
                    {
                        tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Selected =
                            !tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Selected;


                        for (int i = 0; i < tilemap.Objects.Count; ++i)
                        {
                            if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                            {
                                tilemap.Objects[i].Selected = !tilemap.Objects[i].Selected;
                                break;
                            }
                        }
                    }
                }

                if (e.Button == MouseButtons.Right)
                {
                    if (tileFocused && tilemap.TilesetID != -1)
                    {
                        for (int h = 0; h < tilemap.MapSize.Height; ++h)
                        {
                            for (int w = 0; w < tilemap.MapSize.Width; ++w)
                            {
                                if (tilemap.Tiles[renderDepth][w][h].Selected)
                                {
                                    tilemap.Tiles[renderDepth][w][h].Type = new Point(-1, -1);

                                    for (int i = 0; i < tilemap.Objects.Count; ++i)
                                    {
                                        if (tilemap.Objects[i].Position.Equals(new Point3D(w, h, renderDepth)))
                                        {
                                            tilemap.Objects.Remove(tilemap.Objects[i]);
                                            saved = false;
                                            break;
                                        }
                                    }
                                }

                                saved = false;
                            }
                        }
                    }

                    if (!tileFocused && tilemap.ObjectTilesetID != -1 && tilemap.TilesetID != -1)
                    {
                        for(int i = 0; i < tilemap.Objects.Count; ++i)
                        {
                            if (tilemap.Objects[i].Selected)
                            {
                                tilemap.Objects.Remove(tilemap.Objects[i--]);
                                saved = false;
                            }
                        }
                    }
                }

                if (e.Button == MouseButtons.Middle)
                {
                    for (int h = 0; h < tilemap.MapSize.Height; ++h)
                    {
                        for (int w = 0; w < tilemap.MapSize.Width; ++w)
                        {
                            if (tilemap.Tiles[renderDepth][w][h].Selected)
                            {
                                if ((Properties.Settings.Default.Override || tilemap.Tiles[renderDepth][w][h].Type == new Point(-1, -1)) &&
                                    tileFocused && tilemap.TilesetID != -1)
                                {
                                    bool alreadythere = false;
                                    for (int i = 0; i < tilemap.Objects.Count; ++i)
                                    {
                                        if (tilemap.Objects[i].Position.Equals(new Point3D(w, h, renderDepth)) && tilesetselected != new Point(0, 0))
                                        {
                                            alreadythere = true;
                                            if (Properties.Settings.Default.Override)
                                            {
                                                tilemap.Objects.Remove(tilemap.Objects[i]);
                                                saved = false;
                                            }
                                            break;
                                        }

                                    }

                                    if (!alreadythere || Properties.Settings.Default.Override)
                                    {
                                        tilemap.Tiles[renderDepth][w][h].Type = tilesetselected;

                                        saved = false;
                                    }
                                }

                                if ((tilemap.Tiles[renderDepth][w][h].Type == new Point(0, 0) || Properties.Settings.Default.Override) && 
                                    !tileFocused && tilemap.ObjectTilesetID != -1 && tilemap.TilesetID != -1)
                                {
                                    bool alreadythere = false;
                                    for (int i = 0; i < tilemap.Objects.Count; ++i)
                                    {
                                        if (tilemap.Objects[i].Position.Equals(new Point3D(w, h, renderDepth)))
                                        {
                                            alreadythere = true;
                                            if (Properties.Settings.Default.Override)
                                            {
                                                tilemap.Objects.Remove(tilemap.Objects[i]);
                                                saved = false;
                                            }
                                            break;
                                        }

                                    }

                                    if (!alreadythere || Properties.Settings.Default.Override)
                                    {
                                        tilemap.Objects.Add(new TileObject(w, h, renderDepth, tilesetselected, true));

                                        if (tilemap.Tiles[renderDepth][w][h].Type != new Point(0, 0))
                                            tilemap.Tiles[renderDepth][w][h].Type = new Point(0, 0);

                                        saved = false;
                                    }
                                }
                            }
                        }
                    }
                }

            }
            else
            {
                if (offset.X < tilemap.MapSize.Width * tilemap.TileSize.Width &&
                    offset.Y < tilemap.MapSize.Height * tilemap.TileSize.Height)
                {
                    if (e.Button == System.Windows.Forms.MouseButtons.Left)
                    {
                        if (tileFocused && tilemap.TilesetID != -1)
                        {
                            if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type != tilesetselected)
                            {
                                tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type = tilesetselected;
                                saved = false;
                            }
                        }
                        else if(tilemap.ObjectTilesetID != -1)
                        {
                            if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type == new Point(0, 0) || 
                                Properties.Settings.Default.Override)
                            {
                                bool alreadythere = false;
                                for (int i = 0; i < tilemap.Objects.Count; ++i)
                                {
                                    if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                                    {
                                        alreadythere = true;
                                        if (Properties.Settings.Default.Override)
                                        {
                                            tilemap.Objects.Remove(tilemap.Objects[i]);
                                            saved = false;
                                        }
                                        break;
                                    }

                                }

                                if (!alreadythere || Properties.Settings.Default.Override)
                                {
                                    tilemap.Objects.Add(new TileObject(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth, tilesetselected, true));

                                    if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type != new Point(0, 0))
                                        tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type = new Point(0, 0);

                                    saved = false;
                                }
                            }
                        }
                    }
                    else if (e.Button == MouseButtons.Right)
                    {
                        if (tileFocused && tilemap.TilesetID != -1)
                        {
                            if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type != new Point(-1, -1))
                            {
                                tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type = new Point(-1, -1);

                                for (int i = 0; i < tilemap.Objects.Count; ++i)
                                {
                                    if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                                    {
                                        tilemap.Objects.Remove(tilemap.Objects[i]);
                                        saved = false;
                                        break;
                                    }
                                }

                                saved = false;
                            }
                        }
                        else if(tilemap.ObjectTilesetID != -1)
                        {
                            for (int i = 0; i < tilemap.Objects.Count; ++i)
                            {
                                if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                                {
                                    tilemap.Objects.Remove(tilemap.Objects[i]);
                                    saved = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

private void splitContainer1_Panel1_MouseDoubleClick(object sender, MouseEventArgs e)
{
    if (Properties.Settings.Default.TileSelection)
    {
        Point offset = e.Location;

        offset.X -= splitContainer1.Panel1.AutoScrollPosition.X;
        offset.Y -= splitContainer1.Panel1.AutoScrollPosition.Y;

        if (offset.X < 0)
            offset.X = 0;

        if (offset.Y < 0)
            offset.Y = 0;

        if (e.Button == MouseButtons.Left)
        {
            if (offset.X < tilemap.MapSize.Width * tilemap.TileSize.Width &&
                    offset.Y < tilemap.MapSize.Height * tilemap.TileSize.Height)
            {
                if (tileFocused)
                {
                    if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Selected)
                    {
                        tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Selected = true;
                        SimilarSelect(tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height],
                            offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height);
                    }
                    else
                    {
                        tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Selected = true;
                        SimilarDeSelect(tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height],
                            offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height);
                    }
                }
                else
                {

                    for (int i = 0; i < tilemap.Objects.Count; ++i)
                    {
                        if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                        {
                            if (tilemap.Objects[i].Selected)
                            {
                                tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Selected = true;
                                tilemap.Objects[i].Selected = true;

                                SimilarSelect(tilemap.Objects[i], offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height);
                            }
                            else
                            {
                                tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Selected = true;
                                tilemap.Objects[i].Selected = true;

                                SimilarDeSelect(tilemap.Objects[i], offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height);
                            }

                            break;
                        }
                    }


                }
            }
        }
    }
}

private void splitContainer1_Panel1_MouseDown(object sender, MouseEventArgs e)
{
    if (e.Button == MouseButtons.Left)
    {
        Point offset = e.Location;

        offset.X -= splitContainer1.Panel1.AutoScrollPosition.X;
        offset.Y -= splitContainer1.Panel1.AutoScrollPosition.Y;

        if (offset.X < 0)
            offset.X = 0;

        if (offset.Y < 0)
            offset.Y = 0;


        if (Properties.Settings.Default.TileSelection)
        {
            selectionBox.X = offset.X;
            selectionBox.Y = offset.Y;
            startboxPoint = offset;
        }


    }
}

private void splitContainer1_Panel1_MouseMove(object sender, MouseEventArgs e)
        {
            Point offset = e.Location;

            offset.X -= splitContainer1.Panel1.AutoScrollPosition.X;
            offset.Y -= splitContainer1.Panel1.AutoScrollPosition.Y;

            if (offset.X < 0)
                offset.X = 0;

            if (offset.Y < 0)
                offset.Y = 0;

            toolStripStatusLabel2.Text = offset.ToString();

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                if (!Properties.Settings.Default.TileSelection)
                {
                    if (offset.X < tilemap.MapSize.Width * tilemap.TileSize.Width &&
                            offset.Y < tilemap.MapSize.Height * tilemap.TileSize.Height )
                    {

                        if (tileFocused && tilemap.TilesetID != -1)
                        {
                            if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type != tilesetselected)
                            {
                                tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type = tilesetselected;
                                saved = false;
                            }
                        }
                        else
                        {
                            if ((tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type == new Point(0, 0)
                            || Properties.Settings.Default.Override) && tilemap.ObjectTilesetID != -1)
                            {
                                bool alreadythere = false;
                                for (int i = 0; i < tilemap.Objects.Count; ++i)
                                {
                                    if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                                    {
                                        alreadythere = true;
                                        if (Properties.Settings.Default.Override)
                                        {
                                            tilemap.Objects.Remove(tilemap.Objects[i]);
                                            saved = false;
                                        }
                                        break;
                                    }

                                }

                                if (!alreadythere || Properties.Settings.Default.Override)
                                {
                                    tilemap.Objects.Add(new TileObject(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth, tilesetselected, true));

                                    if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type != new Point(0, 0))
                                        tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type = new Point(0, 0);

                                    saved = false;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (selectionBox.X < offset.X)
                    {
                        selectionBox.Width = offset.X - selectionBox.X;
                    }
                    else if (selectionBox.X > offset.X)
                    {
                        selectionBox.X = offset.X;
                        selectionBox.Width = startboxPoint.X - offset.X;
                    }

                    if (selectionBox.Y < offset.Y)
                    {
                        selectionBox.Height = offset.Y - selectionBox.Y;
                    }
                    else if (selectionBox.Y > offset.Y)
                    {
                        selectionBox.Y = offset.Y;
                        selectionBox.Height = startboxPoint.Y - offset.Y;
                    }
                }
            }
                


            if (e.Button == System.Windows.Forms.MouseButtons.Right && !Properties.Settings.Default.TileSelection)
            {
                if (offset.X < tilemap.MapSize.Width * tilemap.TileSize.Width &&
                        offset.Y < tilemap.MapSize.Height * tilemap.TileSize.Height)
                {
                    if (tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type != new Point(-1, -1) && 
                        tileFocused && tilemap.TilesetID != -1)
                    {
                        tilemap.Tiles[renderDepth][offset.X / tilemap.TileSize.Width][offset.Y / tilemap.TileSize.Height].Type = new Point(-1, -1);
                        saved = false;

                        for (int i = 0; i < tilemap.Objects.Count; ++i)
                        {
                            if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                            {
                                tilemap.Objects.Remove(tilemap.Objects[i]);
                                saved = false;
                                break;
                            }
                        }

                        saved = false;
                    }

                    if (!tileFocused && tilemap.ObjectTilesetID != -1 && tilemap.TilesetID != -1)
                    {
                        for (int i = 0; i < tilemap.Objects.Count; ++i)
                        {
                            if (tilemap.Objects[i].Position.Equals(new Point3D(offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height, renderDepth)))
                            {
                                tilemap.Objects.Remove(tilemap.Objects[i]);
                                saved = false;
                                break;
                            }
                        }
                    }
                }
            }


            if (offset.X < tilemap.MapSize.Width * tilemap.TileSize.Width &&
                    offset.Y < tilemap.MapSize.Height * tilemap.TileSize.Height)
            {
                string s = "{";
                s += string.Format("X={0},Y={1}", offset.X / tilemap.TileSize.Width, offset.Y / tilemap.TileSize.Height);
                s += "}";

                toolStripStatusLabel4.Text = s;
            }
            
        }

private void splitContainer1_Panel1_MouseUp(object sender, MouseEventArgs e)
{
    if (e.Button == MouseButtons.Left && Properties.Settings.Default.TileSelection)
    {
        Point offset = e.Location;

        offset.X -= splitContainer1.Panel1.AutoScrollPosition.X;
        offset.Y -= splitContainer1.Panel1.AutoScrollPosition.Y;

        if (offset.X < 0)
            offset.X = 0;

        if (offset.Y < 0)
            offset.Y = 0;

        if (offset != startboxPoint)
        {
            if (selectionBox.X < offset.X)
            {
                selectionBox.Width = offset.X - selectionBox.X;
            }
            else if (selectionBox.X > offset.X)
            {
                selectionBox.X = offset.X;
                selectionBox.Width = startboxPoint.X - offset.X;
            }

            if (selectionBox.Y < offset.Y)
            {
                selectionBox.Height = offset.Y - selectionBox.Y;
            }
            else if (selectionBox.Y > offset.Y)
            {
                selectionBox.Y = offset.Y;
                selectionBox.Height = startboxPoint.Y - offset.Y;
            }

            for (int h = (cameraPos.Y * -1) / tilemap.TileSize.Height; h < tilemap.MapSize.Height; ++h)
            {
                for (int w = (cameraPos.X * -1) / tilemap.TileSize.Width; w < tilemap.MapSize.Width; ++w)
                {
                    Rectangle rect = new Rectangle(w * tilemap.TileSize.Width, h * tilemap.TileSize.Height, tilemap.TileSize.Width, tilemap.TileSize.Height);

                    if (rect.IntersectsWith(selectionBox))
                    {
                        tilemap.Tiles[renderDepth][w][h].Selected = true;

                        for (int i = 0; i < tilemap.Objects.Count; ++i)
                        {
                            if (tilemap.Objects[i].Position.Equals(new Point3D(w, h, renderDepth)))
                            {
                                tilemap.Objects[i].Selected = true;
                                break;
                            }
                        }
                    }
                }
            }

            selectionBox.X = -1;
            selectionBox.Y = -1;
            selectionBox.Width = -1;
            selectionBox.Height = -1;
            startboxPoint.X = -1;
            startboxPoint.Y = -1;
        }
    }
}

private void splitContainer1_Panel1_Scroll(object sender, ScrollEventArgs e)
        {
            cameraPos = splitContainer1.Panel1.AutoScrollPosition;
            RenderMap();
        }

private void tabPage1_MouseClick_1(object sender, MouseEventArgs e)
        {
            Point offset = e.Location;

            offset.X -= splitContainer2.Panel1.AutoScrollPosition.X;
            offset.Y -= splitContainer2.Panel1.AutoScrollPosition.Y;

            if (offset.X < 0)
                offset.X = 0;

            if (offset.Y < 0)
                offset.Y = 0;

            for (int h = 0; h < tilemap.TileSetSize.Height; ++h)
            {
                for (int w = 0; w < tilemap.TileSetSize.Width; ++w)
                {
                    Rectangle rect = new Rectangle(w * tilemap.TileSize.Width
                        , h * tilemap.TileSize.Height, tilemap.TileSize.Width, tilemap.TileSize.Height);

                    if (rect.Contains(offset))
                    {
                        tilesetselected.X = w;
                        tilesetselected.Y = h;
                    }
                }
            }

            if (Properties.Settings.Default.TileSelection && tilemap.TilesetID != -1)
            {
                for (int h = 0; h < tilemap.MapSize.Height; ++h)
                {
                    for (int w = 0; w < tilemap.MapSize.Width; ++w)
                    {
                        if (tilemap.Tiles[renderDepth][w][h].Selected)
                        {
                            if (Properties.Settings.Default.Override || tilemap.Tiles[renderDepth][w][h].Type == new Point(-1, -1))
                            {
                                bool alreadythere = false;
                                for (int i = 0; i < tilemap.Objects.Count; ++i)
                                {
                                    if (tilemap.Objects[i].Position.Equals(new Point3D(w, h, renderDepth)) && tilesetselected != new Point(0, 0))
                                    {
                                        alreadythere = true;
                                        if (Properties.Settings.Default.Override)
                                        {
                                            tilemap.Objects.Remove(tilemap.Objects[i]);
                                            saved = false;
                                        }
                                        break;
                                    }

                                }

                                if (!alreadythere || Properties.Settings.Default.Override)
                                {
                                    tilemap.Tiles[renderDepth][w][h].Type = tilesetselected;

                                    saved = false;
                                }
                            }
                        }
                    }
                }
            }
        }

new public void Update()
        {
            if (tilemap.Tiles.Count == 1 && button_RemoveLayer.Enabled)
                button_RemoveLayer.Enabled = false;
            else if (tilemap.Tiles.Count > 1 && !button_RemoveLayer.Enabled)
                button_RemoveLayer.Enabled = true;


            if (tilemap.Tiles.Count == 9 && button_RemoveLayer.Enabled)
                button_AddLayer.Enabled = false;
            else if (tilemap.Tiles.Count < 9 && !button_AddLayer.Enabled)
                button_AddLayer.Enabled = true;


            if (tileFocused)
            {
                for (int h = 0; h < tilemap.MapSize.Height; ++h)
                {
                    for (int w = 0; w < tilemap.MapSize.Width; ++w)
                    {
                        if (tilemap.Tiles[renderDepth][w][h].Selected)
                        {
                            label2.Text = tilemap.Tiles[renderDepth][w][h].Property;
                            break;
                        }
                    }
                }
            }
            else
            {
                bool oneselected = false;
                for (int i = 0; i < tilemap.Objects.Count; ++i)
                {
                    if (tilemap.Objects[i].Selected)
                    {
                        label2.Text = tilemap.Objects[i].Property;
                        oneselected = true;
                        break;
                    }
                }

                if (!oneselected)
                    label2.Text = "";
            }
        }

private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
{
    tileFocused = !tileFocused;
    tilesetselected = new Point(0, 0);
    if (tileFocused)
    {        
        tabPage1.AutoScrollMinSize = new Size(tilemap.TileSetSize.Width * tilemap.TileSize.Width, tilemap.TileSetSize.Height * tilemap.TileSize.Height);
        tabPage1_Scroll(this, null);
    }
    else
    {
        tabPage2.AutoScrollMinSize = new Size(tilemap.ObjectTilSet.Width * tilemap.TileSize.Width, tilemap.ObjectTilSet.Height * tilemap.TileSize.Height);
        tabPage2_Scroll(this, null);
    }
    layerbuttons[renderDepth].Focus();
}
    
private void tabPage2_MouseClick(object sender, MouseEventArgs e)
{
    Point offset = e.Location;

    offset.X -= splitContainer2.Panel1.AutoScrollPosition.X;
    offset.Y -= splitContainer2.Panel1.AutoScrollPosition.Y;

    if (offset.X < 0)
        offset.X = 0;

    if (offset.Y < 0)
        offset.Y = 0;

    for (int h = 0; h < tilemap.ObjectTilSet.Height; ++h)
    {
        for (int w = 0; w < tilemap.ObjectTilSet.Width; ++w)
        {
            Rectangle rect = new Rectangle(w * tilemap.TileSize.Width
                , h * tilemap.TileSize.Height, tilemap.TileSize.Width, tilemap.TileSize.Height);

            if (rect.Contains(offset))
            {
                tilesetselected.X = w;
                tilesetselected.Y = h;
            }
        }
    }

    if (Properties.Settings.Default.TileSelection && tilemap.ObjectTilesetID != -1 && tilemap.TilesetID != -1)
    {
        for (int h = 0; h < tilemap.MapSize.Height; ++h)
        {
            for (int w = 0; w < tilemap.MapSize.Width; ++w)
            {
                if (tilemap.Tiles[renderDepth][w][h].Selected)
                {
                    if (tilemap.Tiles[renderDepth][w][h].Type == new Point(0, 0) || Properties.Settings.Default.Override)
                    {
                        bool alreadythere = false;
                        for (int i = 0; i < tilemap.Objects.Count; ++i)
                        {
                            if (tilemap.Objects[i].Position.Equals(new Point3D(w, h, renderDepth)))
                            {
                                alreadythere = true;
                                if (Properties.Settings.Default.Override)
                                {
                                    tilemap.Objects.Remove(tilemap.Objects[i]);
                                    saved = false;
                                }
                                break;
                            }

                        }

                        if (!alreadythere || Properties.Settings.Default.Override)
                        {
                            tilemap.Objects.Add(new TileObject(w, h, renderDepth, tilesetselected, true));

                            if (tilemap.Tiles[renderDepth][w][h].Type != new Point(0, 0))
                                tilemap.Tiles[renderDepth][w][h].Type = new Point(0, 0);

                            saved = false;
                        }
                    }
                }
            }
        }
    }
}

private void tabControl1_Click(object sender, EventArgs e)
{
    layerbuttons[renderDepth].Focus();
}

private void tabControl2_Click(object sender, EventArgs e)
{
    layerbuttons[renderDepth].Focus();
}

private void newToolStripButton_Click(object sender, EventArgs e)
{
    newToolStripMenuItem_Click(this, e);
    layerbuttons[renderDepth].Focus();
}

private void openToolStripButton_Click(object sender, EventArgs e)
{
    openToolStripMenuItem_Click(this, e);
    layerbuttons[renderDepth].Focus();
}

private void saveToolStripButton_Click(object sender, EventArgs e)
{
    if (tilemap.FileName == "Untitled")
        saveAsToolStripMenuItem_Click(this, e);
    else
    {
        string filetype = "";

        for (int i = 3; i > 0; --i)
            filetype += tilemap.FileName[tilemap.FileName.Length - i];

        if (filetype == "xml")
        {
            TextWriter writer = new StreamWriter(tilemap.FileName);

            XmlSerializer serializer = new XmlSerializer(typeof(TileMap));

            serializer.Serialize(writer, tilemap);

            writer.Close();

            saved = true;
        }
        else if(filetype == "dat")
        {

        }
    }
    layerbuttons[renderDepth].Focus();
}

private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
{
    MessageBox.Show("Designed and developed by Jon \"Alfonso\" Taylor\nAssisted by:\n\t-Ethan Pendergraft\n\t-Rob Taylor\nPurpose:\n\t-Tool used to help design and create\n\tlevels for the game Labrinthium.\n\n-Property of Team Battle Finch.", "Labrinthium - World Editor: About");
    layerbuttons[renderDepth].Focus();
}

private void CreateAppDataPath()
{
    try
    {
        string pathtoEXE = Application.UserAppDataPath;
        string newPath = Path.Combine(pathtoEXE, @"Settings");
        string pathtofile = Path.Combine(pathtoEXE, @"Settings\settings.dat");
        System.IO.Directory.CreateDirectory(newPath);
        File.Create(pathtofile);
    }
    catch (Exception)
    {

    }
}

private bool LoadSettings()
{
    string pathtoEXE = Application.UserAppDataPath;
    string pathtofile = Path.Combine(pathtoEXE, @"Settings\settings.dat");
    BinaryReader br;

    try
    {
        br = new BinaryReader(File.Open(pathtofile, FileMode.Open));
    }
    catch (DirectoryNotFoundException)
    {
        return false;
    }

    try
    {
        relitivePath = br.ReadString();
    }
    catch (EndOfStreamException)
    {
        return false;
    }
    finally
    {
        br.Close();
    }

    return true;
}

private void SaveSettings()
{
    try
    {
        string pathtoEXE = Application.UserAppDataPath;
        string pathtofile = Path.Combine(pathtoEXE, @"Settings\settings.dat");
        BinaryWriter bw = new BinaryWriter(File.Open(pathtofile, FileMode.Truncate));
        bw.Write(relitivePath);
        bw.Close();
    }
    catch (DirectoryNotFoundException)
    {
        return;
    }
}

private void setSaveDirectoryToolStripMenuItem_Click(object sender, EventArgs e)
{
    FolderBrowserDialog fbd = new FolderBrowserDialog();
    fbd.Description = "Set your default directory...";

    if (DialogResult.OK == fbd.ShowDialog())
    {
        relitivePath = fbd.SelectedPath;
        SaveSettings();
    }
}

private void timerAutoSave_Tick(object sender, EventArgs e)
{
    if (tilemap.FileName != "Untitled" && Properties.Settings.Default.autoSave)
    {
        string filetype = "";

        for (int i = 3; i > 0; --i)
            filetype += tilemap.FileName[tilemap.FileName.Length - i];


        if (filetype == "xml")
        {
            TextWriter writer = new StreamWriter(tilemap.FileName);

            XmlSerializer serializer = new XmlSerializer(typeof(TileMap));

            serializer.Serialize(writer, tilemap);

            writer.Close();
        }
        else if (filetype == "dat")
        {

        }
        saved = true;
    }
}

private void tabPage1_Scroll(object sender, ScrollEventArgs e)
{
    tilesetCamera = tabPage1.AutoScrollPosition;
    RenderTileSet();
}

private void tabPage2_Scroll(object sender, ScrollEventArgs e)
{
    tilesetCamera = tabPage2.AutoScrollPosition;
    RenderObjectTiles();
}

private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
{
    if (!saved)
    {
        if (MessageBox.Show("You have unsaved work, would you like to save before you quit? You will loose all unsaved work.", "WorldEditor - NOT saved",
            MessageBoxButtons.YesNo, MessageBoxIcon.Warning)
            == DialogResult.Yes)
        {
            saveToolStripButton_Click(this, e);
            e.Cancel = true;
        }
    }
}

private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
{
    if (e.KeyChar == (char)Keys.Enter)
    {
        if (tileFocused)
        {
            for (int h = 0; h < tilemap.MapSize.Height; ++h)
            {
                for (int w = 0; w < tilemap.MapSize.Width; ++w)
                {
                    if (tilemap.Tiles[renderDepth][w][h].Selected)
                    {
                        tilemap.Tiles[renderDepth][w][h].Property = textBox1.Text;
                        saved = false;
                    }
                }
            }
        }
        else
        {
            bool oneselected = false;
            for (int i = 0; i < tilemap.Objects.Count; ++i)
            {
                if (tilemap.Objects[i].Selected)
                {
                    tilemap.Objects[i].Property = textBox1.Text;
                    oneselected = true;
                    saved = false;
                }
            }

            if (!oneselected)
                label2.Text = "";
        }
    }
}

private void searchToolStripMenuItem_Click(object sender, EventArgs e)
{
    MessageBox.Show("This is a quick guide to help you understand how the properties work.\n\n-When using normal or electricfied buttons (the blue and yellow similar tiles) and doors enter a string, the buttons that match the doors will make them open (I suggest numbers as they are easy to remember).\n\n-When using the stairs specify \"up\" or \"down\" for which way to travel between floors.\n\n-When using the spawner (first under objects):\n\t-\"player\" spawns the player\n\t-\"lightorb\" spawns a lightorb\n\t-\"golem.[type]\" spawns a golem of the specified type\n\t-\"attractor.[type]\" spawns an attractor of the specified type\n\nNOTE: Make sure when setting the property of a object or tile that you are on the correct tab", "Labrinthium - World Editor: Help");
    layerbuttons[renderDepth].Focus();
}

private void helpToolStripButton_Click(object sender, EventArgs e)
{
    searchToolStripMenuItem_Click(this, e);
}

}
}
