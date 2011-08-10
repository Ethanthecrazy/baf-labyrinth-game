using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

using System.Xml;
using System.Xml.Serialization;
using System.Xml.Linq;

using System.IO;
using System.ComponentModel;

namespace Labrinthium___World_Editor
{
    public class Point3D
    {
        private int x, y, z;

        public int X
        {
            get { return x; }
            set { x = value; }
        }

        public int Y
        {
            get { return y; }
            set { y = value; }
        }

        public int Z
        {
            get { return z; }
            set { z = value; }
        }

        public Point3D(int _x, int _y, int _z)
        {
            x = _x;
            y = _y;
            z = _z;            
        }

        public Point3D()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        public bool Equals(Point3D that)
        {
            return (this.x == that.x && this.y == that.y && this.z == that.z);
        }
    }
    public class Size3D
    {
        private int width, height, depth;

        public int Width
        {
            get { return width; }
            set { width = value; }
        }

        public int Height
        {
            get { return height; }
            set { height = value; }
        }

        public int Depth
        {
            get { return depth; }
            set { depth = value; }
        }

        public Size3D(int _width, int _height, int _depth)
        {
            width = _width;
            height = _height;
            depth = _depth;
        }

        public Size3D()
        {
            width = 0;
            height = 0;
            depth = 0;
        }
    }

    public class Tile
    {
        // point b/c its going to use x/y corridant of tile set
        private Point type;
        public Point Type
        {
            get { return type; }
            set { type = value; }
        }

        private string property;
        public string Property
        {
            get { return property; }
            set { property = value; }
        }

        private bool selected = false;
        [XmlIgnore]
        public bool Selected
        {
            get { return selected; }
            set { selected = value; }
        }

        public Tile()
        {
            type.X = 2;
            type.Y = 2;
        }

        public override string ToString()
        {
            return type.ToString();
        }
    }

    public class TileObject
    {
        // point b/c its going to use x/y corridant of tile set
        private Point type;
        public Point Type
        {
            get { return type; }
            set { type = value; }
        }

        private bool selected = false;
        [XmlIgnore]
        public bool Selected
        {
            get { return selected; }
            set { selected = value; }
        }

        private string property;
        public string Property
        {
            get { return property; }
            set { property = value; }
        }

        private Point3D position;
        public Point3D Position
        {
            get { return position; }
            set { position = value; }
        }

        public TileObject(int _x, int _y, int _z, int _type_x, int _type_y, bool _sel = false)
        {
            type.X = _type_x;
            type.Y = _type_y;
            position = new Point3D(_x, _y, _z);
            selected = _sel;
        }
        public TileObject(int _x, int _y, int _z,  Point _type, bool _sel = false)
        {
            type = _type;
            position = new Point3D(_x, _y, _z);
            selected = _sel;
        }
        public TileObject(Point3D _pos, Point _type, bool _sel = false)
        {
            type = _type;
            position = _pos;
            selected = _sel;
        }

        public TileObject()
        {
            position = new Point3D(-1,-1,-1);
            type = new Point(-1, -1);
        }

        public override string ToString()
        {
            return type.ToString();
        }
    }

    public class TileMap
    {
        private Size3D mapsize;
        public Size3D MapSize
        {
            get { return mapsize; }
            set { mapsize = value; }
        }

        private Size tileSize;
        public Size TileSize
        {
            get { return tileSize; }
            set { tileSize = value; }
        }

        private Size tileSetSize;
        public Size TileSetSize
        {
            get { return tileSetSize; }
            set { tileSetSize = value; }
        }

        private List<Tile[][]> tiles = new List<Tile[][]>();
        public List<Tile[][]> Tiles
        {
            get { return tiles; }
            set { tiles = value; }
        }

        private List<TileObject> objects = new List<TileObject>();
        public List<TileObject> Objects
        {
            get { return objects; }
            set { objects = value; }
        }

        private string Filename = "Untitled";
        public string FileName
        {
            get { return Filename; }
            set { Filename = value; }
        }

        private string tilesetpath;
        public string Tilesetpath
        {
            get { return tilesetpath; }
            set { tilesetpath = value; }
        }

        private int tilesetID = -1;
        [XmlIgnore]
        public int TilesetID
        {
            get { return tilesetID; }
            set { tilesetID = value; }
        }

        private int objecttilesetID = -1;
        [XmlIgnore]
        public int ObjectTilesetID
        {
            get { return objecttilesetID; }
            set { objecttilesetID = value; }
        }

        private string objecttilesetpath;
        public string ObjectTilesetpath
        {
            get { return objecttilesetpath; }
            set { objecttilesetpath = value; }
        }

        Size objectTilSet = new Size(5, 4);
        public Size ObjectTilSet
        {
            get { return objectTilSet; }
            set { objectTilSet = value; }
        }

        public TileMap()
        {

        }

