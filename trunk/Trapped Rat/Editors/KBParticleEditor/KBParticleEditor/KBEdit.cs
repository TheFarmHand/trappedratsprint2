using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Xml.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows;

namespace KBParticleEditor
{

    public partial class KBEdit : Form
    {
        SGP.CSGP_Direct3D d3D = SGP.CSGP_Direct3D.GetInstance();
        SGP.CSGP_TextureManager tm = SGP.CSGP_TextureManager.GetInstance();
        DateTime timer = new DateTime();
        TimeSpan dt = new TimeSpan();

        static Random rand;


        float spawnTime = 0.0f;
        public bool Looping1 = true;
        public bool load = false;
        int totalParticles = 0;

        List<Particle> myParticles = new List<Particle>();
        public KBEdit()
        {
            InitializeComponent();
            d3D.Initialize(panel2, true);
            tm.Initialize(d3D.Device, d3D.Sprite);
            timer = DateTime.Now;
            dt = timer - timer;
            rand = new Random();
            cmbShape.SelectedItem = "Point";
            //string blah = System.IO.Directory.GetCurrentDirectory();
            // Hardcoded default with path saving options in editor
            fdImage.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() +
                "/../../../../../Assets/Textures");

            MessageBox.Show("Pick the File Path where images exist", "Path Picker", MessageBoxButtons.OK);
            FolderBrowserDialog defpath = new FolderBrowserDialog();
            if (DialogResult.OK == defpath.ShowDialog())
            {
                fdImage.InitialDirectory = defpath.SelectedPath;
            }
            
        }

        public new void Update()
        {


            dt = DateTime.Now - timer;
            spawnTime += (float)dt.TotalSeconds;

            for (int i = 0; i < myParticles.Count; i++)
            {
                // Update particle data
                myParticles[i].LifeCurrent += (float)dt.TotalSeconds;

                // Remove Particle from list if off screen or lifespawn expires
                if (myParticles[i].LifeCurrent >= myParticles[i].LifeEnd
                    || myParticles[i].Position.X > panel2.Width
                    || myParticles[i].Position.Y > panel2.Height
                    || myParticles[i].Position.X < 0
                    || myParticles[i].Position.Y < 0)
                {
                    myParticles.RemoveAt(i);
                    i--;
                    continue;
                }

                // Helper variabls
                float percentLife = myParticles[i].LifeCurrent / myParticles[i].LifeEnd;


                // Position Update
                PointF temp = new PointF(myParticles[i].Position.X, myParticles[i].Position.Y);
                temp.X += (myParticles[i].Velocity * myParticles[i].Direction.X * (float)dt.TotalSeconds);
                temp.Y += (myParticles[i].Velocity * myParticles[i].Direction.Y * (float)dt.TotalSeconds);
                myParticles[i].Position = temp;

                // Velocity update
                float newvelocity = (float)nVelEnd.Value - (float)nVelStart.Value;
                newvelocity = myParticles[i].Velocity + (newvelocity / myParticles[i].LifeEnd * (float)dt.TotalSeconds);
                myParticles[i].Velocity = newvelocity;

                // Scale update
                float newscale = (float)nScaleEnd.Value - (float)nScaleStart.Value;
                newscale = myParticles[i].Scale + (newscale / myParticles[i].LifeEnd * (float)dt.TotalSeconds);
                myParticles[i].Scale = newscale;

                // Rotation udpate
                float newrot = (float)nRotEnd.Value - (float)nRotStart.Value;
                newrot = myParticles[i].Rotation + (newrot / myParticles[i].LifeEnd * (float)dt.TotalSeconds);
                myParticles[i].Rotation = newrot;

                // Color udpate
                Color newcolor = bColorStart.BackColor;
                float a, r, g, b;
                a = (float)(bColorEnd.BackColor.A - bColorStart.BackColor.A); a *= percentLife;
                r = (float)(bColorEnd.BackColor.R - bColorStart.BackColor.R); r *= percentLife;
                g = (float)(bColorEnd.BackColor.G - bColorStart.BackColor.G); g *= percentLife;
                b = (float)(bColorEnd.BackColor.B - bColorStart.BackColor.B); b *= percentLife;

                myParticles[i].Color = Color.FromArgb((int)(newcolor.A + a), (int)(newcolor.R + r), (int)(newcolor.G + g), (int)(newcolor.B + b));

            }

            if (load && Looping.Checked && spawnTime > 0.01f)
            {
                for (int i = 0; i < nSpawn.Value; i++)
                {
                    myParticles.Add(new Particle(this));
                }

                spawnTime = 0.0f;
            }

            else if (load && totalParticles < nMaxPart.Value && spawnTime > 0.01f)
            {
                for (int i = 0; i < nSpawn.Value; i++)
                {
                    totalParticles++;
                    myParticles.Add(new Particle(this));
                }
                spawnTime = 0.0f;
            }


            timer = DateTime.Now;
        }
        public void Render()
        {
            //if (dt.TotalSeconds < 0.1f) return;
            // Do the particle stuff in here  
            d3D.Clear(panel2, panel2.BackColor);

            d3D.DeviceBegin();
            d3D.SpriteBegin();

            if (load)
            {
                for (int i = 0; i < myParticles.Count; i++)
                {
                    // Half width/height of image * myParticles[i].Scale
                    tm.Draw(0, (int)myParticles[i].Position.X - ((int)nOffsetX.Value) * (int)myParticles[i].Scale, (int)myParticles[i].Position.Y - ((int)nOffsetY.Value) * (int)myParticles[i].Scale,
                        myParticles[i].Scale, myParticles[i].Scale, Rectangle.Empty, (float)nOffsetX.Value * myParticles[i].Scale,
                        (float)nOffsetY.Value * myParticles[i].Scale, (float)myParticles[i].Rotation, myParticles[i].Color);

                }
            }

            d3D.SpriteEnd();
            d3D.DeviceEnd();

            d3D.Present();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Looping1 = false;
            Looping1 = false;
            this.Close();

        }

