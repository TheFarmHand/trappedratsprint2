namespace DialogueEditor
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1 = new System.Windows.Forms.Panel();
            this.DeleteButton = new System.Windows.Forms.Button();
            this.DeselectButton = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.PortraitBox = new System.Windows.Forms.TextBox();
            this.DownButton = new System.Windows.Forms.Button();
            this.UpButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.SaveButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.NameBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.WordsLabel = new System.Windows.Forms.Label();
            this.WordsBox = new System.Windows.Forms.TextBox();
            this.MessagesBox = new System.Windows.Forms.ListBox();
            this.BrowseButton = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(371, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToolStripMenuItem,
            this.loadToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.saveToolStripMenuItem.Text = "&Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.loadToolStripMenuItem.Text = "&Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panel1.Controls.Add(this.BrowseButton);
            this.panel1.Controls.Add(this.DeleteButton);
            this.panel1.Controls.Add(this.DeselectButton);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.PortraitBox);
            this.panel1.Controls.Add(this.DownButton);
            this.panel1.Controls.Add(this.UpButton);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.SaveButton);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.NameBox);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.WordsLabel);
            this.panel1.Controls.Add(this.WordsBox);
            this.panel1.Controls.Add(this.MessagesBox);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 24);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(371, 401);
            this.panel1.TabIndex = 1;
            // 
            // DeleteButton
            // 
            this.DeleteButton.Location = new System.Drawing.Point(231, 371);
            this.DeleteButton.Name = "DeleteButton";
            this.DeleteButton.Size = new System.Drawing.Size(123, 23);
            this.DeleteButton.TabIndex = 13;
            this.DeleteButton.Text = "Delete";
            this.DeleteButton.UseVisualStyleBackColor = true;
            this.DeleteButton.Click += new System.EventHandler(this.DeleteButton_Click);
            // 
            // DeselectButton
            // 
            this.DeselectButton.Location = new System.Drawing.Point(12, 371);
            this.DeselectButton.Name = "DeselectButton";
            this.DeselectButton.Size = new System.Drawing.Size(120, 23);
            this.DeselectButton.TabIndex = 12;
            this.DeselectButton.Text = "Deselect";
            this.DeselectButton.UseVisualStyleBackColor = true;
            this.DeselectButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.DeselectButton_MouseClick);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(162, 228);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Portrait Path";
            // 
            // PortraitBox
            // 
            this.PortraitBox.Location = new System.Drawing.Point(233, 220);
            this.PortraitBox.Name = "PortraitBox";
            this.PortraitBox.Size = new System.Drawing.Size(123, 20);
            this.PortraitBox.TabIndex = 10;
            // 
            // DownButton
            // 
            this.DownButton.Location = new System.Drawing.Point(231, 342);
            this.DownButton.Name = "DownButton";
            this.DownButton.Size = new System.Drawing.Size(123, 23);
            this.DownButton.TabIndex = 9;
            this.DownButton.Text = "Down";
            this.DownButton.UseVisualStyleBackColor = true;
            this.DownButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.DownButton_MouseClick);
            // 
            // UpButton
            // 
            this.UpButton.Location = new System.Drawing.Point(231, 313);
            this.UpButton.Name = "UpButton";
            this.UpButton.Size = new System.Drawing.Size(123, 23);
            this.UpButton.TabIndex = 8;
            this.UpButton.Text = "Up";
            this.UpButton.UseVisualStyleBackColor = true;
            this.UpButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.UpButton_MouseClick);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(145, 332);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(79, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Move Selected";
            // 
            // SaveButton
            // 
            this.SaveButton.Location = new System.Drawing.Point(232, 284);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(123, 23);
            this.SaveButton.TabIndex = 6;
            this.SaveButton.Text = "Save Message";
            this.SaveButton.UseVisualStyleBackColor = true;
            this.SaveButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.SaveButton_MouseClick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(145, 201);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Message Name";
            // 
            // NameBox
            // 
            this.NameBox.Location = new System.Drawing.Point(232, 194);
            this.NameBox.Name = "NameBox";
            this.NameBox.Size = new System.Drawing.Size(123, 20);
            this.NameBox.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 201);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Messages";
            // 
            // WordsLabel
            // 
            this.WordsLabel.AutoSize = true;
            this.WordsLabel.Location = new System.Drawing.Point(9, 10);
            this.WordsLabel.Name = "WordsLabel";
            this.WordsLabel.Size = new System.Drawing.Size(38, 13);
            this.WordsLabel.TabIndex = 2;
            this.WordsLabel.Text = "Words";
            // 
            // WordsBox
            // 
            this.WordsBox.Location = new System.Drawing.Point(12, 26);
            this.WordsBox.Multiline = true;
            this.WordsBox.Name = "WordsBox";
            this.WordsBox.Size = new System.Drawing.Size(343, 147);
            this.WordsBox.TabIndex = 1;
            // 
            // MessagesBox
            // 
            this.MessagesBox.FormattingEnabled = true;
            this.MessagesBox.Location = new System.Drawing.Point(12, 220);
            this.MessagesBox.Name = "MessagesBox";
            this.MessagesBox.Size = new System.Drawing.Size(120, 147);
            this.MessagesBox.TabIndex = 0;
            this.MessagesBox.SelectedValueChanged += new System.EventHandler(this.MessagesBox_SelectedValueChanged);
            // 
            // BrowseButton
            // 
            this.BrowseButton.Location = new System.Drawing.Point(232, 246);
            this.BrowseButton.Name = "BrowseButton";
            this.BrowseButton.Size = new System.Drawing.Size(122, 23);
            this.BrowseButton.TabIndex = 14;
            this.BrowseButton.Text = "Browse";
            this.BrowseButton.UseVisualStyleBackColor = true;
            this.BrowseButton.Click += new System.EventHandler(this.BrowseButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(371, 425);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Dialogue Editor";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label WordsLabel;
        private System.Windows.Forms.TextBox WordsBox;
        private System.Windows.Forms.ListBox MessagesBox;
        private System.Windows.Forms.TextBox NameBox;
        private System.Windows.Forms.Button DownButton;
        private System.Windows.Forms.Button UpButton;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox PortraitBox;
        private System.Windows.Forms.Button DeselectButton;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.Button DeleteButton;
        private System.Windows.Forms.Button BrowseButton;
    }
}

