using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TR_TileEditor
{
    public partial class OverworldMap : Form
    {
        frmTileEditor main;
        float overScaleWidth, overScaleHeight;
        
        public OverworldMap(frmTileEditor owner)
        {
            main = owner;

            InitializeComponent();
            Point spot = new Point();
            spot.X = main.Location.X + main.Width + 3;
            spot.Y = main.Location.Y;
            this.Location = spot;

            main.OverworldDX.AddRenderTarget(pnlOverMap);

        }
        public void Render()
        {
            overScaleWidth = (float)(pnlOverMap.Width / main.OverworldMapSize.Width);
            overScaleHeight = (float)(pnlOverMap.Height / main.OverworldMapSize.Height);

            main.OverworldDX.Clear(pnlOverMap, Color.White);
            main.OverworldDX.DeviceBegin();
            main.OverworldDX.SpriteBegin();

            for (int x = 0; x < main.OverworldMapSize.Width; ++x)
            {
                for (int y = 0; y < main.OverworldMapSize.Height; ++y)
                {
                    Rectangle destRect = Rectangle.Empty;
                    destRect.X = (int)(x * overScaleWidth);
                    destRect.Y = (int)(y * overScaleHeight);

                    int drawnTile = main.OverworldTileMap[x, y].TileID;
                    Rectangle srcRect = Rectangle.Empty;
                    srcRect.X = (drawnTile % main.OverworldTileSetSize.Width) * main.OverworldTileSize.Width;
                    srcRect.Y = (drawnTile / main.OverworldTileSetSize.Width) * main.OverworldTileSize.Height;
                    srcRect.Size = main.OverworldTileSize;
                    
                    if (main.OverworldTileSetID != -1)
                        main.OverworldTM.Draw(main.OverworldTileSetID, destRect.X, destRect.Y, overScaleWidth / main.OverworldTileSize.Width, overScaleHeight / main.OverworldTileSize.Height, srcRect);
                }
            }
            main.OverworldDX.SpriteEnd();
            main.OverworldDX.DeviceEnd();

            main.OverworldDX.Present();
        }

        private void pnlOverMap_MouseClick(object sender, MouseEventArgs e)
        {
            overScaleWidth = (float)(pnlOverMap.Width / main.OverworldMapSize.Width);
            overScaleHeight = (float)(pnlOverMap.Height / main.OverworldMapSize.Height);

            if(e.Button == MouseButtons.Left)
            {
                Point spot = new Point();
                spot.X = (int)((e.Location.X / overScaleWidth) * main.OverworldTileSize.Width) - main.OverworldTileSize.Width;
                spot.Y = (int)((e.Location.Y / overScaleHeight) * main.OverworldTileSize.Height) - main.OverworldTileSize.Height;

                main.OverworldMapLocation = spot;
                main.pnlTileMapScroll();
            }
        }
    }
}
