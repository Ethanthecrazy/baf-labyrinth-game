using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace AnimationEditor
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

            Form1 form = new Form1();
            form.Show();
            while (!form.Exit) 
            {
                form.Update();
                form.Render();
                Application.DoEvents();
            }
        }
    }
}