        private void KBEdit_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping1 = false;
            //d3D.SpriteEnd();
            //d3D.DeviceEnd();

            //d3D.Terminate();
            //tm.Terminate();
        }


        private void bColorStart_Click(object sender, EventArgs e)
        {
            // Open ColorDialog cdStart
            cdStart.ShowDialog();
            bColorStart.BackColor = cdStart.Color;
        }

        private void bColorEnd_Click(object sender, EventArgs e)
        {
            cdEnd.ShowDialog();
            bColorEnd.BackColor = cdEnd.Color;
        }


        private void pictureBox1_Click(object sender, EventArgs e)
        {
            // Clicking image does same as file menu Load Image
            loadImageToolStripMenuItem_Click(sender, e);
        }

        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //string filename = fdImage.FileName;
            if (DialogResult.OK == fdImage.ShowDialog())
            {
                if (load)
                    tm.UnloadTexture(0);
                tm.LoadTexture(fdImage.FileName);
                load = true;


                // Probably should change this
                pbImage.Image = Image.FromFile(fdImage.FileName);
                pbImage.SizeMode = PictureBoxSizeMode.Zoom;

                fdImage.InitialDirectory = System.IO.Path.GetDirectoryName(fdImage.FileName);

            }
        }

        // Object Values Changing, matching partner objects

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            nSpawn.Value = e.NewValue;
        }

        private void nSpawn_ValueChanged(object sender, EventArgs e)
        {
            hsbSpawn.Value = (int)nSpawn.Value;
        }

        private void hsbMinPart_Scroll(object sender, ScrollEventArgs e)
        {
            nMinParts.Value = e.NewValue;
        }

        private void nMinParts_ValueChanged(object sender, EventArgs e)
        {
            hsbMinPart.Value = (int)nMinParts.Value;
        }

        private void hsbMaxPart_Scroll(object sender, ScrollEventArgs e)
        {
            nMaxPart.Value = e.NewValue;
        }

        private void nMaxPart_ValueChanged(object sender, EventArgs e)
        {
            hsbMaxPart.Value = (int)nMaxPart.Value;
        }

        public class Particle
        {
            float lifeEnd, lifeCurrent;
            float rotation, scale, velocity;

            Color color;
            PointF position;
            PointF direction;

            public Particle(KBEdit data)
            {
                lifeCurrent = 0;
                lifeEnd = (float)(rand.Next((int)data.nMinLife.Value, (int)data.nMaxLife.Value + 1));
                velocity = (float)data.nVelStart.Value;

                // Changing Direction (steering)
                int min, max;

                if (data.cbDirRangeX.Checked)
                // If this is checked, we can spawn both left and right directions
                {
                    if (data.nDirectionX.Value < 0)
                    {
                        min = (int)data.nDirectionX.Value;
                        max = -min;
                    }

                    else
                    {
                        max = (int)data.nDirectionX.Value;
                        min = -max;
                    }
                }

                else
                // particles spawn in only Left or Right direction
                {
                    if (data.nDirectionX.Value < 0)
                    {
                        min = (int)data.nDirectionX.Value;
                        max = 0;
                    }
                    else
                    {
                        min = 0;
                        max = (int)data.nDirectionX.Value;
                    }
                }

                direction.X = (float)rand.Next(min, max + 1);

                if (data.cbDirRangeY.Checked)
                // If this is checked, we can spawn particles with up AND down movements
                {
                    if (data.nDirectionY.Value < 0)
                    {
                        min = (int)data.nDirectionY.Value;
                        max = -min;
                    }

                    else
                    {
                        max = (int)data.nDirectionY.Value;
                        min = -max;
                    }
                }

                else
                // All particles will move either up OR down
                {
                    if (data.nDirectionY.Value < 0)
                    {
                        min = (int)data.nDirectionY.Value;
                        max = 0;
                    }
                    else
                    {
                        min = 0;
                        max = (int)data.nDirectionY.Value;
                    }
                }

                direction.Y = rand.Next(min, max + 1);

                rotation = (float)data.nRotStart.Value;
                scale = (float)data.nScaleStart.Value;
                color = data.bColorStart.BackColor;

                // Change this based on Point, Line, Circle, Rectangle
                position.X = (int)data.nPosX.Value;
                position.Y = (int)data.nPosY.Value;

                int size = (int)data.nSize.Value;

                if (data.cmbShape.SelectedItem.ToString() == "Line")
                {
                    position.X += rand.Next(0, size);
                }

                else if (data.cmbShape.SelectedItem.ToString() == "Circle")
                {
                    //float theta = 2 * (float)Math.PI * (float)rand.Next(0, size + 1);
                    float a = rand.Next(0, 1000); a /= 1000;
                    float b = rand.Next(0, 1000); b /= 1000;
                    if (b < a)
                    {
                        float temp = b;
                        b = a;
                        a = temp;
                    }

                    position.X += (b * size * (float)Math.Cos(2 * Math.PI * a / b));
                    position.Y += (b * size * (float)Math.Sin(2 * Math.PI * a / b));

                }

                else if (data.cmbShape.SelectedItem.ToString() == "Rectangle")
                {
                    position.X += rand.Next(-size, size);
                    position.Y += rand.Next(-size, size);

                }
            }

            public PointF Direction
            {
                get { return direction; }
                set { direction = value; }
            }

            public PointF Position
            {
                get { return position; }
                set { position = value; }
            }

            public Color Color
            {
                get { return color; }
                set { color = value; }
            }

            public float LifeCurrent
            {
                get { return lifeCurrent; }
                set { lifeCurrent = value; }
            }

            public float LifeEnd
            {
                get { return lifeEnd; }
                set { lifeEnd = value; }
            }

            public float Velocity
            {
                get { return velocity; }
                set { velocity = value; }
            }

            public float Scale
            {
                get { return scale; }
                set { scale = value; }
            }

            public float Rotation
            {
                get { return rotation; }
                set { rotation = value; }
            }

        }

        private void Looping_CheckedChanged(object sender, EventArgs e)
        {
            totalParticles = 0;
            myParticles.Clear();
        }

        private void nMinLife_ValueChanged(object sender, EventArgs e)
        {
            if (nMinLife.Value > nMaxLife.Value) nMaxLife.Value = nMinLife.Value;
        }

        private void panel2_MouseClick(object sender, MouseEventArgs e)
        {
            nPosX.Value = e.X;
            nPosY.Value = e.Y;
            totalParticles = 0;
            //myParticles.Clear();
        }

        private void panel2_MouseDown(object sender, MouseEventArgs e)
        {
            nPosX.Value = e.X;
            nPosY.Value = e.Y;
        }


        private void panel2_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (e.X > nPosX.Maximum)
                    nPosX.Value = nPosX.Maximum;

                if (e.Y > nPosY.Maximum)
                    nPosY.Value = nPosY.Maximum;

                if (e.X >= 0)
                    nPosX.Value = e.X;

                if (e.Y >= 0)
                    nPosY.Value = e.Y;
            }
        }

        private void nPosX_ValueChanged(object sender, EventArgs e)
        {
            if (nPosX.Value < 0)
                nPosX.Value = 0;
        }

        private void nPosY_ValueChanged(object sender, EventArgs e)
        {
            if (nPosY.Value < 0)
                nPosY.Value = 0;
        }

        private void KBEdit_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                Looping1 = false;
                this.Close();
            }
        }

        private void exportToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            // Export XML file to someplace
            string filename;
            filename = System.IO.Path.GetFileName(fdImage.FileName);
            if (filename == "")
            {
                MessageBox.Show("You didn't load an image, can't save the effect.");

                return;
            }

            saveXML.AddExtension = true;
            saveXML.DefaultExt = ".xml";

            if (DialogResult.OK == saveXML.ShowDialog())
            {
                XDocument SaveData = new XDocument(new XDeclaration("1.0", "utf8", "yes"));
                                
                SaveData.Add(new XElement("PEffect"));
                SaveData.Element("PEffect").Add(new XElement("FlyWeight"),
                    new XElement("Emitter"));
                //SaveData.Add(new XElement("Emitter"));
                SaveData.Element("PEffect").Element("FlyWeight").Add(
                    new XElement("MinLife", nMinLife.Value.ToString()),
                    new XElement("MaxLife", nMaxLife.Value.ToString()),
                    new XElement("ScaleStart", nScaleStart.Value.ToString()),
                    new XElement("ScaleEnd", nScaleEnd.Value.ToString()),
                    new XElement("VelocityStart", nVelStart.Value.ToString()),
                    new XElement("VelocityEnd", nVelEnd.Value.ToString()),
                    new XElement("RotationStart", nRotStart.Value.ToString()),
                    new XElement("RotationEnd", nRotEnd.Value.ToString()),
                    new XElement("ROffsetX", nOffsetX.Value.ToString()),
                    new XElement("ROffsetY", nOffsetY.Value.ToString()),
                    new XElement("ColorSA", bColorStart.BackColor.A.ToString()),
                    new XElement("ColorSR", bColorStart.BackColor.R.ToString()),
                    new XElement("ColorSG", bColorStart.BackColor.G.ToString()),
                    new XElement("ColorSB", bColorStart.BackColor.B.ToString()),
                    new XElement("ColorEA", bColorEnd.BackColor.A.ToString()),
                    new XElement("ColorER", bColorEnd.BackColor.R.ToString()),
                    new XElement("ColorEG", bColorEnd.BackColor.G.ToString()),
                    new XElement("ColorEB", bColorEnd.BackColor.B.ToString()),
                    new XElement("FileName", filename)
                    );

                
                SaveData.Element("PEffect").Element("Emitter").Add(
                    new XElement("Shape", cmbShape.SelectedItem.ToString()),
                    new XElement("Size", nSize.Value.ToString()),
                    new XElement("SpawnRate", nSpawn.Value.ToString()),
                    new XElement("MinParticles", nMinParts.Value.ToString()),
                    new XElement("MaxParticles", nMaxPart.Value.ToString()),
                    new XElement("Looping", Looping.Checked.ToString()),
                    new XElement("PositionX", nPosX.Value.ToString()),
                    new XElement("PositionY", nPosY.Value.ToString()),
                    new XElement("DirectionX", nDirectionX.Value.ToString()),
                    new XElement("DirectionY", nDirectionY.Value.ToString()),
                    new XElement("RangeX", cbDirRangeX.Checked.ToString()),
                    new XElement("RangeY", cbDirRangeY.Checked.ToString())

                    );

                SaveData.Save(saveXML.FileName);
                saveXML.InitialDirectory = System.IO.Path.GetDirectoryName(saveXML.FileName);

            }
            
        }

        private void imageFolderToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            FolderBrowserDialog fdbd = new FolderBrowserDialog();

            if (DialogResult.OK == fdbd.ShowDialog())
            {
                //System.IO.Directory.SetCurrentDirectory(fdbd.SelectedPath);
                fdImage.InitialDirectory = fdbd.SelectedPath;
            }
        }

        private void xMLPathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fdbd = new FolderBrowserDialog();
            if (DialogResult.OK == fdbd.ShowDialog())
            {
                saveXML.InitialDirectory = fdbd.SelectedPath;
            }
        }

        private void importToolStripMenuItem_Click(object sender, EventArgs e)
        {
            loadXML.InitialDirectory = saveXML.InitialDirectory;
            if (DialogResult.OK == loadXML.ShowDialog())
            {
                //XElement root = XElement.Load(loadXML.FileName);

                XDocument root = XDocument.Load(loadXML.FileName);
                
                //XNode fly = root.FirstNode;
                //nMinLife.Value = Convert.ToInt32(fly.Document.Element("MinLife").Value);
                nMinLife.Value =    Convert.ToInt32(root.Root.Element("FlyWeight").Element("MinLife").Value);
                nMaxLife.Value =    Convert.ToInt32(root.Root.Element("FlyWeight").Element("MaxLife").Value);
                nScaleStart.Value = Convert.ToInt32(root.Root.Element("FlyWeight").Element("ScaleStart").Value);
                nScaleEnd.Value =   Convert.ToInt32(root.Root.Element("FlyWeight").Element("ScaleEnd").Value);
                nVelStart.Value =   Convert.ToInt32(root.Root.Element("FlyWeight").Element("VelocityStart").Value);
                nVelEnd.Value =     Convert.ToInt32(root.Root.Element("FlyWeight").Element("VelocityEnd").Value);
                nRotStart.Value =   Convert.ToInt32(root.Root.Element("FlyWeight").Element("RotationStart").Value);
                nRotEnd.Value =     Convert.ToInt32(root.Root.Element("FlyWeight").Element("RotationEnd").Value);
                nOffsetX.Value =    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ROffsetX").Value);
                nOffsetY.Value = Convert.ToInt32(root.Root.Element("FlyWeight").Element("ROffsetY").Value);

                bColorStart.BackColor = Color.FromArgb(
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorSA").Value),
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorSR").Value),
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorSG").Value),
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorSB").Value));

                bColorEnd.BackColor = Color.FromArgb(
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorEA").Value),
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorER").Value),
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorEG").Value),
                    Convert.ToInt32(root.Root.Element("FlyWeight").Element("ColorEB").Value));

                string file;
                file = fdImage.InitialDirectory + "\\" + root.Root.Element("FlyWeight").Element("FileName").Value;
                pbImage.Image = Image.FromFile(file);

                if (load)
                    tm.UnloadTexture(0);
                tm.LoadTexture(file);
                load = true;
                pbImage.SizeMode = PictureBoxSizeMode.Zoom;

                // EMITTER DATA NOW
                cmbShape.SelectedValue = (root.Root.Element("Emitter").Element("Shape").Value);
                nSize.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("Size").Value);
                nSpawn.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("SpawnRate").Value);
                nMinParts.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("MinParticles").Value);
                nMaxPart.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("MaxParticles").Value);
                
                if (root.Root.Element("Emitter").Element("Looping").Value == "True")
                {
                    Looping.Checked = true;
                }
                else
                    Looping.Checked = false;

                nPosX.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("PositionX").Value);
                nPosY.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("PositionY").Value);
                nDirectionX.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("DirectionX").Value);
                nDirectionY.Value = Convert.ToInt32(root.Root.Element("Emitter").Element("DirectionY").Value);
                if ((root.Root.Element("Emitter").Element("RangeX").Value) == "True")
                    cbDirRangeX.Checked = true;
                else
                    cbDirRangeX.Checked = false;

                if ((root.Root.Element("Emitter").Element("RangeY").Value) == "True")
                    cbDirRangeY.Checked = true;
                else
                    cbDirRangeY.Checked = false;

                
            }

        }

        private void nMaxLife_ValueChanged(object sender, EventArgs e)
        {

        }
    }
}
