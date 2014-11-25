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
    public partial class EventDialog : Form
    {
        frmTileEditor main;
        int _x, _y;
        public EventDialog(frmTileEditor owner, int x, int y)
        {
            main = owner;
            _x = x;
            _y = y;
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            main.OverworldTileMap[_x, _y].EventID = txtEventID.Text;
            main.OverworldTileMap[_x, _y].OnFireID = txtonTrigger.Text;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }
}
