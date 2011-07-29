using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Labrinthium___World_Editor
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
           // Application.Run(new MainForm());

            MainForm theform = new MainForm();
            

            theform.Show();

            while (!theform.Exit)  {

                theform.Update();

                theform.Render();

                Application.DoEvents();
                
            }

            Application.Exit();
        }
    }
}
