using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KBParticleEditor
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
            //Application.Run(new Form1());

            KBEdit myForm = new KBEdit();
            myForm.Show();

            while (myForm.Looping1)
            {
                myForm.Update();
                //myForm.update();
                myForm.Render();
                Application.DoEvents();
                
            }
        }
    }
}
