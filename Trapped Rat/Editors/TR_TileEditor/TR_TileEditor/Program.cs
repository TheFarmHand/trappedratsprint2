using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TR_TileEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            frmTileEditor editor = new frmTileEditor();
            editor.Show();

            while(editor.Looping)
            {
                editor.Update();
                editor.Render();

                Application.DoEvents();
            }
        }
    }
}
