using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Labrinthium___World_Editor
{
    public class Tile
    {
        // point b/c its going to use x/y corridant of tile set
        Point type;
        public Point Type
        {
            get { return type; }
            set { type = value; }
        }

        bool selected = false;
        public bool Selected
        {
            get { return selected; }
            set { selected = value; }
        }

        public Tile()
        {
            type.X = -1;
            type.Y = -1;
        }

        public override string ToString()
        {
            return type.ToString();
        }
    }

    public class TileMap
    {
        Size mapsize;
        public Size MapSize
        {
            get { return mapsize; }
            set { mapsize = value; }
        }

        Size tileSize;
        public Size TileSize
        {
            get { return tileSize; }
            set { tileSize = value; }
        }

        Size tileSetSize;
        public Size TileSetSize
        {
            get { return tileSetSize; }
            set { tileSetSize = value; }
        }

        Tile[,] tiles;
        public Tile[,] Tiles
        {
            get { return tiles; }
            set { tiles = value; }
        }

        string Filename = "Untitled";
        public string FileName
        {
            get { return Filename; }
            set { Filename = value; }
        }

        public TileMap()
        {
          //  mapsize = new Size(Properties.Settings.Default.DefaultNewWidth, Properties.Settings.Default.DefaultNewHeight);
            // tiles = new Tile[Properties.Settings.Default.DefaultNewWidth, Properties.Settings.Default.DefaultNewHeight];
              mapsize = new Size(20, 20);
             tiles = new Tile[20, 20];

            tileSize = new Size(32, 32);

            tileSetSize = new Size(4, 2);

            for (int h = 0; h < mapsize.Height; ++h)
            {
                for (int w = 0; w < mapsize.Width; ++w)
                {
                    tiles[w, h] = new Tile();
                }
            }
        }
    }
}