        public TileMap(int _width, int _height, int _depth)
        {
            mapsize = new Size3D(_width, _height, _depth);

            tileSize = new Size(32, 32);

            tileSetSize = new Size(5, 4);

            for (int i = 0; i < _depth; ++i)
                AddLayer();
        }

        public void AddLayer(int _index)
        {
            //Tile[][] tmp = new Tile[mapsize.Width][mapsize.Height];
            tiles.Insert(_index, new Tile[mapsize.Width][]); 
            for (int i = 0; i < mapsize.Width; i++)
            {
                tiles[_index][i] = new Tile[mapsize.Height];
            }


            for (int h = 0; h < mapsize.Height; ++h)
            {
                for (int w = 0; w < mapsize.Width; ++w)
                {
                    tiles[_index][w][h] = new Tile();
                }
            }

            mapsize.Depth = tiles.Count;
        }

        public void AddLayer()
        {
            AddLayer(tiles.Count);
        }

        public void RemoveLayer(int _index)
        {
            tiles.RemoveAt(_index);

            mapsize.Depth = tiles.Count;
        }

        public void RemoveLayer()
        {
            RemoveLayer(tiles.Count - 1);
        }

        public void ClearMap()
        {
            tiles.Clear();
            objects.Clear();

            mapsize = new Size3D(Properties.Settings.Default.DefaultNewWidth,
                Properties.Settings.Default.DefaultNewHeight,
                Properties.Settings.Default.DefaultNewDepth);

            for (int i = 0; i < mapsize.Depth; ++i)
                AddLayer();

            mapsize.Depth = tiles.Count;

            Filename = "Untitled";
        }

        public void OpenXML(string _file)
        {
            XElement xRoot = XElement.Load(_file);

            IEnumerable<XElement> xTileMap = xRoot.Elements();

            //foreach (XElement tilemap in xTileMap)
            //{
            //    XElement xMapSize = tilemap.Element("MapSize");
            //    if (xMapSize != null)
            //    {
            //        v.Make = xMake.Value;
                    
            //    }
            //}

            //foreach (XElement xVehicle in xVehicles)
            //{
            //    Vehicle v = new Vehicle();

            //    XElement xMake = xVehicle.Element("Make");
            //    if (xMake != null)
            //    {
            //        v.Make = xMake.Value;
            //    }

            //    XElement xModel = xVehicle.Element("Model");
            //    if (xModel != null)
            //    {
            //        v.Model = xModel.Value;
            //    }

            //    XAttribute xYear = xVehicle.Attribute("Year");
            //    if (xYear != null)
            //    {
            //        v.Year = Convert.ToInt32(xYear.Value);
            //    }

            //    listBox1.Items.Add(v);
            //}
        }

        public void OpenBIN(string _file)
        {

        }

        public void SaveXML(string _file)
        {

            //XElement xRoot = new XElement("TileMap");

            //XElement xMapSize = new XElement("MapSize", mapsize.Width, mapsize.Height, mapsize.Depth);
            //xRoot.Add(xMapSize);


            //XElement xTileSize = new XElement("TileSize", tileSize.Width, tileSize.Height);
            //xRoot.Add(xTileSize);


            //XElement xTileSetSize = new XElement("xTileSetSize", tileSetSize.Width, tileSetSize.Height);
            //xRoot.Add(xTileSetSize);


            //XElement xTiles = new XElement("Tiles");
            //xRoot.Add(xTiles);
            //for (int d = 0; d < mapsize.Depth; ++d)
            //{
            //    for (int h = 0; h < mapsize.Height; ++h)
            //    {
            //        for (int w = 0; w < mapsize.Width; ++w)
            //        {
            //            xTiles.Add(new XElement("Type", tiles[d][w, h].Type.X + (tiles[d][w, h].Type.Y * tileSetSize.Width)));
            //        }
            //    }
            //}

            //XElement xObjects = new XElement("Objects");
            //xRoot.Add(xObjects);
            //for (int i = 0; i < objects.Count; ++i)
            //{
            //    xObjects.Add(new XElement("Type", objects[i].Type.X + (objects[i].Type.Y * tileSetSize.Width)));
            //    if (objects[i].Type == new Point(0, 0))
            //    {
            //        XElement xType = new XElement("Golem_Spawner");
            //    }
            //    else if (objects[i].Type == new Point(1, 0))
            //    {
            //        xObjects.Add(new XElement("Attractor_Spawner"));
            //    }
            //}

            //xRoot.Add(new XElement("FileName", Filename));
            //xRoot.Add(new XElement("TileSetPath", tilesetpath));

            //xRoot.Save(_file);
        }

        public void SaveBIN(string _filename)
        {
            BinaryWriter bw;
            if (File.Exists(_filename))
                bw = new BinaryWriter(File.Open(_filename, FileMode.Truncate));
            else
                bw = new BinaryWriter(File.Open(_filename, FileMode.Create));

            //bw.Write();

            bw.Close();
        }
    }
}
