namespace AnimationEditor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btnReset = new System.Windows.Forms.Button();
            this.btnPrevFrame = new System.Windows.Forms.Button();
            this.btnPlay = new System.Windows.Forms.Button();
            this.btnNextFrame = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnAnchorPoint = new System.Windows.Forms.Button();
            this.btnColRect = new System.Windows.Forms.Button();
            this.btnDrawRect = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.numUDPointY = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.numUDPointX = new System.Windows.Forms.NumericUpDown();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.numUDDrawBottom = new System.Windows.Forms.NumericUpDown();
            this.numUDDrawTop = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.numUDDrawRight = new System.Windows.Forms.NumericUpDown();
            this.numUDDrawLeft = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.numUDColBottom = new System.Windows.Forms.NumericUpDown();
            this.numUDColTop = new System.Windows.Forms.NumericUpDown();
            this.label11 = new System.Windows.Forms.Label();
            this.numUDColRight = new System.Windows.Forms.NumericUpDown();
            this.numUDColLeft = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.listBoxFrames = new System.Windows.Forms.ListBox();
            this.label14 = new System.Windows.Forms.Label();
            this.txtName = new System.Windows.Forms.TextBox();
            this.txtNext = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.checkBoxLooping = new System.Windows.Forms.CheckBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.btnAddAnimation = new System.Windows.Forms.Button();
            this.listBoxAnimation = new System.Windows.Forms.ListBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.txtEventName = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.numUDDuration = new System.Windows.Forms.NumericUpDown();
            this.btnRemoveFrame = new System.Windows.Forms.Button();
            this.btnAddFrame = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDPointY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDPointX)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawBottom)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawTop)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawRight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawLeft)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColBottom)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColTop)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColRight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColLeft)).BeginInit();
            this.groupBox6.SuspendLayout();
            this.groupBox7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDuration)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.Location = new System.Drawing.Point(12, 46);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(813, 370);
            this.panel1.TabIndex = 0;
            this.panel1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseClick);
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseDown);
            this.panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseMove);
            this.panel1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseUp);
            this.panel1.Resize += new System.EventHandler(this.panel1_Resize);
            // 
            // panel2
            // 
            this.panel2.AutoScroll = true;
            this.panel2.Location = new System.Drawing.Point(659, 441);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(550, 320);
            this.panel2.TabIndex = 1;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.importToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1224, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.saveToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.loadToolStripMenuItem.Text = "Load From XML";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.saveToolStripMenuItem.Text = "Save To XML";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // importToolStripMenuItem
            // 
            this.importToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.imageToolStripMenuItem});
            this.importToolStripMenuItem.Name = "importToolStripMenuItem";
            this.importToolStripMenuItem.Size = new System.Drawing.Size(55, 20);
            this.importToolStripMenuItem.Text = "Import";
            // 
            // imageToolStripMenuItem
            // 
            this.imageToolStripMenuItem.Name = "imageToolStripMenuItem";
            this.imageToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.imageToolStripMenuItem.Text = "Image";
            this.imageToolStripMenuItem.Click += new System.EventHandler(this.imageToolStripMenuItem_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(211, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Image";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(752, 425);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Animation";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // btnReset
            // 
            this.btnReset.Location = new System.Drawing.Point(15, 19);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(75, 23);
            this.btnReset.TabIndex = 5;
            this.btnReset.Text = "Reset";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // btnPrevFrame
            // 
            this.btnPrevFrame.Location = new System.Drawing.Point(15, 122);
            this.btnPrevFrame.Name = "btnPrevFrame";
            this.btnPrevFrame.Size = new System.Drawing.Size(75, 23);
            this.btnPrevFrame.TabIndex = 6;
            this.btnPrevFrame.Text = "Prev Frame";
            this.btnPrevFrame.UseVisualStyleBackColor = true;
            this.btnPrevFrame.Click += new System.EventHandler(this.btnPrevFrame_Click);
            // 
            // btnPlay
            // 
            this.btnPlay.Location = new System.Drawing.Point(15, 77);
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(75, 23);
            this.btnPlay.TabIndex = 7;
            this.btnPlay.Text = "Play";
            this.btnPlay.UseVisualStyleBackColor = true;
            this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // btnNextFrame
            // 
            this.btnNextFrame.Location = new System.Drawing.Point(15, 151);
            this.btnNextFrame.Name = "btnNextFrame";
            this.btnNextFrame.Size = new System.Drawing.Size(75, 23);
            this.btnNextFrame.TabIndex = 8;
            this.btnNextFrame.Text = "Next Frame";
            this.btnNextFrame.UseVisualStyleBackColor = true;
            this.btnNextFrame.Click += new System.EventHandler(this.btnNextFrame_Click);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(15, 48);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(75, 23);
            this.btnStop.TabIndex = 9;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnReset);
            this.groupBox1.Controls.Add(this.btnStop);
            this.groupBox1.Controls.Add(this.btnPrevFrame);
            this.groupBox1.Controls.Add(this.btnNextFrame);
            this.groupBox1.Controls.Add(this.btnPlay);
            this.groupBox1.Location = new System.Drawing.Point(531, 441);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(118, 188);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "PlayBackControls";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnAnchorPoint);
            this.groupBox2.Controls.Add(this.btnColRect);
            this.groupBox2.Controls.Add(this.btnDrawRect);
            this.groupBox2.Location = new System.Drawing.Point(846, 39);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(151, 152);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Frame";
            // 
            // btnAnchorPoint
            // 
            this.btnAnchorPoint.BackColor = System.Drawing.Color.White;
            this.btnAnchorPoint.Location = new System.Drawing.Point(6, 101);
            this.btnAnchorPoint.Name = "btnAnchorPoint";
            this.btnAnchorPoint.Size = new System.Drawing.Size(101, 23);
            this.btnAnchorPoint.TabIndex = 2;
            this.btnAnchorPoint.Text = "(A)nchor Point";
            this.btnAnchorPoint.UseVisualStyleBackColor = false;
            this.btnAnchorPoint.Click += new System.EventHandler(this.btnAnchorPoint_Click);
            // 
            // btnColRect
            // 
            this.btnColRect.BackColor = System.Drawing.Color.White;
            this.btnColRect.Location = new System.Drawing.Point(6, 60);
            this.btnColRect.Name = "btnColRect";
            this.btnColRect.Size = new System.Drawing.Size(101, 23);
            this.btnColRect.TabIndex = 1;
            this.btnColRect.Text = "Colli(S)ion Rect";
            this.btnColRect.UseVisualStyleBackColor = false;
            this.btnColRect.Click += new System.EventHandler(this.btnColRect_Click);
            // 
            // btnDrawRect
            // 
            this.btnDrawRect.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.btnDrawRect.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.btnDrawRect.Location = new System.Drawing.Point(6, 19);
            this.btnDrawRect.Name = "btnDrawRect";
            this.btnDrawRect.Size = new System.Drawing.Size(101, 23);
            this.btnDrawRect.TabIndex = 0;
            this.btnDrawRect.Text = "(D)raw Rect";
            this.btnDrawRect.UseVisualStyleBackColor = false;
            this.btnDrawRect.Click += new System.EventHandler(this.btnDrawRect_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.numUDPointY);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.numUDPointX);
            this.groupBox3.Location = new System.Drawing.Point(846, 197);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(141, 152);
            this.groupBox3.TabIndex = 12;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Fine Tune Point";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 60);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Y Pos";
            // 
            // numUDPointY
            // 
            this.numUDPointY.Location = new System.Drawing.Point(47, 60);
            this.numUDPointY.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDPointY.Name = "numUDPointY";
            this.numUDPointY.Size = new System.Drawing.Size(73, 20);
            this.numUDPointY.TabIndex = 2;
            this.numUDPointY.ValueChanged += new System.EventHandler(this.numUDPointY_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 24);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 1;
            this.label3.Text = "X Pos";
            // 
            // numUDPointX
            // 
            this.numUDPointX.Location = new System.Drawing.Point(47, 24);
            this.numUDPointX.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDPointX.Name = "numUDPointX";
            this.numUDPointX.Size = new System.Drawing.Size(73, 20);
            this.numUDPointX.TabIndex = 0;
            this.numUDPointX.ValueChanged += new System.EventHandler(this.numUDPointX_ValueChanged);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.numUDDrawBottom);
            this.groupBox4.Controls.Add(this.numUDDrawTop);
            this.groupBox4.Controls.Add(this.label8);
            this.groupBox4.Controls.Add(this.numUDDrawRight);
            this.groupBox4.Controls.Add(this.numUDDrawLeft);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Location = new System.Drawing.Point(1052, 39);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(172, 152);
            this.groupBox4.TabIndex = 13;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Fine Tune Draw Rect";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 120);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(38, 13);
            this.label7.TabIndex = 17;
            this.label7.Text = "Height";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 51);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(26, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Top";
            // 
            // numUDDrawBottom
            // 
            this.numUDDrawBottom.Location = new System.Drawing.Point(61, 120);
            this.numUDDrawBottom.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDDrawBottom.Name = "numUDDrawBottom";
            this.numUDDrawBottom.Size = new System.Drawing.Size(73, 20);
            this.numUDDrawBottom.TabIndex = 16;
            this.numUDDrawBottom.ValueChanged += new System.EventHandler(this.numUDDrawBottom_ValueChanged);
            // 
            // numUDDrawTop
            // 
            this.numUDDrawTop.Location = new System.Drawing.Point(61, 51);
            this.numUDDrawTop.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDDrawTop.Name = "numUDDrawTop";
            this.numUDDrawTop.Size = new System.Drawing.Size(73, 20);
            this.numUDDrawTop.TabIndex = 6;
            this.numUDDrawTop.ValueChanged += new System.EventHandler(this.numUDDrawTop_ValueChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 85);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(35, 13);
            this.label8.TabIndex = 15;
            this.label8.Text = "Width";
            // 
            // numUDDrawRight
            // 
            this.numUDDrawRight.Location = new System.Drawing.Point(61, 85);
            this.numUDDrawRight.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDDrawRight.Name = "numUDDrawRight";
            this.numUDDrawRight.Size = new System.Drawing.Size(73, 20);
            this.numUDDrawRight.TabIndex = 14;
            this.numUDDrawRight.ValueChanged += new System.EventHandler(this.numericUpDown4_ValueChanged);
            // 
            // numUDDrawLeft
            // 
            this.numUDDrawLeft.Location = new System.Drawing.Point(61, 21);
            this.numUDDrawLeft.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDDrawLeft.Name = "numUDDrawLeft";
            this.numUDDrawLeft.Size = new System.Drawing.Size(73, 20);
            this.numUDDrawLeft.TabIndex = 4;
            this.numUDDrawLeft.ValueChanged += new System.EventHandler(this.numUDDrawLeft_ValueChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 21);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(25, 13);
            this.label6.TabIndex = 5;
            this.label6.Text = "Left";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Controls.Add(this.label10);
            this.groupBox5.Controls.Add(this.numUDColBottom);
            this.groupBox5.Controls.Add(this.numUDColTop);
            this.groupBox5.Controls.Add(this.label11);
            this.groupBox5.Controls.Add(this.numUDColRight);
            this.groupBox5.Controls.Add(this.numUDColLeft);
            this.groupBox5.Controls.Add(this.label12);
            this.groupBox5.Location = new System.Drawing.Point(1052, 197);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(172, 152);
            this.groupBox5.TabIndex = 14;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Fine Tune Collision Rect";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 120);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(38, 13);
            this.label9.TabIndex = 17;
            this.label9.Text = "Height";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 51);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(26, 13);
            this.label10.TabIndex = 7;
            this.label10.Text = "Top";
            // 
            // numUDColBottom
            // 
            this.numUDColBottom.Location = new System.Drawing.Point(61, 120);
            this.numUDColBottom.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDColBottom.Name = "numUDColBottom";
            this.numUDColBottom.Size = new System.Drawing.Size(73, 20);
            this.numUDColBottom.TabIndex = 16;
            this.numUDColBottom.ValueChanged += new System.EventHandler(this.numUDColBottom_ValueChanged);
            // 
            // numUDColTop
            // 
            this.numUDColTop.Location = new System.Drawing.Point(61, 51);
            this.numUDColTop.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDColTop.Name = "numUDColTop";
            this.numUDColTop.Size = new System.Drawing.Size(73, 20);
            this.numUDColTop.TabIndex = 6;
            this.numUDColTop.ValueChanged += new System.EventHandler(this.numUDColTop_ValueChanged);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(6, 85);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(35, 13);
            this.label11.TabIndex = 15;
            this.label11.Text = "Width";
            // 
            // numUDColRight
            // 
            this.numUDColRight.Location = new System.Drawing.Point(61, 85);
            this.numUDColRight.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDColRight.Name = "numUDColRight";
            this.numUDColRight.Size = new System.Drawing.Size(73, 20);
            this.numUDColRight.TabIndex = 14;
            this.numUDColRight.ValueChanged += new System.EventHandler(this.numUDColRight_ValueChanged);
            // 
            // numUDColLeft
            // 
            this.numUDColLeft.Location = new System.Drawing.Point(61, 21);
            this.numUDColLeft.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numUDColLeft.Name = "numUDColLeft";
            this.numUDColLeft.Size = new System.Drawing.Size(73, 20);
            this.numUDColLeft.TabIndex = 4;
            this.numUDColLeft.ValueChanged += new System.EventHandler(this.numUDColLeft_ValueChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(6, 21);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(25, 13);
            this.label12.TabIndex = 5;
            this.label12.Text = "Left";
            // 
            // listBoxFrames
            // 
            this.listBoxFrames.FormattingEnabled = true;
            this.listBoxFrames.Location = new System.Drawing.Point(6, 20);
            this.listBoxFrames.Name = "listBoxFrames";
            this.listBoxFrames.Size = new System.Drawing.Size(136, 134);
            this.listBoxFrames.TabIndex = 15;
            this.listBoxFrames.SelectedIndexChanged += new System.EventHandler(this.listBoxFrames_SelectedIndexChanged);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(6, 26);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(84, 13);
            this.label14.TabIndex = 17;
            this.label14.Text = "Animation Name";
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(121, 23);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(100, 20);
            this.txtName.TabIndex = 18;
            this.txtName.TextChanged += new System.EventHandler(this.txtName_TextChanged);
            // 
            // txtNext
            // 
            this.txtNext.Location = new System.Drawing.Point(121, 53);
            this.txtNext.Name = "txtNext";
            this.txtNext.Size = new System.Drawing.Size(100, 20);
            this.txtNext.TabIndex = 20;
            this.txtNext.TextChanged += new System.EventHandler(this.txtNext_TextChanged);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(6, 53);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(109, 13);
            this.label15.TabIndex = 19;
            this.label15.Text = "Next Animation Name";
            // 
            // checkBoxLooping
            // 
            this.checkBoxLooping.AutoSize = true;
            this.checkBoxLooping.Location = new System.Drawing.Point(9, 84);
            this.checkBoxLooping.Name = "checkBoxLooping";
            this.checkBoxLooping.Size = new System.Drawing.Size(64, 17);
            this.checkBoxLooping.TabIndex = 22;
            this.checkBoxLooping.Text = "Looping";
            this.checkBoxLooping.UseVisualStyleBackColor = true;
            this.checkBoxLooping.CheckedChanged += new System.EventHandler(this.checkBoxLooping_CheckedChanged);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.button1);
            this.groupBox6.Controls.Add(this.btnAddAnimation);
            this.groupBox6.Controls.Add(this.listBoxAnimation);
            this.groupBox6.Controls.Add(this.label14);
            this.groupBox6.Controls.Add(this.checkBoxLooping);
            this.groupBox6.Controls.Add(this.txtName);
            this.groupBox6.Controls.Add(this.txtNext);
            this.groupBox6.Controls.Add(this.label15);
            this.groupBox6.Location = new System.Drawing.Point(289, 441);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(236, 248);
            this.groupBox6.TabIndex = 23;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Animation Data";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(127, 137);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(106, 23);
            this.button1.TabIndex = 25;
            this.button1.Text = "Remove Animation";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btnAddAnimation
            // 
            this.btnAddAnimation.Location = new System.Drawing.Point(127, 108);
            this.btnAddAnimation.Name = "btnAddAnimation";
            this.btnAddAnimation.Size = new System.Drawing.Size(106, 23);
            this.btnAddAnimation.TabIndex = 24;
            this.btnAddAnimation.Text = "Add Animation";
            this.btnAddAnimation.UseVisualStyleBackColor = true;
            this.btnAddAnimation.Click += new System.EventHandler(this.btnAddAnimation_Click);
            // 
            // listBoxAnimation
            // 
            this.listBoxAnimation.FormattingEnabled = true;
            this.listBoxAnimation.Location = new System.Drawing.Point(9, 108);
            this.listBoxAnimation.Name = "listBoxAnimation";
            this.listBoxAnimation.Size = new System.Drawing.Size(112, 134);
            this.listBoxAnimation.TabIndex = 23;
            this.listBoxAnimation.SelectedIndexChanged += new System.EventHandler(this.listBoxAnimation_SelectedIndexChanged);
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.txtEventName);
            this.groupBox7.Controls.Add(this.label16);
            this.groupBox7.Controls.Add(this.label13);
            this.groupBox7.Controls.Add(this.numUDDuration);
            this.groupBox7.Controls.Add(this.btnRemoveFrame);
            this.groupBox7.Controls.Add(this.btnAddFrame);
            this.groupBox7.Controls.Add(this.listBoxFrames);
            this.groupBox7.Location = new System.Drawing.Point(12, 441);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(271, 257);
            this.groupBox7.TabIndex = 24;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Frames";
            // 
            // txtEventName
            // 
            this.txtEventName.Location = new System.Drawing.Point(117, 202);
            this.txtEventName.Name = "txtEventName";
            this.txtEventName.Size = new System.Drawing.Size(139, 20);
            this.txtEventName.TabIndex = 21;
            this.txtEventName.Text = "NoEvent";
            this.txtEventName.TextChanged += new System.EventHandler(this.txtEventName_TextChanged);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(6, 202);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(71, 13);
            this.label16.TabIndex = 20;
            this.label16.Text = "Trigger Name";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(6, 168);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(79, 13);
            this.label13.TabIndex = 19;
            this.label13.Text = "Frame Duration";
            // 
            // numUDDuration
            // 
            this.numUDDuration.DecimalPlaces = 2;
            this.numUDDuration.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.numUDDuration.Location = new System.Drawing.Point(157, 168);
            this.numUDDuration.Name = "numUDDuration";
            this.numUDDuration.Size = new System.Drawing.Size(99, 20);
            this.numUDDuration.TabIndex = 18;
            this.numUDDuration.Value = new decimal(new int[] {
            25,
            0,
            0,
            131072});
            this.numUDDuration.ValueChanged += new System.EventHandler(this.numUDDuration_ValueChanged);
            // 
            // btnRemoveFrame
            // 
            this.btnRemoveFrame.Location = new System.Drawing.Point(157, 53);
            this.btnRemoveFrame.Name = "btnRemoveFrame";
            this.btnRemoveFrame.Size = new System.Drawing.Size(99, 23);
            this.btnRemoveFrame.TabIndex = 17;
            this.btnRemoveFrame.Text = "Remove Frame";
            this.btnRemoveFrame.UseVisualStyleBackColor = true;
            this.btnRemoveFrame.Click += new System.EventHandler(this.btnRemoveFrame_Click);
            // 
            // btnAddFrame
            // 
            this.btnAddFrame.Location = new System.Drawing.Point(157, 20);
            this.btnAddFrame.Name = "btnAddFrame";
            this.btnAddFrame.Size = new System.Drawing.Size(99, 23);
            this.btnAddFrame.TabIndex = 16;
            this.btnAddFrame.Text = "Add Frame";
            this.btnAddFrame.UseVisualStyleBackColor = true;
            this.btnAddFrame.Click += new System.EventHandler(this.btnAddFrame_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1224, 764);
            this.Controls.Add(this.groupBox7);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.menuStrip1);
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Animation Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Form1_KeyPress);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDPointY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDPointX)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawBottom)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawTop)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawRight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDrawLeft)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColBottom)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColTop)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColRight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUDColLeft)).EndInit();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUDDuration)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.Button btnPrevFrame;
        private System.Windows.Forms.Button btnPlay;
        private System.Windows.Forms.Button btnNextFrame;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnAnchorPoint;
        private System.Windows.Forms.Button btnColRect;
        private System.Windows.Forms.Button btnDrawRect;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown numUDPointY;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numUDPointX;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown numUDDrawBottom;
        private System.Windows.Forms.NumericUpDown numUDDrawTop;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown numUDDrawRight;
        private System.Windows.Forms.NumericUpDown numUDDrawLeft;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.NumericUpDown numUDColBottom;
        private System.Windows.Forms.NumericUpDown numUDColTop;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.NumericUpDown numUDColRight;
        private System.Windows.Forms.NumericUpDown numUDColLeft;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ListBox listBoxFrames;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.TextBox txtNext;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.CheckBox checkBoxLooping;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.TextBox txtEventName;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.NumericUpDown numUDDuration;
        private System.Windows.Forms.Button btnRemoveFrame;
        private System.Windows.Forms.Button btnAddFrame;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem imageToolStripMenuItem;
        private System.Windows.Forms.Button btnAddAnimation;
        private System.Windows.Forms.ListBox listBoxAnimation;
        private System.Windows.Forms.Button button1;

    }
}

