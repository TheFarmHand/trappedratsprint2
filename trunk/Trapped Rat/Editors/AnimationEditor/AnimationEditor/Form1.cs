using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using System.Xml;


namespace AnimationEditor
{
    public partial class Form1 : Form
    {

        public struct Frame
        {
            bool exitTrigger;

            public bool ExitTrigger
            {
                get { return exitTrigger; }
                set { exitTrigger = value; }
            }
            string eventname;

            public string Eventname
            {
                get { return eventname; }
                set { eventname = value; }
            }

            decimal duration;

            public decimal Duration
            {
                get { return duration; }
                set { duration = value; }
            }


            Rectangle drawRect;

            public Rectangle DrawRect1
            {
                get { return drawRect; }
                set { drawRect = value; }
            }
            Rectangle colRect;

            public Rectangle ColRect
            {
                get { return colRect; }
                set { colRect = value; }
            }
            Point anchor;

            public Point Anchor
            {
                get { return anchor; }
                set { anchor = value; }
            }
            int index;

            public int Index
            {
                get { return index; }
                set { index = value; }
            }
            public override string ToString()
            {
                return "Frame " + index.ToString();
            }
        }
        struct Animation
        {
            string imagefilereadin;

            public string Imagefilereadin
            {
                get { return imagefilereadin; }
                set { imagefilereadin = value; }
            }

            List<Frame> frames;
           
            public List<Frame> Frames
            {
                get { return frames; }
                set { frames = value; }
            }

            private int index;

            public int Index
            {
                get { return index; }
                set { index = value; }
            }
            public override string ToString()
            {
                return "Animation " + index.ToString();
            }

            string name;

            public string Name
            {
                get { return name; }
                set { name = value; }
            }

            string nextname;

            public string Nextname
            {
                get { return nextname; }
                set { nextname = value; }
            }

            bool animlooping;

            public bool Animlooping
            {
                get { return animlooping; }
                set { animlooping = value; }
            }
        }
        bool looping = true;
        bool playbackpause = true;
        int curFrame = 0;
        double elapsedtime = 0;
        DateTime curtime;

        SGP.CSGP_Direct3D DX = SGP.CSGP_Direct3D.GetInstance();
        SGP.CSGP_TextureManager TM = SGP.CSGP_TextureManager.GetInstance();
        int TextureID = -1;
        string imagepath;
        Size asmin;
        //float zoom = 1.0f;
        Point Draw1;
        Point Draw2;
        Point DrawInProg;
        Point Col1;
        Point Col2;
        Point AnchorPoint;
        int state = 0;//0 = DrawRect    1 = Collision Rect   2 = AnchorPoint
        string relativepath;
        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }


