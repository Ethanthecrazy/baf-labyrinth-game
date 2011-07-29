using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Labrinthium___World_Editor
{
    public partial class OptForm : Form
    {
        public OptForm()
        {
            InitializeComponent();
            checkBoxAutoSave.Checked = Properties.Settings.Default.autoSave;
            numericUpDownAutoSave.Value = Properties.Settings.Default.AutoSaveInterval;
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            buttonAPPLY.PerformClick();

            this.Close();
        }

        private void buttonAPPLY_Click(object sender, EventArgs e)
        {
            MainForm form = (MainForm)this.Owner;

            Properties.Settings.Default.autoSave = checkBoxAutoSave.Checked;
            Properties.Settings.Default.AutoSaveInterval = (int)numericUpDownAutoSave.Value;
            Properties.Settings.Default.DefaultNewWidth = (int)numericUpDown8.Value;
            Properties.Settings.Default.DefaultNewHeight = (int)numericUpDown7.Value;
            Properties.Settings.Default.DefaultNewDepth = (int)numericUpDown9.Value;
            Properties.Settings.Default.Override = checkBoxOverride.Checked;
            Properties.Settings.Default.TileSelection = checkBoxSelection.Checked;
            Properties.Settings.Default.Save();

            TileMap newMap = new TileMap((int)numericUpDown4.Value, (int)numericUpDown3.Value, (int)numericUpDown9.Value);
            for (int d = 0; (d < form.Tilemap.MapSize.Depth) && (d < newMap.MapSize.Depth); ++d)
            {
                for (int h = 0; (h < form.Tilemap.MapSize.Height) && (h < newMap.MapSize.Height); ++h)
                {
                    for (int w = 0; (w < form.Tilemap.MapSize.Width) && (w < newMap.MapSize.Width); ++w)
                    {
                        newMap.Tiles[d][w][h] = form.Tilemap.Tiles[d][w][h];
                    }
                }
            }

            newMap.Objects = form.Tilemap.Objects;            
            newMap.TileSize = new Size((int)numericUpDown6.Value, (int)numericUpDown5.Value);
            newMap.TileSetSize = new Size((int)numericUpDown1.Value, (int)numericUpDown2.Value);
            newMap.ObjectTilesetID = form.Tilemap.ObjectTilesetID;
            newMap.ObjectTilesetpath = form.Tilemap.ObjectTilesetpath;
            newMap.ObjectTilSet = new Size((int)numericUpDown12.Value, (int)numericUpDown11.Value);
            newMap.FileName = form.Tilemap.FileName;
            newMap.TilesetID = form.Tilemap.TilesetID;
            newMap.Tilesetpath = form.Tilemap.Tilesetpath;

            for (int i = 0; i < 9; ++i)
                form.Layerbuttons[i].Visible = false;

            for (int i = 0; i < newMap.Tiles.Count; ++i)
                form.Layerbuttons[i].Visible = true;

            form.Tilemap = newMap;

            form.TimerInterval = (int)numericUpDownAutoSave.Value * 1000 * 60;

            form.WorldSplitC.Panel1.AutoScrollMinSize =
                new Size(form.Tilemap.MapSize.Width * form.Tilemap.TileSize.Width, form.Tilemap.MapSize.Height * form.Tilemap.TileSize.Height);

            form.Tb1.AutoScrollMinSize =
                new Size(form.Tilemap.TileSetSize.Width * form.Tilemap.TileSize.Width, form.Tilemap.TileSetSize.Height * form.Tilemap.TileSize.Height);

            form.Tb2.AutoScrollMinSize =
                new Size(form.Tilemap.ObjectTilSet.Width * form.Tilemap.TileSize.Width, form.Tilemap.ObjectTilSet.Height * form.Tilemap.TileSize.Height);

            while (form.RenderDepth >= newMap.MapSize.Depth)
                form.RenderDepth--;

            form.Layerbuttons[form.RenderDepth].Focus();
        }

        private void Options_FormClosing(object sender, FormClosingEventArgs e)
        {
            MainForm form = (MainForm)this.Owner;
            form.Optform = null;
        }

        private void Options_Load(object sender, EventArgs e)
        {
            MainForm form = (MainForm)this.Owner;
            numericUpDown9.Value = form.Tilemap.MapSize.Depth;
            numericUpDown4.Value = form.Tilemap.MapSize.Width;
            numericUpDown3.Value = form.Tilemap.MapSize.Height;

            numericUpDown6.Value = form.Tilemap.TileSize.Width;
            numericUpDown5.Value = form.Tilemap.TileSize.Height;

            numericUpDown1.Value = form.Tilemap.TileSetSize.Width;
            numericUpDown2.Value = form.Tilemap.TileSetSize.Height;

            numericUpDown12.Value = form.Tilemap.ObjectTilSet.Width;
            numericUpDown11.Value = form.Tilemap.ObjectTilSet.Height;

            numericUpDown10.Value = Properties.Settings.Default.DefaultNewDepth;
            numericUpDown8.Value =  Properties.Settings.Default.DefaultNewWidth;
            numericUpDown7.Value =  Properties.Settings.Default.DefaultNewHeight;

            checkBoxAutoSave.Checked = Properties.Settings.Default.autoSave;
            numericUpDownAutoSave.Value = Properties.Settings.Default.AutoSaveInterval;
            checkBoxAutoSave_CheckedChanged(this, e);

            checkBoxOverride.Checked = Properties.Settings.Default.Override;
            checkBoxSelection.Checked = Properties.Settings.Default.TileSelection;
        }

        private void checkBoxAutoSave_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxAutoSave.Checked)
            {
                numericUpDownAutoSave.Enabled = true;
            }
            else
            {
                numericUpDownAutoSave.Enabled = false;
            }
        }

        private void buttonCANCEL_Click_1(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
