using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Xml.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TR_TileEditor
{
    public partial class frmTileEditor : Form
    {
        SGP.CSGP_Direct3D DX = SGP.CSGP_Direct3D.GetInstance();
        SGP.CSGP_TextureManager TM = SGP.CSGP_TextureManager.GetInstance();

        OverworldMap over = null;
        EventDialog EID = null;
        int tileSetID = -1;
        Size tileSetSize;
        Size tileSize;
        Tile[,] map;
        Size mapSize;
        int selectedTile;
        Point mapLocation;
        string eventID;
        List<ObjectStruct> levelObjects;
        ObjectStruct placeHolder = new ObjectStruct();
        int objectCount;
        bool playerSpawned = false;
        bool objectRemoved = false;
        int objectIndexAbove = 0;
        string imageName;
        string filePath;

        Color CollisionColor = Color.FromArgb(125, Color.MediumPurple);
        Color EventColor = Color.FromArgb(125, Color.Yellow);
        Color ObjectColor = Color.FromArgb(125, Color.SandyBrown);

        bool grid = true;
        char layerMap = 'm';
        char eventMode = 'a';
        char paintMode = 'p';
        char objectMode = 'o';

        bool looping = true;
        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }

        public frmTileEditor()
        {
            InitializeComponent();
            DX.Initialize(pnlTileSet, true);
            DX.AddRenderTarget(pnlTileMap);

            TM.Initialize(DX.Device, DX.Sprite);

            for (int i = 0; i < 100; ++i)
            {
                toolTileSetWidth.Items.Add(i + 1);
                toolTileSetHeight.Items.Add(i + 1);
            }
            for (int i = 1; i < 1025; i *= 2)
            {
                toolTileWidth.Items.Add(i);
                toolTileHeight.Items.Add(i);
            }
            for (int i = 0; i < 200; ++i)
            {
                toolTileMapWidth.Items.Add(i + 1);
                toolTileMapHeight.Items.Add(i + 1);
            }
            toolTileSetWidth.SelectedIndex = 3;
            toolTileSetHeight.SelectedIndex = 1;
            toolTileWidth.SelectedIndex = 6;
            toolTileHeight.SelectedIndex = 6;
            toolTileMapWidth.SelectedIndex = 4;
            toolTileMapHeight.SelectedIndex = 4;

            cmbObjects.SelectedIndex = 0;

            tileSetSize.Width = (int)toolTileSetWidth.SelectedItem;
            tileSetSize.Height = (int)toolTileSetHeight.SelectedItem;
            tileSize.Width = (int)toolTileWidth.SelectedItem;
            tileSize.Height = (int)toolTileHeight.SelectedItem;
            mapSize.Width = (int)toolTileMapWidth.SelectedItem;
            mapSize.Height = (int)toolTileMapHeight.SelectedItem;

            map = new Tile[mapSize.Width, mapSize.Height];
            Tile temp = new Tile();
            temp.TileID = 0;
            temp.CollisionID = 0;
            temp.EventID = "none";
            temp.OnFireID = "none";
            temp.ObjectID = '0';
            temp.ObjectNumber = 0;
            for (int x = 0; x < mapSize.Width; ++x)
            {
                for (int y = 0; y < mapSize.Height; ++y)
                {
                    map[x, y] = temp;
                }
            }

            objectCount = 0;
            levelObjects = new List<ObjectStruct>();

            btnMapLayer.BackColor = Color.ForestGreen;
            btnPaint.BackColor = Color.PowderBlue;
            btnAddEvent.BackColor = Color.PaleGreen;
            btnAddObject.BackColor = Color.Green;

            mapLocation = new Point(0, 0);

            MessageBox.Show("Please select the folder that contains your tilesets and map scripts.\nThis folder must have the following 2 folders within it:\nTileSets   - Contains the tilesets that are used to paint maps\nTileMaps - Contains the xml scripts that are used to build levels");
            FolderBrowserDialog folder = new FolderBrowserDialog();
            if (DialogResult.OK == folder.ShowDialog())
            {
                filePath = System.IO.Path.GetFullPath(folder.SelectedPath);
            }

            this.Focus();
        }
        public new void Update()
        {
            if (objectRemoved)
            {
                objectRemoved = false;
                for (int y = 0; y < mapSize.Height; y++)
                {
                    for (int x = 0; x < mapSize.Width; x++)
                    {
                        if (map[x, y].ObjectNumber != 0 && map[x, y].ObjectNumber > objectIndexAbove)
                            map[x, y].ObjectNumber -= 1;
                    }
                }
            }
        }
        public void Render()
        {
            Point offset = new Point(0, 0);
            Size autoScroll = new Size(0, 0);

            if (tileSetID != -1)
            {
                DX.Clear(pnlTileSet, Color.White);
                DX.DeviceBegin();
                DX.SpriteBegin();

                offset.X += pnlTileSet.AutoScrollPosition.X;
                offset.Y += pnlTileSet.AutoScrollPosition.Y;

                TM.Draw(tileSetID, offset.X, offset.Y);

                for (int x = 0; x < tileSetSize.Width; x++)
                {
                    for (int y = 0; y < tileSetSize.Height; y++)
                    {
                        Rectangle destRect = Rectangle.Empty;
                        destRect.X = (x * tileSize.Width) + offset.X;
                        destRect.Y = (y * tileSize.Height) + offset.Y;
                        destRect.Size = tileSize;

                        if (grid)
                            DX.DrawHollowRect(destRect, Color.Red, 1);
                    }
                }

                DX.SpriteEnd();
                DX.DeviceEnd();

                offset = new Point(0, 0);
                DX.Present();

                DX.Clear(pnlTileMap, Color.White);
                DX.DeviceBegin();
                DX.SpriteBegin();

                offset.X += pnlTileMap.AutoScrollPosition.X;
                offset.Y += pnlTileMap.AutoScrollPosition.Y;

                for (int y = 0; y < mapSize.Height; y++)
                {
                    Rectangle destRect = Rectangle.Empty;
                    destRect.Y = (y * tileSize.Height) + offset.Y;
                    if (destRect.Y < -tileSize.Height)
                        continue;
                    if (destRect.Y > pnlTileMap.Height + tileSize.Height)
                        break;
                    for (int x = 0; x < mapSize.Width; x++)
                    {

                        destRect.X = (x * tileSize.Width) + offset.X;
                        destRect.Size = tileSize;
                        if (destRect.X < -tileSize.Width)
                            continue;
                        if (destRect.X > pnlTileMap.Width + tileSize.Width)
                            break;
                        int drawnTile = map[x, y].TileID;
                        Rectangle srcRect = Rectangle.Empty;
                        srcRect.X = (drawnTile % tileSetSize.Width) * tileSize.Width;
                        srcRect.Y = (drawnTile / tileSetSize.Width) * tileSize.Height;
                        srcRect.Size = tileSize;

                        if (layerMap == 'm')
                        {
                            if (tileSetID != -1)
                                TM.Draw(tileSetID, destRect.X, destRect.Y, 1.0f, 1.0f, srcRect, 32, 32, 0, Color.FromArgb(255, Color.Transparent));
                        }
                        else if (layerMap == 'c')
                        {
                            if (tileSetID != -1)
                            {
                                TM.Draw(tileSetID, destRect.X, destRect.Y, 1.0f, 1.0f, srcRect, 32, 32, 0, Color.FromArgb(125, Color.Transparent));
                                if (map[x, y].CollisionID != 0)
                                {
                                    DX.DrawRect(destRect, CollisionColor);
                                    DX.DrawHollowRect(destRect, Color.DarkSlateBlue, 4);

                                }
                            }
                        }
                        else if (layerMap == 'e')
                        {
                            if (tileSetID != -1)
                            {
                                TM.Draw(tileSetID, destRect.X, destRect.Y, 1.0f, 1.0f, srcRect, 32, 32, 0, Color.FromArgb(125, Color.Transparent));
                                if (map[x, y].EventID != "none")
                                {
                                    DX.DrawRect(destRect, EventColor);
                                    DX.DrawHollowRect(destRect, Color.DarkOrange, 4);

                                }
                            }
                        }
                        else if (layerMap == 'o')
                        {
                            TM.Draw(tileSetID, destRect.X, destRect.Y, 1.0f, 1.0f, srcRect, 32, 32, 0, Color.FromArgb(125, Color.Transparent));
                            if (map[x, y].ObjectID == 'p' || map[x, y].ObjectID == 'g')
                            {
                                DX.DrawRect(destRect, ObjectColor);
                                if (map[x, y].ObjectID == 'p')
                                    DX.DrawText("P", destRect.X + 4, destRect.Y + 2, Color.Black);
                                else
                                    DX.DrawText("G", destRect.X + 4, destRect.Y + 2, Color.Black);
                                if (map[x, y].ObjectNumber == lstObjects.SelectedIndex)
                                    DX.DrawHollowRect(destRect, Color.SaddleBrown, 4);
                            }
                            if (lstObjects.Items.Count > 0 && lstObjects.SelectedIndex != -1)
                            {
                                for (int i = 0; i < levelObjects[lstObjects.SelectedIndex].WaypointTiles.Count; ++i)
                                {
                                    if (y * mapSize.Width + x == levelObjects[lstObjects.SelectedIndex].WaypointTiles[i])
                                    {
                                        DX.DrawRect(new Rectangle(destRect.X + tileSize.Width / 2 - (tileSize.Width / 8), destRect.Y + tileSize.Height / 2 - (tileSize.Height / 8), tileSize.Width / 4, tileSize.Height / 4), Color.AliceBlue);
                                    }
                                }
                            }
                        }
                        if (grid)
                            DX.DrawHollowRect(destRect, Color.Red, 1);
                    }
                }

                if (layerMap == 'o')
                {
                    if (lstObjects.Items.Count > 0 && lstObjects.SelectedIndex != -1)
                    {
                        for (int i = 1; i < levelObjects[lstObjects.SelectedIndex].WaypointTiles.Count; ++i)
                        {
                            int lineIndex1, lineX1, lineY1, linePX1, linePY1;
                            int lineIndex2, lineX2, lineY2, linePX2, linePY2;
                            lineIndex1 = levelObjects[lstObjects.SelectedIndex].WaypointTiles[i - 1];
                            lineX1 = lineIndex1 % mapSize.Width;
                            lineY1 = lineIndex1 / mapSize.Width;
                            linePX1 = lineX1 * tileSize.Width + (tileSize.Width / 2);
                            linePY1 = lineY1 * tileSize.Height + (tileSize.Height / 2);
                            lineIndex2 = levelObjects[lstObjects.SelectedIndex].WaypointTiles[i];
                            lineX2 = lineIndex2 % mapSize.Width;
                            lineY2 = lineIndex2 / mapSize.Width;
                            linePX2 = lineX2 * tileSize.Width + (tileSize.Width / 2);
                            linePY2 = lineY2 * tileSize.Height + (tileSize.Height / 2);
                            DX.DrawLine(linePX1, linePY1, linePX2, linePY2, Color.AliceBlue, 3);
                        }
                    }
                }
                DX.SpriteEnd();
                DX.DeviceEnd();

                DX.Present();
            }
            else
            {
                DX.Clear(pnlTileSet, Color.White);
                DX.DeviceBegin();
                DX.SpriteBegin();
                DX.SpriteEnd();
                DX.DeviceEnd();

                DX.Present();

                DX.Clear(pnlTileMap, Color.White);
                DX.DeviceBegin();
                DX.SpriteBegin();
                DX.SpriteEnd();
                DX.DeviceEnd();

                DX.Present();
            }
        }
        private void pnlTileSet_MouseClick(object sender, MouseEventArgs e)
        {
            Point offset = e.Location;
            offset.X -= pnlTileSet.AutoScrollPosition.X;
            offset.Y -= pnlTileSet.AutoScrollPosition.Y;
            if (offset.X / tileSize.Width < tileSetSize.Width && offset.Y / tileSize.Height < tileSetSize.Height)
            {
                int tempIndexX, tempIndexY;
                tempIndexX = offset.X / tileSize.Width;
                tempIndexY = offset.Y / tileSize.Height;
                selectedTile = (tempIndexY * tileSetSize.Width) + tempIndexX;
            }
        }
        private void pnlTileMap_MouseClick(object sender, MouseEventArgs e)
        {
            Rectangle mapRect = new Rectangle(0, 0, pnlTileMap.Width, pnlTileMap.Height);
            if (e.Button == MouseButtons.Left)
            {
                Point offset = e.Location;
                offset.X -= pnlTileMap.AutoScrollPosition.X;
                offset.Y -= pnlTileMap.AutoScrollPosition.Y;

                int tempIndexX, tempIndexY;
                tempIndexX = offset.X / tileSize.Width;
                tempIndexY = offset.Y / tileSize.Height;
                int mapTile = (tempIndexY * mapSize.Width) + tempIndexX;
                if (layerMap == 'm')
                {
                    if (tempIndexX < mapSize.Width && tempIndexY < mapSize.Height && mapRect.Contains(e.Location))
                    {
                        if (paintMode == 'p')
                            map[tempIndexX, tempIndexY].TileID = selectedTile;
                        else if (paintMode == 'e')
                            map[tempIndexX, tempIndexY].TileID = 0;
                        else if (paintMode == 'f')
                        {
                            tileFill(tempIndexX, tempIndexY, map[tempIndexX, tempIndexY].TileID);
                        }
                    }
                }
                else if (layerMap == 'c')
                {
                    if (tempIndexX < mapSize.Width && tempIndexY < mapSize.Height && mapRect.Contains(e.Location))
                    {
                        map[tempIndexX, tempIndexY].CollisionID = (int)numCollisionIDS.Value;
                    }
                }
                else if (layerMap == 'e')
                {
                    if (tempIndexX < mapSize.Width && tempIndexY < mapSize.Height && mapRect.Contains(e.Location))
                    {
                        if (eventMode == 'a')
                        {
                            if (EID == null)
                            {
                                EID = new EventDialog(this, tempIndexX, tempIndexY);
                                EID.FormClosed += EID_FormClosed;
                                EID.Show(this);
                            }
                        }
                        else if (eventMode == 'r')
                        {
                            map[tempIndexX, tempIndexY].EventID = "none";
                        }

                    }
                }
                else if (layerMap == 'o')
                {
                    if (tempIndexX < mapSize.Width && tempIndexY < mapSize.Height && mapRect.Contains(e.Location))
                    {
                        if (objectMode == 'o')
                        {
                            if (map[tempIndexX, tempIndexY].ObjectID == '0')
                            {
                                if (cmbObjects.SelectedItem.ToString() == "Player" && !playerSpawned)
                                {
                                    playerSpawned = true;
                                    map[tempIndexX, tempIndexY].ObjectID = 'p';
                                    placeHolder = new ObjectStruct("Player");
                                    placeHolder.WaypointTiles.Add(mapTile);
                                    lstWaypoints.Items.Add(mapTile);
                                    levelObjects.Add(placeHolder);
                                    map[tempIndexX, tempIndexY].ObjectNumber = objectCount;
                                    lstObjects.Items.Add(placeHolder.Name);
                                    lstObjects.SelectedIndex = objectCount;
                                    ++objectCount;
                                }

                                else if (cmbObjects.SelectedItem.ToString() == "Guard")
                                {
                                    map[tempIndexX, tempIndexY].ObjectID = 'g';
                                    placeHolder = new ObjectStruct("Guard");
                                    placeHolder.WaypointTiles.Add(mapTile);
                                    lstWaypoints.Items.Add(mapTile);
                                    levelObjects.Add(placeHolder);
                                    map[tempIndexX, tempIndexY].ObjectNumber = objectCount;
                                    lstObjects.Items.Add(placeHolder.Name);
                                    lstObjects.SelectedIndex = objectCount;
                                    ++objectCount;
                                }

                            }
                        }
                        else if (objectMode == 'j')
                        {
                            if (map[tempIndexX, tempIndexY].ObjectID == 'p')
                                playerSpawned = false;
                            if (map[tempIndexX, tempIndexY].ObjectID == 'p' || map[tempIndexX, tempIndexY].ObjectID == 'g')
                            {
                                objectRemoved = true;
                                levelObjects.RemoveAt(map[tempIndexX, tempIndexY].ObjectNumber);
                                lstObjects.Items.RemoveAt(map[tempIndexX, tempIndexY].ObjectNumber);
                                objectIndexAbove = map[tempIndexX, tempIndexY].ObjectNumber;
                                --objectCount;
                                lstWaypoints.Items.Clear();
                            }
                            map[tempIndexX, tempIndexY].ObjectID = '0';
                        }
                        else if (objectMode == 'w')
                        {
                            if (!levelObjects[lstObjects.SelectedIndex].WaypointTiles.Contains(mapTile))
                            {
                                levelObjects[lstObjects.SelectedIndex].WaypointTiles.Add(mapTile);
                                lstWaypoints.Items.Add(levelObjects[lstObjects.SelectedIndex].WaypointTiles[levelObjects[lstObjects.SelectedIndex].WaypointTiles.Count - 1]);
                            }
                        }
                        else if (objectMode == 'd')
                        {
                            for (int i = 0; i < levelObjects[lstObjects.SelectedIndex].WaypointTiles.Count; ++i)
                            {
                                if (mapTile == levelObjects[lstObjects.SelectedIndex].WaypointTiles[i])
                                {
                                    levelObjects[lstObjects.SelectedIndex].WaypointTiles.RemoveAt(i);
                                    lstWaypoints.Items.RemoveAt(i);
                                }
                            }
                        }
                    }
                }
                if (over != null)
                    over.Render();
            }
            else if (e.Button == MouseButtons.Right)
            {
                Point offset = e.Location;
                offset.X -= pnlTileMap.AutoScrollPosition.X;
                offset.Y -= pnlTileMap.AutoScrollPosition.Y;

                int tempIndexX, tempIndexY;
                tempIndexX = offset.X / tileSize.Width;
                tempIndexY = offset.Y / tileSize.Height;
                if (tempIndexX < mapSize.Width && tempIndexY < mapSize.Height && mapRect.Contains(e.Location))
                {
                    tipMap.ToolTipTitle = "Tile Info";
                    string caption = "Tile ID: " + map[tempIndexX, tempIndexY].TileID.ToString() +
                        "\nTile Collision: " + map[tempIndexX, tempIndexY].CollisionID.ToString() +
                        "\nEvent: " + map[tempIndexX, tempIndexY].EventID +
                        "\nObject ID: " + map[tempIndexX, tempIndexY].ObjectID;
                    tipMap.Show(caption, pnlTileMap, 5000);
                }
            }
        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false;
        }
        private void btnGrid_Click(object sender, EventArgs e)
        {
            grid = !grid;
        }
        private void btnLoadTileSet_Click(object sender, EventArgs e)
        {
            OpenFileDialog tileSetOpen = new OpenFileDialog();
            tileSetOpen.Filter = "Tile Sets(*.bmp *.png)|*.bmp;*.png";

            tileSetOpen.InitialDirectory = System.IO.Path.GetFullPath(filePath + "\\TileSets");
            if (DialogResult.OK == tileSetOpen.ShowDialog())
            {
                imageName = System.IO.Path.GetFileName(tileSetOpen.FileName);
                tileSetID = TM.LoadTexture(tileSetOpen.FileName);
                Size autoScroll = new Size(0, 0);
                autoScroll.Width = TM.GetTextureWidth(tileSetID);
                autoScroll.Height = TM.GetTextureHeight(tileSetID);
                pnlTileSet.AutoScrollMinSize = autoScroll;
            }
        }
        private void toolTileSetHeight_TextChanged(object sender, EventArgs e)
        {
            toolTileSetHeight.SelectionStart = toolTileSetHeight.Text.Length;
            toolTileSetHeight.SelectionLength = 0;
            if (toolTileSetHeight.FindStringExact(toolTileSetHeight.Text) >= 0)
                toolTileSetHeight.SelectedIndex = toolTileSetHeight.FindStringExact(toolTileSetHeight.Text);
            else
            {
                toolTileSetHeight.SelectedIndex = 99;
                toolTileSetHeight.SelectionStart = 0;
                toolTileSetHeight.SelectionLength = toolTileSetHeight.Text.Length;
            }
            tileSetSize.Height = (int)toolTileSetHeight.SelectedItem;
        }
        private void toolTileSetWidth_TextChanged(object sender, EventArgs e)
        {
            toolTileSetWidth.SelectionStart = toolTileSetWidth.Text.Length;
            toolTileSetWidth.SelectionLength = 0;
            if (toolTileSetWidth.FindStringExact(toolTileSetWidth.Text) >= 0)
                toolTileSetWidth.SelectedIndex = toolTileSetWidth.FindStringExact(toolTileSetWidth.Text);
            else
            {
                toolTileSetWidth.SelectedIndex = 99;
                toolTileSetWidth.SelectionStart = 0;
                toolTileSetWidth.SelectionLength = toolTileSetWidth.Text.Length;
            }
            tileSetSize.Width = (int)toolTileSetWidth.SelectedItem;
        }
        private void toolTileWidth_TextChanged(object sender, EventArgs e)
        {
            toolTileWidth.SelectionStart = toolTileWidth.Text.Length;
            toolTileWidth.SelectionLength = 0;
            if (toolTileWidth.FindStringExact(toolTileWidth.Text) >= 0)
                toolTileWidth.SelectedIndex = toolTileWidth.FindStringExact(toolTileWidth.Text);
            else
            {
                toolTileWidth.SelectedIndex = 10;
                toolTileWidth.SelectionStart = 0;
                toolTileWidth.SelectionLength = toolTileWidth.Text.Length;
            }
            tileSize.Width = (int)toolTileWidth.SelectedItem;
        }
        private void toolTileHeight_TextChanged(object sender, EventArgs e)
        {
            toolTileHeight.SelectionStart = toolTileHeight.Text.Length;
            toolTileHeight.SelectionLength = 0;
            if (toolTileHeight.FindStringExact(toolTileWidth.Text) >= 0)
                toolTileHeight.SelectedIndex = toolTileHeight.FindStringExact(toolTileHeight.Text);
            else
            {
                toolTileHeight.SelectedIndex = 10;
                toolTileHeight.SelectionStart = 0;
                toolTileHeight.SelectionLength = toolTileHeight.Text.Length;
            }
            tileSize.Height = (int)toolTileHeight.SelectedItem;
        }
        private void toolTileMapWidth_TextChanged(object sender, EventArgs e)
        {
            toolTileMapWidth.SelectionStart = toolTileMapWidth.Text.Length;
            toolTileMapWidth.SelectionLength = 0;
            if (toolTileMapWidth.FindStringExact(toolTileMapWidth.Text) >= 0)
                toolTileMapWidth.SelectedIndex = toolTileMapWidth.FindStringExact(toolTileMapWidth.Text);
            else
            {
                toolTileMapWidth.SelectedIndex = 199;
                toolTileMapWidth.SelectionStart = 0;
                toolTileMapWidth.SelectionLength = toolTileMapWidth.Text.Length;
            }
            int smallX;
            Tile[,] newMap = new Tile[(int)toolTileMapWidth.SelectedItem, mapSize.Height];
            Tile temp = new Tile(0, 0);
            temp.EventID = "none";
            for (int x = 0; x < (int)toolTileMapWidth.SelectedItem; ++x)
            {
                for (int y = 0; y < mapSize.Height; ++y)
                {
                    newMap[x, y] = temp;
                }
            }

            if (mapSize.Width < (int)toolTileMapWidth.SelectedItem)
                smallX = mapSize.Width;
            else
                smallX = (int)toolTileMapWidth.SelectedItem;
            for (int x = 0; x < smallX; ++x)
            {
                for (int y = 0; y < mapSize.Height; ++y)
                {
                    newMap[x, y] = map[x, y];
                }
            }
            mapSize.Width = (int)toolTileMapWidth.SelectedItem;
            map = new Tile[mapSize.Width, mapSize.Height];
            map = newMap;

            Size autoScroll = new Size(0, 0);
            autoScroll.Width = mapSize.Width * tileSize.Width;
            autoScroll.Height = mapSize.Height * tileSize.Height;
            pnlTileMap.AutoScrollMinSize = autoScroll;
            if (over != null)
                over.Render();
        }
        private void toolTileMapHeight_TextChanged(object sender, EventArgs e)
        {
            toolTileMapHeight.SelectionStart = toolTileMapHeight.Text.Length;
            toolTileMapHeight.SelectionLength = 0;
            if (toolTileMapHeight.FindStringExact(toolTileMapHeight.Text) >= 0)
                toolTileMapHeight.SelectedIndex = toolTileMapHeight.FindStringExact(toolTileMapHeight.Text);
            else
            {
                toolTileMapHeight.SelectedIndex = 199;
                toolTileMapHeight.SelectionStart = 0;
                toolTileMapHeight.SelectionLength = toolTileMapHeight.Text.Length;
            }
            int smallY;
            Tile[,] newMap = new Tile[mapSize.Width, (int)toolTileMapHeight.SelectedItem];
            Tile temp = new Tile(0, 0);
            temp.EventID = "none";
            for (int x = 0; x < mapSize.Width; ++x)
            {
                for (int y = 0; y < (int)toolTileMapHeight.SelectedItem; ++y)
                {
                    newMap[x, y] = temp;
                }
            }

            if (mapSize.Height < (int)toolTileMapHeight.SelectedItem)
                smallY = mapSize.Height;
            else
                smallY = (int)toolTileMapHeight.SelectedItem;
            for (int x = 0; x < mapSize.Width; ++x)
            {
                for (int y = 0; y < smallY; ++y)
                {
                    newMap[x, y] = map[x, y];
                }
            }
            mapSize.Height = (int)toolTileMapHeight.SelectedItem;
            map = new Tile[mapSize.Width, mapSize.Height];
            map = newMap;

            Size autoScroll = new Size(0, 0);
            autoScroll.Width = mapSize.Width * tileSize.Width;
            autoScroll.Height = mapSize.Height * tileSize.Height;
            pnlTileMap.AutoScrollMinSize = autoScroll;
            if (over != null)
                over.Render();
        }
        private void btnMapLayer_Click(object sender, EventArgs e)
        {
            layerMap = 'm';
            btnMapLayer.BackColor = Color.ForestGreen;
            btnCollisionLayer.BackColor = SystemColors.ControlLight;
            btnEventLayer.BackColor = SystemColors.ControlLight;
            btnObject.BackColor = SystemColors.ControlLight;

            btnPaint.Enabled = true;
            btnFill.Enabled = true;
            btnErase.Enabled = true;
            numCollisionIDS.Enabled = false;
            btnAddEvent.Enabled = false;
            btnRemoveEvent.Enabled = false;
            cmbObjects.Enabled = false;
            btnAddWaypoint.Enabled = false;
            btnDeleteWaypoint.Enabled = false;
            btnAddObject.Enabled = false;
            btnDeleteObject.Enabled = false;

        }
        private void btnCollisionLayer_Click(object sender, EventArgs e)
        {
            layerMap = 'c';
            btnMapLayer.BackColor = SystemColors.ControlLight;
            btnCollisionLayer.BackColor = Color.MediumPurple;
            btnEventLayer.BackColor = SystemColors.ControlLight;
            btnObject.BackColor = SystemColors.ControlLight;

            btnPaint.Enabled = false;
            btnFill.Enabled = false;
            btnErase.Enabled = false;
            numCollisionIDS.Enabled = true;
            btnAddEvent.Enabled = false;
            btnRemoveEvent.Enabled = false;
            cmbObjects.Enabled = false;
            btnAddWaypoint.Enabled = false;
            btnDeleteWaypoint.Enabled = false;
            btnAddObject.Enabled = false;
            btnDeleteObject.Enabled = false;
        }
        private void btnEventLayer_Click(object sender, EventArgs e)
        {
            layerMap = 'e';
            btnMapLayer.BackColor = SystemColors.ControlLight;
            btnCollisionLayer.BackColor = SystemColors.ControlLight;
            btnEventLayer.BackColor = Color.Yellow;
            btnObject.BackColor = SystemColors.ControlLight;

            btnPaint.Enabled = false;
            btnFill.Enabled = false;
            btnErase.Enabled = false;
            numCollisionIDS.Enabled = false;
            btnAddEvent.Enabled = true;
            btnRemoveEvent.Enabled = true;
            cmbObjects.Enabled = false;
            btnAddWaypoint.Enabled = false;
            btnDeleteWaypoint.Enabled = false;
            btnAddObject.Enabled = false;
            btnDeleteObject.Enabled = false;
        }
        private void btnObject_Click(object sender, EventArgs e)
        {
            layerMap = 'o';
            btnMapLayer.BackColor = SystemColors.ControlLight;
            btnCollisionLayer.BackColor = SystemColors.ControlLight;
            btnEventLayer.BackColor = SystemColors.ControlLight;
            btnObject.BackColor = Color.SandyBrown;

            btnPaint.Enabled = false;
            btnFill.Enabled = false;
            btnErase.Enabled = false;
            numCollisionIDS.Enabled = false;
            btnAddEvent.Enabled = false;
            btnRemoveEvent.Enabled = false;
            cmbObjects.Enabled = true;
            btnAddWaypoint.Enabled = true;
            btnDeleteWaypoint.Enabled = true;
            btnAddObject.Enabled = true;
            btnDeleteObject.Enabled = true;
        }
        private void btnPaint_Click(object sender, EventArgs e)
        {
            paintMode = 'p';
            btnPaint.BackColor = Color.PowderBlue;
            btnErase.BackColor = SystemColors.ControlLight;
            btnFill.BackColor = SystemColors.ControlLight;
        }
        private void btnErase_Click(object sender, EventArgs e)
        {
            paintMode = 'e';
            btnPaint.BackColor = SystemColors.ControlLight;
            btnErase.BackColor = Color.PaleVioletRed;
            btnFill.BackColor = SystemColors.ControlLight;
        }
        private void btnFill_Click(object sender, EventArgs e)
        {
            paintMode = 'f';
            btnPaint.BackColor = SystemColors.ControlLight;
            btnErase.BackColor = SystemColors.ControlLight;
            btnFill.BackColor = Color.PowderBlue;
        }
        private void btnAddEvent_Click(object sender, EventArgs e)
        {
            eventMode = 'a';
            btnAddEvent.BackColor = Color.PaleGreen;
            btnRemoveEvent.BackColor = SystemColors.ControlLight;
        }
        private void btnRemoveEvent_Click(object sender, EventArgs e)
        {
            eventMode = 'r';
            btnAddEvent.BackColor = SystemColors.ControlLight;
            btnRemoveEvent.BackColor = Color.PaleVioletRed;
        }
        private void btnAddWaypoint_Click(object sender, EventArgs e)
        {
            objectMode = 'w';
            btnAddWaypoint.BackColor = Color.Green;
            btnDeleteWaypoint.BackColor = SystemColors.ControlLight;
            btnAddObject.BackColor = SystemColors.ControlLight;
            btnDeleteObject.BackColor = SystemColors.ControlLight;
        }
        private void btnDeleteWaypoint_Click(object sender, EventArgs e)
        {
            objectMode = 'd';
            btnAddWaypoint.BackColor = SystemColors.ControlLight;
            btnDeleteWaypoint.BackColor = Color.OrangeRed;
            btnAddObject.BackColor = SystemColors.ControlLight;
            btnDeleteObject.BackColor = SystemColors.ControlLight;
        }
        private void btnAddObject_Click(object sender, EventArgs e)
        {
            objectMode = 'o';
            btnAddWaypoint.BackColor = SystemColors.ControlLight;
            btnDeleteWaypoint.BackColor = SystemColors.ControlLight;
            btnAddObject.BackColor = Color.Green;
            btnDeleteObject.BackColor = SystemColors.ControlLight;
        }
        private void btnDeleteObject_Click(object sender, EventArgs e)
        {
            objectMode = 'j';
            btnAddWaypoint.BackColor = SystemColors.ControlLight;
            btnDeleteWaypoint.BackColor = SystemColors.ControlLight;
            btnAddObject.BackColor = SystemColors.ControlLight;
            btnDeleteObject.BackColor = Color.OrangeRed;
        }
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TM.UnloadTexture(0);
            --tileSetID;

            toolTileSetWidth.SelectedIndex = 3;
            toolTileSetHeight.SelectedIndex = 1;
            toolTileWidth.SelectedIndex = 6;
            toolTileHeight.SelectedIndex = 6;
            toolTileMapWidth.SelectedIndex = 4;
            toolTileMapHeight.SelectedIndex = 4;

            tileSetSize.Width = (int)toolTileSetWidth.SelectedItem;
            tileSetSize.Height = (int)toolTileSetHeight.SelectedItem;
            tileSize.Width = (int)toolTileWidth.SelectedItem;
            tileSize.Height = (int)toolTileHeight.SelectedItem;
            mapSize.Width = (int)toolTileMapWidth.SelectedItem;
            mapSize.Height = (int)toolTileMapHeight.SelectedItem;

            levelObjects.Clear();
            lstObjects.Items.Clear();
            lstWaypoints.Items.Clear();

            Tile temp = new Tile();
            temp.TileID = 0;
            temp.CollisionID = 0;
            temp.EventID = "none";
            for (int x = 0; x < mapSize.Width; ++x)
            {
                for (int y = 0; y < mapSize.Height; ++y)
                {
                    map[x, y] = temp;
                }
            }
        }
        private void saveMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog save = new SaveFileDialog();
            save.Filter = "Tile Maps(*.xml)|*.xml";
            save.DefaultExt = "xml";
            save.InitialDirectory = System.IO.Path.GetFullPath(filePath + "\\TileMaps");

            if (DialogResult.OK == save.ShowDialog())
            {
                XElement root = new XElement("tileInfo");
                XAttribute rootTileSet = new XAttribute("tileSetFile", imageName);
                root.Add(rootTileSet);

                XElement childLayer = new XElement("layer");
                XAttribute layerH = new XAttribute("height", mapSize.Height);
                XAttribute layerW = new XAttribute("width", mapSize.Width);
                childLayer.Add(layerH);
                childLayer.Add(layerW);
                root.Add(childLayer);

                XElement childTileset = new XElement("tileset");
                XAttribute tilesetH = new XAttribute("height", tileSetSize.Height);
                XAttribute tilesetW = new XAttribute("width", tileSetSize.Width);
                childTileset.Add(tilesetH);
                childTileset.Add(tilesetW);
                root.Add(childTileset);

                XElement childTilesize = new XElement("tilesize");
                XAttribute tilesizeH = new XAttribute("height", tileSize.Height);
                XAttribute tilesizeW = new XAttribute("width", tileSize.Width);
                childTilesize.Add(tilesizeH);
                childTilesize.Add(tilesizeW);
                root.Add(childTilesize);

                XElement childPlayer = new XElement("player");
                int tempObjectX, tempObjectY;
                for (int i = 0; i < levelObjects.Count; ++i)
                {
                    if (levelObjects[i].Name == "Player")
                    {
                        tempObjectX = levelObjects[i].WaypointTiles[0] % mapSize.Width;
                        tempObjectY = levelObjects[i].WaypointTiles[0] / mapSize.Width;

                        XAttribute playerN = new XAttribute("name", "Player");
                        XAttribute playerX = new XAttribute("startX", tempObjectX * tileSize.Width);
                        XAttribute playerY = new XAttribute("startY", tempObjectY * tileSize.Height);
                        childPlayer.Add(playerN);
                        childPlayer.Add(playerX);
                        childPlayer.Add(playerY);
                        root.Add(childPlayer);

                        break;
                    }
                }

                XElement childGuard = new XElement("guard");
                for (int i = 0; i < levelObjects.Count; ++i)
                {
                    if (levelObjects[i].Name == "Guard")
                    {
                        tempObjectX = levelObjects[i].WaypointTiles[0] % mapSize.Width;
                        tempObjectY = levelObjects[i].WaypointTiles[0] / mapSize.Width;

                        XAttribute guardN = new XAttribute("name", levelObjects[i].Name);
                        XAttribute guardX = new XAttribute("startX", tempObjectX * tileSize.Width);
                        XAttribute guardY = new XAttribute("startY", tempObjectY * tileSize.Height);
                        childGuard.Add(guardN);
                        childGuard.Add(guardX);
                        childGuard.Add(guardY);

                        XElement grandchildWaypoint = new XElement("waypoint");
                        for (int k = 1; k < levelObjects[i].WaypointTiles.Count; ++k)
                        {
                            XAttribute waypointP = new XAttribute("tilePoint", levelObjects[i].WaypointTiles[k]);
                            grandchildWaypoint.Add(waypointP);
                            grandchildWaypoint = new XElement("waypoint");
                        }
                        childGuard.Add(grandchildWaypoint);
                        root.Add(childGuard);
                        childGuard = new XElement("guard");
                    }
                }

                XElement childTiles = new XElement("tiles");
                for (int y = 0; y < mapSize.Height; ++y)
                {
                    for (int x = 0; x < mapSize.Width; ++x)
                    {
                        XAttribute tilesID = new XAttribute("id", map[x, y].TileID);
                        XAttribute tilesC = new XAttribute("collision", map[x, y].CollisionID);
                        XAttribute tilesE = new XAttribute("event", map[x, y].EventID);
                        XAttribute tilesF = new XAttribute("onfire", map[x, y].OnFireID);
                        XAttribute tilesOID = new XAttribute("objectID", map[x, y].ObjectID);
                        XAttribute tilesON = new XAttribute("objectNumber", map[x, y].ObjectNumber);
                        childTiles.Add(tilesID);
                        childTiles.Add(tilesC);
                        childTiles.Add(tilesE);
                        childTiles.Add(tilesF);
                        childTiles.Add(tilesOID);
                        childTiles.Add(tilesON);
                        root.Add(childTiles);

                        childTiles = new XElement("tiles");
                    }
                }

                root.Save(save.FileName);

            }
        }
        private void openMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();
            open.Filter = "Tile Maps(.xml)|*.xml";
            open.InitialDirectory = System.IO.Path.GetFullPath(filePath + "\\TileMaps");

            if (DialogResult.OK == open.ShowDialog())
            {
                XElement root = XElement.Load(open.FileName);
                ObjectStruct tempObjects = new ObjectStruct();

                XAttribute tileSetFile = root.Attribute("tileSetFile");
                imageName = tileSetFile.Value;
                try
                {
                    tileSetID = TM.LoadTexture(filePath + "\\TileSets\\" + tileSetFile.Value);
                }
                catch (Exception)
                {
                    MessageBox.Show("Failed to load the required Tile Set, please check that you have a folder named TileSets");
                    throw;
                }

                int tempX, tempY, tempIndex, tempPixX, tempPixY;
                XElement layer = root.Element("layer");
                XAttribute layerHeight = layer.Attribute("height");
                XAttribute layerWidth = layer.Attribute("width");
                toolTileMapWidth.SelectedIndex = int.Parse(layerWidth.Value) - 1;
                toolTileMapHeight.SelectedIndex = int.Parse(layerHeight.Value) - 1;

                map = new Tile[mapSize.Width, mapSize.Height];

                XElement tileset = root.Element("tileset");
                XAttribute tilesetHeight = tileset.Attribute("height");
                XAttribute tilesetWidth = tileset.Attribute("width");
                toolTileSetHeight.SelectedIndex = int.Parse(tilesetHeight.Value) -1;
                toolTileSetWidth.SelectedIndex = int.Parse(tilesetWidth.Value) - 1;

                XElement tilesize = root.Element("tilesize");
                XAttribute tilesizeHeight = tilesize.Attribute("height");
                XAttribute tilesizeWidth = tilesize.Attribute("width");
                toolTileHeight.SelectedItem = int.Parse(tilesizeHeight.Value);
                toolTileWidth.SelectedItem = int.Parse(tilesizeWidth.Value);

                XElement player = root.Element("player");
                XAttribute playerName = player.Attribute("name");
                XAttribute playerX = player.Attribute("startX");
                XAttribute playerY = player.Attribute("startY");
                tempObjects = new ObjectStruct(playerName.Value);
                tempPixX = int.Parse(playerX.Value);
                tempPixY = int.Parse(playerY.Value);
                tempX = tempPixX / tileSize.Width;
                tempY = tempPixY / tileSize.Height;
                tempIndex = tempY * mapSize.Width + tempX;
                tempObjects.WaypointTiles.Add(tempIndex);
                levelObjects.Add(tempObjects);
                lstObjects.Items.Add(levelObjects[0].Name);

                IEnumerable<XElement> guards = root.Elements("guard");
                int objIndex = 1;
                foreach (XElement guard in guards)
                {
                    XAttribute guardName = guard.Attribute("name");
                    XAttribute guardX = guard.Attribute("startX");
                    XAttribute guardY = guard.Attribute("startY");
                    tempObjects = new ObjectStruct(guardName.Value);
                    tempPixX = int.Parse(guardX.Value);
                    tempPixY = int.Parse(guardY.Value);
                    tempX = tempPixX / tileSize.Width;
                    tempY = tempPixY / tileSize.Height;
                    tempIndex = tempY * mapSize.Width + tempX;
                    tempObjects.WaypointTiles.Add(tempIndex);
                    IEnumerable<XElement> waypoints = guard.Elements("waypoint");
                    foreach (XElement way in waypoints)
                    {
                        XAttribute point = way.Attribute("tilePoint");
                        tempObjects.WaypointTiles.Add(int.Parse(point.Value));
                    }
                    levelObjects.Add(tempObjects);
                    lstObjects.Items.Add(levelObjects[objIndex].Name);
                    ++objIndex;
                }

                IEnumerable<XElement> tiles = root.Elements("tiles");
                int x = 0, y = 0;
                foreach (XElement tile in tiles)
                {
                    XAttribute id = tile.Attribute("id");
                    XAttribute col = tile.Attribute("collision");
                    XAttribute eid = tile.Attribute("event");
                    XAttribute fid = tile.Attribute("onfire");
                    XAttribute objID = tile.Attribute("objectID");
                    XAttribute objNum = tile.Attribute("objectNumber");
                    Tile tempTile = new Tile();
                    tempTile.TileID = int.Parse(id.Value);
                    tempTile.CollisionID = int.Parse(col.Value);
                    tempTile.EventID = eid.Value;
                    tempTile.OnFireID = fid.Value;
                    tempTile.ObjectID = char.Parse(objID.Value);
                    tempTile.ObjectNumber = int.Parse(objNum.Value);

                    map[x, y] = tempTile;
                    ++x;
                    if (x >= mapSize.Width)
                    {
                        x = 0;
                        ++y;
                    }
                }
            }
        }
        private void showToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (over == null)
            {
                over = new OverworldMap(this);
                over.FormClosed += over_FormClosed;
                over.Show(this);
                showToolStripMenuItem.Checked = true;
                over.Render();
                this.Focus();
            }
            else
            {
                over.Close();
                over = null;
            }
        }
        void over_FormClosed(object sender, FormClosedEventArgs e)
        {
            over = null;
            showToolStripMenuItem.Checked = false;
        }
        void EID_FormClosed(object sender, FormClosedEventArgs e)
        {
            EID = null;
        }
        private void pnlTileSet_Resize(object sender, EventArgs e)
        {
            DX.Resize(pnlTileSet, pnlTileSet.ClientSize.Width, pnlTileSet.ClientSize.Height, true);
        }
        private void pnlTileMap_Resize(object sender, EventArgs e)
        {
            DX.Resize(pnlTileMap, pnlTileMap.ClientSize.Width, pnlTileMap.ClientSize.Height, true);
        }
        public void pnlTileMapScroll()
        {
            Point spot = new Point(0, 0);
            spot.X = mapLocation.X;
            spot.Y = mapLocation.Y;
            pnlTileMap.AutoScrollPosition = spot;
        }
        public Size OverworldMapSize
        {
            get { return mapSize; }
        }
        public Size OverworldTileSetSize
        {
            get { return tileSetSize; }
        }
        public Size OverworldTileSize
        {
            get { return tileSize; }
        }
        public Tile[,] OverworldTileMap
        {
            get { return map; }
            set { map = value; }
        }
        public int OverworldTileSetID
        {
            get { return tileSetID; }
        }
        public Point OverworldMapLocation
        {
            get { return mapLocation; }
            set { mapLocation = value; }
        }
        public string EventDialogEventID
        {
            get { return eventID; }
            set { eventID = value; }
        }
        public SGP.CSGP_Direct3D OverworldDX
        {
            get { return DX; }
        }
        public SGP.CSGP_TextureManager OverworldTM
        {
            get { return TM; }
        }
        private void pnlTileMap_MouseEnter(object sender, EventArgs e)
        {
            tipMap.Hide(pnlTileMap);
        }
        private void lstObjects_MouseClick(object sender, MouseEventArgs e)
        {
            if (lstObjects.SelectedIndex != -1)
            {
                lstWaypoints.Items.Clear();
                for (int i = 0; i < levelObjects[lstObjects.SelectedIndex].WaypointTiles.Count; ++i)
                {
                    lstWaypoints.Items.Add(levelObjects[lstObjects.SelectedIndex].WaypointTiles[i]);
                }
            }
        }
        private void tileFill(int x, int y, int overriddenTileID)
        {
            int startX = x, startY = y;
            int newX = startX, newY = startY;
            int midY;

            if (newY < 0)
                newY = 0;
            while (map[newX, newY].TileID == overriddenTileID || map[newX, newY].TileID == selectedTile)
            {
                map[newX, newY].TileID = selectedTile;
                ++newY;
                if (newY >= mapSize.Height)
                    break;
                while (map[newX, newY].TileID == overriddenTileID || map[newX, newY].TileID == selectedTile)
                {
                    map[newX, newY].TileID = selectedTile;
                    ++newX;
                    if (newX >= mapSize.Width)
                        break;
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        ++midY;
                        if (midY >= mapSize.Height)
                            break;
                    }
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        --midY;
                        if (midY < 0)
                            break;
                    }
                }

                newX = startX;
                while (map[newX, newY].TileID == overriddenTileID || map[newX, newY].TileID == selectedTile)
                {
                    map[newX, newY].TileID = selectedTile;
                    --newX;
                    if (newX < 0)
                        break;
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        ++midY;
                        if (midY >= mapSize.Height)
                            break;
                    }
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        --midY;
                        if (midY < 0)
                            break;
                    }
                }
                newX = startX;

            }
            newX = startX;
            newY = startY;
            if (newY >= mapSize.Height)
                newY = mapSize.Height - 1;
            while (map[newX, newY].TileID == overriddenTileID || map[newX, newY].TileID == selectedTile)
            {
                map[newX, newY].TileID = selectedTile;
                --newY;
                if (newY < 0)
                    break;
                while (map[newX, newY].TileID == overriddenTileID || map[newX, newY].TileID == selectedTile)
                {
                    map[newX, newY].TileID = selectedTile;
                    ++newX;
                    if (newX >= mapSize.Width)
                        break;
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        ++midY;
                        if (midY >= mapSize.Height)
                            break;
                    }
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        --midY;
                        if (midY < 0)
                            break;
                    }
                }
                newX = startX;
                while (map[newX, newY].TileID == overriddenTileID || map[newX, newY].TileID == selectedTile)
                {
                    map[newX, newY].TileID = selectedTile;
                    --newX;
                    if (newX < 0)
                        break;
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        ++midY;
                        if (midY >= mapSize.Height)
                            break;
                    }
                    midY = newY;
                    while (map[newX, midY].TileID == overriddenTileID || map[newX, midY].TileID == selectedTile)
                    {
                        map[newX, midY].TileID = selectedTile;
                        --midY;
                        if (midY < 0)
                            break;
                    }
                }
                newX = startX;
            }
        }
    }
}