        public Form1()
        {
            InitializeComponent();

            DX.Initialize(panel1, true);
            DX.AddRenderTarget(panel2);

            TM.Initialize(DX.Device, DX.Sprite);
            TextureID = -1;
            //TextureID = TM.LoadTexture("../../pkmntest.jpg");
            asmin = new Size();
            //asmin.Width = TM.GetTextureWidth(TextureID);
            //asmin.Height = TM.GetTextureHeight(TextureID);
            //panel1.AutoScrollMinSize = asmin;

            Animation an = new Animation();
            an.Frames = new List<Frame>();
            an.Index = listBoxAnimation.Items.Count;
            listBoxAnimation.Items.Add(an);
            listBoxAnimation.SelectedIndex = an.Index;


            Frame f = new Frame();
            f.Eventname = "NoEvent";
            f.Index = listBoxFrames.Items.Count;
            f.Duration = .25M;
            listBoxFrames.Items.Add(f);
            listBoxFrames.SelectedIndex = f.Index;

            Animation a = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            a.Frames.Add(f);


            DrawInProg.X = -1;
            curtime = System.DateTime.Now;


            MessageBox.Show("Choose the Assets Folder.  \nThe Assets Folder Must Have a subfolder called Textures and another called Scripts", "Animation Editor",
MessageBoxButtons.OKCancel);
            using (FolderBrowserDialog dlg = new FolderBrowserDialog())
            {
               // dlg.Title = "Choose Folder To Load Images From";
                //Credit for this line of Code goes to Robert Martinez
               // dlg.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\..\\..\\..\\Assets\\Textures");
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    relativepath = System.IO.Path.GetFullPath(dlg.SelectedPath);

                }
            }
        }

        public new void Update()
        {
            TimeSpan ts = System.DateTime.Now - curtime;
            double dt = ts.TotalSeconds;
            curtime = System.DateTime.Now;
            if (playbackpause == false)
            {
                elapsedtime += dt;
                Animation an = (Animation)(listBoxAnimation.Items[listBoxAnimation.SelectedIndex]);
                if ((decimal)elapsedtime > an.Frames[curFrame].Duration)
                {
                    if (listBoxFrames.Items.Count <= curFrame + 1)
                    {
                        if (checkBoxLooping.Checked == false)
                        {
                            playbackpause = true;
                        }
                        curFrame = 0;
                    }
                    else
                        curFrame++;
                    elapsedtime = 0;
                }
            }
        }
        public void Render()
        {
            //panel 1 render
            DX.Clear(panel1, Color.Indigo);
            DX.DeviceBegin();
            DX.SpriteBegin();

            //DX.DrawRect(new Rectangle(20, 20, 15, 15), Color.FromArgb(255,200,150,150));
            int xoffset;
            int yoffset;
            xoffset = panel1.AutoScrollPosition.X;
            yoffset = panel1.AutoScrollPosition.Y;
            //TM.Draw(TextureID, 0, 0, 1, 1, new Rectangle(xoffset, yoffset, TM.GetTextureWidth(TextureID), TM.GetTextureHeight(TextureID)));
            if (TextureID != -1)
            {
                TM.Draw(TextureID, xoffset, yoffset);
            }
            for (int i = 0; i < listBoxFrames.Items.Count; i++)
            {
                Frame temp = (Frame)listBoxFrames.Items[i];
                DX.DrawRect(new Rectangle((int)((temp.Anchor.X - 3)) + xoffset, (int)((temp.Anchor.Y - 3)) + yoffset, 6, 6), Color.FromArgb(255, 255, 128));
                DX.DrawHollowRect(new Rectangle((int)(temp.DrawRect1.Left) + xoffset, (int)(temp.DrawRect1.Top) + yoffset, (int)(temp.DrawRect1.Width), (int)(temp.DrawRect1.Height)), Color.FromArgb(120, 255, 128, 255), 5);
                DX.DrawHollowRect(new Rectangle((int)(temp.ColRect.Left) + xoffset, (int)(temp.ColRect.Top) + yoffset, (int)(temp.ColRect.Width), (int)(temp.ColRect.Height)), Color.FromArgb(120, 128, 255, 255), 5);

            }
            if (listBoxFrames.SelectedIndex != -1)
            {
                Frame temp = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
                DX.DrawRect(new Rectangle((int)((temp.Anchor.X - 3)) + xoffset, (int)((temp.Anchor.Y - 3)) + yoffset, 6, 6), Color.FromArgb(255, 255, 0));
                DX.DrawHollowRect(new Rectangle((int)(temp.DrawRect1.Left) + xoffset, (int)(temp.DrawRect1.Top) + yoffset, (int)(temp.DrawRect1.Width), (int)(temp.DrawRect1.Height)), Color.FromArgb(120, 255, 0, 255), 5);
                DX.DrawHollowRect(new Rectangle((int)(temp.ColRect.Left) + xoffset, (int)(temp.ColRect.Top) + yoffset, (int)(temp.ColRect.Width), (int)(temp.ColRect.Height)), Color.FromArgb(120, 0, 255, 255), 5);


            }
            if(DrawInProg.X != -1)
            {
                if (state == 0)
                {
                    DX.DrawHollowRect(new Rectangle((int)(Draw1.X) + xoffset, (int)(Draw1.Y) + yoffset, (int)(DrawInProg.X) - Draw1.X, (int)(DrawInProg.Y) - Draw1.Y), Color.FromArgb(120, 255, 0, 255), 5);
                }
                else if( state == 1)
                {
                    DX.DrawHollowRect(new Rectangle((int)(Col1.X) + xoffset, (int)(Col1.Y) + yoffset, (int)(DrawInProg.X) - Col1.X, (int)(DrawInProg.Y) - Col1.Y), Color.FromArgb(120, 0, 255, 255), 5);

                }
            }

            DX.SpriteEnd();
            DX.DeviceEnd();
            DX.Present();

            //panel 2 render
            DX.Clear(panel2, Color.LimeGreen);
            DX.DeviceBegin();
            DX.SpriteBegin();

            //275 160
            if (listBoxAnimation.SelectedIndex != -1)
            {
                
                Animation an = (Animation)(listBoxAnimation.Items[listBoxAnimation.SelectedIndex]);
                if (curFrame > an.Frames.Count-1)
                {
                    curFrame = 0;
                }
                if (curFrame == -1)
                    curFrame = 0;
                Frame playfram = an.Frames[curFrame];
                
                Point Drawpoint = new Point(275, 160);
                int doffsetx;
                int doffsety;


                if (state == 1)
                {
                    doffsetx = playfram.ColRect.X - playfram.Anchor.X;
                    doffsety = playfram.ColRect.Y - playfram.Anchor.Y;
                    Drawpoint.X += doffsetx;
                    Drawpoint.Y += doffsety;
                    if (playfram.ColRect.Height == 0 || playfram.ColRect.Width == 0)
                    {

                    }
                    else
                    {
                        if(TextureID != -1)
                        TM.Draw(TextureID, Drawpoint.X, Drawpoint.Y, 1, 1, playfram.ColRect);
                    }
                }
                else
                {
                    doffsetx = playfram.DrawRect1.X - playfram.Anchor.X;
                    doffsety = playfram.DrawRect1.Y - playfram.Anchor.Y;
                    Drawpoint.X += doffsetx;
                    Drawpoint.Y += doffsety;
                    if (playfram.DrawRect1.Height == 0 || playfram.DrawRect1.Width == 0)
                    {

                    }
                    else
                    {
                        if(TextureID != -1)
                        TM.Draw(TextureID, Drawpoint.X, Drawpoint.Y, 1, 1, playfram.DrawRect1);
                    }
                }
            }
            else
            {
                //listBoxAnimation.SelectedIndex = 0;
            }
            DX.SpriteEnd();
            DX.DeviceEnd();
            DX.Present();
        }



        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false;
        }

        private void panel1_Resize(object sender, EventArgs e)
        {
            DX.Resize(panel1, panel1.ClientSize.Width, panel1.ClientSize.Height, true);
        }







        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            curFrame = 0;
            listBoxFrames.SelectedIndex = 0;
        }

        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle(f.DrawRect1.Left, f.DrawRect1.Top, (int)numUDDrawRight.Value, f.DrawRect1.Height);

            f.DrawRect1 = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
         
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int numanimations = listBoxAnimation.Items.Count;
            
            XElement data = new XElement("Data",
                new XAttribute("numAnimations",numanimations.ToString()));

            for (int i = 0; i < numanimations; i++)
            {
                Animation an = (Animation)listBoxAnimation.Items[i];
                if (an.Nextname == null)
                {
                    an.Nextname = "None";
                }
                if (an.Name == null)
                {
                    an.Name = "Name";
                }
                int numFrames = an.Frames.Count;
                XElement animation = new XElement("Animation",
                    new XElement("Image",
                      new XAttribute("numFrames", numFrames.ToString()),
                         new XElement("Path", imagepath)),
                    new XElement("Looping",an.Animlooping.ToString()),
                    new XElement("Name", an.Name),
                    new XElement("NextName", an.Nextname));


                for (int frameindex = 0; frameindex < numFrames; frameindex++)
			    {
                    Frame f = an.Frames[frameindex];
                    if(f.Eventname == null || f.Eventname == "")
                    {
                        f.Eventname = "NoEvent";
                    }
                    if(f.Duration == 0.0M)
                    {
                        f.Duration = 0.01M;
                    }
			         XElement frame = new XElement("Frame",
                        new XAttribute("delay",f.Duration),
                        new XElement("AnchorPoint",
                            new XAttribute("x",f.Anchor.X.ToString()),
                            new XAttribute("y",f.Anchor.Y.ToString())),
                        new XElement("DrawRect",
                            new XAttribute("left",f.DrawRect1.Left.ToString()),
                            new XAttribute("top",f.DrawRect1.Top.ToString()),
                            new XAttribute("right",f.DrawRect1.Right.ToString()),
                            new XAttribute("bottom",f.DrawRect1.Bottom.ToString())),
                        new XElement("CollisionRect",
                            new XAttribute("left",f.ColRect.Left.ToString()),
                            new XAttribute("top",f.ColRect.Top.ToString()),
                            new XAttribute("right",f.ColRect.Right.ToString()),
                            new XAttribute("bottom",f.ColRect.Bottom.ToString())),
                        new XElement("Event",f.Eventname));
                     animation.Add(frame);
			    }
                    

                data.Add(animation);
            }

            XDocument doc = new XDocument(data);
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.InitialDirectory = System.IO.Path.GetFullPath(relativepath + "\\Scripts");
            dlg.Filter = "All Files|*.*| XML Files |*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                doc.Save(dlg.FileName);
            }

           
            
            //XmlWriter writer = XmlWriter.Create("TestOutPutFile.xml");
            //writer = animations.CreateWriter();
            //writer.WriteStartDocument();
            //animations.WriteTo(writer);
            //writer.WriteEndDocument();
            

            //using (XmlWriter writer = XmlWriter.Create("TestOutputFile.xml"))
            //{
             
            //    writer.WriteStartDocument();
            //    writer.WriteStartElement("Data");
            //    writer.WriteStartAttribute("numAnimations", "2");
            //    writer.WriteEndAttribute();
            //    writer.WriteEndElement();
                
            //    writer.WriteStartElement("Animation");
            //    writer.WriteStartElement("Image");
            //    writer.WriteAttributeString("numFrames", "2");
            //    writer.WriteElementString("Path", "TESTPATH.png");
            //    writer.WriteEndElement();
            //    writer.WriteEndElement();
            //    writer.WriteEndDocument();
            //}
          
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                dlg.Title = "Open Animation File";
                //Credit for this line of Code goes to Robert Martinez
                dlg.InitialDirectory = System.IO.Path.GetFullPath(relativepath + "\\Scripts");
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    XmlDocument doc = new XmlDocument();
                    doc.Load(dlg.FileName);

                    XmlNodeList nodes = doc.SelectSingleNode("Data").SelectNodes("Animation");
                    
                    listBoxAnimation.Items.Clear();
                    int animdex = 0;
                    
                    foreach(XmlNode node in nodes)
                    {
                        int framedex = 0;
                        string x = node.SelectSingleNode("Image").SelectSingleNode("Path").InnerText;
                        imagepath = x;
                        x = System.IO.Path.GetFullPath(relativepath + "\\Textures\\" + x);

                        TextureID = TM.LoadTexture(x);

                        if (TextureID != -1)
                        {
                            asmin.Width = (int)(TM.GetTextureWidth(TextureID));
                            asmin.Height = (int)(TM.GetTextureHeight(TextureID));
                            panel1.AutoScrollMinSize = asmin;
                        }

                        Animation an = new Animation();
                        an.Index = animdex;
                        animdex++;
                        an.Frames = new List<Frame>();
                        an.Animlooping = Convert.ToBoolean(node.SelectSingleNode("Looping").InnerText);
                        an.Name = node.SelectSingleNode("Name").InnerText;
                        an.Nextname = node.SelectSingleNode("NextName").InnerText;

                        XmlNodeList framenodes = node.SelectNodes("Frame");

                        foreach(XmlNode fnode in framenodes)
                        {
                            Frame f = new Frame();
                            f.Index = framedex;
                            framedex++;
                            f.Duration = Convert.ToDecimal( fnode.Attributes.GetNamedItem("delay").Value);
                            Point ap = new Point();
                            ap.X = Convert.ToInt32( fnode.SelectSingleNode("AnchorPoint").Attributes.GetNamedItem("x").Value);
                            ap.Y = Convert.ToInt32(fnode.SelectSingleNode("AnchorPoint").Attributes.GetNamedItem("y").Value);
                            f.Anchor = ap;
                            int left, top, right, bottom;
                            left = Convert.ToInt32(fnode.SelectSingleNode("DrawRect").Attributes.GetNamedItem("left").Value);
                            top = Convert.ToInt32(fnode.SelectSingleNode("DrawRect").Attributes.GetNamedItem("top").Value);
                            right = Convert.ToInt32(fnode.SelectSingleNode("DrawRect").Attributes.GetNamedItem("right").Value);
                            bottom = Convert.ToInt32(fnode.SelectSingleNode("DrawRect").Attributes.GetNamedItem("bottom").Value);
                            Rectangle rect = new Rectangle(left, top, right - left, bottom - top);
                            f.DrawRect1 = rect;
                            left = Convert.ToInt32(fnode.SelectSingleNode("CollisionRect").Attributes.GetNamedItem("left").Value);
                            top = Convert.ToInt32(fnode.SelectSingleNode("CollisionRect").Attributes.GetNamedItem("top").Value);
                            right = Convert.ToInt32(fnode.SelectSingleNode("CollisionRect").Attributes.GetNamedItem("right").Value);
                            bottom = Convert.ToInt32(fnode.SelectSingleNode("CollisionRect").Attributes.GetNamedItem("bottom").Value);
                            rect = new Rectangle(left, top, right - left, bottom - top);
                            f.ColRect = rect;
                            f.Eventname = fnode.SelectSingleNode("Event").InnerText;

                            an.Frames.Add(f);
                        }
                        listBoxAnimation.Items.Add(an);
                    }
                    listBoxAnimation.SelectedIndex = 0;
                }
            }
        }

        private void btnAddAnimation_Click(object sender, EventArgs e)
        {
            Animation an = new Animation();
            an.Frames = new List<Frame>();
            an.Index = listBoxAnimation.Items.Count;
            listBoxAnimation.Items.Add(an);
            listBoxAnimation.SelectedIndex = an.Index;

            listBoxFrames.Items.Clear();

            Frame f = new Frame();
            an.Frames.Add(f);
            f.Index = listBoxFrames.Items.Count;
            listBoxFrames.Items.Add(f);
            listBoxFrames.SelectedIndex = f.Index;
            an.Frames[0] = f;
            
        }

        private void imageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                dlg.Title = "Open Image";
                //Credit for this line of Code goes to Robert Martinez
                //int dirpathsize = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\..\\..\\..\\Assets\\Textures").Length;
                dlg.InitialDirectory = System.IO.Path.GetFullPath(relativepath + "\\Textures");
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    imagepath = System.IO.Path.GetFileName(dlg.FileName);
                    TextureID = TM.LoadTexture(dlg.FileName);
                    //imagepath = dlg.FileName;
                    //imagepath = imagepath.Remove(0, dirpathsize+1);
                    asmin.Width = (int)(TM.GetTextureWidth(TextureID));
                    asmin.Height = (int)(TM.GetTextureHeight(TextureID));
                    panel1.AutoScrollMinSize = asmin;

                }
            }
            panel1.Invalidate();
        }





        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {

            Point offset = new Point(0, 0);

            offset.X += panel1.AutoScrollPosition.X;
            offset.Y += panel1.AutoScrollPosition.Y;

            if (state == 0)
            {
                //Draw Rect
                Draw1.X = (int)((e.X) - offset.X);
                Draw1.Y = (int)((e.Y) - offset.Y);

                DrawInProg.X = Draw1.X;

            }
            else if (state == 1)
            {
                Col1.X = (int)((e.X) - offset.X);
                Col1.Y = (int)((e.Y) - offset.Y);
                DrawInProg.X = Col1.X;
            }
            
        }

        private void panel1_MouseClick(object sender, MouseEventArgs e)
        {
            if (listBoxFrames.SelectedIndex == -1)
            {
                return;
            }
            Point offset = new Point(0, 0);

            offset.X += panel1.AutoScrollPosition.X;
            offset.Y += panel1.AutoScrollPosition.Y;
            if (state == 2)
            {
                AnchorPoint.X = (int)((e.X) - offset.X);
                AnchorPoint.Y = (int)((e.Y) - offset.Y);
                Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
                f.Anchor = AnchorPoint;
                listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;

                Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
                an.Frames[listBoxFrames.SelectedIndex] = f;
            }
        }

        private void btnAddFrame_Click(object sender, EventArgs e)
        {
            Animation a = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            
            Frame f = new Frame();
            a.Frames.Add(f);
            f.Duration = .25M;
            f.Eventname = "NoEvent";
            f.Index = listBoxFrames.Items.Count;
            listBoxFrames.Items.Add(f);
            listBoxFrames.SelectedIndex = f.Index;

           
            a.Frames[f.Index] = f;

        }

        private void btnDrawRect_Click(object sender, EventArgs e)
        {
            btnDrawRect.BackColor = Color.FromArgb(255, 255, 128, 255);
            btnColRect.BackColor = Color.White;
            btnAnchorPoint.BackColor = Color.White;
            state = 0;
        }

        private void btnColRect_Click(object sender, EventArgs e)
        {
            btnColRect.BackColor = Color.FromArgb(255, 128, 255, 255);
            btnDrawRect.BackColor = Color.White;
            btnAnchorPoint.BackColor = Color.White;
            state = 1;
        }

        private void btnAnchorPoint_Click(object sender, EventArgs e)
        {
            btnAnchorPoint.BackColor = Color.FromArgb(255, 255, 255, 128);
            btnDrawRect.BackColor = Color.White;
            btnColRect.BackColor = Color.White;
            state = 2;
        }
        private void FlipIfNecessary(Point p1, Point p2)
        {
            if (p1.X > p2.X)
            {
                int x = p1.X;
                p1.X = p2.X;
                p2.X = x;
            }
            if (p1.Y > p2.Y)
            {
                int y = p1.Y;
                p1.Y = p2.Y;
                p2.Y = y;
            }
            if (p1.X < 0)
                p1.X = 0;
            if (p1.Y < 0)
                p1.Y = 0;
            Draw1 = Col1 = p1;
            Draw2 = Col2 = p2;
        }

        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            if (listBoxFrames.SelectedIndex == -1)
                return;
            Point offset = new Point(0, 0);

            offset.X += panel1.AutoScrollPosition.X;
            offset.Y += panel1.AutoScrollPosition.Y;

            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];

            if (state == 0)
            {
                Draw2.X = (int)((e.X) - offset.X);
                Draw2.Y = (int)((e.Y) - offset.Y);
                FlipIfNecessary(Draw1, Draw2);

                Rectangle r = new Rectangle(Draw1.X, Draw1.Y, Draw2.X - Draw1.X, Draw2.Y - Draw1.Y);
                f.DrawRect1 = r;
                listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            }
            else if (state == 1)
            {
                Col2.X = (int)((e.X) - offset.X);
                Col2.Y = (int)((e.Y) - offset.Y);
                
                FlipIfNecessary(Col1, Col2);
                Rectangle r = new Rectangle(Col1.X, Col1.Y, Col2.X - Col1.X, Col2.Y - Col1.Y);
                f.ColRect = r;
                listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            }
            if (listBoxAnimation.SelectedIndex == -1)
                return;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            an.Frames[listBoxFrames.SelectedIndex] = f;

            DrawInProg.X = -1;
        }

        private void listBoxAnimation_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBoxAnimation.SelectedIndex == -1)
                return;
            
            Animation a = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items.Clear();
            checkBoxLooping.Checked = a.Animlooping;
            txtName.Text = a.Name;
            txtNext.Text = a.Nextname;
            for (int i = 0; i < a.Frames.Count; i++)
            {
                Frame temp = a.Frames[i];
                listBoxFrames.Items.Add(temp);
            }
        }

        private void UpdateNumericData()
        {
            if (listBoxFrames.SelectedIndex == -1)
                return;
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
            numUDPointX.Value = f.Anchor.X;
            numUDPointY.Value = f.Anchor.Y;
            numUDDrawLeft.Value = f.DrawRect1.Left;
            numUDDrawTop.Value = f.DrawRect1.Top;
            numUDDrawRight.Value = f.DrawRect1.Width;
            numUDDrawBottom.Value = f.DrawRect1.Height;
            numUDColLeft.Value = f.ColRect.Left;
            numUDColRight.Value = f.ColRect.Width;
            numUDColTop.Value = f.ColRect.Top;
            numUDColBottom.Value = f.ColRect.Height;

            numUDDuration.Value = (decimal)f.Duration;

            txtEventName.Text = f.Eventname;
      
        }

        private void numUDDuration_ValueChanged(object sender, EventArgs e)
        {
            if (listBoxFrames.SelectedIndex == -1)
                return;
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
            f.Duration = numUDDuration.Value;
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            an.Frames[listBoxFrames.SelectedIndex] = f;
        }

        private void listBoxFrames_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateNumericData();
            curFrame = listBoxFrames.SelectedIndex;
        }

        private void numUDPointX_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
            Point pt = f.Anchor;

            pt.X = (int)numUDPointX.Value;
            f.Anchor = pt;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
        }

        private void numUDPointY_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
            Point pt = f.Anchor;

            pt.Y = (int)numUDPointY.Value;
            f.Anchor = pt;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
            
        }

        private void numUDDrawLeft_ValueChanged(object sender, EventArgs e)
        {

            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle((int)numUDDrawLeft.Value, f.DrawRect1.Top, f.DrawRect1.Width, f.DrawRect1.Height);

            f.DrawRect1 = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
            
        }

        private void numUDColLeft_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle((int)numUDColLeft.Value, f.ColRect.Top, f.ColRect.Width, f.ColRect.Height);

            f.ColRect = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
           
        }

        private void numUDDrawTop_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle(f.DrawRect1.Left, (int)numUDDrawTop.Value, f.DrawRect1.Width, f.DrawRect1.Height);

            f.DrawRect1 = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
          
        }

        private void numUDDrawBottom_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle(f.DrawRect1.Left, f.DrawRect1.Top, f.DrawRect1.Width, (int)numUDDrawBottom.Value);

            f.DrawRect1 = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
           
        }

        private void numUDColTop_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle(f.ColRect.Left, (int)numUDColTop.Value, f.ColRect.Width, f.ColRect.Height);

            f.ColRect = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
           
        }

        private void numUDColRight_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle(f.ColRect.Left, f.ColRect.Top, (int)numUDColRight.Value, f.ColRect.Height);

            f.ColRect = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
            
        }

        private void numUDColBottom_ValueChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];


            Rectangle rect = new Rectangle(f.ColRect.Left, f.ColRect.Top, f.ColRect.Width, (int)numUDColBottom.Value);

            f.ColRect = rect;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
           
        }

        private void txtEventName_TextChanged(object sender, EventArgs e)
        {
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
            f.Eventname = txtEventName.Text;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxFrames.Items[listBoxFrames.SelectedIndex] = f;
            an.Frames[listBoxFrames.SelectedIndex] = f;
           
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            playbackpause = false;
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            playbackpause = true;
        }

        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == 'd' )
            {
                btnDrawRect.BackColor = Color.FromArgb(255, 255, 128, 255);
                btnColRect.BackColor = Color.White;
                btnAnchorPoint.BackColor = Color.White;
                state = 0;
            }
            else if(e.KeyChar == 's')
            {
                btnColRect.BackColor = Color.FromArgb(255, 128, 255, 255);
                btnDrawRect.BackColor = Color.White;
                btnAnchorPoint.BackColor = Color.White;
                state = 1;
            }
            else if(e.KeyChar == 'a')
            {
                btnAnchorPoint.BackColor = Color.FromArgb(255, 255, 255, 128);
                btnDrawRect.BackColor = Color.White;
                btnColRect.BackColor = Color.White;
                state = 2;
            }
            //else if(e.KeyChar == 'f')
            //{
            //    Frame f = new Frame();
            //    f.Duration = .25M;
            //    f.Eventname = "NoEvent";
            //    f.Index = listBoxFrames.Items.Count;
            //    listBoxFrames.Items.Add(f);
            //    listBoxFrames.SelectedIndex = f.Index;

            //    Animation a = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            //    a.Frames.Add(f);
            //}
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (DrawInProg.X != -1)
            {
                Point offset = new Point(0, 0);

                offset.X += panel1.AutoScrollPosition.X;
                offset.Y += panel1.AutoScrollPosition.Y;

                DrawInProg.X = (int)((e.X) - offset.X);
                DrawInProg.Y = (int)((e.Y) - offset.Y);
            }
        }

        private void btnPrevFrame_Click(object sender, EventArgs e)
        {
            if (playbackpause == false)
                return;

            if (curFrame > 0)
                curFrame--;

            listBoxFrames.SelectedIndex = curFrame;
        }

        private void btnNextFrame_Click(object sender, EventArgs e)
        {
            if (playbackpause == false)
                return;
            if (listBoxFrames.Items.Count <= curFrame + 1)
            {
                curFrame = 0;
            }
            else
            {
                curFrame++;
            }

            listBoxFrames.SelectedIndex = curFrame;
        }

        private void btnRemoveFrame_Click(object sender, EventArgs e)
        {
            if (listBoxFrames.Items.Count == 1)
                return;
            if (listBoxFrames.SelectedIndex == -1)
                return;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            Frame f = (Frame)listBoxFrames.Items[listBoxFrames.SelectedIndex];
            an.Frames.Remove(f);
            listBoxFrames.Items.Remove(f);
            listBoxFrames.SelectedIndex = 0;
            listBoxAnimation.Items[listBoxAnimation.SelectedIndex] = an;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (listBoxAnimation.Items.Count == 1)
                return;
            if (listBoxAnimation.SelectedIndex == -1)
                return;
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            listBoxAnimation.Items.Remove(an);
        }

        private void checkBoxLooping_CheckedChanged(object sender, EventArgs e)
        {
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            if (an.Animlooping == checkBoxLooping.Checked)
                return;
            an.Animlooping = checkBoxLooping.Checked;
            listBoxAnimation.Items[listBoxAnimation.SelectedIndex] = an;
        }

        private void txtName_TextChanged(object sender, EventArgs e)
        {
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            if (an.Name == txtName.Text)
                return;
            an.Name = txtName.Text;
            listBoxAnimation.Items[listBoxAnimation.SelectedIndex] = an;

        }

        private void txtNext_TextChanged(object sender, EventArgs e)
        {
            Animation an = (Animation)listBoxAnimation.Items[listBoxAnimation.SelectedIndex];
            if (an.Nextname == txtNext.Text)
                return;
            an.Nextname = txtNext.Text;
            listBoxAnimation.Items[listBoxAnimation.SelectedIndex] = an;
        }
        
    }
}
